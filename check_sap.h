/* following included structures were generated with genh
 * call as follows:
 * bin/genh ashost=... sysnr=.. user=... passwd=... BAPITNDEXT > bapi/bapitndext.h
 */




#ifdef HAVE_NETWEAVER
#include "sapnwrfc.h"
#else
#include "saprfc.h"
#include "sapitab.h"
#include "rfcsi.h"

/* following doesn't work */
#undef strlenU
#define strlenU(x) mystrlen(x)
#endif /* HAVE_NETWEAVER */

#ifdef HAVE_NETWEAVER
/* necessary defines if we want to include the bapi/ files
 *
 * they assume 2bytes per SAP_CHAR
 */
#define rfc_padd_4_1bperC(n) 
#define rfc_padd_4_2bperC(n) SAP_RAW __rfc_padding4_2c##n ;
#define rfc_padd_4_4bperC(n)
#define rfc_padd_4_1bperCF(n,s)      
#define rfc_padd_4_2bperCF(n,s) SAP_RAW __rfc_padding4_2c##n [s];
#define rfc_padd_4_4bperCF(n,s)

typedef struct
{
    SAP_UC         *name;
    unsigned       type;
    unsigned       decimals;
    unsigned       c1_length;
    unsigned       c1_offset;
    unsigned       c2_length;/* @field length of the field in bytes
                              * in a 2 bytes per SAP_CHAR system. 
                              */
    unsigned       c2_offset;/* @field byte offset of the field from 
                              * the beginning of the structure
                              * in a 2 bytes per SAP_CHAR system
                              */
    unsigned       c4_length;
    unsigned       c4_offset;
}
RFC_UNICODE_TYPE_ELEMENT;

typedef RFC_BYTE RFC_STRUCT_TYPE_ID[16];

/* use old name for handle */
#define RFC_HANDLE_NULL NULL
#define RFC_HANDLE RFC_CONNECTION_HANDLE 

/* parameters are done differently */
#define RFC_PARAMETER char
#define RFC_TABLE char

#endif /* HAVE_NETWEAVER */

#ifndef HAVE_NETWEAVER

/* errors are simple strings */
#define RFC_ERROR_INFO SAP_UC *

/* func handles are pointers to the name string */
#define RFC_FUNCTION_HANDLE SAP_UC *
#define DATA_CONTAINER_HANDLE void *

/* table handles are mapped */
#define RFC_TABLE_HANDLE ITAB_H

#define RFC_DEFINE_U_STRUCTURE
#endif /* !HAVE_NETWEAVER */

/* include the BAPI structures for both SDKs */
#define RFC_DEFINE_U_STRUCTURE
#include "bapi/bapimsname.h"
#include "bapi/bapimnname.h"
#include "bapi/bapitndext.h"
#include "bapi/bapitid.h"
#include "bapi/bapipacval.h"
#include "bapi/bapialdata.h"
#include "bapi/bapipaprop.h"
#include "bapi/bapismcval.h"

/* #define MAXSTRLEN 60  // too small for messages */
#define MAXSTRLEN 65536
#define MAXPERFDATA 4096
#define MAXFIRSTLINE 1024
#define MAXMULTILINE 8192
#define MAXCONNPARAMS 6
#define MAXFUNCPARAMS 6



/* create our own struct to make life easier */
typedef struct MTE {
 SAP_UC             name_u2[MAXSTRLEN]; /* short name in UCS-2 format */
 char               name[MAXSTRLEN*4]; /* short name in locale format */
 BAPITID            tid; /* enough of the TID to allow us to query perfdata */
 struct BAPIPACVAL  val; /* perf data */
} MTE;


/* store our parameters
 * N.B. a variable name with "." in it will
 * be assumed to be a part of a struct
 */
typedef struct func_param {
	SAP_UC		*name;
	RFCTYPE		rfctype;
	SAP_UC		*value;
    size_t      len; /* only used for imports */
} func_param;
	


#define QUOTE(arg) #arg



#define CLEAR(x) memset(x, 0, sizeof(x)) 




#ifndef HAVE_NETWEAVER
/* unicode structures are quite complex
 * 
 *
 * e.g. create a structure for BAPIMSNAME thus
    RFC_TYPEHANDLE  typeHandle;
    rfc_rc = RfcInstallUnicodeStructure( cU("BAPIMSNAME"),
        Description_U_BAPIMSNAME,
        ENTRIES_OF_U_BAPIMSNAME,
        0,
        NULL,
        &typeHandle );
*/

#define BUILD_UNICODE_STRUCT(x,y) RfcInstallUnicodeStructure(cU(QUOTE(x)), Description_U_##x, ENTRIES_OF_U_##x, 0, NULL, &y)
#define BUILD_UNICODE_STRUCTP(x,y) RfcInstallUnicodeStructure(cU(QUOTE(x)), Description_U_##x, ENTRIES_OF_U_##x, 0, NULL, y)

#define CREATE_BLANK_TABLE(name,strct) ItCreateU(name, sizeofR(strct), 0, 0)

#else

#define BUILD_UNICODE_STRUCT(x,y) 0
#define BUILD_UNICODE_STRUCTP(x,y) 0
#define CREATE_BLANK_TABLE(name,strct) 0


#define FILL_STRUCTURE(handle,name,var) fill_structure(handle, Description_U_##name, (sizeof(Description_U_##name)/sizeof(RFC_UNICODE_TYPE_ELEMENT)), var)


#endif
