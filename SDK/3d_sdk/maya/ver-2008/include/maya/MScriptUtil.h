#ifndef _MScriptUtil
#define _MScriptUtil
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors,  which is protected by U.S. and Canadian federal copyright law 
// and by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be 
// copied or duplicated, in whole or in part, without the prior written 
// consent of Autodesk.
// 
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the following 
// disclaimer, must be included in all copies of the Software, in whole 
// or in part, and all derivative works of the Software, unless such copies 
// or derivative works are solely in the form of machine-executable object 
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, 
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO 
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST 
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR 
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS 
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES. 
// ==========================================================================
//+
//
// CLASS:    MScriptUtil
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MScriptUtil)
//
//  Utility class for working with pointers to basic types such as int, float
//	and arrays of these types.  Class is more applicable for script programming 
//	rather than C++. C++ pointers and references have no equivalent in
//	script so this class provides utility methods for working with these
//	type of parameters and return values.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

class MIntArray;
class MDoubleArray;

#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MUintArray.h>
#include <maya/MUint64Array.h>

#include <maya/MMatrix.h>
#include <maya/MFloatMatrix.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MScriptUtil)

/// Utility class for working with pointers and references. (OpenMaya) (OpenMaya.py)
/**
  General class for scripting.
*/

class OPENMAYA_EXPORT MScriptUtil
{
public:

	///
	MScriptUtil();
	///
	~MScriptUtil();

	///
	MScriptUtil( int& value );
	///
	MScriptUtil( short& value );
	///
	MScriptUtil( float& value );
	///
	MScriptUtil( double& value );
	///
	MScriptUtil( unsigned int& value );
	///
	MScriptUtil( bool& value );
	///
	MScriptUtil( MIntArray& array );
	///
	MScriptUtil( MDoubleArray& array );
	///
	MScriptUtil( MUintArray& array );
	///
	MScriptUtil( MUint64Array& array );
	
	///
	void createFromInt( int x, int y=0, int z=0, int w=0 );
	///
	void createFromDouble( double x, double y=0, double z=0, double w=0 );

	///
	bool createFromList( void *pythonList, unsigned int length );

	///
	int asInt();
	/// 
	short asShort();
	///
	float asFloat();
	///
	double asDouble();
	///
	unsigned int asUint();
	///
	bool asBool();
	
	///
	int *asIntPtr();
	/// 
	short *asShortPtr();
	///
	float *asFloatPtr();
	///
	double *asDoublePtr();
	///
	unsigned int *asUintPtr();
	///
	bool* asBoolPtr();
	///
	char *asCharPtr();
	///
	unsigned char *asUcharPtr();
	
	//
	int (*asInt2Ptr())[2];
	//
	int (*asInt3Ptr())[3];
	//
	int (*asInt4Ptr())[4];

	//
	short (*asShort2Ptr())[2];
	//
	short (*asShort3Ptr())[3];
	//
	short (*asShort4Ptr())[4];

	//
	float (*asFloat2Ptr())[2];
	//
	float (*asFloat3Ptr())[3];
	//
	float (*asFloat4Ptr())[4];

	//
	double (*asDouble2Ptr())[2];
	//
	double (*asDouble3Ptr())[3];
	//
	double (*asDouble4Ptr())[4];

	//
	unsigned int (*asUint2Ptr())[2];
	//
	unsigned int (*asUint3Ptr())[3];
	//
	unsigned int (*asUint4Ptr())[4];

	///
	static void setInt( int& var, int value );
	///
	static void setShort( short& var, int value  );
	///
	static void setFloat( float& var, double value );
	///
	static void setDouble( double& var, double value );
	///
	static void setUint( unsigned int& var, int value  );
	///
	static void setBool( bool& var, int value  );
	///
	static void setChar( char& var, int value  );
	///
	static void setUchar( unsigned char& var, int value  );
	
	///
	static int getInt( int& var );
	///
	static int getShort( short& var );
	///
	static double getFloat( float& var );
	///
	static double getDouble( double& var );
	///
	static int getUint( unsigned int& var );
	///
	static int getBool( bool& var );
	///
	static int getChar( char& var );
	///
	static int getUchar( unsigned char& var );

	///
	static bool setIntArray( int* var, unsigned int index, int value );
	///
	static bool setShortArray( short* var, unsigned int index, int value  );
	///
	static bool setFloatArray( float* var, unsigned int index, double value );
	///
	static bool setDoubleArray( double* var, unsigned int index, double value );
	///
	static bool setUintArray( unsigned int* var, unsigned int index, int value  );
	///
	static bool setBoolArray( bool* var, unsigned int index, int value  );
	///
	static bool setCharArray( char* var, unsigned int index, int value  );
	///
	static bool setUcharArray( unsigned char* var, unsigned int index, int value  );

