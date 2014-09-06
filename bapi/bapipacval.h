/*@(#)BAPIPACVAL $Id$ SAP */
/*
 *   BAPIPACVAL
 *
 *
 * File generated from SAP Data Dictionary
 * SAP system:.. CET
 * SAP release:. 701 
 * Date:........ Thu Sep 30 20:26:58 2010
 *
 *
 * # Of fields:.................. 20
 * Size in 1 byte  per SAP_UC:... 104
 * Size in 2 bytes per SAP_UC:... 140
 * Size in 4 bytes per SAP_UC:... 224
 *
 */ 

#ifndef BAPIPACVAL_H
#define BAPIPACVAL_H

typedef struct BAPIPACVAL
{
	SAP_INT	Alrelevval;
#define BAPIPACVAL_ALRELEVVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_ALRELEVVAL_TYP 	RFCTYPE_INT

	SAP_CHAR	Alrelvaldt [8];
#define BAPIPACVAL_ALRELVALDT_LN  	CCQ_CAST(intR)(8 * sizeofR(SAP_CHAR))
#define BAPIPACVAL_ALRELVALDT_CCNT	CCQ_CAST(intU)8
#define BAPIPACVAL_ALRELVALDT_TYP 	RFCTYPE_DATE

	SAP_CHAR	Alrelvalti [6];
#define BAPIPACVAL_ALRELVALTI_LN  	CCQ_CAST(intR)(6 * sizeofR(SAP_CHAR))
#define BAPIPACVAL_ALRELVALTI_CCNT	CCQ_CAST(intU)6
#define BAPIPACVAL_ALRELVALTI_TYP 	RFCTYPE_TIME

/* Filler[s] for compiler independent alignment of fields: */
	rfc_padd_4_1bperC(0)	/* filler  for 1 byte  per SAP_UC */
	rfc_padd_4_1bperC(1)	/* filler  for 1 byte  per SAP_UC */

	SAP_INT	Lastalstat;
#define BAPIPACVAL_LASTALSTAT_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_LASTALSTAT_TYP 	RFCTYPE_INT

	SAP_INT	Lastperval;
#define BAPIPACVAL_LASTPERVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_LASTPERVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg01pval;
#define BAPIPACVAL_AVG01PVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG01PVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg05pval;
#define BAPIPACVAL_AVG05PVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG05PVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg15pval;
#define BAPIPACVAL_AVG15PVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG15PVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg01sval;
#define BAPIPACVAL_AVG01SVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG01SVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg05sval;
#define BAPIPACVAL_AVG05SVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG05SVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg15sval;
#define BAPIPACVAL_AVG15SVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG15SVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg01cval;
#define BAPIPACVAL_AVG01CVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG01CVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg05cval;
#define BAPIPACVAL_AVG05CVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG05CVAL_TYP 	RFCTYPE_INT

	SAP_INT	Avg15cval;
#define BAPIPACVAL_AVG15CVAL_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_AVG15CVAL_TYP 	RFCTYPE_INT

	SAP_INT	Maxpfvalue;
#define BAPIPACVAL_MAXPFVALUE_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_MAXPFVALUE_TYP 	RFCTYPE_INT

	SAP_CHAR	Maxpfdate [8];
#define BAPIPACVAL_MAXPFDATE_LN  	CCQ_CAST(intR)(8 * sizeofR(SAP_CHAR))
#define BAPIPACVAL_MAXPFDATE_CCNT	CCQ_CAST(intU)8
#define BAPIPACVAL_MAXPFDATE_TYP 	RFCTYPE_DATE

	SAP_CHAR	Maxpftime [6];
#define BAPIPACVAL_MAXPFTIME_LN  	CCQ_CAST(intR)(6 * sizeofR(SAP_CHAR))
#define BAPIPACVAL_MAXPFTIME_CCNT	CCQ_CAST(intU)6
#define BAPIPACVAL_MAXPFTIME_TYP 	RFCTYPE_TIME

/* Filler[s] for compiler independent alignment of fields: */
	rfc_padd_4_1bperC(2)	/* filler  for 1 byte  per SAP_UC */
	rfc_padd_4_1bperC(3)	/* filler  for 1 byte  per SAP_UC */

	SAP_INT	Minpfvalue;
#define BAPIPACVAL_MINPFVALUE_LN  	CCQ_CAST(intR)4
#define BAPIPACVAL_MINPFVALUE_TYP 	RFCTYPE_INT

	SAP_CHAR	Minpfdate [8];
#define BAPIPACVAL_MINPFDATE_LN  	CCQ_CAST(intR)(8 * sizeofR(SAP_CHAR))
#define BAPIPACVAL_MINPFDATE_CCNT	CCQ_CAST(intU)8
#define BAPIPACVAL_MINPFDATE_TYP 	RFCTYPE_DATE

	SAP_CHAR	Minpftime [6];
#define BAPIPACVAL_MINPFTIME_LN  	CCQ_CAST(intR)(6 * sizeofR(SAP_CHAR))
#define BAPIPACVAL_MINPFTIME_CCNT	CCQ_CAST(intU)6
#define BAPIPACVAL_MINPFTIME_TYP 	RFCTYPE_TIME

/* Filler for compiler independent alignment of arrays from this structure: */
	rfc_padd_4_1bperCF(4,2)	/* filler for 1 byte  per SAP_UC */
}
BAPIPACVAL;

