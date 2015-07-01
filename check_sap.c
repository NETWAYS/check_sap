/*
 * check_sap
 *
 * This program is designed to interface between SAP CCMS and Nagios
 * using the classic rfcsdk
 * or the Netweaver SDK
 * from SAP
 *
 * William Preston, NETWAYS GmbH 2010,2013
 * GPLv2 (see included LICENCE file for full copyright details)
 * 
 * check_sap -d <destination e.g. SAPID> -c <client e.g. 000> -u <user> -p <pass> [-l <lang>] [-3|-2 (version)] [-t (trace on)] \
 *  -h <hostname> -s <sysnr e.g. 00> [-g <gateway (if not hostname)>] [-x TCP/IP service default sapgwNN (where NN is sysnr)]
 *  [-m <modename>]
 *
 */

#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <sys/types.h>
#include <inttypes.h>
#include <math.h>
#include "mt.h"



/*
 * NOTE: Linux uses 4 Byte WChars, whereas SAP use 2 Byte WChars (UTF-16)
 * To work around this compile with -DWCHAR_is_2B -fshort-wchar
 * which will however break functions from libc like strlen...
 * Hacks to handle this are scattered through the code.
 *
 * The alternative is to exclusively use the ...U/U16 functions available in the library

    wchar_t test[] = cU("RF");
    size_t  fix;
    fix = sizeof(wchar_t)/strlenU(test);
*/


#define SAPwithUNICODE

#ifdef WCHAR_WORKAROUND
#undef WCHAR_is_4B
#define WCHAR_is_2B
#endif /* WCHAR_WORKAROUND */

#include "check_sap.h"


#define STRCPY(x,y) memcpy(x, y, (sizeofR(y)<sizeofR(x))?sizeofR(y):sizeofR(x))
#define STRCPYSTR(x,y) memcpy(x, y, (strlen(y)<sizeofR(x))?strlen(y):sizeofR(x))
#define STRCPYUSTR(x,y) memcpy(x, y, (mystrlen(y)<sizeofR(x))?mystrlen(y):sizeofR(x))
#define STRCPY2(x,y) memcpy(&x, y, sizeof(x))
#define STRADD(x,y) strncat(x, y, sizeof(x) - strlen(x) - 1)
#define FREE(x) if (x) { free(x); x = 0; }
/* #define SNPRINTF(buf,fmt,str) if (snprintf(buf, sizeof(buf), fmt, str) >= sizeof(buf)) { fprintf(stderr, "Buffer too small at line %d\n", __line__); } */

/* flag to build fullpaths for the matching */
int flag_fullpath = 0;
#define PATH_SEPARATOR '/'


/* store the connection options in a struct */
struct sap_options_s
{
    char *client;
    char *dest;
    char *gwhost;
    char *ashost;
    char *sysnr;
    char *lang;
    char *user;
    char *passwd;
    char *gwserv;
    int trace;
    int ver;
} sap_options;


/* enable tree caching? */
int cache = 0;


const char *modes[] = { "listsets", "listmonitors", "sapinfo", "listtree", NULL };
enum mode_enum { LISTSETS, LISTMONITORS, SAPINFO, LISTTREE, CHECK};

const char *exitval[] = { "UNKNOWN", "OK", "WARNING", "CRITICAL", NULL };


/* prototypes */
int xmi_logon(RFC_HANDLE);
int xmi_getmslist(RFC_HANDLE, SAP_UC *);
int xmi_getmonlist(RFC_HANDLE, SAP_UC *, SAP_UC *);
int xmi_getmontree(RFC_HANDLE, SAP_UC *, SAP_UC *, SAP_UC *, BAPITNDEXT **);
int xmi_get_value(RFC_HANDLE, SAP_UC *, BAPITID, BAPIALDATA *);
int displaytree(int, BAPITNDEXT *);
int matchitems(int, BAPITNDEXT *, MTE **, wchar_t *);
#ifdef CACHE
int tree_from_cache(SAP_UC *, SAP_UC *, int, BAPITNDEXT **);
int tree_to_cache(SAP_UC *, rcf_char_t *, int, BAPITNDEXT **);
#endif /* CACHE */

#ifdef HAVE_NETWEAVER
unsigned build_param_nw(RFC_CONNECTION_PARAMETER (*)[], struct sap_options_s *);
#else
int build_param(char **, struct sap_options_s *);
char *quote_param(char *);
void str_param(RFC_PARAMETER *, SAP_UC *, SAP_UC *);
int bind_struct_to_table(RFC_TABLE *, SAP_UC *, RFC_TABLE_HANDLE, size_t, RFC_TYPEHANDLE);
#endif /* HAVE_NETWEAVER */

SAP_UC *toU16(const char *);
size_t mystrlen(const wchar_t *);
wchar_t *mystrchr(const wchar_t *, const wchar_t);
int mbsToUCS2(wchar_t **, const char *, size_t);
size_t mystrfill(wchar_t *, const wchar_t, size_t);
wchar_t *mystrclean(wchar_t *, size_t);
int usage(char *);
void strcpy_nospaces(char *, char *);

int UCS4LEToUTF8(unsigned char **, const unsigned char *, size_t);
int UTF8ToUCS4LE(unsigned char **, const unsigned char *, size_t);
void show_error(RFC_RC, SAP_UC *, RFC_ERROR_INFO);
RFC_FUNCTION_HANDLE prepare_function(RFC_HANDLE, SAP_UC *, RFC_ERROR_INFO *);
RFC_RC call_function(RFC_HANDLE, RFC_FUNCTION_HANDLE, RFC_PARAMETER *, RFC_PARAMETER *, RFC_TABLE *, RFC_ERROR_INFO *);
unsigned count_table_rows(RFC_TABLE_HANDLE);
int fill_table(RFC_FUNCTION_HANDLE, SAP_UC *, RFC_TABLE_HANDLE *, RFC_ERROR_INFO *);
int add_export_param(RFC_FUNCTION_HANDLE, func_param *, RFC_PARAMETER *);