	///
	static int getIntArrayItem( int* var, unsigned int index );
	///
	static int getShortArrayItem( short* var, unsigned int index  );
	///
	static double getFloatArrayItem( float* var, unsigned int index );
	///
	static double getDoubleArrayItem( double* var, unsigned int index );
	///
	static int getUintArrayItem( unsigned int* var, unsigned int index  );
	///
	static int getBoolArrayItem( bool* var, unsigned int index  );
	///
	static int getCharArrayItem( char* var, unsigned int index  );
	///
	static int getUcharArrayItem( unsigned char* var, unsigned int index  );
	
	///
	static bool createMatrixFromList( void *pythonList, MMatrix& matrix );
	///
	static bool createFloatMatrixFromList( void *pythonList, MFloatMatrix& matrix );
	
	///
	static bool createIntArrayFromList( void *pythonList, MIntArray& array );
	///
	static bool createFloatArrayFromList( void *pythonList, MFloatArray& array );

	///
	static int getInt2ArrayItem( int (*var)[2], unsigned int r, unsigned int c);
	///
	static bool setInt2ArrayItem( int (*var)[2], unsigned int r, unsigned int c, int value );
	///
	static int getInt3ArrayItem( int (*var)[3], unsigned int r, unsigned int c);
	///
	static bool setInt3ArrayItem( int (*var)[3], unsigned int r, unsigned int c, int value );
	///
	static int getInt4ArrayItem( int (*var)[4], unsigned int r, unsigned int c);
	///
	static bool setInt4ArrayItem( int (*var)[4], unsigned int r, unsigned int c, int value );

	///
	static short getShort2ArrayItem( short (*var)[2], unsigned int r, unsigned int c);
	///
	static bool setShort2ArrayItem( short (*var)[2], unsigned int r, unsigned int c, short value );
	///
	static short getShort3ArrayItem( short (*var)[3], unsigned int r, unsigned int c);
	///
	static bool setShort3ArrayItem( short (*var)[3], unsigned int r, unsigned int c, short value );
	///
	static short getShort4ArrayItem( short (*var)[4], unsigned int r, unsigned int c);
	///
	static bool setShort4ArrayItem( short (*var)[4], unsigned int r, unsigned int c, short value );

	///
	static float getFloat2ArrayItem( float (*var)[2], unsigned int r, unsigned int c);
	///
	static bool setFloat2ArrayItem( float (*var)[2], unsigned int r, unsigned int c, float value );
	///
	static float getFloat3ArrayItem( float (*var)[3], unsigned int r, unsigned int c);
	///
	static bool setFloat3ArrayItem( float (*var)[3], unsigned int r, unsigned int c, float value );
	///
	static float getFloat4ArrayItem( float (*var)[4], unsigned int r, unsigned int c);
	///
	static bool setFloat4ArrayItem( float (*var)[4], unsigned int r, unsigned int c, float value );

	///
	static double getDouble2ArrayItem( double (*var)[2], unsigned int r, unsigned int c);
	///
	static bool setDouble2ArrayItem( double (*var)[2], unsigned int r, unsigned int c, double value );
	///
	static double getDouble3ArrayItem( double (*var)[3], unsigned int r, unsigned int c);
	///
	static bool setDouble3ArrayItem( double (*var)[3], unsigned int r, unsigned int c, double value );
	///
	static double getDouble4ArrayItem( double (*var)[4], unsigned int r, unsigned int c);
	///
	static bool setDouble4ArrayItem( double (*var)[4], unsigned int r, unsigned int c, double value );
	
	///
	static unsigned int getUint2ArrayItem( unsigned int (*var)[2], unsigned int r, unsigned int c);
	///
	static bool setUint2ArrayItem( unsigned int (*var)[2], unsigned int r, unsigned int c, unsigned int value );
	///
	static unsigned int getUint3ArrayItem( unsigned int (*var)[3], unsigned int r, unsigned int c);
	///
	static bool setUint3ArrayItem( unsigned int (*var)[3], unsigned int r, unsigned int c, unsigned int value );
	///
	static unsigned int getUint4ArrayItem( unsigned int (*var)[4], unsigned int r, unsigned int c);
	///
	static bool setUint4ArrayItem( unsigned int (*var)[4], unsigned int r, unsigned int c, unsigned int value );
		
protected:
// No protected members		
private: 
	MDoubleArray _data;
	MUint64Array _data64;
	char* _ptr;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MScriptUtil */
