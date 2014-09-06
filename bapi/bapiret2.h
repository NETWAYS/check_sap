/*@(#)BAPIRET2 $Id$ SAP */
/*
 *   BAPIRET2
 *
 *
 * File generated from SAP Data Dictionary
 * SAP system:.. CET
 * SAP release:. 701 
 * Date:........ Thu Sep 30 19:57:12 2010
 *
 *
 * # Of fields:.................. 14
 * Size in 1 byte  per SAP_UC:... 548
 * Size in 2 bytes per SAP_UC:... 1088
 * Size in 4 bytes per SAP_UC:... 2172
 *
 */ 

#ifndef BAPIRET2_H
#define BAPIRET2_H

typedef struct BAPIRET2
{
	SAP_CHAR	Type [1];
#define BAPIRET2_TYPE_LN  	CCQ_CAST(intR)(1 * sizeofR(SAP_CHAR))
#define BAPIRET2_TYPE_CCNT	CCQ_CAST(intU)1
#define BAPIRET2_TYPE_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Id [20];
#define BAPIRET2_ID_LN  	CCQ_CAST(intR)(20 * sizeofR(SAP_CHAR))
#define BAPIRET2_ID_CCNT	CCQ_CAST(intU)20
#define BAPIRET2_ID_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Number [3];
#define BAPIRET2_NUMBER_LN  	CCQ_CAST(intR)(3 * sizeofR(SAP_CHAR))
#define BAPIRET2_NUMBER_CCNT	CCQ_CAST(intU)3
#define BAPIRET2_NUMBER_TYP 	RFCTYPE_NUM

	SAP_CHAR	Message [220];
#define BAPIRET2_MESSAGE_LN  	CCQ_CAST(intR)(220 * sizeofR(SAP_CHAR))
#define BAPIRET2_MESSAGE_CCNT	CCQ_CAST(intU)220
#define BAPIRET2_MESSAGE_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Log_no [20];
#define BAPIRET2_LOG_NO_LN  	CCQ_CAST(intR)(20 * sizeofR(SAP_CHAR))
#define BAPIRET2_LOG_NO_CCNT	CCQ_CAST(intU)20
#define BAPIRET2_LOG_NO_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Log_msg_no [6];
#define BAPIRET2_LOG_MSG_NO_LN  	CCQ_CAST(intR)(6 * sizeofR(SAP_CHAR))
#define BAPIRET2_LOG_MSG_NO_CCNT	CCQ_CAST(intU)6
#define BAPIRET2_LOG_MSG_NO_TYP 	RFCTYPE_NUM

	SAP_CHAR	Message_v1 [50];
#define BAPIRET2_MESSAGE_V1_LN  	CCQ_CAST(intR)(50 * sizeofR(SAP_CHAR))
#define BAPIRET2_MESSAGE_V1_CCNT	CCQ_CAST(intU)50
#define BAPIRET2_MESSAGE_V1_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Message_v2 [50];
#define BAPIRET2_MESSAGE_V2_LN  	CCQ_CAST(intR)(50 * sizeofR(SAP_CHAR))
#define BAPIRET2_MESSAGE_V2_CCNT	CCQ_CAST(intU)50
#define BAPIRET2_MESSAGE_V2_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Message_v3 [50];
#define BAPIRET2_MESSAGE_V3_LN  	CCQ_CAST(intR)(50 * sizeofR(SAP_CHAR))
#define BAPIRET2_MESSAGE_V3_CCNT	CCQ_CAST(intU)50
#define BAPIRET2_MESSAGE_V3_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Message_v4 [50];
#define BAPIRET2_MESSAGE_V4_LN  	CCQ_CAST(intR)(50 * sizeofR(SAP_CHAR))
#define BAPIRET2_MESSAGE_V4_CCNT	CCQ_CAST(intU)50
#define BAPIRET2_MESSAGE_V4_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Parameter [32];
#define BAPIRET2_PARAMETER_LN  	CCQ_CAST(intR)(32 * sizeofR(SAP_CHAR))
#define BAPIRET2_PARAMETER_CCNT	CCQ_CAST(intU)32
#define BAPIRET2_PARAMETER_TYP 	RFCTYPE_CHAR

/* Filler[s] for compiler independent alignment of fields: */
	rfc_padd_4_1bperC(0)	/* filler  for 1 byte  per SAP_UC */
	rfc_padd_4_1bperC(1)	/* filler  for 1 byte  per SAP_UC */

	SAP_INT	Row;
#define BAPIRET2_ROW_LN  	CCQ_CAST(intR)4
#define BAPIRET2_ROW_TYP 	RFCTYPE_INT

	SAP_CHAR	Field [30];
#define BAPIRET2_FIELD_LN  	CCQ_CAST(intR)(30 * sizeofR(SAP_CHAR))
#define BAPIRET2_FIELD_CCNT	CCQ_CAST(intU)30
#define BAPIRET2_FIELD_TYP 	RFCTYPE_CHAR

	SAP_CHAR	System [10];
#define BAPIRET2_SYSTEM_LN  	CCQ_CAST(intR)(10 * sizeofR(SAP_CHAR))
#define BAPIRET2_SYSTEM_CCNT	CCQ_CAST(intU)10
#define BAPIRET2_SYSTEM_TYP 	RFCTYPE_CHAR

}
BAPIRET2;