int main(int argc, char **argv)
{
    int             opt;
    char            *myname;

#ifdef HAVE_NETWEAVER
    RFC_CONNECTION_PARAMETER
                    conn_params[MAXCONNPARAMS];
    unsigned        param_count;
    RFC_ERROR_INFO  error_info;
    RFC_CONNECTION_HANDLE
                    handle = RFC_HANDLE_NULL;
#else
    char            *connstr;
    RFC_ERROR_INFO_EX
                    error_info;
    RFC_HANDLE      handle = RFC_HANDLE_NULL;
    RFC_RC          rfc_rc = RFC_OK;

#endif /* HAVE_NETWEAVER */

    BAPITNDEXT      *tree = 0;
    BAPITID         current;
    BAPITID         **tids = 0;
    BAPIPACVAL      perfval;
    BAPIPAPROP      perfprop;
    BAPISMCVAL      status;
    BAPIALDATA      data;
    int             i, tree_items, count, exitcode = 1;
    int             status_count[4];
    int             rc;

    SAP_UC      *buffer;
    SAP_UC      *tmpstr;

    MTE             *mteptr; /* list of MTEs */
    MTE             *mteptr2;
    char            perfdata[MAXPERFDATA];
    char            output[MAXFIRSTLINE];
    char            multi_output[MAXMULTILINE];
    int             output_lines = 0;
    char            *valunit;
    char            *valstr;
    char            *perfstr;  /* perfdata for the grapher */
    char            tmpbuf[1024];
    wchar_t         *cleanstr;

    /* thresholds returned by SAP */
    float             warn_threshold = 0;
    float             crit_threshold = 0;

    float             alrelevval = 0; /* floating point copy of ALRELEVVAL */
    int               dec = 1; /* factor by which ALRELEVVAL needs to be divided */
    float             treshy2g = 0;
    float             treshg2y = 0;
    float             treshr2y = 0;
    float             treshy2r = 0;



    /* user supplied parameters */
    int             mode = CHECK;
    int             flag_thresholds = 0, flag_ranges = 0;
    unsigned    trim = 0;
    SAP_UC      *user;
    SAP_UC      *monitor_set, *monitor_name;
    SAP_UC      *match;

    /* fill options with defaults */
    sap_options.trace = 0;
    sap_options.gwserv = NULL;
    sap_options.ver = 3;
    sap_options.lang = "E";
    sap_options.client = "000";
    sap_options.sysnr = "00";
    sap_options.user = "user";
    sap_options.passwd = "pass";
    sap_options.ashost = "localhost";
    sap_options.gwhost = "localhost";
    sap_options.dest = "SAP";

    memset(perfdata, 0, sizeof(perfdata));
    memset(output, 0, sizeof(output));
    memset(multi_output, 0, sizeof(multi_output));
    memset(status_count, 0, sizeof(status_count));

    myname = argv[0];

    static struct option longopts[] =
    {
        {"client",      required_argument,  NULL,   'c'},
        {"dest",        required_argument,  NULL,   'd'},
        {"fullpath",    no_argument,        NULL,   'f'},
        {"gateway",     required_argument,  NULL,   'g'},
        {"host",        required_argument,  NULL,   'h'},
        {"lang",        required_argument,  NULL,   'l'},
        {"mode",        required_argument,  NULL,   'm'},
        {"pass",        required_argument,  NULL,   'p'},
        {"sysnr",       required_argument,  NULL,   's'},
        {"trace",       no_argument,        NULL,   't'},
        {"user",        required_argument,  NULL,   'u'},
        {"gwserv",      required_argument,  NULL,   'x'},
        {"help",        no_argument,        NULL,   '?'},
        {"thresholds",  no_argument,        NULL,   'j'},
        {"use-ranges",  no_argument,        NULL,   'k'},
        {"trim",        required_argument,  NULL,   2},
#ifdef CACHE
        {"cache",       required_argument,  NULL,   'C'},
#endif /* CACHE */
        {NULL,          0,                  NULL,   0}
    };

    while ((opt = getopt_long(argc, argv, "c:d:fg:h:jl:m:p:s:tu:x:23C:", longopts, NULL)) != -1)
    {
        switch(opt)
        {
            case 'c':
                sap_options.client = optarg;
                break;
            case 'd':
                sap_options.dest = optarg;
                break;
            case 'f':
                flag_fullpath = 1;
                break;
            case 'g':
                sap_options.gwhost = optarg;
                break;
            case 'h':
                sap_options.ashost = optarg;
                break;
            case 'k':
                flag_ranges = 1;
                /* fall through to j */
            case 'j':
                flag_thresholds = 1;
                break;
            case 'l':
                sap_options.lang = optarg;
                break;
            case 'm':
                /* mode switch */
                for (i=0; modes[i]; i++)
                {
                    if (!strcmp(optarg, modes[i]))
                    {
                        mode = i;
                        break;
                    }
                }
                break;
            case 'p':
                sap_options.passwd = optarg;
                break;
            case 's':
                sap_options.sysnr = optarg;
                break;
            case 't':
                sap_options.trace = 1;
                break;
            case 'u':
                sap_options.user = optarg;
                break;
            case 'x':
                sap_options.gwserv = optarg;
                break;
            case '2':
                sap_options.ver = 2;
                break;
            case 'C':
#ifdef CACHE
                cache = (int)strtol(optarg, NULL, 10);
#endif /* CACHE */
                break;
            case 0:
                /* long options */
                break;
            case 2:
                /* trim leading paths from output; useful if fullpath option is set */
		trim = (unsigned)strtol(optarg, NULL, 10);
                break;
            default:
                /* usage */
                return(usage(myname));
                break;
        }
    }
    argc -= optind;
    argv += optind;

    switch(mode)
    {
        case LISTSETS:
            if (argc != 0)
            {
                printf("Mode %s does not take parameters\n", modes[LISTSETS]);
                return(usage(myname));
            }
            break;
        case LISTMONITORS:
            if (argc != 1)
            {
                printf("Mode %s takes exactly 1 parameter\n", modes[LISTMONITORS]);
                return(usage(myname));
            }
            mbsToUCS2(&monitor_set, argv[0], strlen(argv[0]));
            break;
        case LISTTREE:
            if (argc != 2)
            {
                printf("Mode %s takes exactly 2 parameters\n", modes[LISTTREE]);
                return(usage(myname));
            }
            mbsToUCS2(&monitor_set, argv[0], strlen(argv[0]));
            mbsToUCS2(&monitor_name, argv[1], strlen(argv[1]));
            break;
        case CHECK:
            if (argc < 3)
            {
                printf("Not enough parameters\n");
                return(usage(myname));
            }
            mbsToUCS2(&monitor_set, argv[0], strlen(argv[0]));
            mbsToUCS2(&monitor_name, argv[1], strlen(argv[1]));
            mbsToUCS2(&match, argv[2], strlen(argv[2]));
            break;
    }

    /* convert to UCS-2 */
    mbsToUCS2(&user, sap_options.user, strlen(sap_options.user));

#ifdef HAVE_NETWEAVER
    
    param_count = build_param_nw(&conn_params, &sap_options);

    handle = RfcOpenConnection(conn_params, param_count, &error_info);
#else
    build_param(&connstr, &sap_options);


    /* convert to UCS-2 */
    mbsToUCS2(&buffer, connstr, strlen(connstr));

    handle = RfcOpenEx(buffer, &error_info);
#endif /* HAVE_NETWEAVER */

    if (handle == RFC_HANDLE_NULL)
    {
        printf("Error: ");
        printfU (cU("(%s) %s\n"), error_info.key, error_info.message);

        return 3;
    }

    switch(mode)
    {
        case SAPINFO:
            get_info(handle);
            break;
        case LISTSETS:
            xmi_logon(handle);
            xmi_getmslist(handle, user);
            xmi_logoff(handle);
            break;
        case LISTMONITORS:
            xmi_logon(handle);
            xmi_getmonlist(handle, user, monitor_set);
            xmi_logoff(handle);
            break;
        case LISTTREE:
            xmi_logon(handle);
            tree_items = xmi_getmontree(handle, user, monitor_set, monitor_name, &tree);
            displaytree(tree_items, tree);
            // FREE(tree);
            xmi_logoff(handle);
            break;
        case CHECK:
            xmi_logon(handle);

            /* TODO read the tree and values out of a cache */
#ifdef CACHE
            if (!(tree_items = tree_from_cache(monitor_set, monitor_name, cache, &tree)))
            {
#endif /* CACHE */
                tree_items = xmi_getmontree(handle, user, monitor_set, monitor_name, &tree);
#ifdef CACHE
                tree_to_cache(monitor_set, monitor_name, tree_items, &tree);
            }
#endif /* CACHE */

            if (tree_items == -1)
            {
                /* error */
                printfU(cU("%s in %s/%s not found\n"), match, monitor_set, monitor_name);
                xmi_logoff(handle);
                exit(3);
            }
            count = matchitems(tree_items, tree, &mteptr, match);
            if (count > 0)
            {
#ifdef DEBUG
                printf("%d matches\n", count);
#endif /* DEBUG */
                mteptr2 = mteptr;
                for (i = 0; i < count; i++)
                {
                    memset(&current, 0, sizeof(current));
                    STRCPY2(current.Mtuid, mteptr2->tid.Mtuid);
                    STRCPY2(current.Mtsysid, mteptr2->tid.Mtsysid);
                    STRCPY2(current.Mtnumrange, mteptr2->tid.Mtnumrange);
                    STRCPY2(current.Mtclass, mteptr2->tid.Mtclass);
                    STRCPY2(current.Mtmcname, mteptr2->tid.Mtmcname);
                    xmi_get_value(handle, user, current, &data);

#ifdef DEBUG
                    printf("  DBG i=%d,val=%d,last=%u  ",i, data.Value, perfval.Lastalstat);
#endif /* DEBUG */
                    /* ignore items without a status */
                    if (data.Value != 0)
                    {

                        if (output_lines++ == 1)
                        {
                            /* we have more than 1 result -> switch to multi-line output mode */

                            /* add the 1st line */
                            STRADD(multi_output, output);
                        }

                        memset(output, 0, sizeof(output));

                        /* add [status] if it is a valid value */
                        if (data.Value> 0 && data.Value < 4) {
                            snprintf(tmpbuf, sizeof(tmpbuf), "[%s] ", exitval[data.Value]);
                            STRADD(output, tmpbuf);
                        }
                        snprintf(tmpbuf, sizeof(tmpbuf), "%s%c", sap_options.dest, PATH_SEPARATOR);
                        STRADD(output, tmpbuf);
                        STRADD(output, mteptr2->name);

#ifdef DEBUG
                        printfU(cU(" Class %s "),mteptr2->tid.Mtclass);
#endif /* DEBUG */
                        /* only print perfdata if the class is 100 */
                        if (!memcmp(mteptr2->tid.Mtclass, MT_CLASS_PERFORMANCE, sizeof(mteptr2->tid.Mtclass)))
                        {
                            rc = xmi_get_perfdata(handle, user, current, &perfval);
                            rc += xmi_get_perfprop(handle, user, current, &perfprop);
                            if (rc != 0)
                            {
                                /* error */
                                printfU(cU("Failed to retrieve perfdata for %s in %s/%s\n"), match, monitor_set, monitor_name);
                                xmi_logoff(handle);
                                exit(3);
                            }

                            alrelevval = (float)perfval.Alrelevval;
                            if (perfprop.Decimals > 0) {
                                dec = pow(10, perfprop.Decimals);
                                alrelevval /= dec;
                            }
#ifdef DEBUG
                            snprintf(tmpbuf, sizeof(tmpbuf), ": %d(warn %d ~ %d, crit %d ~ %d)", perfval.Alrelevval, perfprop.Treshg2y, perfprop.Treshy2g, perfprop.Treshy2r, perfprop.Treshr2y);
#else
                            snprintf(tmpbuf, sizeof(tmpbuf), ": %.*f", perfprop.Decimals, alrelevval);
#endif /* DEBUG */
                            STRADD(output, tmpbuf);

                            tmpstr = mystrclean(perfprop.Valunit, 4);
                            UCS2ToMbs(&valunit, tmpstr, mystrlen(tmpstr)); /* valunit is defined as RFC_CHAR length 4 */
                            FREE(tmpstr);

                            if (valunit)
                            {
                                snprintf(tmpbuf, sizeof(tmpbuf), " %s", valunit);
                                STRADD(output, tmpbuf);
                            }

                            /* SAP uses thresholds for state changes, but we should assume that the grapher cannot cope
                             * with ranges, so we extract the warning and critical thresholds using the following logic:
                             *
                             *  In status OK:    Warn=green->yellow, Crit=yellow->red
                             *  In status WARN:  Warn=yellow->green, Crit=yellow->red
                             *  In status CRIT:  Warn=yellow->green, Crit=red->yellow
                             *
                             * But if it can then use the '@x:y' format, which is also not correct but brings us closer...
                             */

                            treshy2g = (float)perfprop.Treshy2g;
                            treshg2y = (float)perfprop.Treshg2y;
                            treshr2y = (float)perfprop.Treshr2y;
                            treshy2r = (float)perfprop.Treshy2r;
                            if (perfprop.Decimals > 0) {
                                treshy2g /= dec;
                                treshg2y /= dec;
                                treshr2y /= dec;
                                treshy2r /= dec;
                            }
                            switch(data.Value)
                            {
                                case 1:
                                    warn_threshold = treshg2y;
                                    crit_threshold = treshy2r;
                                    break;
                                case 2:
                                    warn_threshold = treshy2g;
                                    crit_threshold = treshy2r;
                                    break;
                                case 3:
                                    warn_threshold = treshy2g;
                                    crit_threshold = treshr2y;
                                    break;
                            }

                            /* store any perfdata for the grapher */
                            /* first convert the name to a grapher friendly tag */
                            valstr = malloc(strlen(mteptr2->name) + 1);
                            perfstr = mteptr2->name; /* default is to show the full perfdata */
                            if (trim > 0)
                            {
                                i = 0;
                                while(*perfstr++ && i < trim)
                                {
                                    if(*perfstr == PATH_SEPARATOR) i++;
                                }
                                if (!*perfstr) perfstr = mteptr2->name; /* ensure we show something */
                            }

                            strcpy_nospaces(valstr, perfstr);
                            if (flag_ranges)
                            {
                                snprintf(tmpbuf, sizeof(tmpbuf), "%s=%.*f%s;@%.*f:%.*f;@%.*f:%.*f ", valstr, perfprop.Decimals, alrelevval, valunit?valunit:"",
                                    perfprop.Decimals, treshy2g, perfprop.Decimals, treshg2y,
                                    perfprop.Decimals, treshr2y, perfprop.Decimals, treshy2r);
                            } else if (flag_thresholds) {
                                snprintf(tmpbuf, sizeof(tmpbuf), "%s=%.*f%s;%.*f;%.*f ", valstr, perfprop.Decimals, alrelevval, valunit?valunit:"",
                                    perfprop.Decimals, warn_threshold, perfprop.Decimals, crit_threshold);
                            } else {
                                snprintf(tmpbuf, sizeof(tmpbuf), "%s=%.*f%s ", valstr, perfprop.Decimals, alrelevval, valunit?valunit:"");
                            }
                            FREE(valunit);
                            FREE(valstr);
                            STRADD(perfdata, tmpbuf);

                        }
                        else if (!memcmp(mteptr2->tid.Mtclass, MT_CLASS_SINGLE_MSG, sizeof(mteptr2->tid.Mtclass)))
                        {
                            xmi_get_status_msg(handle, user, current, &status);

                            tmpstr = mystrclean(status.Msg, sizeofU(status.Msg));
                            UCS2ToMbs(&valstr, tmpstr, mystrlen(tmpstr));
                            /* printfU(cU(": %s"), tmpstr); */
                            FREE(tmpstr);

                            snprintf(tmpbuf, sizeof(tmpbuf), ": %s", valstr);
                            FREE(valstr);
                            STRADD(output, tmpbuf);
                        }

                        status_count[data.Value]++;
                        if (data.Value > exitcode) { exitcode = data.Value; }

                        if (output_lines > 1)
                        {

                            STRADD(multi_output, "\n");
                            STRADD(multi_output, output);


                            /* use the first line as a status line */
                            if (status_count[3])
                            {
                                snprintf(output, sizeof(output), "[%s] %d criticals and %d warnings", exitval[exitcode], status_count[3], status_count[2]);
                            } else if (status_count[2]) {
                                snprintf(output, sizeof(output), "[%s] %d warnings", exitval[exitcode], status_count[2]);
                            } else {
                                snprintf(output, sizeof(output), "[%s] %d matching lines found", exitval[exitcode], output_lines);
                            }
                        }
                    }
                    mteptr2++;
                }
            }
            if (status_count[0] + status_count[1] + status_count[2] + status_count[3] == 0)
            {
                exitcode = 0;
                snprintf(output, sizeof(output), "[%s] No data available", exitval[exitcode]);
            }

            /* now output everything */
            printf("%s", output);
            if (strlen(perfdata)>0) printf(" | %s", perfdata);
            if (strlen(multi_output)>0) printf("\n%s", multi_output);
            printf("\n");

            // free(tids);
            FREE(tree);
            xmi_logoff(handle);
            break;
            
    }


#ifdef HAVE_NETWEAVER
    RfcCloseConnection(handle, &error_info); // ignore any errors :-)
#else
    RfcClose(handle);
#endif /* HAVE_NETWEAVER */

    if (exitcode > 0 && exitcode < 4)
    {
        return (exitcode - 1);
    } else {
        return 3;
    }
}


