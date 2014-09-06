/*@(#)BAPITID $Id$ SAP */
/*
 *   BAPITID
 *
 *
 * File generated from SAP Data Dictionary
 * SAP system:.. CET
 * SAP release:. 701 
 * Date:........ Thu Sep 30 18:46:44 2010
 *
 *
 * # Of fields:.................. 7
 * Size in 1 byte  per SAP_UC:... 84
 * Size in 2 bytes per SAP_UC:... 168
 * Size in 4 bytes per SAP_UC:... 336
 *
 */ 

#ifndef BAPITID_H
#define BAPITID_H

typedef struct BAPITID
{
	SAP_CHAR	Mtsysid [8];
#define BAPITID_MTSYSID_LN  	CCQ_CAST(intR)(8 * sizeofR(SAP_CHAR))
#define BAPITID_MTSYSID_CCNT	CCQ_CAST(intU)8
#define BAPITID_MTSYSID_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Mtmcname [40];
#define BAPITID_MTMCNAME_LN  	CCQ_CAST(intR)(40 * sizeofR(SAP_CHAR))
#define BAPITID_MTMCNAME_CCNT	CCQ_CAST(intU)40
#define BAPITID_MTMCNAME_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Mtnumrange [3];
#define BAPITID_MTNUMRANGE_LN  	CCQ_CAST(intR)(3 * sizeofR(SAP_CHAR))
#define BAPITID_MTNUMRANGE_CCNT	CCQ_CAST(intU)3
#define BAPITID_MTNUMRANGE_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Mtuid [10];
#define BAPITID_MTUID_LN  	CCQ_CAST(intR)(10 * sizeofR(SAP_CHAR))
#define BAPITID_MTUID_CCNT	CCQ_CAST(intU)10
#define BAPITID_MTUID_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Mtclass [3];
#define BAPITID_MTCLASS_LN  	CCQ_CAST(intR)(3 * sizeofR(SAP_CHAR))
#define BAPITID_MTCLASS_CCNT	CCQ_CAST(intU)3
#define BAPITID_MTCLASS_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Mtindex [10];
#define BAPITID_MTINDEX_LN  	CCQ_CAST(intR)(10 * sizeofR(SAP_CHAR))
#define BAPITID_MTINDEX_CCNT	CCQ_CAST(intU)10
#define BAPITID_MTINDEX_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Extindex [10];
#define BAPITID_EXTINDEX_LN  	CCQ_CAST(intR)(10 * sizeofR(SAP_CHAR))
#define BAPITID_EXTINDEX_CCNT	CCQ_CAST(intU)10
#define BAPITID_EXTINDEX_TYP 	RFCTYPE_CHAR

}
BAPITID;

/* Warning : Structure may need 4-bytes alignment. */

#define BAPITID_LN 	sizeofR( BAPITID )

#ifdef RFC_DEFINE_TYPE
#ifdef __cplusplus
  // define the (template) class for the actual type
  // and declare the one instance of the type describing class.

  typedef CAbapType< BAPITID > BAPITID_TYPE;
  extern BAPITID_TYPE BAPITID_Type;
  typedef CAbapTable< BAPITID > BAPITID_TABLE;

#endif /* C++ */
#endif /* RFC_DEFINE_TYPE */


#endif  /* BAPITID_H */

#ifdef RFC_DEFINE_U_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this on both UNICODE and non-UNICODE Programs
 * 
 */

static RFC_UNICODE_TYPE_ELEMENT Description_U_BAPITID[] =
{ 
{ cU("Mtsysid"),	 BAPITID_MTSYSID_TYP,	 0,   8,   0,  16,   0,  32,   0 },
{ cU("Mtmcname"),	 BAPITID_MTMCNAME_TYP,	 0,  40,   8,  80,  16, 160,  32 },
{ cU("Mtnumrange"),	 BAPITID_MTNUMRANGE_TYP,	 0,   3,  48,   6,  96,  12, 192 },
{ cU("Mtuid"),	 BAPITID_MTUID_TYP,	 0,  10,  51,  20, 102,  40, 204 },
{ cU("Mtclass"),	 BAPITID_MTCLASS_TYP,	 0,   3,  61,   6, 122,  12, 244 },
{ cU("Mtindex"),	 BAPITID_MTINDEX_TYP,	 0,  10,  64,  20, 128,  40, 256 },
{ cU("Extindex"),	 BAPITID_EXTINDEX_TYP,	 0,  10,  74,  20, 148,  40, 296 }
};

#define ENTRIES_OF_U_BAPITID (sizeofR(Description_U_BAPITID)/sizeofR((Description_U_BAPITID)[0]))

RFC_STRUCT_TYPE_ID STRUCT_ID_BAPITID = {
0x47, 0xb4, 0xfd, 0x83, 0x69, 0x7d, 0x0e, 0x2a,
0xe1, 0x00, 0x00, 0x00, 0x0a, 0x15, 0x72, 0x98} ;

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_U_STRUCTURE
 * #include "bapitid.h"
 *
 * RFC_RC rc = RfcInstallUnicodeStructure( cU("BAPITID"),
 *                                  Description_U_BAPITID,
 *                                  ENTRIES_OF_U_BAPITID,
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

static RFC_TYPE_ELEMENT Description_BAPITID[] =
{ 
{ cU("Mtsysid"),	 BAPITID_MTSYSID_TYP, BAPITID_MTSYSID_LN, 0 },
{ cU("Mtmcname"),	 BAPITID_MTMCNAME_TYP, BAPITID_MTMCNAME_LN, 0 },
{ cU("Mtnumrange"),	 BAPITID_MTNUMRANGE_TYP, BAPITID_MTNUMRANGE_LN, 0 },
{ cU("Mtuid"),	 BAPITID_MTUID_TYP, BAPITID_MTUID_LN, 0 },
{ cU("Mtclass"),	 BAPITID_MTCLASS_TYP, BAPITID_MTCLASS_LN, 0 },
{ cU("Mtindex"),	 BAPITID_MTINDEX_TYP, BAPITID_MTINDEX_LN, 0 },
{ cU("Extindex"),	 BAPITID_EXTINDEX_TYP, BAPITID_EXTINDEX_LN, 0 }
};

#define ENTRIES_OF_BAPITID (sizeofR(Description_BAPITID)/sizeofR((Description_BAPITID)[0]))

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_STRUCTURE
 * #include "bapitid.h"
 *
 * RFC_RC rc = RfcInstallStructure( cU("BAPITID"),
 *                                  Description_BAPITID,
 *                                  ENTRIES_OF_BAPITID,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_STRUCTURE */


#ifdef RFC_DEFINE_TYPE
#ifdef RFC_DEFINE_STRUCTURE
#ifdef __cplusplus
  // define structure id
  const GUID DDICID_BAPITID = {0x47b4fd83L,(USHORT) 0x697d,(USHORT) 0x0e2a, 
                           {0xe1,0x00,0x00,0x00,0x0a,0x15,0x72,0x98}};
  
  // define instance of (template) class describing the type.
  BAPITID_TYPE BAPITID_Type ( "BAPITID", Description_BAPITID, ENTRIES_OF_BAPITID, DDICID_BAPITID );
#endif /* C++ */
#endif /* RFC_DEFINE_STRUCTURE */
#endif /* RFC_DEFINE_TYPE */