/* Warning : Structure may need 4-bytes alignment. */

/* Warning : up to 2 filler bytes inserted. */

#define BAPIRET2_LN 	sizeofR( BAPIRET2 )

#ifdef RFC_DEFINE_TYPE
#ifdef __cplusplus
  // define the (template) class for the actual type
  // and declare the one instance of the type describing class.

  typedef CAbapType< BAPIRET2 > BAPIRET2_TYPE;
  extern BAPIRET2_TYPE BAPIRET2_Type;
  typedef CAbapTable< BAPIRET2 > BAPIRET2_TABLE;

#endif /* C++ */
#endif /* RFC_DEFINE_TYPE */


#endif  /* BAPIRET2_H */

#ifdef RFC_DEFINE_U_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this on both UNICODE and non-UNICODE Programs
 * 
 */

static RFC_UNICODE_TYPE_ELEMENT Description_U_BAPIRET2[] =
{ 
{ cU("Type"),	 BAPIRET2_TYPE_TYP,	 0,   1,   0,   2,   0,   4,   0 },
{ cU("Id"),	 BAPIRET2_ID_TYP,	 0,  20,   1,  40,   2,  80,   4 },
{ cU("Number"),	 BAPIRET2_NUMBER_TYP,	 0,   3,  21,   6,  42,  12,  84 },
{ cU("Message"),	 BAPIRET2_MESSAGE_TYP,	 0, 220,  24, 440,  48, 880,  96 },
{ cU("Log_no"),	 BAPIRET2_LOG_NO_TYP,	 0,  20, 244,  40, 488,  80, 976 },
{ cU("Log_msg_no"),	 BAPIRET2_LOG_MSG_NO_TYP,	 0,   6, 264,  12, 528,  24, 1056 },
{ cU("Message_v1"),	 BAPIRET2_MESSAGE_V1_TYP,	 0,  50, 270, 100, 540, 200, 1080 },
{ cU("Message_v2"),	 BAPIRET2_MESSAGE_V2_TYP,	 0,  50, 320, 100, 640, 200, 1280 },
{ cU("Message_v3"),	 BAPIRET2_MESSAGE_V3_TYP,	 0,  50, 370, 100, 740, 200, 1480 },
{ cU("Message_v4"),	 BAPIRET2_MESSAGE_V4_TYP,	 0,  50, 420, 100, 840, 200, 1680 },
{ cU("Parameter"),	 BAPIRET2_PARAMETER_TYP,	 0,  32, 470,  64, 940, 128, 1880 },
{ cU("Row"),	 BAPIRET2_ROW_TYP,	 0,   4, 504,   4, 1004,   4, 2008 },
{ cU("Field"),	 BAPIRET2_FIELD_TYP,	 0,  30, 508,  60, 1008, 120, 2012 },
{ cU("System"),	 BAPIRET2_SYSTEM_TYP,	 0,  10, 538,  20, 1068,  40, 2132 }
};