/* convenience wrapper for locale -> U16 */
SAP_UC *toU16(const char *src)
{
    int ret;
    wchar_t *dest;

    ret = mbsToUCS2(&dest, src, strlen(src) + 1);

    return (SAP_UC *)dest;
}


/* this function calls mbstowcs to convert a string to a 4 byte wide-character string (UCS-4)
 * and then packs it into UCS-2 (2 byte)
 *
 * This is a workaround for the gcc option -fshort-wchar
 * because mbstowcs is probably compiled with long wchar
 *
 * Assumptions:
 *  mbctowcs function compiled with 4-byte wchar
 *  this program compiled with 2-byte wchar
 *  no code points exist outside UCS-2 range
 *
 * Ensure that len includes terminating \0 (i.e. strlen+1)
 */
int mbsToUCS2(wchar_t **dest, const char *src, size_t len)
{
    /* worst case memory requirement is 4*len */

    unsigned char   *buffer, *ucs2;
    unsigned char   *from, *to;
    size_t      ret;
    int     i;

    
    buffer = (unsigned char *) malloc((len+1)*4);
    if (buffer == NULL) return -1;


    /* clear buffer to ensure terminating 0 */
    memset(buffer, 0, (len+1)*4);

    /* ret = mbstowcs((wchar_t *)buffer, src, len); */
    ret = UTF8ToUCS4LE(&buffer, src, len);
    if (ret == -1) return -1;
    
#ifdef WCHAR_WORKAROUND
    /* ucs2 should always contain terminated string */
    ucs2 = (unsigned char*) malloc((ret+1)*2);
    memset(ucs2, 0, (ret+1)*2);

    from = buffer;
    to = ucs2;

    for (i=0; *from; i+=4)
    {
        *to = *from;
        *(to+1) = *(from+1);
        from+=4;
        to+=2;
    }

    *dest = (wchar_t *)ucs2;
    FREE(buffer);
#else
    *dest = (wchar_t *)buffer
#endif /* WCHAR_WORKAROUND */

    return ret;
}
    
/* this function packs a UCS-2 or UCS-4 string into a locale dependent character string (e.g. UTF-8)
 *
 * This is a workaround to allow us to use glibc functions compiled with long wchar
 *
 * Assumptions:
 *  wcstombs function compiled with 4-byte wchar
 *  this program compiled with 2-byte wchar
 *  no code points exist outside UCS-2 range
 *
 * Adds a terminating \0 if not present
 */
int UCS2ToMbs(char **dest, const wchar_t *src, size_t len)
{
    unsigned char   *buffer, *ucs4;
    unsigned char   *from, *to;
    size_t      ret, maxsize;
    int     i;
    wchar_t     *ptr;

    
    maxsize = (len+1)*4;
#ifdef WCHAR_WORKAROUND
    ucs4 = (unsigned char *) malloc(maxsize);
    if (ucs4 == NULL) return -1;


    /* clear ucs4 to ensure terminating 0 */
    memset(ucs4, 0, maxsize);
    to = ucs4;
    from = (unsigned char *)src;

    for (i=0; *from; i+=4)
    {
        *to = *from;
        *(to+1) = *(from+1);
        from+=2;
        to+=4;
    }
#else
    ucs4 = src;
#endif /* WCHAR_WORKAROUND */
    
    /* how much space does the buffer need?? */
    buffer =  (unsigned char *) malloc(maxsize);
    memset(buffer, 0, maxsize);
    ret = UCS4LEToUTF8(&buffer, ucs4, maxsize);
    /* ret = wcstombs(buffer, ucs4, maxsize); */
    if (ret == -1)
    {
        printf("Failed to convert from UCS-2 - check system locale\n");
        exit(3);
    }

    *dest = (char *)realloc(buffer, ret);

#ifdef WCHAR_WORKAROUND
    FREE(ucs4);
#endif /* WCHAR_WORKAROUND */

    return 0;
}

    
/*  Convert a null terminated UCS4 string (upto len 4-byte characters)
    into UTF-8, and replace any invalid points with '?'

*/
int UCS4LEToUTF8(unsigned char **dest, const unsigned char *src, size_t len)
{
    unsigned char   *buffer;
    unsigned char   *from, *to;
    size_t      maxsize;
    int     i;
    unsigned long val;



    maxsize = (len+1)*4;
    buffer = (unsigned char *) malloc(maxsize);

    if (buffer == NULL) return -1;


    /* clear buffer */
    memset(buffer, 0, maxsize);
    to = buffer;
    from = (unsigned char *)src;

    for (i=0; i<len; i++)
    {
        val = (*(from+3) << 24) + (*(from+2) << 16) + (*(from+1) << 8) + (*from);

        from+=4;
        if (!val) { break; }

        if (val < 0x80)
        {
            *to++ = val;
        }
        else if (val < 0x800)
        {
            *to++ = ((val & 0x7C0) >> 6) + 0xC0;
            *to++ = (val & 0x3F) + 0x80;
        }
        else if (val > 0xD7FF && val < 0xE000)
        {
            /* Invalid coding */
            *to++ = '?';
        }
        else if (val > 0xFFFD && val < 0x10000)
        {
            /* Invalid coding */
            *to++ = '?';
        }
        else if (val < 0x10000)
        {
            *to++ = ((val & 0xF000) >> 12) + 0xE0;
            *to++ = ((val & 0xFC0) >> 6) + 0x80;
            *to++ = (val & 0x3F) + 0x80;
        }
        else if (val < 0x110000)
        {
            *to++ = ((val & 0x1C0000) >> 18) + 0xF0;
            *to++ = ((val & 0x3F000) >> 12) + 0x80;
            *to++ = ((val & 0xFC0) >> 6) + 0x80;
            *to++ = (val & 0x3F) + 0x80;
        }
        else
        {
            /* Invalid coding */
            *to++ = '?';
        }
        
    }
   
    *to = 0; /* null terminate the string */
    *dest = (char *)realloc(buffer, (to - buffer + 1));

    return (to - buffer + 1);
}


/*  Convert a null terminated UTF-8 string (upto len bytes)
    into UCS-4, and replace any invalid sequences with '?'

*/
int UTF8ToUCS4LE(unsigned char **dest, const unsigned char *src, size_t len)
{
    unsigned char   *buffer;
    unsigned char   *from, *to;
    size_t      maxsize;
    int     i, count, expected;
    unsigned long val;
    unsigned char seq[4];



    maxsize = (len+1)*4;
    buffer = (unsigned char *) malloc(maxsize);

    if (buffer == NULL) return -1;


    /* clear buffer */
    memset(buffer, 0, maxsize);
    to = buffer;
    from = (unsigned char *)src;
    count = 0;
    expected = 1;

    for (i=0; i<len; i++)
    {
        if (!*from)
        {
            /* trailing null */
            break;
        }

        /* buffer last 4 values */
        seq[3] = seq[2];
        seq[2] = seq[1];
        seq[1] = seq[0];
        seq[0] = *from++;

        if (!(seq[0] & 0x80))
        {
            /* ASCII */
            *to++ = seq[0];
            to += 3;

            /* reset sequence counter,
             * ignoring any incomplete sequence
             */
            count = 0;
            expected = 1;
        }
        else if ((seq[0] & 0xC0) == 0xC0)
        {
            /* start of seq */
            count = 1;
            val = (seq[0] & 0x30);
            if (val == 0x00)
            {
                /* 2 byte seq */
                expected = 2;
            }
            else if (val == 0x10)
            {
                /* 3 byte seq */
                expected = 3;
            }
            else if (val == 0x11)
            {
                /* 4 byte seq */
                expected = 4;
            }
            else
            {
                /* invalid -> display a '?' */
                *to++ = '?';
                *to++ = 0;
                *to++ = 0;
                *to++ = 0;
                count = 0;
                expected = 1;
            }
        }
        else
        {
            /* continuation characters */
            count++;
            if (count >= expected)
            {
                /* we have received enough bytes */
                switch(count)
                {
                    case 2:
                        *to++ = (seq[0] & 0x3F) + ((seq[1] & 0x3) << 6);
                        *to++ = ((seq[1] & 0x1C) >> 2);
                        to += 2;
                        break;
                    case 3:
                        *to++ = (seq[0] & 0x3F) + ((seq[1] & 0x3) << 6);
                        *to++ = ((seq[1] & 0x3C) >> 2) + ((seq[2] & 0xF) << 4);
                        to += 2;
                        break;
                    case 4:
                        *to++ = (seq[0] & 0x3F) + ((seq[1] & 0x3) << 6);
                        *to++ = ((seq[1] & 0x3C) >> 2) + ((seq[2] & 0xF) << 4);
                        *to++ = ((seq[2] & 0x30) >> 4) + ((seq[3] & 0x7) << 2);
                        to += 1;
                        break;
                    default:
                        /* we should never be here */
                        *to++ = '?';
                        *to++ = 0;
                        *to++ = 0;
                        *to++ = 0;
                        break;
                }
                count = 0;
                expected = 1;
            }

        }

    }
        
   
    /* null terminate the string */
    to+=4;
    *dest = (char *)realloc(buffer, (to - buffer));

    return (to - buffer);
}