/* Warning : Structure may need 4-bytes alignment. */

/* Warning : up to 6 filler bytes inserted. */

#define BAPIPACVAL_LN 	sizeofR( BAPIPACVAL )

#ifdef RFC_DEFINE_TYPE
#ifdef __cplusplus
  // define the (template) class for the actual type
  // and declare the one instance of the type describing class.

  typedef CAbapType< BAPIPACVAL > BAPIPACVAL_TYPE;
  extern BAPIPACVAL_TYPE BAPIPACVAL_Type;
  typedef CAbapTable< BAPIPACVAL > BAPIPACVAL_TABLE;

#endif /* C++ */
#endif /* RFC_DEFINE_TYPE */


#endif  /* BAPIPACVAL_H */

#ifdef RFC_DEFINE_U_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this on both UNICODE and non-UNICODE Programs
 * 
 */

static RFC_UNICODE_TYPE_ELEMENT Description_U_BAPIPACVAL[] =
{ 
{ cU("Alrelevval"),	 BAPIPACVAL_ALRELEVVAL_TYP,	 0,   4,   0,   4,   0,   4,   0 },
{ cU("Alrelvaldt"),	 BAPIPACVAL_ALRELVALDT_TYP,	 0,   8,   4,  16,   4,  32,   4 },
{ cU("Alrelvalti"),	 BAPIPACVAL_ALRELVALTI_TYP,	 0,   6,  12,  12,  20,  24,  36 },
{ cU("Lastalstat"),	 BAPIPACVAL_LASTALSTAT_TYP,	 0,   4,  20,   4,  32,   4,  60 },
{ cU("Lastperval"),	 BAPIPACVAL_LASTPERVAL_TYP,	 0,   4,  24,   4,  36,   4,  64 },
{ cU("Avg01pval"),	 BAPIPACVAL_AVG01PVAL_TYP,	 0,   4,  28,   4,  40,   4,  68 },
{ cU("Avg05pval"),	 BAPIPACVAL_AVG05PVAL_TYP,	 0,   4,  32,   4,  44,   4,  72 },
{ cU("Avg15pval"),	 BAPIPACVAL_AVG15PVAL_TYP,	 0,   4,  36,   4,  48,   4,  76 },
{ cU("Avg01sval"),	 BAPIPACVAL_AVG01SVAL_TYP,	 0,   4,  40,   4,  52,   4,  80 },
{ cU("Avg05sval"),	 BAPIPACVAL_AVG05SVAL_TYP,	 0,   4,  44,   4,  56,   4,  84 },
{ cU("Avg15sval"),	 BAPIPACVAL_AVG15SVAL_TYP,	 0,   4,  48,   4,  60,   4,  88 },
{ cU("Avg01cval"),	 BAPIPACVAL_AVG01CVAL_TYP,	 0,   4,  52,   4,  64,   4,  92 },
{ cU("Avg05cval"),	 BAPIPACVAL_AVG05CVAL_TYP,	 0,   4,  56,   4,  68,   4,  96 },
{ cU("Avg15cval"),	 BAPIPACVAL_AVG15CVAL_TYP,	 0,   4,  60,   4,  72,   4, 100 },
{ cU("Maxpfvalue"),	 BAPIPACVAL_MAXPFVALUE_TYP,	 0,   4,  64,   4,  76,   4, 104 },
{ cU("Maxpfdate"),	 BAPIPACVAL_MAXPFDATE_TYP,	 0,   8,  68,  16,  80,  32, 108 },
{ cU("Maxpftime"),	 BAPIPACVAL_MAXPFTIME_TYP,	 0,   6,  76,  12,  96,  24, 140 },
{ cU("Minpfvalue"),	 BAPIPACVAL_MINPFVALUE_TYP,	 0,   4,  84,   4, 108,   4, 164 },
{ cU("Minpfdate"),	 BAPIPACVAL_MINPFDATE_TYP,	 0,   8,  88,  16, 112,  32, 168 },
{ cU("Minpftime"),	 BAPIPACVAL_MINPFTIME_TYP,	 0,   6,  96,  12, 128,  24, 200 }
};

#define ENTRIES_OF_U_BAPIPACVAL (sizeofR(Description_U_BAPIPACVAL)/sizeofR((Description_U_BAPIPACVAL)[0]))