#define ENTRIES_OF_U_BAPIRET2 (sizeofR(Description_U_BAPIRET2)/sizeofR((Description_U_BAPIRET2)[0]))

RFC_STRUCT_TYPE_ID STRUCT_ID_BAPIRET2 = {
0x47, 0xb3, 0xe9, 0xf9, 0x1d, 0x79, 0x0e, 0x2a,
0xe1, 0x00, 0x00, 0x00, 0x0a, 0x15, 0x72, 0x98} ;

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_U_STRUCTURE
 * #include "bapiret2.h"
 *
 * RFC_RC rc = RfcInstallUnicodeStructure( cU("BAPIRET2"),
 *                                  Description_U_BAPIRET2,
 *                                  ENTRIES_OF_U_BAPIRET2,
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

static RFC_TYPE_ELEMENT Description_BAPIRET2[] =
{ 
{ cU("Type"),	 BAPIRET2_TYPE_TYP, BAPIRET2_TYPE_LN, 0 },
{ cU("Id"),	 BAPIRET2_ID_TYP, BAPIRET2_ID_LN, 0 },
{ cU("Number"),	 BAPIRET2_NUMBER_TYP, BAPIRET2_NUMBER_LN, 0 },
{ cU("Message"),	 BAPIRET2_MESSAGE_TYP, BAPIRET2_MESSAGE_LN, 0 },
{ cU("Log_no"),	 BAPIRET2_LOG_NO_TYP, BAPIRET2_LOG_NO_LN, 0 },
{ cU("Log_msg_no"),	 BAPIRET2_LOG_MSG_NO_TYP, BAPIRET2_LOG_MSG_NO_LN, 0 },
{ cU("Message_v1"),	 BAPIRET2_MESSAGE_V1_TYP, BAPIRET2_MESSAGE_V1_LN, 0 },
{ cU("Message_v2"),	 BAPIRET2_MESSAGE_V2_TYP, BAPIRET2_MESSAGE_V2_LN, 0 },
{ cU("Message_v3"),	 BAPIRET2_MESSAGE_V3_TYP, BAPIRET2_MESSAGE_V3_LN, 0 },
{ cU("Message_v4"),	 BAPIRET2_MESSAGE_V4_TYP, BAPIRET2_MESSAGE_V4_LN, 0 },
{ cU("Parameter"),	 BAPIRET2_PARAMETER_TYP, BAPIRET2_PARAMETER_LN, 0 },
{ cU("Row"),	 BAPIRET2_ROW_TYP, BAPIRET2_ROW_LN, 0 },
{ cU("Field"),	 BAPIRET2_FIELD_TYP, BAPIRET2_FIELD_LN, 0 },
{ cU("System"),	 BAPIRET2_SYSTEM_TYP, BAPIRET2_SYSTEM_LN, 0 }
};

#define ENTRIES_OF_BAPIRET2 (sizeofR(Description_BAPIRET2)/sizeofR((Description_BAPIRET2)[0]))

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_STRUCTURE
 * #include "bapiret2.h"
 *
 * RFC_RC rc = RfcInstallStructure( cU("BAPIRET2"),
 *                                  Description_BAPIRET2,
 *                                  ENTRIES_OF_BAPIRET2,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_STRUCTURE */


#ifdef RFC_DEFINE_TYPE
#ifdef RFC_DEFINE_STRUCTURE
#ifdef __cplusplus
  // define structure id
  const GUID DDICID_BAPIRET2 = {0x47b3e9f9L,(USHORT) 0x1d79,(USHORT) 0x0e2a, 
                           {0xe1,0x00,0x00,0x00,0x0a,0x15,0x72,0x98}};
  
  // define instance of (template) class describing the type.
  BAPIRET2_TYPE BAPIRET2_Type ( "BAPIRET2", Description_BAPIRET2, ENTRIES_OF_BAPIRET2, DDICID_BAPIRET2 );
#endif /* C++ */
#endif /* RFC_DEFINE_STRUCTURE */
#endif /* RFC_DEFINE_TYPE */