/* call RFC_SYSTEM_INFO
 *
 * Returns the system version number
 */
int get_info(RFC_HANDLE handle)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("RFC_SYSTEM_INFO");
    RFC_FUNCTION_HANDLE rfc_func;
    func_param params[] = {
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
    SAP_UC import_name[] = cU("RFCSI_EXPORT");
    int     i;

    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];

    SAP_UC message[256];
    CLEAR(message);

    /* structures */
#ifdef HAVE_NETWEAVER
    RFC_STRUCTURE_HANDLE rfc_struct = NULL;
    unsigned message_len;
    int retval;
#else
    RFCSI       rfcsi;
#endif


    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);

    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }

#ifndef HAVE_NETWEAVER
    bind_struct_to_parameter(&importing[0], import_name, &rfcsi, sizeofR(rfcsi), RFCTYPE_CHAR);
#endif

    
    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        exit(3);
    }


#ifndef HAVE_NETWEAVER
    memcpy(message, rfcsi.rfcsaprl, sizeofR(rfcsi.rfcsaprl));
#else
    if (RfcGetStructure(rfc_func, import_name, &rfc_struct, &error_info))
        show_error(rfc_rc, function, error_info);
    if (RfcGetString(rfc_struct, cU("RFCSAPRL"), message, 256, &message_len, &error_info))
        show_error(rfc_rc, function, error_info);
#endif

    printfU(cU("Connected to SAP release %s\n"), message);


#ifdef HAVE_NETWEAVER
    RfcDestroyFunction(rfc_func, &error_info);
#endif
}

#ifndef HAVE_NETWEAVER
void str_param(RFC_PARAMETER *param, SAP_UC *field, SAP_UC *value)
{

    param->name = field;
    param->nlen = mystrlen(field);
    param->addr = value;
    param->leng = mystrlen(value)*sizeof(SAP_UC);
    param->type = RFCTYPE_CHAR;

}
#endif /* HAVE_NETWEAVER */


/* add a parameter to our outgoing parameter set
 * note the len is the unicode length, not number of bytes
 *
 * It is important to clear the exports array as we search it to look for existing structures
 * And make sure it is large enough to contain all the variables
 *
 * Classic requires the structures and returns all the members in the structure,
 * Netweaver requires the individual variable names.
 */
int add_export_param(RFC_FUNCTION_HANDLE rfc_func, func_param *param, RFC_PARAMETER *exports)
{
    size_t  len;
    size_t  nlen;
    DATA_CONTAINER_HANDLE parent;
    SAP_UC *dotpos = 0;
    SAP_UC struct_name[64];
    SAP_UC *varname = param->name;
    RFC_PARAMETER *export;
    int i;

    /* find next free export parameter 
     * WARNING: ensure that we have enough space...
     */

#ifndef HAVE_NETWEAVER
    for (export = exports; export->nlen; export++) {}
#endif

    nlen = mystrlen(varname);
    CLEAR(struct_name);

    /* specify variables in structures as STRUCT.VARIABLE */
    if (dotpos = mystrchr(varname, cU('.')))
    {
        /* we have a structure */
        memcpy(struct_name, varname, (dotpos - varname)*sizeof(SAP_UC));
        varname = dotpos + 1;
#ifdef DEBUG
        printfU(cU("splitting export param into %s/%s\n"), struct_name, varname);
#endif
#ifdef HAVE_NETWEAVER
        RfcGetStructure(rfc_func, struct_name, &parent, NULL);
        rfc_func = parent;
#else
        /* ignore this parameter - it will be added manually
        nlen = mystrlen(struct_name);
        varname = param->name; */
        return 0;
#endif
    }

    if (!(len = param->len))
      len = mystrlen(param->value);

    switch (param->rfctype)
    {
        case RFCTYPE_CHAR:
#ifdef HAVE_NETWEAVER
            RfcSetChars(rfc_func, varname, param->value, len, NULL);
#else
            export->name = varname;
            export->nlen = nlen;
            export->addr = param->value;
            export->leng = len*sizeof(SAP_UC);
            export->type = RFCTYPE_CHAR;
#endif
            break;
#ifndef HAVE_NETWEAVER
        case RFCTYPE_STRUCTURE:
            /* RfcGetStructureInfoFlat */
            break;
#endif
        default:
            break;
    }
    
    return 0;
}

/* add a parameter to our incoming parameter set
 * note the len is the unicode length, not number of bytes
 */
int add_import_param(func_param *param, RFC_PARAMETER *imports)
{

#ifdef HAVE_NETWEAVER
#else
    switch (param->rfctype)
    {
        case RFCTYPE_CHAR:
            imports->name = param->name;
            imports->nlen = mystrlen(param->name);
            imports->addr = param->value;
            imports->leng = param->len*sizeof(SAP_UC);
            imports->type = RFCTYPE_CHAR;
            break;
        default:
            break;
    }
#endif
    
    return 0;
}


/* Logon */
int xmi_logon(RFC_HANDLE handle)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("BAPI_XMI_LOGON");
    RFC_FUNCTION_HANDLE rfc_func;
#ifdef PRETEND_TO_BE_CHECK_SAP
    func_param params[] = {
        { cU("EXTCOMPANY"), RFCTYPE_CHAR, cU("SAP_MONI_LIB"), 0},
        { cU("EXTPRODUCT"), RFCTYPE_CHAR, cU("LL_MONITORING"), 0},
        { cU("INTERFACE"), RFCTYPE_CHAR, cU("XAL"), 0},
        { cU("VERSION"), RFCTYPE_CHAR, cU("1.0"), 0},
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
#else
    func_param params[] = {
        { cU("EXTCOMPANY"), RFCTYPE_CHAR, cU("NETWAYS"), 0},
        { cU("EXTPRODUCT"), RFCTYPE_CHAR, cU("CHECK_CCMS"), 0},
        { cU("INTERFACE"), RFCTYPE_CHAR, cU("XAL"), 0},
        { cU("VERSION"), RFCTYPE_CHAR, cU("1.0"), 0},
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
#endif /* PRETEND_TO_BE_CHECK_SAP */
    int     i;

    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];

    SAP_UC session_id[25];
    func_param imports[] = {
        { cU("SESSIONID"), RFCTYPE_CHAR, session_id, sizeofU(session_id) -1 },
        { NULL, RFCTYPE_NULL, NULL, 0}
    };

    /* structures */
#ifdef HAVE_NETWEAVER
    unsigned message_len;
#else
#endif

    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);

    CLEAR(session_id);

    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }

    for (i=0; imports[i].name; i++)
    {
        add_import_param(&imports[i], &importing[i]);
    }


    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        exit(3);
    }


#ifdef HAVE_NETWEAVER
    if (RfcGetString(rfc_func, imports[0].name, session_id, sizeofU(session_id), &message_len, &error_info))
        show_error(rfc_rc, function, error_info);
#else
    /* already in session_id var */
#endif

#ifdef HAVE_NETWEAVER
    RfcDestroyFunction(rfc_func, &error_info);
#endif

#ifdef DEBUG
    printfU(cU("Got session ID %s\n"), session_id);
#endif /* DEBUG */

    
}

/* get list of all monitor sets */
int xmi_getmslist(RFC_HANDLE handle, SAP_UC *user)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("BAPI_SYSTEM_MS_GETLIST");
    RFC_FUNCTION_HANDLE rfc_func;
    func_param params[] = {
        { cU("EXTERNAL_USER_NAME"), RFCTYPE_CHAR, user, 0},
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
    SAP_UC *table_name = cU("MONITOR_SETS");
    SAP_UC  *tmpstr;
    int     i;
    unsigned count;

    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];
    RFC_TABLE_HANDLE rfc_table;

    /* structures */
#ifdef HAVE_NETWEAVER
    RFC_STRUCTURE_HANDLE rfc_struct = NULL;
    RFC_STRUCTURE_HANDLE row = NULL;
    SAP_UC msname[256];
#else
    RFC_TYPEHANDLE  typeHandle;
    BAPIMSNAME  msname;
#endif

    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);

    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }

#ifndef HAVE_NETWEAVER
    rfc_rc = BUILD_UNICODE_STRUCT(BAPIMSNAME, typeHandle);
    rfc_table = CREATE_BLANK_TABLE(table_name, BAPIMSNAME);
    bind_struct_to_table(&tables[0], table_name, rfc_table, sizeofR(BAPIMSNAME), typeHandle); 
#endif

    /* TODO consider what happens if we exceed max rows (255?)
     * NetWeaver offers RfcSetInt(rfc_func, cU("MAX_ROWS"), 100, &error_info); */

    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        return -1;
    }

    fill_table(rfc_func, table_name, &rfc_table, &error_info);

    count = count_table_rows(rfc_table);  
#ifdef DEBUG
    printf("Got %d monitor sets\n", count);
#endif /* DEBUG */

    for(i = 0; i < count; i++)
    {
#ifdef HAVE_NETWEAVER
        row = RfcGetCurrentRow(rfc_table, 0);
        RfcGetChars(row, cU("NAME"),  msname, sizeofU(msname), NULL);
        RfcMoveToNextRow(rfc_table, NULL);
        tmpstr = mystrclean(msname, sizeofU(msname));
#else
        ItCopyRow(rfc_table, i+1, &msname, 0);
        tmpstr = mystrclean(msname.Name, sizeofU(msname.Name));
#endif
        printfU(cU("%s\n"), tmpstr);
        FREE(tmpstr);
    }


#ifdef HAVE_NETWEAVER
    RfcDestroyFunction(rfc_func, &error_info);
#else
    ItDelete(rfc_table);
#endif
    
}

/* get list of monitors in a set */
int xmi_getmonlist(RFC_HANDLE handle, SAP_UC *user, SAP_UC *set)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("BAPI_SYSTEM_MON_GETLIST");
    RFC_FUNCTION_HANDLE rfc_func;
    func_param params[] = {
        { cU("EXTERNAL_USER_NAME"), RFCTYPE_CHAR, user, 0},
        { cU("MONI_SET_NAME.NAME"), RFCTYPE_CHAR, set, 0}, /* member of struct */
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
    SAP_UC *table_name = cU("MONITOR_NAMES");
    SAP_UC  *tmpstr, *tmpstr2;
    int     i;
    unsigned count;


    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];
    RFC_TABLE_HANDLE rfc_table;

    /* structures */
