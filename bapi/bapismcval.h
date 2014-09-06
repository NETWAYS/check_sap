/*@(#)BAPISMCVAL $Id$ SAP */
/*
 *   BAPISMCVAL
 *
 *
 * File generated from SAP Data Dictionary
 * SAP system:.. CET
 * SAP release:. 701 
 * Date:........ Sat Oct  2 18:09:19 2010
 *
 *
 * # Of fields:.................. 4
 * Size in 1 byte  per SAP_UC:... 276
 * Size in 2 bytes per SAP_UC:... 544
 * Size in 4 bytes per SAP_UC:... 1080
 *
 */ 

#ifndef BAPISMCVAL_H
#define BAPISMCVAL_H

typedef struct BAPISMCVAL
{
	SAP_INT	Smsgvalue;
#define BAPISMCVAL_SMSGVALUE_LN  	CCQ_CAST(intR)4
#define BAPISMCVAL_SMSGVALUE_TYP 	RFCTYPE_INT

	SAP_CHAR	Smsgdate [8];
#define BAPISMCVAL_SMSGDATE_LN  	CCQ_CAST(intR)(8 * sizeofR(SAP_CHAR))
#define BAPISMCVAL_SMSGDATE_CCNT	CCQ_CAST(intU)8
#define BAPISMCVAL_SMSGDATE_TYP 	RFCTYPE_DATE

	SAP_CHAR	Smsgtime [6];
#define BAPISMCVAL_SMSGTIME_LN  	CCQ_CAST(intR)(6 * sizeofR(SAP_CHAR))
#define BAPISMCVAL_SMSGTIME_CCNT	CCQ_CAST(intU)6
#define BAPISMCVAL_SMSGTIME_TYP 	RFCTYPE_TIME

	SAP_CHAR	Msg [255];
#define BAPISMCVAL_MSG_LN  	CCQ_CAST(intR)(255 * sizeofR(SAP_CHAR))
#define BAPISMCVAL_MSG_CCNT	CCQ_CAST(intU)255
#define BAPISMCVAL_MSG_TYP 	RFCTYPE_CHAR

/* Filler for compiler independent alignment of arrays from this structure: */
	rfc_padd_4_1bperCF(0,3)	/* filler for 1 byte  per SAP_UC */
	rfc_padd_4_2bperCF(0,2)	/* filler for 2 bytes per SAP_UC */
}
BAPISMCVAL;

/* Warning : Structure may need 4-bytes alignment. */

/* Warning : up to 3 filler bytes inserted. */

#define BAPISMCVAL_LN 	sizeofR( BAPISMCVAL )

#ifdef RFC_DEFINE_TYPE
#ifdef __cplusplus
  // define the (template) class for the actual type
  // and declare the one instance of the type describing class.

  typedef CAbapType< BAPISMCVAL > BAPISMCVAL_TYPE;
  extern BAPISMCVAL_TYPE BAPISMCVAL_Type;
  typedef CAbapTable< BAPISMCVAL > BAPISMCVAL_TABLE;

#endif /* C++ */
#endif /* RFC_DEFINE_TYPE */


#endif  /* BAPISMCVAL_H */

#ifdef RFC_DEFINE_U_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this on both UNICODE and non-UNICODE Programs
 * 
 */

static RFC_UNICODE_TYPE_ELEMENT Description_U_BAPISMCVAL[] =
{ 
{ cU("Smsgvalue"),	 BAPISMCVAL_SMSGVALUE_TYP,	 0,   4,   0,   4,   0,   4,   0 },
{ cU("Smsgdate"),	 BAPISMCVAL_SMSGDATE_TYP,	 0,   8,   4,  16,   4,  32,   4 },
{ cU("Smsgtime"),	 BAPISMCVAL_SMSGTIME_TYP,	 0,   6,  12,  12,  20,  24,  36 },
{ cU("Msg"),	 BAPISMCVAL_MSG_TYP,	 0, 255,  18, 510,  32, 1020,  60 }
};

#define ENTRIES_OF_U_BAPISMCVAL (sizeofR(Description_U_BAPISMCVAL)/sizeofR((Description_U_BAPISMCVAL)[0]))

RFC_STRUCT_TYPE_ID STRUCT_ID_BAPISMCVAL = {
0x47, 0xb4, 0xfd, 0xb2, 0x69, 0x7d, 0x0e, 0x2a,
0xe1, 0x00, 0x00, 0x00, 0x0a, 0x15, 0x72, 0x98} ;

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_U_STRUCTURE
 * #include "bapismcval.h"
 *
 * RFC_RC rc = RfcInstallUnicodeStructure( cU("BAPISMCVAL"),
 *                                  Description_U_BAPISMCVAL,
 *                                  ENTRIES_OF_U_BAPISMCVAL,
 *                                  0,
 *                                  NULL,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_U_STRUCTURE */


#ifdef RFC_DEFINE_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this ONLY in non-UNICODE Programs
 * 
 */

static RFC_TYPE_ELEMENT Description_BAPISMCVAL[] =
{ 
{ cU("Smsgvalue"),	 BAPISMCVAL_SMSGVALUE_TYP, BAPISMCVAL_SMSGVALUE_LN, 0 },
{ cU("Smsgdate"),	 BAPISMCVAL_SMSGDATE_TYP, BAPISMCVAL_SMSGDATE_LN, 0 },
{ cU("Smsgtime"),	 BAPISMCVAL_SMSGTIME_TYP, BAPISMCVAL_SMSGTIME_LN, 0 },
{ cU("Msg"),	 BAPISMCVAL_MSG_TYP, BAPISMCVAL_MSG_LN, 0 }
};

#define ENTRIES_OF_BAPISMCVAL (sizeofR(Description_BAPISMCVAL)/sizeofR((Description_BAPISMCVAL)[0]))

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_STRUCTURE
 * #include "bapismcval.h"
 *
 * RFC_RC rc = RfcInstallStructure( cU("BAPISMCVAL"),
 *                                  Description_BAPISMCVAL,
 *                                  ENTRIES_OF_BAPISMCVAL,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_STRUCTURE */


#ifdef RFC_DEFINE_TYPE
#ifdef RFC_DEFINE_STRUCTURE
#ifdef __cplusplus
  // define structure id
  const GUID DDICID_BAPISMCVAL = {0x47b4fdb2L,(USHORT) 0x697d,(USHORT) 0x0e2a, 
                           {0xe1,0x00,0x00,0x00,0x0a,0x15,0x72,0x98}};
  
  // define instance of (template) class describing the type.
  BAPISMCVAL_TYPE BAPISMCVAL_Type ( "BAPISMCVAL", Description_BAPISMCVAL, ENTRIES_OF_BAPISMCVAL, DDICID_BAPISMCVAL );
#endif /* C++ */
#endif /* RFC_DEFINE_STRUCTURE */
#endif /* RFC_DEFINE_TYPE */

