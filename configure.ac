AC_INIT(check_sap, 1.5)
AC_PREFIX_DEFAULT(/usr/local/nagios)
AC_PROG_CC
AC_PROG_INSTALL

AC_ARG_ENABLE(cache,
    AS_HELP_STRING([--enable-cache], [Enable cacheing of trees]),
    [ENABLE_CACHE="1"]
)

AC_ARG_ENABLE(emulate-ccms,
    AS_HELP_STRING([--enable-emulate-ccms], [Emulate the nagios-sap-ccms plugin]),
    AC_DEFINE(PRETEND_TO_BE_CHECK_SAP, 1)
)


if test "$enable_cache" = "yes"; then
   AC_DEFINE(CACHE, 1)
fi

AC_ARG_WITH(rfc-headers,
   AS_HELP_STRING([--with-rfc-headers=DIR], [RFC SDK Include files location]),
   [RFC_HDR_DIR="$withval"]
   [CPPFLAGS="$CPPFLAGS -I$withval"] 
)

AC_ARG_WITH(rfc-libs,
   AS_HELP_STRING([--with-rfc-libs=DIR], [RFC SDK Shared Library files location]),
   [RFC_LIB_DIR="$withval"]
   [LDFLAGS="$LDFLAGS -L$withval"] 
)

AC_CHECK_SIZEOF([wchar_t])

dnl Workaround for 4 byte wchar_t problem
if test "$ac_cv_sizeof_wchar_t" == "4";
then
    AC_MSG_WARN([Your wchar_t is 4 bytes long - enabling workarounds])
    AC_DEFINE(WCHAR_WORKAROUND, 1)
    EXTRACFLAGS="$EXTRACFLAGS -fshort-wchar"
fi

dnl First try to find netweaver headers and libraries
AC_CHECK_HEADERS([sapnwrfc.h])

if test "$ac_cv_header_sapnwrfc_h" == "yes";
then
    AC_MSG_NOTICE([Netweaver SDK Headers found])
    AC_MSG_CHECKING([for Netweaver Libraries])
    AC_CHECK_LIB(sapnwrfc, RfcOpenConnection)
    if test "$ac_cv_lib_sapnwrfc_RfcOpenConnection" == "yes";
    then
        AC_DEFINE(HAVE_NETWEAVER, 1)
        BUILDLIBS="$BUILDLIBS -lsapnwrfc -lsapucum"
    else
        AC_MSG_WARN([Netweaver libraries missing or unusable])
    fi
fi


dnl TODO check for non-unicode and
dnl link with -lrfccm

dnl TODO check for 32bit libraries
dnl and set the -m32 flag if necessary

dnl Maybe we have old unicode libraries then??
if test "$ac_cv_lib_sapnwrfc_RfcOpenConnection" != "yes";
then
    AC_CHECK_HEADERS([saprfc.h])
    if test "$ac_cv_header_saprfc_h" == "yes";
    then
        AC_MSG_NOTICE([Classical RFC SDK headers found])
        AC_MSG_CHECKING([for RFC Unicode libraries])
        AC_CHECK_LIB(rfcum, RfcOpenExU)
        if test "$ac_cv_lib_rfcum_RfcOpenExU" == "yes";
        then
            AC_DEFINE(HAVE_UNICODE, 1)
            BUILDLIBS="$BUILDLIBS -lsapu16 -lrfcum"
        else
            AC_MSG_ERROR([Classical RFC SDK libraries missing or unusable - is libstdc++ installed?])
        fi
    else
        AC_MSG_ERROR([No usable SDK found])
    fi
fi

dnl Look for typedefs
dnl AC_CHECK_TYPES([rfc_char_t], [], [], [#ifdef HAVE_SAPNWRFC_H #include "sapnwrfc.h" #else #ifdef HAVE_SAPRFC_H #include "saprfc.h" #endif #endif ])


dnl AC_DEFINE(CHECK_SAP_UNICODE, 1)

AC_SUBST([EXTRACFLAGS])
AC_SUBST([BUILDLIBS])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