#ifdef HAVE_NETWEAVER
    RFC_STRUCTURE_HANDLE row = NULL;
    SAP_UC mnname[256];
    SAP_UC msname[256];
#else
    RFC_TYPEHANDLE  typeHandle_BAPIMSNAME;
    RFC_TYPEHANDLE  typeHandle_BAPIMNNAME;
    BAPIMSNAME  Moni_set_name;
    BAPIMNNAME  Monitor_names;
    RFC_PARAMETER *export;
#endif

    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);

    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }

#ifndef HAVE_NETWEAVER
    for (export = exporting; export->nlen; export++) {} /* find next free export */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPIMSNAME, typeHandle_BAPIMSNAME);
    bind_struct_to_parameter(export, cU("MONI_SET_NAME"), &Moni_set_name, sizeofR(BAPIMSNAME), typeHandle_BAPIMSNAME); 

    /* the set name has to be filled with blanks */
    mystrfill(Moni_set_name.Name, ' ', sizeofU(Moni_set_name.Name));
    memcpy(Moni_set_name.Name, set, (mystrlen(set))*2);

    /* now prepare structure for results */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPIMNNAME, typeHandle_BAPIMNNAME);
    rfc_table = CREATE_BLANK_TABLE(table_name, BAPIMNNAME);
    bind_struct_to_table(&tables[0], table_name, rfc_table, sizeofR(BAPIMNNAME), typeHandle_BAPIMNNAME); 
#endif

    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        return -1;
    }

    fill_table(rfc_func, table_name, &rfc_table, &error_info);

    count = count_table_rows(rfc_table);  
#ifdef DEBUG
    printf("got %d Monitor names\n", count);
#endif /* DEBUG */
    for(i = 0; i < count; i++)
    {
#ifdef HAVE_NETWEAVER
        row = RfcGetCurrentRow(rfc_table, 0);
        RfcGetChars(row, cU("MONI_NAME"),  mnname, sizeofU(mnname), NULL);
        RfcGetChars(row, cU("MS_NAME"),  msname, sizeofU(msname), NULL);
        RfcMoveToNextRow(rfc_table, NULL);
        tmpstr = mystrclean(mnname, sizeofU(mnname));
        tmpstr2 = mystrclean(msname, sizeofU(msname));
#else
        ItCopyRow(rfc_table, i+1, &Monitor_names, 0);
        tmpstr = mystrclean(Monitor_names.Moni_name, sizeofU(Monitor_names.Moni_name));
        tmpstr2 = mystrclean(Monitor_names.Ms_name, sizeofU(Monitor_names.Ms_name));
#endif
        printfU(cU("%s / %s\n"), tmpstr2, tmpstr);
        FREE(tmpstr);
        FREE(tmpstr2);
    }

#ifdef HAVE_NETWEAVER
    RfcDestroyFunction(rfc_func, &error_info);
#else
    ItDelete(rfc_table);
#endif

    
}

/* get name and parent of an item in the tree */
int get_ancestor(int tree_items, BAPITNDEXT *tree, int idx, char **name)
{
    int i;
    char *name_l;
    wchar_t *cleanstr;

    /* assume that the index is contiguous and starts from 1 */
    
    cleanstr = mystrclean(tree[idx-1].Mtnameshrt, sizeofU(tree[idx-1].Mtnameshrt));
    UCS2ToMbs(&name_l, cleanstr, mystrlen(cleanstr));
    
    asprintf(&name_l, "%s%c%s", name_l, PATH_SEPARATOR, *name);
    FREE(*name);
    *name = name_l;
    return tree[idx-1].Alparintre;
    
}

/* try matching a regex against a tree
 *
 * Returns a list of TIDs to allow values to be queried
 */
// int matchitems(int tree_items, tree_s *tree, MTE **mteptrs, wchar_t *match2)
int matchitems(int tree_items, BAPITNDEXT *tree, MTE **mteptrs, wchar_t *match2)
{
    char        *match, *test;
    wchar_t     *cleanstr;
    int         i, status, matchcount = 0;
    regex_t     re;
    BAPITID     tid;
    MTE     *mte;
    int     parent_id;
    char        *full_name = 0;
    BAPIPACVAL      perfval;



    /* in order to use external regex functions we need to convert the strings
     * back to locale - will break if our locale doesn't support a character...
     */

    UCS2ToMbs(&match, match2, mystrlen(match2));


    /* How many MTE pointers do we need to return?
     * Assume that every line matches
     */
    mte = malloc(sizeof(struct MTE) * tree_items+10);
    if(!mte) printf("failed to allocate memory\n");
    *mteptrs = mte;


    status = regcomp(&re, match, REG_ICASE|REG_EXTENDED|REG_NOSUB);
    if (status != 0) { printf("Error compiling regex\n"); exit(3); }

    /* loop over the array and look for a match
     */

    for (i = 0; i < tree_items; i++)
    {
        cleanstr = mystrclean(tree[i].Mtnameshrt, sizeofU(tree[i].Mtnameshrt));

        UCS2ToMbs(&test, cleanstr, mystrlen(cleanstr));

        if (flag_fullpath)
        {
            if (tree[i].Alparintre > 0)
            {
#ifdef DEBUG
                // printf("Attempting to build full path to MTE\n");
#endif /* DEBUG */
                parent_id =  tree[i].Alparintre;
                asprintf(&full_name, "%s", test);
                while (parent_id = get_ancestor(tree_items, tree, parent_id, &full_name))
                {
                }
                FREE(test);
                test = full_name;
                full_name = 0;
            }
        }

        status = regexec(&re, test, (size_t) 0, NULL, 0);
        if (!status)
        {
#ifdef DEBUG
            printf("%d: %s matches our regex\n", i, test);
#endif /* DEBUG */

            memset(mte, 0, sizeof(struct MTE));

            STRCPYUSTR(mte->name_u2, cleanstr);
            STRCPYSTR(mte->name, test);
#ifdef DEBUG
            printf("mte->name is %s\n", mte->name);
#endif /* DEBUG */
            STRCPY(mte->tid.Mtuid, tree[i].Mtuid);
            STRCPY(mte->tid.Mtsysid, tree[i].Mtsysid);
            STRCPY(mte->tid.Mtnumrange, tree[i].Mtnumrange);
            STRCPY(mte->tid.Mtclass, tree[i].Mtclass);
            STRCPY(mte->tid.Mtmcname, tree[i].Mtmcname);
            /* should be able to reference the MTE with its UID */
/*
            STRCPY(mte->tid.Mtindex, tree[i].Mtindex);
            STRCPY(mte->tid.Extindex, tree[i].Extindex);
*/

        mte++;
            matchcount++;

        }
        FREE(cleanstr);
        FREE(test);
    }
    FREE(match);

/*
    // don't bother to free up unused memory
    *mteptrs = realloc(*mteptrs, sizeof(struct MTE) * matchcount);

*/

    return matchcount;
}


/* display a monitor tree
 */
// int displaytree(int tree_items, tree_s *tree)
int displaytree(int tree_items, BAPITNDEXT *tree)
{
    int     i, parent = 0;
    // tree_s  *tree_item;
    BAPITNDEXT *tree_item;


    /* loop over the array and ASSUME that the tree is sorted
     * may not be true, but it saves time...
     */

    for (i = 0; i < tree_items; i++)
    {

#ifdef DEBUG
        printfU(cU("[%d]"), tree[i].Alidxintre);
#endif /* DEBUG */
        printfU(cU("%*s%.*s"), tree[i].Allevintre * 2, cU(""), sizeofU(tree[i].Mtnameshrt), tree[i].Mtnameshrt);
#ifdef DEBUG
        printfU(cU("\t\t(child of %d, level %d, class is %.*s)"),  tree[i].Alparintre, tree[i].Allevintre, sizeofU(tree[i].Mtclass), tree[i].Mtclass);
#endif /* DEBUG */
        printfU(cU("\n"));

    }
}

/* get monitor tree
 * fills an array of tree items (BAPITNDEXT)
 * which includes the ID structure necessary to read values
 */
int xmi_getmontree(RFC_HANDLE handle, SAP_UC *user, SAP_UC *set, SAP_UC *mon, BAPITNDEXT **tree)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("BAPI_SYSTEM_MON_GETTREE");
    RFC_FUNCTION_HANDLE rfc_func;
    func_param params[] = {
        { cU("EXTERNAL_USER_NAME"), RFCTYPE_CHAR, user, 0},
        { cU("MONITOR_NAME.MONI_NAME"), RFCTYPE_CHAR, mon, 0}, /* member of struct */
        { cU("MONITOR_NAME.MS_NAME"), RFCTYPE_CHAR, set, 0}, /* member of struct */
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
    SAP_UC *table_name = cU("TREE_NODES");
    SAP_UC  *tmpstr, *tmpstr2;
    int     i;
    unsigned count;


    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];
    RFC_TABLE_HANDLE rfc_table;


    BAPITNDEXT *tree_item;
#ifdef HAVE_NETWEAVER
    RFC_STRUCTURE_HANDLE row = NULL;
    SAP_UC mnname[256];
    SAP_UC msname[256];
    RFC_FUNCTION_DESC_HANDLE rfc_func_desc;
    RFC_PARAMETER_DESC desc;
    RFC_FIELD_DESC field_desc;
#else
    RFC_TYPEHANDLE  typeHandle_BAPIMNNAME;
    RFC_TYPEHANDLE  typeHandle_BAPITNDEXT;
    BAPIMNNAME  Monitor_name;
    BAPITNDEXT  tndext; /* temporary struct */
    BAPITID     item_tid;
    RFC_PARAMETER *export;
#endif

    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);

    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }

#ifndef HAVE_NETWEAVER

    for (export = exporting; export->nlen; export++) {} /* find next free export */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPIMNNAME, typeHandle_BAPIMNNAME);
    bind_struct_to_parameter(export, cU("MONITOR_NAME"), &Monitor_name, sizeofR(BAPIMNNAME), typeHandle_BAPIMNNAME); 

    /* the names have to be filled with blanks */
    /* fill the BAPIMNNAME structure with the set and monitor name */
    mystrfill(Monitor_name.Moni_name, ' ', sizeofU(Monitor_name.Moni_name));
    mystrfill(Monitor_name.Ms_name, ' ', sizeofU(Monitor_name.Ms_name));
    memcpy(Monitor_name.Moni_name, mon, (mystrlen(mon))*2);
    memcpy(Monitor_name.Ms_name, set, (mystrlen(set))*2);

    /* now prepare structure for results */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPITNDEXT, typeHandle_BAPITNDEXT);
    rfc_table = CREATE_BLANK_TABLE(table_name, BAPITNDEXT);
    bind_struct_to_table(&tables[0], table_name, rfc_table, sizeofR(BAPITNDEXT), typeHandle_BAPITNDEXT); 
#endif

