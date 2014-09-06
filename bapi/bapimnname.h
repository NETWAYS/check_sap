/*@(#)BAPIMNNAME $Id$ SAP */
/*
 *   BAPIMNNAME
 *
 *
 * File generated from SAP Data Dictionary
 * SAP system:.. CET
 * SAP release:. 701 
 * Date:........ Thu Sep 30 18:47:40 2010
 *
 *
 * # Of fields:.................. 2
 * Size in 1 byte  per SAP_UC:... 120
 * Size in 2 bytes per SAP_UC:... 240
 * Size in 4 bytes per SAP_UC:... 480
 *
 */ 

#ifndef BAPIMNNAME_H
#define BAPIMNNAME_H

typedef struct BAPIMNNAME
{
	SAP_CHAR	Moni_name [60];
#define BAPIMNNAME_MONI_NAME_LN  	CCQ_CAST(intR)(60 * sizeofR(SAP_CHAR))
#define BAPIMNNAME_MONI_NAME_CCNT	CCQ_CAST(intU)60
#define BAPIMNNAME_MONI_NAME_TYP 	RFCTYPE_CHAR

	SAP_CHAR	Ms_name [60];
#define BAPIMNNAME_MS_NAME_LN  	CCQ_CAST(intR)(60 * sizeofR(SAP_CHAR))
#define BAPIMNNAME_MS_NAME_CCNT	CCQ_CAST(intU)60
#define BAPIMNNAME_MS_NAME_TYP 	RFCTYPE_CHAR

}
BAPIMNNAME;

/* Warning : Structure may need 4-bytes alignment. */

#define BAPIMNNAME_LN 	sizeofR( BAPIMNNAME )

#ifdef RFC_DEFINE_TYPE
#ifdef __cplusplus
  // define the (template) class for the actual type
  // and declare the one instance of the type describing class.

  typedef CAbapType< BAPIMNNAME > BAPIMNNAME_TYPE;
  extern BAPIMNNAME_TYPE BAPIMNNAME_Type;
  typedef CAbapTable< BAPIMNNAME > BAPIMNNAME_TABLE;

#endif /* C++ */
#endif /* RFC_DEFINE_TYPE */


#endif  /* BAPIMNNAME_H */

#ifdef RFC_DEFINE_U_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this on both UNICODE and non-UNICODE Programs
 * 
 */

static RFC_UNICODE_TYPE_ELEMENT Description_U_BAPIMNNAME[] =
{ 
{ cU("Moni_name"),	 BAPIMNNAME_MONI_NAME_TYP,	 0,  60,   0, 120,   0, 240,   0 },
{ cU("Ms_name"),	 BAPIMNNAME_MS_NAME_TYP,	 0,  60,  60, 120, 120, 240, 240 }
};

#define ENTRIES_OF_U_BAPIMNNAME (sizeofR(Description_U_BAPIMNNAME)/sizeofR((Description_U_BAPIMNNAME)[0]))

RFC_STRUCT_TYPE_ID STRUCT_ID_BAPIMNNAME = {
0x47, 0xb4, 0xfe, 0xfb, 0x69, 0x7d, 0x0e, 0x2a,
0xe1, 0x00, 0x00, 0x00, 0x0a, 0x15, 0x72, 0x98} ;

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_U_STRUCTURE
 * #include "bapimnname.h"
 *
 * RFC_RC rc = RfcInstallUnicodeStructure( cU("BAPIMNNAME"),
 *                                  Description_U_BAPIMNNAME,
 *                                  ENTRIES_OF_U_BAPIMNNAME,
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

static RFC_TYPE_ELEMENT Description_BAPIMNNAME[] =
{ 
{ cU("Moni_name"),	 BAPIMNNAME_MONI_NAME_TYP, BAPIMNNAME_MONI_NAME_LN, 0 },
{ cU("Ms_name"),	 BAPIMNNAME_MS_NAME_TYP, BAPIMNNAME_MS_NAME_LN, 0 }
};

#define ENTRIES_OF_BAPIMNNAME (sizeofR(Description_BAPIMNNAME)/sizeofR((Description_BAPIMNNAME)[0]))

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_STRUCTURE
 * #include "bapimnname.h"
 *
 * RFC_RC rc = RfcInstallStructure( cU("BAPIMNNAME"),
 *                                  Description_BAPIMNNAME,
 *                                  ENTRIES_OF_BAPIMNNAME,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_STRUCTURE */


#ifdef RFC_DEFINE_TYPE
#ifdef RFC_DEFINE_STRUCTURE
#ifdef __cplusplus
  // define structure id
  const GUID DDICID_BAPIMNNAME = {0x47b4fefbL,(USHORT) 0x697d,(USHORT) 0x0e2a, 
                           {0xe1,0x00,0x00,0x00,0x0a,0x15,0x72,0x98}};
  
  // define instance of (template) class describing the type.
  BAPIMNNAME_TYPE BAPIMNNAME_Type ( "BAPIMNNAME", Description_BAPIMNNAME, ENTRIES_OF_BAPIMNNAME, DDICID_BAPIMNNAME );
#endif /* C++ */
#endif /* RFC_DEFINE_STRUCTURE */
#endif /* RFC_DEFINE_TYPE */

