/*@(#)BAPIMSNAME $Id$ SAP */
/*
 *   BAPIMSNAME
 *
 *
 * File generated from SAP Data Dictionary
 * SAP system:.. CET
 * SAP release:. 701 
 * Date:........ Thu Sep 30 19:25:12 2010
 *
 *
 * # Of fields:.................. 1
 * Size in 1 byte  per SAP_UC:... 60
 * Size in 2 bytes per SAP_UC:... 120
 * Size in 4 bytes per SAP_UC:... 240
 *
 */ 

#ifndef BAPIMSNAME_H
#define BAPIMSNAME_H

typedef struct BAPIMSNAME
{
	SAP_CHAR	Name [60];
#define BAPIMSNAME_NAME_LN  	CCQ_CAST(intR)(60 * sizeofR(SAP_CHAR))
#define BAPIMSNAME_NAME_CCNT	CCQ_CAST(intU)60
#define BAPIMSNAME_NAME_TYP 	RFCTYPE_CHAR

}
BAPIMSNAME;

/* Warning : Structure may need 4-bytes alignment. */

#define BAPIMSNAME_LN 	sizeofR( BAPIMSNAME )

#ifdef RFC_DEFINE_TYPE
#ifdef __cplusplus
  // define the (template) class for the actual type
  // and declare the one instance of the type describing class.

  typedef CAbapType< BAPIMSNAME > BAPIMSNAME_TYPE;
  extern BAPIMSNAME_TYPE BAPIMSNAME_Type;
  typedef CAbapTable< BAPIMSNAME > BAPIMSNAME_TABLE;

#endif /* C++ */
#endif /* RFC_DEFINE_TYPE */


#endif  /* BAPIMSNAME_H */

#ifdef RFC_DEFINE_U_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this on both UNICODE and non-UNICODE Programs
 * 
 */

static RFC_UNICODE_TYPE_ELEMENT Description_U_BAPIMSNAME[] =
{ 
{ cU("Name"),	 BAPIMSNAME_NAME_TYP,	 0,  60,   0, 120,   0, 240,   0 }
};

#define ENTRIES_OF_U_BAPIMSNAME (sizeofR(Description_U_BAPIMSNAME)/sizeofR((Description_U_BAPIMSNAME)[0]))

RFC_STRUCT_TYPE_ID STRUCT_ID_BAPIMSNAME = {
0x47, 0xb4, 0xfe, 0xf3, 0x69, 0x7d, 0x0e, 0x2a,
0xe1, 0x00, 0x00, 0x00, 0x0a, 0x15, 0x72, 0x98} ;

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_U_STRUCTURE
 * #include "bapimsname.h"
 *
 * RFC_RC rc = RfcInstallUnicodeStructure( cU("BAPIMSNAME"),
 *                                  Description_U_BAPIMSNAME,
 *                                  ENTRIES_OF_U_BAPIMSNAME,
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

static RFC_TYPE_ELEMENT Description_BAPIMSNAME[] =
{ 
{ cU("Name"),	 BAPIMSNAME_NAME_TYP, BAPIMSNAME_NAME_LN, 0 }
};

#define ENTRIES_OF_BAPIMSNAME (sizeofR(Description_BAPIMSNAME)/sizeofR((Description_BAPIMSNAME)[0]))

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_STRUCTURE
 * #include "bapimsname.h"
 *
 * RFC_RC rc = RfcInstallStructure( cU("BAPIMSNAME"),
 *                                  Description_BAPIMSNAME,
 *                                  ENTRIES_OF_BAPIMSNAME,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_STRUCTURE */


#ifdef RFC_DEFINE_TYPE
#ifdef RFC_DEFINE_STRUCTURE
#ifdef __cplusplus
  // define structure id
  const GUID DDICID_BAPIMSNAME = {0x47b4fef3L,(USHORT) 0x697d,(USHORT) 0x0e2a, 
                           {0xe1,0x00,0x00,0x00,0x0a,0x15,0x72,0x98}};
  
  // define instance of (template) class describing the type.
  BAPIMSNAME_TYPE BAPIMSNAME_Type ( "BAPIMSNAME", Description_BAPIMSNAME, ENTRIES_OF_BAPIMSNAME, DDICID_BAPIMSNAME );
#endif /* C++ */
#endif /* RFC_DEFINE_STRUCTURE */
#endif /* RFC_DEFINE_TYPE */