#ifdef DEBUG
    printfU(cU("%s / %s\n"), set, mon);
#endif /* DEBUG */

    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        return -1;
    }

    fill_table(rfc_func, table_name, &rfc_table, &error_info);

    count = count_table_rows(rfc_table);  
#ifdef DEBUG
    printf("got %d tree items\n", count);
#endif /* DEBUG */

#ifdef HAVE_NETWEAVERxDEBUG
    /* print out the function descriptions */
    rfc_func_desc = RfcDescribeFunction(rfc_func, &error_info);
    if (!rfc_func_desc)
    {
        show_error(rfc_rc, function, error_info);
    }
    rfc_rc = RfcGetParameterDescByName(rfc_func_desc, table_name, &desc, &error_info);
    printfU(cU("%d: %s\n"), i, desc.name);
    // get and list fields available
    rfc_rc = RfcGetFieldCount(desc.typeDescHandle, &count, &error_info);
    for (i = 0 ; i < count; i++)
    {
        rfc_rc = RfcGetFieldDescByIndex(desc.typeDescHandle, i, &field_desc, &error_info);
        if (rfc_rc != RFC_OK)
        {
            show_error(rfc_rc, function, error_info);
        }
        printfU(cU("%d: %s\n"), i, field_desc.name);
    }
#endif


/* read each row and add it to our tree which we can then cache */
    *tree = malloc(sizeof(struct BAPITNDEXT) * count);
    memset(*tree, 0, sizeof(struct BAPITNDEXT) * count);
    tree_item = *tree;
    for (i = 0; i<count; i++)
    {
#ifdef HAVE_NETWEAVER
        row = RfcGetCurrentRow(rfc_table, 0);
        FILL_STRUCTURE(row, BAPITNDEXT, tree_item);
        RfcMoveToNextRow(rfc_table, NULL);
        
#else
        ItCopyRow(rfc_table, i+1, tree_item, 0);
#endif
        tree_item++;
    }

#ifdef HAVE_NETWEAVER
    RfcDestroyFunction(rfc_func, &error_info);
#else
    ItDelete(rfc_table);
#endif

    return count;
    
}


#ifdef CACHE
int tree_from_cache(SAP_UC *set, SAP_UC *mon, int cache, BAPITNDEXT **tree)
{
    /*
     * create cache filename (md5sum?)
     * stat file & check age / size for plausibility
     * open with timeout
     * calc number_of_items from st.st_size / sizeofR(struct BAPITNDEXT)
     * mmap to tree var
     */
    int number_of_items = 0;
    return number_of_items;
}

int tree_to_cache(SAP_UC *set, SAP_UC *mon, int number_of_items, BAPITNDEXT **tree)
{
    char cachefile[1024];

    /*
     * create cache filename (md5sum?)
     * warn if another user owns the cache
     * exclusively lock file / with timeout
     * seek to sizeofR(struct BAPITNDEXT) * number_of_items
     * mmap
     * memcpy
     */
    if (!(getuid())) return 0; /* refuse to create cache as root */

    memset(cachefile, 0, sizeof(cachefile));
    if (snprintf(cachefile, sizeof(cachefile), "/tmp/%s_%s_%d", set, mon, number_of_items) >= sizeof(cachefile)) return 0;

    
    
}
#endif /* CACHE */

int xmi_get_perfdata(RFC_HANDLE handle, SAP_UC *user, BAPITID item_tid, BAPIPACVAL *retstr)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("BAPI_SYSTEM_MTE_GETPERFCURVAL");
    RFC_FUNCTION_HANDLE rfc_func;
    func_param params[] = {
        { cU("EXTERNAL_USER_NAME"), RFCTYPE_CHAR, user, 0},
        { cU("TID.MTSYSID"), RFCTYPE_CHAR, item_tid.Mtsysid, 0}, /* member of struct */
        { cU("TID.MTCLASS"), RFCTYPE_CHAR, item_tid.Mtclass, 0}, /* member of struct */
        { cU("TID.MTNUMRANGE"), RFCTYPE_CHAR, item_tid.Mtnumrange, 0}, /* member of struct */
        { cU("TID.MTMCNAME"), RFCTYPE_CHAR, item_tid.Mtmcname, 0}, /* member of struct */
        { cU("TID.MTUID"), RFCTYPE_CHAR, item_tid.Mtuid, 0}, /* member of struct */
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
    SAP_UC import_name[] = cU("CURRENT_VALUE");
    int     i;


    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];
    RFC_TABLE_HANDLE rfc_table;


#ifdef HAVE_NETWEAVER
    RFC_STRUCTURE_HANDLE row = NULL;
    SAP_UC mnname[256];
    SAP_UC msname[256];
    RFC_FUNCTION_DESC_HANDLE rfc_func_desc;
    RFC_PARAMETER_DESC desc;
    RFC_FIELD_DESC field_desc;
    RFC_STRUCTURE_HANDLE rfc_struct;
#else
    RFC_TYPEHANDLE  typeHandle_BAPITID;
    RFC_TYPEHANDLE  typeHandle_BAPIPACVAL;
    RFC_PARAMETER *export, *import;
#endif

    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);

    BAPIPACVAL  values;
    memset(&values, 0, sizeofR(values));

    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }


#ifndef HAVE_NETWEAVER
    for (export = exporting; export->nlen; export++) {} /* find next free export */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPITID, typeHandle_BAPITID);
    bind_struct_to_parameter(export, cU("TID"), &item_tid, sizeofR(item_tid), typeHandle_BAPITID);

    for (import = importing; import->nlen; import++) {} /* find next free import */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPIPACVAL, typeHandle_BAPIPACVAL);
    bind_struct_to_parameter(import, import_name, &values, sizeofR(values), typeHandle_BAPIPACVAL);
#endif


    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        return -1;
    }


#ifdef HAVE_NETWEAVER
    RfcGetStructure(rfc_func, import_name, &rfc_struct, NULL);
    FILL_STRUCTURE(rfc_struct, BAPIPACVAL, &values);
#endif

#ifdef DEBUG
    printfU(cU("  Got value Alrelevval %d\n"), values.Alrelevval);
    printfU(cU("  Got value Alrelvaldt %.*s\n"), sizeofU(values.Alrelvaldt), values.Alrelvaldt);
    printfU(cU("  Got value Alrelvalti %.*s\n"), sizeofU(values.Alrelvalti), values.Alrelvalti);
    printfU(cU("  Got value Lastalstat %d\n"), values.Lastalstat);
    printfU(cU("  Got value Lastperval %d\n"), values.Lastperval);
    printfU(cU("  Got value Avg01pval %d\n"), values.Avg01pval);
    printfU(cU("  Got value Avg05pval %d\n"), values.Avg05pval);
    printfU(cU("  Got value Avg15pval %d\n"), values.Avg15pval);
    printfU(cU("  Got value Avg01sval %d\n"), values.Avg01sval);
    printfU(cU("  Got value Avg05sval %d\n"), values.Avg05sval);
    printfU(cU("  Got value Avg15sval %d\n"), values.Avg15sval);
    printfU(cU("  Got value Avg01cval %d\n"), values.Avg01cval);
    printfU(cU("  Got value Avg05cval %d\n"), values.Avg05cval);
    printfU(cU("  Got value Avg15cval %d\n"), values.Avg15cval);
    printfU(cU("  Got value Maxpfvalue %d\n"), values.Maxpfvalue);
    printfU(cU("  Got value Maxpfdate %.*s\n"), sizeofU(values.Maxpfdate), values.Maxpfdate);
    printfU(cU("  Got value Maxpftime %.*s\n"), sizeofU(values.Maxpftime), values.Maxpftime);
    printfU(cU("  Got value Minpfvalue %d\n"), values.Minpfvalue);
    printfU(cU("  Got value Minpfdate %.*s\n"), sizeofU(values.Minpfdate), values.Minpfdate);
    printfU(cU("  Got value Minpftime %.*s\n"), sizeofU(values.Minpftime), values.Minpftime);
#endif /* DEBUG */

    *retstr = values;
    return 0;

}


/* Get properties of perf data */
int xmi_get_perfprop(RFC_HANDLE handle, SAP_UC *user, BAPITID item_tid, BAPIPAPROP *retstr)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("BAPI_SYSTEM_MTE_GETPERFPROP");
    RFC_FUNCTION_HANDLE rfc_func;
    func_param params[] = {
        { cU("EXTERNAL_USER_NAME"), RFCTYPE_CHAR, user, 0},
        { cU("TID.MTSYSID"), RFCTYPE_CHAR, item_tid.Mtsysid, 0}, /* member of struct */
        { cU("TID.MTCLASS"), RFCTYPE_CHAR, item_tid.Mtclass, 0}, /* member of struct */
        { cU("TID.MTNUMRANGE"), RFCTYPE_CHAR, item_tid.Mtnumrange, 0}, /* member of struct */
        { cU("TID.MTMCNAME"), RFCTYPE_CHAR, item_tid.Mtmcname, 0}, /* member of struct */
        { cU("TID.MTUID"), RFCTYPE_CHAR, item_tid.Mtuid, 0}, /* member of struct */
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
    SAP_UC import_name[] = cU("PROPERTIES");
    int     i;

    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];
    RFC_TABLE_HANDLE rfc_table;


#ifdef HAVE_NETWEAVER
    RFC_STRUCTURE_HANDLE row = NULL;
    SAP_UC mnname[256];
    SAP_UC msname[256];
    RFC_FUNCTION_DESC_HANDLE rfc_func_desc;
    RFC_PARAMETER_DESC desc;
    RFC_FIELD_DESC field_desc;
    RFC_STRUCTURE_HANDLE rfc_struct;
#else
    RFC_TYPEHANDLE  typeHandle_BAPITID;
    RFC_TYPEHANDLE  typeHandle_BAPIPAPROP;
    RFC_PARAMETER *export, *import;
#endif

    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);


    BAPIPAPROP  values;

    memset(&values, 0, sizeofR(values));

    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }


#ifndef HAVE_NETWEAVER
    for (export = exporting; export->nlen; export++) {} /* find next free export */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPITID, typeHandle_BAPITID);
    bind_struct_to_parameter(export, cU("TID"), &item_tid, sizeofR(item_tid), typeHandle_BAPITID);

    for (import = importing; import->nlen; import++) {} /* find next free import */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPIPAPROP, typeHandle_BAPIPAPROP);
    bind_struct_to_parameter(import, import_name, &values, sizeofR(values), typeHandle_BAPIPAPROP);
#endif


    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        return -1;
    }

#ifdef HAVE_NETWEAVER
    RfcGetStructure(rfc_func, import_name, &rfc_struct, NULL);
    FILL_STRUCTURE(rfc_struct, BAPIPAPROP, &values);
#endif
    *retstr = values;

    return 0;
}