RFC_STRUCT_TYPE_ID STRUCT_ID_BAPIPACVAL = {
0x47, 0xb4, 0xfe, 0xd4, 0x69, 0x7d, 0x0e, 0x2a,
0xe1, 0x00, 0x00, 0x00, 0x0a, 0x15, 0x72, 0x98} ;

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_U_STRUCTURE
 * #include "bapipacval.h"
 *
 * RFC_RC rc = RfcInstallUnicodeStructure( cU("BAPIPACVAL"),
 *                                  Description_U_BAPIPACVAL,
 *                                  ENTRIES_OF_U_BAPIPACVAL,
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

static RFC_TYPE_ELEMENT Description_BAPIPACVAL[] =
{ 
{ cU("Alrelevval"),	 BAPIPACVAL_ALRELEVVAL_TYP, BAPIPACVAL_ALRELEVVAL_LN, 0 },
{ cU("Alrelvaldt"),	 BAPIPACVAL_ALRELVALDT_TYP, BAPIPACVAL_ALRELVALDT_LN, 0 },
{ cU("Alrelvalti"),	 BAPIPACVAL_ALRELVALTI_TYP, BAPIPACVAL_ALRELVALTI_LN, 0 },
{ cU("Lastalstat"),	 BAPIPACVAL_LASTALSTAT_TYP, BAPIPACVAL_LASTALSTAT_LN, 0 },
{ cU("Lastperval"),	 BAPIPACVAL_LASTPERVAL_TYP, BAPIPACVAL_LASTPERVAL_LN, 0 },
{ cU("Avg01pval"),	 BAPIPACVAL_AVG01PVAL_TYP, BAPIPACVAL_AVG01PVAL_LN, 0 },
{ cU("Avg05pval"),	 BAPIPACVAL_AVG05PVAL_TYP, BAPIPACVAL_AVG05PVAL_LN, 0 },
{ cU("Avg15pval"),	 BAPIPACVAL_AVG15PVAL_TYP, BAPIPACVAL_AVG15PVAL_LN, 0 },
{ cU("Avg01sval"),	 BAPIPACVAL_AVG01SVAL_TYP, BAPIPACVAL_AVG01SVAL_LN, 0 },
{ cU("Avg05sval"),	 BAPIPACVAL_AVG05SVAL_TYP, BAPIPACVAL_AVG05SVAL_LN, 0 },
{ cU("Avg15sval"),	 BAPIPACVAL_AVG15SVAL_TYP, BAPIPACVAL_AVG15SVAL_LN, 0 },
{ cU("Avg01cval"),	 BAPIPACVAL_AVG01CVAL_TYP, BAPIPACVAL_AVG01CVAL_LN, 0 },
{ cU("Avg05cval"),	 BAPIPACVAL_AVG05CVAL_TYP, BAPIPACVAL_AVG05CVAL_LN, 0 },
{ cU("Avg15cval"),	 BAPIPACVAL_AVG15CVAL_TYP, BAPIPACVAL_AVG15CVAL_LN, 0 },
{ cU("Maxpfvalue"),	 BAPIPACVAL_MAXPFVALUE_TYP, BAPIPACVAL_MAXPFVALUE_LN, 0 },
{ cU("Maxpfdate"),	 BAPIPACVAL_MAXPFDATE_TYP, BAPIPACVAL_MAXPFDATE_LN, 0 },
{ cU("Maxpftime"),	 BAPIPACVAL_MAXPFTIME_TYP, BAPIPACVAL_MAXPFTIME_LN, 0 },
{ cU("Minpfvalue"),	 BAPIPACVAL_MINPFVALUE_TYP, BAPIPACVAL_MINPFVALUE_LN, 0 },
{ cU("Minpfdate"),	 BAPIPACVAL_MINPFDATE_TYP, BAPIPACVAL_MINPFDATE_LN, 0 },
{ cU("Minpftime"),	 BAPIPACVAL_MINPFTIME_TYP, BAPIPACVAL_MINPFTIME_LN, 0 }
};

#define ENTRIES_OF_BAPIPACVAL (sizeofR(Description_BAPIPACVAL)/sizeofR((Description_BAPIPACVAL)[0]))

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_STRUCTURE
 * #include "bapipacval.h"
 *
 * RFC_RC rc = RfcInstallStructure( cU("BAPIPACVAL"),
 *                                  Description_BAPIPACVAL,
 *                                  ENTRIES_OF_BAPIPACVAL,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_STRUCTURE */


#ifdef RFC_DEFINE_TYPE
#ifdef RFC_DEFINE_STRUCTURE
#ifdef __cplusplus
  // define structure id
  const GUID DDICID_BAPIPACVAL = {0x47b4fed4L,(USHORT) 0x697d,(USHORT) 0x0e2a, 
                           {0xe1,0x00,0x00,0x00,0x0a,0x15,0x72,0x98}};
  
  // define instance of (template) class describing the type.
  BAPIPACVAL_TYPE BAPIPACVAL_Type ( "BAPIPACVAL", Description_BAPIPACVAL, ENTRIES_OF_BAPIPACVAL, DDICID_BAPIPACVAL );
#endif /* C++ */
#endif /* RFC_DEFINE_STRUCTURE */
#endif /* RFC_DEFINE_TYPE */

