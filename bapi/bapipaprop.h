/*@(#)BAPIPAPROP $Id$ SAP */
/*
 *   BAPIPAPROP
 *
 *
 * File generated from SAP Data Dictionary
 * SAP system:.. CET
 * SAP release:. 701 
 * Date:........ Sat Oct  2 18:09:18 2010
 *
 *
 * # Of fields:.................. 12
 * Size in 1 byte  per SAP_UC:... 124
 * Size in 2 bytes per SAP_UC:... 212
 * Size in 4 bytes per SAP_UC:... 392
 *
 */ 

#ifndef BAPIPAPROP_H
#define BAPIPAPROP_H

typedef struct BAPIPAPROP
{
	SAP_INT	Relvaltype;
#define BAPIPAPROP_RELVALTYPE_LN  	CCQ_CAST(intR)4
#define BAPIPAPROP_RELVALTYPE_TYP 	RFCTYPE_INT

	SAP_INT	Threshdir;
#define BAPIPAPROP_THRESHDIR_LN  	CCQ_CAST(intR)4
#define BAPIPAPROP_THRESHDIR_TYP 	RFCTYPE_INT

	SAP_INT	Threshstat;
#define BAPIPAPROP_THRESHSTAT_LN  	CCQ_CAST(intR)4
#define BAPIPAPROP_THRESHSTAT_TYP 	RFCTYPE_INT

	SAP_INT	Treshg2y;
#define BAPIPAPROP_TRESHG2Y_LN  	CCQ_CAST(intR)4
#define BAPIPAPROP_TRESHG2Y_TYP 	RFCTYPE_INT

	SAP_INT	Treshy2r;
#define BAPIPAPROP_TRESHY2R_LN  	CCQ_CAST(intR)4
#define BAPIPAPROP_TRESHY2R_TYP 	RFCTYPE_INT

	SAP_INT	Treshy2g;
#define BAPIPAPROP_TRESHY2G_LN  	CCQ_CAST(intR)4
#define BAPIPAPROP_TRESHY2G_TYP 	RFCTYPE_INT

	SAP_INT	Treshr2y;
#define BAPIPAPROP_TRESHR2Y_LN  	CCQ_CAST(intR)4
#define BAPIPAPROP_TRESHR2Y_TYP 	RFCTYPE_INT

	SAP_CHAR	Msgclass [16];
#define BAPIPAPROP_MSGCLASS_LN  	CCQ_CAST(intR)(16 * sizeofR(SAP_CHAR))
#define BAPIPAPROP_MSGCLASS_CCNT	CCQ_CAST(intU)16
#define BAPIPAPROP_MSGCLASS_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Msgid [30];
#define BAPIPAPROP_MSGID_LN  	CCQ_CAST(intR)(30 * sizeofR(SAP_CHAR))
#define BAPIPAPROP_MSGID_CCNT	CCQ_CAST(intU)30
#define BAPIPAPROP_MSGID_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Attrgroup [40];
#define BAPIPAPROP_ATTRGROUP_LN  	CCQ_CAST(intR)(40 * sizeofR(SAP_CHAR))
#define BAPIPAPROP_ATTRGROUP_CCNT	CCQ_CAST(intU)40
#define BAPIPAPROP_ATTRGROUP_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Valunit [4];
#define BAPIPAPROP_VALUNIT_LN  	CCQ_CAST(intR)(4 * sizeofR(SAP_CHAR))
#define BAPIPAPROP_VALUNIT_CCNT	CCQ_CAST(intU)4
#define BAPIPAPROP_VALUNIT_TYP 	RFCTYPE_CHAR

/* Filler[s] for compiler independent alignment of fields: */
	rfc_padd_4_1bperC(0)	/* filler  for 1 byte  per SAP_UC */
	rfc_padd_4_1bperC(1)	/* filler  for 1 byte  per SAP_UC */

	SAP_INT	Decimals;
#define BAPIPAPROP_DECIMALS_LN  	CCQ_CAST(intR)4
#define BAPIPAPROP_DECIMALS_TYP 	RFCTYPE_INT

}
BAPIPAPROP;

/* Warning : Structure may need 4-bytes alignment. */

/* Warning : up to 2 filler bytes inserted. */

#define BAPIPAPROP_LN 	sizeofR( BAPIPAPROP )

#ifdef RFC_DEFINE_TYPE
#ifdef __cplusplus
  // define the (template) class for the actual type
  // and declare the one instance of the type describing class.

  typedef CAbapType< BAPIPAPROP > BAPIPAPROP_TYPE;
  extern BAPIPAPROP_TYPE BAPIPAPROP_Type;
  typedef CAbapTable< BAPIPAPROP > BAPIPAPROP_TABLE;

#endif /* C++ */
#endif /* RFC_DEFINE_TYPE */


#endif  /* BAPIPAPROP_H */

#ifdef RFC_DEFINE_U_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this on both UNICODE and non-UNICODE Programs
 * 
 */