/* Get last status message */
int xmi_get_status_msg(RFC_HANDLE handle, SAP_UC *user, BAPITID item_tid, BAPISMCVAL *retstr)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("BAPI_SYSTEM_MTE_GETSMVALUE");
    RFC_FUNCTION_HANDLE rfc_func;
    func_param params[] = {
        { cU("EXTERNAL_USER_NAME"), RFCTYPE_CHAR, user, 0},
        { cU("TID.MTSYSID"), RFCTYPE_CHAR, item_tid.Mtsysid, 0}, /* member of struct */
        { cU("TID.MTCLASS"), RFCTYPE_CHAR, item_tid.Mtclass, 0}, /* member of struct */
        { cU("TID.MTNUMRANGE"), RFCTYPE_CHAR, item_tid.Mtnumrange, 0}, /* member of struct */
        { cU("TID.MTMCNAME"), RFCTYPE_CHAR, item_tid.Mtmcname, 0}, /* member of struct */
        { cU("TID.MTUID"), RFCTYPE_CHAR, item_tid.Mtuid, 0}, /* member of struct */
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
    SAP_UC import_name[] = cU("VALUE");
    int     i;


    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];
    RFC_TABLE_HANDLE rfc_table;

#ifdef HAVE_NETWEAVER
    RFC_STRUCTURE_HANDLE row = NULL;
    SAP_UC mnname[256];
    SAP_UC msname[256];
    RFC_FUNCTION_DESC_HANDLE rfc_func_desc;
    RFC_PARAMETER_DESC desc;
    RFC_FIELD_DESC field_desc;
    RFC_STRUCTURE_HANDLE rfc_struct;
#else
    RFC_TYPEHANDLE  typeHandle_BAPITID;
    RFC_TYPEHANDLE  typeHandle_BAPISMCVAL;
    RFC_PARAMETER *export, *import;
#endif

    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);

    BAPISMCVAL  values;

    memset(&values, 0, sizeofR(values));


    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }


#ifndef HAVE_NETWEAVER
    for (export = exporting; export->nlen; export++) {} /* find next free export */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPITID, typeHandle_BAPITID);
    bind_struct_to_parameter(export, cU("TID"), &item_tid, sizeofR(item_tid), typeHandle_BAPITID);

    for (import = importing; import->nlen; import++) {} /* find next free import */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPISMCVAL, typeHandle_BAPISMCVAL);
    bind_struct_to_parameter(import, import_name, &values, sizeofR(values), typeHandle_BAPISMCVAL);
#endif


    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        return -1;
    }

#ifdef HAVE_NETWEAVER
    RfcGetStructure(rfc_func, import_name, &rfc_struct, NULL);
    FILL_STRUCTURE(rfc_struct, BAPISMCVAL, &values);
#endif
    *retstr = values;

    return 0;

}

/* get general properties */
int xmi_get_value(RFC_HANDLE handle, SAP_UC *user, BAPITID item_tid, BAPIALDATA *retstr)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_ERROR_INFO  error_info;
    SAP_UC function[] = cU("BAPI_SYSTEM_MTE_GETGENPROP");
    RFC_FUNCTION_HANDLE rfc_func;
    func_param params[] = {
        { cU("EXTERNAL_USER_NAME"), RFCTYPE_CHAR, user, 0},
        { cU("TID.MTSYSID"), RFCTYPE_CHAR, item_tid.Mtsysid, 0}, /* member of struct */
        { cU("TID.MTCLASS"), RFCTYPE_CHAR, item_tid.Mtclass, 0}, /* member of struct */
        { cU("TID.MTNUMRANGE"), RFCTYPE_CHAR, item_tid.Mtnumrange, 0}, /* member of struct */
        { cU("TID.MTMCNAME"), RFCTYPE_CHAR, item_tid.Mtmcname, 0}, /* member of struct */
        { cU("TID.MTUID"), RFCTYPE_CHAR, item_tid.Mtuid, 0}, /* member of struct */
        { NULL, RFCTYPE_NULL, NULL, 0}
    };
    SAP_UC import_name[] = cU("ACTUAL_ALERT_DATA");
    int     i;


    RFC_PARAMETER   exporting[MAXFUNCPARAMS];
    RFC_PARAMETER   importing[MAXFUNCPARAMS];
    RFC_TABLE   tables[MAXFUNCPARAMS];
    RFC_TABLE_HANDLE rfc_table;

#ifdef HAVE_NETWEAVER
    RFC_STRUCTURE_HANDLE row = NULL;
    SAP_UC mnname[256];
    SAP_UC msname[256];
    RFC_FUNCTION_DESC_HANDLE rfc_func_desc;
    RFC_PARAMETER_DESC desc;
    RFC_FIELD_DESC field_desc;
    RFC_STRUCTURE_HANDLE rfc_struct;
#else
    RFC_TYPEHANDLE  typeHandle_BAPITID;
    RFC_TYPEHANDLE  typeHandle_BAPIALDATA;
    RFC_PARAMETER *export, *import;
#endif

    CLEAR(exporting);
    CLEAR(importing);
    CLEAR(tables);

    BAPIALDATA  values;

    memset(&values, 0, sizeofR(values));


    if (!(rfc_func = prepare_function(handle, function, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; params[i].name; i++)
    {
        add_export_param(rfc_func, &params[i], exporting);
    }


#ifndef HAVE_NETWEAVER
    for (export = exporting; export->nlen; export++) {} /* find next free export */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPITID, typeHandle_BAPITID);
    bind_struct_to_parameter(export, cU("TID"), &item_tid, sizeofR(item_tid), typeHandle_BAPITID);

    for (import = importing; import->nlen; import++) {} /* find next free import */
    rfc_rc = BUILD_UNICODE_STRUCT(BAPIALDATA, typeHandle_BAPIALDATA);
    bind_struct_to_parameter(import, import_name, &values, sizeofR(values), typeHandle_BAPIALDATA);
#endif


    rfc_rc = call_function(handle, rfc_func, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        exit(3);
    }

#ifdef HAVE_NETWEAVER
    RfcGetStructure(rfc_func, import_name, &rfc_struct, NULL);
    FILL_STRUCTURE(rfc_struct, BAPIALDATA, &values);
#endif
    *retstr = values;

}
/* Logoff */
#ifdef HAVE_NETWEAVER
int xmi_logoff(RFC_HANDLE handle)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_FUNCTION_DESC_HANDLE rfc_funcdesc;
    RFC_FUNCTION_HANDLE rfc_func;
    RFC_ERROR_INFO  error_info;
    RFC_STRUCTURE_HANDLE rfc_importing;
    int i;

    SAP_UC function[] = cU("BAPI_XMI_LOGOFF");
    SAP_UC *exp[] = { cU("INTERFACE"), NULL};
    SAP_UC *exp_val[] = { cU("XAL"), NULL};

    if (!(rfc_funcdesc = RfcGetFunctionDesc(handle, function, &error_info)))
        printfU(cU("Error getting func desc for %s\n"), function);

    if (!(rfc_func = RfcCreateFunction(rfc_funcdesc, &error_info)))
        printfU(cU("Error creating func %s\n"), function);

    for (i=0; exp[i]; i++)
    {
        RfcSetChars(rfc_func, exp[i], exp_val[i], mystrlen(exp_val[i]), NULL);
    }


    rfc_rc = RfcInvoke(handle, rfc_func, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        return -1;
    }

#ifdef DEBUG
    printfU(cU("Logged off\n"));
#endif /* DEBUG */
    
}
#else
int xmi_logoff(RFC_HANDLE handle)
{
    RFC_RC  rfc_rc = RFC_OK;
    RFC_PARAMETER   exporting[32];
    RFC_PARAMETER   importing[32];
    RFC_TABLE   tables[32];
    RFC_ERROR_INFO  error_info;
    int     i;

    SAP_UC function[] = cU("BAPI_XMI_LOGOFF");
    SAP_UC *exp[] = { cU("INTERFACE"), NULL};
    SAP_UC *exp_val[] = { cU("XAL"), NULL};

    memset(&exporting, 0, sizeof(exporting));
    memset(&importing, 0, sizeof(importing));
    memset(&tables, 0, sizeof(tables));

    for (i=0; exp[i]; i++)
    {
        str_param(&exporting[i], exp[i], exp_val[i]);
    }


    rfc_rc = RfcCallReceive(handle, function, exporting, importing, tables, &error_info);

    if (rfc_rc != RFC_OK)
    {
        show_error(rfc_rc, function, error_info);
        exit(3);
    }

#ifdef DEBUG
    printfU(cU("Logged off\n"));
#endif /* DEBUG */
    
}
#endif /* HAVE_NETWEAVER */

#ifndef HAVE_NETWEAVER
#endif /* HAVE_NETWEAVER */

size_t mystrlen(const wchar_t *s)
{
    size_t n = 0;
    while (*s++) n++;
    return(n);
}

wchar_t *mystrchr(const wchar_t *s, const wchar_t c)
{
    while (*s++)
    {
        if (*s == c) return (wchar_t *)s;
    }
    return 0;
}

size_t mystrfill(wchar_t *s, const wchar_t c, size_t n)
{
    int i;

    for (i=0; i<n; i++)
    {
        (*s++) = c;
    }   
    return(n);
}


/* can we be sure that a varname is only in the ascii range? */
void varname_toupper(wchar_t *dest, const wchar_t *src)
{
    while (*src)
    {
        if (*src & 0x40)
        {
            *dest = *src & 0x5F;
        }
        else
        {
            (*dest) = (*src);
        }
        dest++; src++;
    }
    *dest = 0;
}

/* only use for null-terminated strings we already know the size of */
void strcpy_nospaces(char *dest, char *src)
{
    static unsigned char allowed[256] = "_________________________________!_#_%__()*+,-.-0123456789_____?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[_]^__abcdefghijklmnopqrstuvwxyz{_}__________________________________________________________________________________________________________________________________";

    while(*src)
    {
        *(dest++) = allowed[(unsigned char) *(src++)];
    }
    *dest = 0;
}

/* remove trailing spaces and null terminate a string
 * N.B. returns a new string because we can't be sure
 * that we have enough space for the trailing 0 in the
 * original string
 */
wchar_t *mystrclean(wchar_t *s, size_t n)
{
    int i, endpos = 0;
    wchar_t *newstr;


    for (i=n; i; i--)
    {
        if (!endpos && *(s+i-1) != ' ')
        {
            endpos = i;
        }
    }   
    
    newstr =  malloc(sizeof(wchar_t)*(endpos+1));
    if (newstr == NULL)
    {
        printf("Out of Memory\n");
        exit(3);
    }

    for (i=endpos; i--; )
    {
        *(newstr+i) = *(s+i);
    }
    newstr[endpos] = L'\0';
    return newstr;
}

