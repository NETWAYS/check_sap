/*@(#)BAPIALDATA $Id$ SAP */
/*
 *   BAPIALDATA
 *
 *
 * File generated from SAP Data Dictionary
 * SAP system:.. CET
 * SAP release:. 701 
 * Date:........ Sat Oct  2 17:49:20 2010
 *
 *
 * # Of fields:.................. 2
 * Size in 1 byte  per SAP_UC:... 8
 * Size in 2 bytes per SAP_UC:... 8
 * Size in 4 bytes per SAP_UC:... 8
 *
 */ 

#ifndef BAPIALDATA_H
#define BAPIALDATA_H

typedef struct BAPIALDATA
{
	SAP_INT	Value;
#define BAPIALDATA_VALUE_LN  	CCQ_CAST(intR)4
#define BAPIALDATA_VALUE_TYP 	RFCTYPE_INT

	SAP_INT	Severity;
#define BAPIALDATA_SEVERITY_LN  	CCQ_CAST(intR)4
#define BAPIALDATA_SEVERITY_TYP 	RFCTYPE_INT

}
BAPIALDATA;

/* Warning : Structure may need 4-bytes alignment. */

#define BAPIALDATA_LN 	sizeofR( BAPIALDATA )

#ifdef RFC_DEFINE_TYPE
#ifdef __cplusplus
  // define the (template) class for the actual type
  // and declare the one instance of the type describing class.

  typedef CAbapType< BAPIALDATA > BAPIALDATA_TYPE;
  extern BAPIALDATA_TYPE BAPIALDATA_Type;
  typedef CAbapTable< BAPIALDATA > BAPIALDATA_TABLE;

#endif /* C++ */
#endif /* RFC_DEFINE_TYPE */


#endif  /* BAPIALDATA_H */

#ifdef RFC_DEFINE_U_STRUCTURE
/* set this to generate also structure information for
 * RFC programs.
 * 
 * You can use this on both UNICODE and non-UNICODE Programs
 * 
 */

static RFC_UNICODE_TYPE_ELEMENT Description_U_BAPIALDATA[] =
{ 
{ cU("Value"),	 BAPIALDATA_VALUE_TYP,	 0,   4,   0,   4,   0,   4,   0 },
{ cU("Severity"),	 BAPIALDATA_SEVERITY_TYP,	 0,   4,   4,   4,   4,   4,   4 }
};

#define ENTRIES_OF_U_BAPIALDATA (sizeofR(Description_U_BAPIALDATA)/sizeofR((Description_U_BAPIALDATA)[0]))

RFC_STRUCT_TYPE_ID STRUCT_ID_BAPIALDATA = {
0x47, 0xb4, 0x02, 0xb6, 0x69, 0x7d, 0x0e, 0x2a,
0xe1, 0x00, 0x00, 0x00, 0x0a, 0x15, 0x72, 0x98} ;

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_U_STRUCTURE
 * #include "bapialdata.h"
 *
 * RFC_RC rc = RfcInstallUnicodeStructure( cU("BAPIALDATA"),
 *                                  Description_U_BAPIALDATA,
 *                                  ENTRIES_OF_U_BAPIALDATA,
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

static RFC_TYPE_ELEMENT Description_BAPIALDATA[] =
{ 
{ cU("Value"),	 BAPIALDATA_VALUE_TYP, BAPIALDATA_VALUE_LN, 0 },
{ cU("Severity"),	 BAPIALDATA_SEVERITY_TYP, BAPIALDATA_SEVERITY_LN, 0 }
};

#define ENTRIES_OF_BAPIALDATA (sizeofR(Description_BAPIALDATA)/sizeofR((Description_BAPIALDATA)[0]))

/*
 * install structure in RFC program by
 *
 * #define RFC_DEFINE_STRUCTURE
 * #include "bapialdata.h"
 *
 * RFC_RC rc = RfcInstallStructure( cU("BAPIALDATA"),
 *                                  Description_BAPIALDATA,
 *                                  ENTRIES_OF_BAPIALDATA,
 *                                  &typeHandle );
 */

#endif  /* RFC_DEFINE_STRUCTURE */


#ifdef RFC_DEFINE_TYPE
#ifdef RFC_DEFINE_STRUCTURE
#ifdef __cplusplus
  // define structure id
  const GUID DDICID_BAPIALDATA = {0x47b402b6L,(USHORT) 0x697d,(USHORT) 0x0e2a, 
                           {0xe1,0x00,0x00,0x00,0x0a,0x15,0x72,0x98}};
  
  // define instance of (template) class describing the type.
  BAPIALDATA_TYPE BAPIALDATA_Type ( "BAPIALDATA", Description_BAPIALDATA, ENTRIES_OF_BAPIALDATA, DDICID_BAPIALDATA );
#endif /* C++ */
#endif /* RFC_DEFINE_STRUCTURE */
#endif /* RFC_DEFINE_TYPE */