static RFC_UNICODE_TYPE_ELEMENT Description_U_BAPIPAPROP[] =
{ 
{ cU("Relvaltype"),	 BAPIPAPROP_RELVALTYPE_TYP,	 0,   4,   0,   4,   0,   4,   0 },
{ cU("Threshdir"),	 BAPIPAPROP_THRESHDIR_TYP,	 0,   4,   4,   4,   4,   4,   4 },
{ cU("Threshstat"),	 BAPIPAPROP_THRESHSTAT_TYP,	 0,   4,   8,   4,   8,   4,   8 },
{ cU("Treshg2y"),	 BAPIPAPROP_TRESHG2Y_TYP,	 0,   4,  12,   4,  12,   4,  12 },
{ cU("Treshy2r"),	 BAPIPAPROP_TRESHY2R_TYP,	 0,   4,  16,   4,  16,   4,  16 },
{ cU("Treshy2g"),	 BAPIPAPROP_TRESHY2G_TYP,	 0,   4,  20,   4,  20,   4,  20 },
{ cU("Treshr2y"),	 BAPIPAPROP_TRESHR2Y_TYP,	 0,   4,  24,   4,  24,   4,  24 },
{ cU("Msgclass"),	 BAPIPAPROP_MSGCLASS_TYP,	 0,  16,  28,  32,  28,  64,  28 },
{ cU("Msgid"),	 BAPIPAPROP_MSGID_TYP,	 0,  30,  44,  60,  60, 120,  92 },
{ cU("Attrgroup"),	 BAPIPAPROP_ATTRGROUP_TYP,	 0,  40,  74,  80, 120, 160, 212 },
{ cU("Valunit"),	 BAPIPAPROP_VALUNIT_TYP,	 0,   4, 114,   8, 200,  16, 372 },
{ cU("Decimals"),	 BAPIPAPROP_DECIMALS_TYP,	 0,   4, 120,   4, 208,   4, 388 }
};

#define ENTRIES_OF_U_BAPIPAPROP (sizeofR(Description_U_BAPIPAPROP)/sizeofR((Description_U_BAPIPAPROP)[0]))

RFC_STRUCT_TYPE_ID STRUCT_ID_BAPIPAPROP = {
0x47, 0xb4, 0xfe, 0xd0, 0x69, 0x7d, 0x0e, 0x2a,
0xe1, 0x00, 0x00, 0x00, 0x0a, 0x15, 0x72, 0x98} ;

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_U_STRUCTURE
 * #include "bapipaprop.h"
 *
 * RFC_RC rc = RfcInstallUnicodeStructure( cU("BAPIPAPROP"),
 *                                  Description_U_BAPIPAPROP,
 *                                  ENTRIES_OF_U_BAPIPAPROP,
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

static RFC_TYPE_ELEMENT Description_BAPIPAPROP[] =
{ 
{ cU("Relvaltype"),	 BAPIPAPROP_RELVALTYPE_TYP, BAPIPAPROP_RELVALTYPE_LN, 0 },
{ cU("Threshdir"),	 BAPIPAPROP_THRESHDIR_TYP, BAPIPAPROP_THRESHDIR_LN, 0 },
{ cU("Threshstat"),	 BAPIPAPROP_THRESHSTAT_TYP, BAPIPAPROP_THRESHSTAT_LN, 0 },
{ cU("Treshg2y"),	 BAPIPAPROP_TRESHG2Y_TYP, BAPIPAPROP_TRESHG2Y_LN, 0 },
{ cU("Treshy2r"),	 BAPIPAPROP_TRESHY2R_TYP, BAPIPAPROP_TRESHY2R_LN, 0 },
{ cU("Treshy2g"),	 BAPIPAPROP_TRESHY2G_TYP, BAPIPAPROP_TRESHY2G_LN, 0 },
{ cU("Treshr2y"),	 BAPIPAPROP_TRESHR2Y_TYP, BAPIPAPROP_TRESHR2Y_LN, 0 },
{ cU("Msgclass"),	 BAPIPAPROP_MSGCLASS_TYP, BAPIPAPROP_MSGCLASS_LN, 0 },
{ cU("Msgid"),	 BAPIPAPROP_MSGID_TYP, BAPIPAPROP_MSGID_LN, 0 },
{ cU("Attrgroup"),	 BAPIPAPROP_ATTRGROUP_TYP, BAPIPAPROP_ATTRGROUP_LN, 0 },
{ cU("Valunit"),	 BAPIPAPROP_VALUNIT_TYP, BAPIPAPROP_VALUNIT_LN, 0 },
{ cU("Decimals"),	 BAPIPAPROP_DECIMALS_TYP, BAPIPAPROP_DECIMALS_LN, 0 }
};

#define ENTRIES_OF_BAPIPAPROP (sizeofR(Description_BAPIPAPROP)/sizeofR((Description_BAPIPAPROP)[0]))

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_STRUCTURE
 * #include "bapipaprop.h"
 *
 * RFC_RC rc = RfcInstallStructure( cU("BAPIPAPROP"),
 *                                  Description_BAPIPAPROP,
 *                                  ENTRIES_OF_BAPIPAPROP,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_STRUCTURE */


#ifdef RFC_DEFINE_TYPE
#ifdef RFC_DEFINE_STRUCTURE
#ifdef __cplusplus
  // define structure id
  const GUID DDICID_BAPIPAPROP = {0x47b4fed0L,(USHORT) 0x697d,(USHORT) 0x0e2a, 
                           {0xe1,0x00,0x00,0x00,0x0a,0x15,0x72,0x98}};
  
  // define instance of (template) class describing the type.
  BAPIPAPROP_TYPE BAPIPAPROP_Type ( "BAPIPAPROP", Description_BAPIPAPROP, ENTRIES_OF_BAPIPAPROP, DDICID_BAPIPAPROP );
#endif /* C++ */
#endif /* RFC_DEFINE_STRUCTURE */
#endif /* RFC_DEFINE_TYPE */