#ifdef HAVE_NETWEAVER
/* build a SAP connection parameter array from the options */
unsigned build_param_nw(RFC_CONNECTION_PARAMETER (*param)[], struct sap_options_s *opt)
{

    char **tmp;
    char gwserv[9];
    unsigned cnt = 0;


    (*param)[cnt].name = cU("user"); (*param)[cnt++].value = toU16(opt->user);
    (*param)[cnt].name = cU("passwd"); (*param)[cnt++].value = toU16(opt->passwd);
    (*param)[cnt].name = cU("lang"); (*param)[cnt++].value = toU16(opt->lang);
    (*param)[cnt].name = cU("client"); (*param)[cnt++].value = toU16(opt->client);
/*
    (*param)[cnt].name = cU("type"); (*param)[cnt++].value = toU16(opt->ver);
    (*param)[cnt].name = cU("trace"); (*param)[cnt++].value = toU16(opt->trace);
*/
    (*param)[cnt].name = cU("trace"); (*param)[cnt++].value = opt->trace ? cU("1") : cU("0");

    if (opt->ver > 2)
    {
        (*param)[cnt].name = cU("ashost"); (*param)[cnt++].value = toU16(opt->ashost);
        (*param)[cnt].name = cU("sysnr"); (*param)[cnt++].value = toU16(opt->sysnr);
    } else {
        if (!opt->gwserv)
        {
            snprintf(gwserv, sizeof(gwserv), "sapgw%s", opt->sysnr);
        }
        (*param)[cnt].name = cU("dest"); (*param)[cnt++].value = toU16(opt->dest);
        (*param)[cnt].name = cU("gwhost"); (*param)[cnt++].value = toU16(opt->gwhost);
        (*param)[cnt].name = cU("gwserv");
        (*param)[cnt++].value = toU16(opt->gwserv?opt->gwserv:gwserv);
    }
    return cnt;
}

#else

/* build a SAP connection parameter string from the options */
int build_param(char **param, struct sap_options_s *opt)
{

    char **tmp;
    char gwserv[9];

    asprintf(param, "TYPE=%d USER=%s PASSWD=%s CLIENT=%s LANG=%s TRACE=%d",
        opt->ver, quote_param(opt->user), quote_param(opt->passwd), opt->client, opt->lang, opt->trace);
    
    if (opt->ver > 2)
    {
        asprintf(param, "%s ASHOST=%s SYSNR=%s", *param, opt->ashost, opt->sysnr);
    } else {
        if (!opt->gwserv)
        {
            snprintf(gwserv, sizeof(gwserv), "sapgw%s", opt->sysnr);
        }
        asprintf(param, "%s DEST=%s GWHOST=%s GWSERV=%s", *param, quote_param(opt->dest), opt->gwhost, opt->gwserv?opt->gwserv:gwserv);
    }
    return 0;
}


char *quote_param(char *param)
{
    char *tmp;

    tmp = param;

    if (param && strchr(param, ' '))
    {
        asprintf(&tmp, "...%s...", param);
    }
    return tmp;
}

#endif /* HAVE_NETWEAVER */

int usage(char *me)
{
    int i;

    printf("Usage: %s <options> <Monitor Set> [<Monitor Name> [<Item>]]\n", me);
    printf("\n where\n");
    printf("  --host|-h\t\tSAP Hostname\n");
    printf("  --user|-u\t\tSAP RFC user\n");
    printf("  --pass|-p\t\tPassword for this user\n");
    printf("  --client|-c\t\tClient (default 000)\n");
    printf("  --sysnr|-s\t\tSystem number (default 00)\n");
    printf("  --lang|-l\t\tLanguage (default E)\n");
    printf("  --trace|-t\t\tCreate .trc file in working directory\n");
    printf("  -2\t\t\tUse Version 2\n");
    printf("  --dest|-d\t\tSAP Destination (V2)\n");
    printf("  --gateway|-g\t\tSAP Gateway (V2)\n");
    printf("  --gwserv|-g\t\tSAP Gateway Server (V2 - defaults to sapgwxx where xx is the sysnr)\n");
    printf("\n");
    printf("  --fullpath|-f\t\tMatch against the full path of the Object (Separator is %c )\n",
        PATH_SEPARATOR);
    printf("  --trim\t\tRemove this number of leading parts from the performance data (useful with fullpath)\n");
    printf("  --thresholds\t\tInclude the SAP thresholds in the perf data (simplified)\n");
    printf("  --use-ranges\t\tInclude the SAP thresholds in the perf data (range format)\n");
#ifdef CACHE
    printf("  --cache\t\tEnable cacheing of the monitor tree for this number of seconds\n");
#endif /* CACHE */
    printf("\n  --mode|-m\t\tBeing one of: ");
    for (i=0; modes[i]; i++)
    {
        printf("%s ", modes[i]);
    }
    printf("\n\n");
    return 3;
}

/* display the error returned from RfcInvoke, RfcCallReceive etc */
void show_error(RFC_RC code, SAP_UC *function, RFC_ERROR_INFO error_info)
{
#ifdef HAVE_NETWEAVER
    printfU(cU("Error calling %s - %s (%d)\n"), function, error_info.message, code);
#else
    if (code == RFC_EXCEPTION || code == RFC_SYS_EXCEPTION)
    {
        printfU(cU("Error calling %s - %s (%d)\n"), function, error_info, code);
    } else {
        printfU(cU("Error calling %s - (%d)\n"), function, code);
    }
#endif
}

/* prepare the function
 * returns a function handle for netweaver
 * or a dummy pointer otherwise
 */
RFC_FUNCTION_HANDLE prepare_function(RFC_HANDLE handle, SAP_UC *function, RFC_ERROR_INFO *error)
{
    RFC_FUNCTION_HANDLE rfc_func;
#ifndef HAVE_NETWEAVER
    return function;
#else
    RFC_FUNCTION_DESC_HANDLE rfc_funcdesc;

    if (!(rfc_funcdesc = RfcGetFunctionDesc(handle, function, error)))
        return 0;

    return RfcCreateFunction(rfc_funcdesc, error);
#endif
}

RFC_RC call_function(RFC_HANDLE handle, RFC_FUNCTION_HANDLE rfc_func, RFC_PARAMETER *exporting, RFC_PARAMETER *importing, RFC_TABLE *tables, RFC_ERROR_INFO *error_infop)
{
    RFC_RC retval;
#ifndef HAVE_NETWEAVER
    retval = RfcCallReceive(handle, rfc_func, exporting, importing, tables, error_infop);
#else
    retval = RfcInvoke(handle, rfc_func, error_infop);
#endif
}

unsigned count_table_rows(RFC_TABLE_HANDLE rfc_table)
{
    int count = 0;
#ifndef HAVE_NETWEAVER
    count = ItFill(rfc_table);
#else
    RfcGetRowCount(rfc_table, &count, NULL);
    RfcMoveToFirstRow(rfc_table, NULL);
#endif

    return count;
}

#ifndef HAVE_NETWEAVER
int bind_struct_to_table(RFC_TABLE *table_entry, SAP_UC *table_name, RFC_TABLE_HANDLE rfc_table, size_t size_of_struct, RFC_TYPEHANDLE type_handle)
{
    table_entry->name = table_name;
    table_entry->nlen = mystrlen(table_name);
    table_entry->leng = size_of_struct;
    table_entry->type = type_handle;
    table_entry->itmode = RFC_ITMODE_BYREFERENCE;
    table_entry->ithandle = rfc_table;
    return 0;
}

/* fill a parameter entry with the values for a structure */
int bind_struct_to_parameter(RFC_PARAMETER *parameter_entry, SAP_UC *parameter_name, RFCSI *rfc_sip, size_t size_of_struct, RFC_TYPEHANDLE type_handle)
{
    /* do not overwrite pre-filled structs */
    if (parameter_name)
    {
        parameter_entry->name = parameter_name;
        parameter_entry->nlen = mystrlen(parameter_name);
    }
    parameter_entry->addr = rfc_sip;
    parameter_entry->leng = size_of_struct;
    parameter_entry->type = type_handle;
    return 0;
}
#endif

int fill_table(RFC_FUNCTION_HANDLE rfc_func, SAP_UC *table_name, RFC_TABLE_HANDLE *rfc_tablep, RFC_ERROR_INFO *error_infop)
{
#ifdef HAVE_NETWEAVER
    return RfcGetTable(rfc_func, table_name, rfc_tablep, error_infop);
#else
    return 0;
#endif
}


#ifdef HAVE_NETWEAVER

/* fill a BAPI structure with variables */
int fill_structure(DATA_CONTAINER_HANDLE handle, RFC_UNICODE_TYPE_ELEMENT *descr, unsigned count, void *values)
{
    int i;

    SAP_UC upper[MAXSTRLEN];
    SAP_INT intval;
    SAP_UC strval[MAXSTRLEN + 1];
    RFC_ERROR_INFO  error_info;
    RFC_RC rfc_rc;
    RFC_DATE rfc_date;
    RFC_TIME rfc_time;
    RFC_NUM rfc_num[MAXSTRLEN];
    CLEAR(strval);
    CLEAR(upper);


    for (i=0; i<count; i++)
    {
        varname_toupper(upper, descr->name);
        if (descr->type == RFCTYPE_INT)
        {
            // 4 byte int
            RfcGetInt(handle, upper, &intval, NULL);
            *(SAP_INT *)(values + descr->c2_offset) = intval;
            
        }
        else if (descr->type == RFCTYPE_CHAR)
        {
            rfc_rc = RfcGetChars(handle, upper, strval, sizeofU(strval) - 1, &error_info);
            if (rfc_rc) { 
                show_error(rfc_rc, upper, error_info);
            }
            memset(values + descr->c2_offset, 0, descr->c2_length);
            memcpy(values + descr->c2_offset, strval, descr->c2_length);
        }
        else if (descr->type == RFCTYPE_DATE)
        {
            RfcGetDate(handle, upper, rfc_date, NULL);
            memset(values + descr->c2_offset, 0, descr->c2_length);
            memcpy(values + descr->c2_offset, rfc_date, descr->c2_length);
        }
        else if (descr->type == RFCTYPE_TIME)
        {
            RfcGetTime(handle, upper, rfc_time, NULL);
            memset(values + descr->c2_offset, 0, descr->c2_length);
            memcpy(values + descr->c2_offset, rfc_time, descr->c2_length);
        }
        else if (descr->type == RFCTYPE_NUM)
        {
#ifdef DEBUG
            printf("RFCTYPE_NUM not tested in %s\n", __func__);
#endif
            CLEAR(rfc_num);
            RfcGetNum(handle, upper, rfc_num, sizeofU(rfc_num) - 1, NULL);
            memset(values + descr->c2_offset, 0, descr->c2_length);
            memcpy(values + descr->c2_offset, rfc_num, descr->c2_length);
        }
        descr++;
    }


}
#endif
