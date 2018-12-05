#pragma once 

#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <list>
//#include "KxmlApi.h"
#include "KErrorCodeDef.h"
//#include "KXmlLibUtil.h"

#ifndef _WIN32
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <netdb.h>
#include <signal.h>
#include <dirent.h>
#include <pthread.h>
#include <stropts.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#ifndef PLATFORM_LINUX
#include <sys/mode.h>
#endif
#include <sys/types.h>
#include <sys/statvfs.h>

typedef void *LPVOID;
//	typedef int BOOL;
typedef long HWND;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef const char *LPCSTR;
typedef unsigned short USHORT;
typedef unsigned int UINT;

#ifndef _AIX_64
typedef unsigned long ULONG;
#endif

typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef int SOCKET;
typedef unsigned int WPARAM;
typedef unsigned long LPARAM;
typedef long LONG;

//#ifdef PLATFORM_AIX
typedef unsigned int DWORD;	
//#endif
#include <stdarg.h>
#else
#include <afxtempl.h>
#include <afxmt.h>
#include <afxinet.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>
#include <winsock2.h>
#include <sys/timeb.h>
#include <process.h>
#include <errno.h>
#include <DbgHelp.h>

#if (_WIN32 && _MSC_VER >= 1400)
#define strcasecmp _stricmp
#else
#define strcasecmp stricmp
#endif	

#if (_WIN32 && _MSC_VER >= 1400)
#define strncasecmp _strnicmp
#else
#define strncasecmp strnicmp
#endif	
#endif

enum EnumCompWorkReportType
{
	ENUM_COMPWORK_REPORTTYPE_SUCCESS = 1,
	ENUM_COMPWORK_REPORTTYPE_FAILURE = 2,
	ENUM_COMPWORK_REPORTTYPE_RETRY = 3,
	ENUM_COMPWORK_REPORTTYPE_POSTPONE = 4,
	ENUM_COMPWORK_REPORTTYPE_ABORT = 5,
};

//----------------------------------------------------------------------------

#define THROWERROR(err) \
{ \
	fprintf(stdout, "%s/%d[errcode: %d]\n", __FILE__, __LINE__, err );\
	fflush(stdout); \
	throw err; \
}

//----------------------------------------------------------------------------

#define PRINTERR(err) \
{ \
	fprintf(stdout, "%s/%d[errcode: %d]\n", __FILE__, __LINE__, err );\
	fflush(stdout); \
}

//----------------------------------------------------------------------------------------------

#define CALLXML(exp) \
	if ( lXmlErr == 0 ) \
{ \
	lXmlErr = exp; \
}

//----------------------------------------------------------------------------

#define DBAPICALL(func) \
	nDbErr = func; \
	if ( nDbErr != 0 ) THROWERR(nDbErr);

//----------------------------------------------------------------------------

#define TKACE_AUTONOTIFY_STATUS_PUSH			0
#define TKACE_AUTONOTIFY_STATUS_PROCESSING		1
#define TKACE_AUTONOTIFY_STATUS_COMPLETED		2

//----------------------------------------------------------------------------
#define CKVSSUTIL_CHARCAT(str1, char1) \
{ \
	int nLength = strlen(str1); \
	str1 = (char*) realloc(str1, nLength+2); \
	if ( char1 != NULL )  \
{ \
	str1[nLength] = char1; \
	str1[nLength+1] = '\0'; \
} \
}

#define CKVSSUTIL_MALLOC(var, type, size) \
{ \
	int _nSize = size; \
	int _nAllocSize = sizeof(type) * _nSize; \
	if ( _nAllocSize < 0 ) \
{ \
	var = NULL; \
} \
	else \
{ \
	var = new type [_nSize]; \
} \
}

#define CKVSSUTIL_MZALLOC(var, type, size) \
{ \
	int _nSize = size; \
	int _nAllocSize = sizeof(type) * _nSize; \
	if ( _nAllocSize < 0 ) \
{ \
	var = NULL; \
} \
	else \
{ \
	var = new type [_nSize]; \
	if ( var != NULL ) \
{ \
	memset(var, 0, _nAllocSize); \
	break; \
} \
} \
}

#define CKVSSUTIL_MDALLOC(var, type, size) \
	type* var; \
{ \
	int _nSize = size; \
	int _nAllocSize = sizeof(type) * _nSize; \
	if ( _nAllocSize < 0 ) \
{ \
	var = NULL; \
} \
	else \
{ \
	var = new type [_nSize]; \
} \
}

#define CKVSSUTIL_ALLOC(var, type) \
{ \
	var = new type; \
}

#define CKVSSUTIL_ZALLOC(var, type) \
{ \
	var = new type; \
	if ( var != NULL ) \
{ \
	memset(var, 0, sizeof(type)); \
} \
}

#define CKVSSUTIL_DALLOC(var, type) \
	type* var; \
{ \
	var = new type; \
}
#define CKVSSUTIL_FREE(a)		{ if ( a != NULL ) delete a; a = NULL; }
#define CKVSSUTIL_MFREE(a)		{ if ( a != NULL ) delete [] a; a = NULL; }
#define CKVSSUTIL_FREETHIS()	delete this;


//----------------------------------------------------------------------------

#ifdef _WIN32
#define TKACE_PATH_DELIMETER_CHAR		'\\'
#define TKACE_PATH_DELIMETER_STR		"\\"
#define TKACE_PATH_DELIMETER_BADCHAR	'/'
#define TKACE_PATH_DELIMETER_BADSTR		"/"
#define TKACE_PATH_DELIMETER_DOUBLE_STR		"\\\\"
#define TKACE_PATH_DELIMETER_DOUBLE_BADSTR		"//"
#else
#define TKACE_PATH_DELIMETER_CHAR		'/'
#define TKACE_PATH_DELIMETER_STR		"/"
#define TKACE_PATH_DELIMETER_BADCHAR	'\\'
#define TKACE_PATH_DELIMETER_BADSTR		"\\"
#define TKACE_PATH_DELIMETER_DOUBLE_STR		"//"
#define TKACE_PATH_DELIMETER_DOUBLE_BADSTR		"\\\\"
#endif

#define KEYTYPE_PRIMARY	1
#define KEYTYPE_FOREIGN	2
//----------------------------------------------------------------------------------------------

#if (_WIN32 && _MSC_VER >= 1400 ) 
#define FORMAT1(tempsize, format, arg1) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1);
#define FORMAT2(tempsize, format, arg1, arg2) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2);
#define FORMAT3(tempsize, format, arg1, arg2, arg3) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3);
#define FORMAT4(tempsize, format, arg1, arg2, arg3, arg4) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4);
#define FORMAT5(tempsize, format, arg1, arg2, arg3, arg4, arg5) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4, arg5);
#define FORMAT6(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6);
#define FORMAT7(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
#define FORMAT8(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
#define FORMAT9(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
#define FORMAT10(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
#define FORMAT11(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
#define FORMAT12(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12) strFormatTemp[0]='\0'; sprintf_s(strFormatTemp, tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11,arg12);
#else 
#define FORMAT1(tempsize, format, arg1) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1);
#define FORMAT2(tempsize, format, arg1, arg2) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2);
#define FORMAT3(tempsize, format, arg1, arg2, arg3) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3);
#define FORMAT4(tempsize, format, arg1, arg2, arg3, arg4) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4);
#define FORMAT5(tempsize, format, arg1, arg2, arg3, arg4, arg5) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4, arg5);
#define FORMAT6(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4, arg5, arg6);
#define FORMAT7(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
#define FORMAT8(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
#define FORMAT9(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
#define FORMAT10(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
#define FORMAT11(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
#define FORMAT12(tempsize, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12) strFormatTemp[0]='\0'; sprintf(strFormatTemp, format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11,arg12);
#endif 


//----------------------------------------------------------------------------------------------

#if (_WIN32 && _MSC_VER >= 1400)
#define CKVSSUTIL_STRCPY(str1, str2) \
{ \
	char*& dest = str1; \
	const char* src = str2; \
	if ( src == NULL ) dest = NULL; \
		else \
{ \
	CKVSSUTIL_MALLOC(dest, char, (int) strlen(src)+1); \
	strcpy_s(dest, (int) strlen(src)+1, src); \
} \
}
#define CKVSSUTIL_DSTRCPY(dest, str) \
	char* dest = NULL; \
{ \
	const char* src = str; \
	if ( src == NULL ) dest = NULL; \
		else \
{ \
	CKVSSUTIL_MALLOC(dest, char, (int) strlen(src)+1);\
	strcpy_s(dest, (int)strlen(src)+1, src); \
} \
}

#define CKVSSUTIL_RETURN_STRCPY(str) \
{ \
	char* dest = NULL; \
	const char* src = str; \
	if ( src == NULL ) dest = NULL; \
		else \
{ \
	CKVSSUTIL_MALLOC(dest, char, (int) strlen(src)+1); \
	strcpy_s(dest, (int) strlen(src)+1, src); \
} \
	return dest; \
}
#else
#define CKVSSUTIL_STRCPY(str1, str2) \
{ \
	char*& dest = str1; \
	const char* src = str2; \
	if ( src == NULL ) dest = NULL; \
		else \
{ \
	CKVSSUTIL_MALLOC(dest, char, (int) strlen(src)+1); \
	strcpy(dest, src); \
} \
}
#define CKVSSUTIL_DSTRCPY(dest, str) \
	char* dest = NULL; \
{ \
	const char* src = str; \
	if ( src == NULL ) dest = NULL; \
		else \
{ \
	CKVSSUTIL_MALLOC(dest, char, (int) strlen(src)+1); \
	strcpy(dest, src); \
} \
}
#define CKVSSUTIL_RETURN_STRCPY(str) \
{ \
	char* dest = NULL; \
	const char* src = str; \
	if ( src == NULL ) dest = NULL; \
		else \
{ \
	CKVSSUTIL_MALLOC(dest, char, (int) strlen(src)+1); \
	strcpy(dest, src); \
	dest[strlen(src)] = '\0'; \
} \
	return dest; \
}

#define CKVSSUTIL_MDALLOC(var, type, size) \
	type* var; \
{ \
	int _nSize = size; \
	int _nAllocSize = sizeof(type) * _nSize; \
	if ( _nAllocSize < 0 ) \
{ \
	CKVSSUtil::BookeepNegativeSizeAlloc(__FILE__, __LINE__, _nSize, _nAllocSize); \
	var = NULL; \
} \
	else \
{ \
	if ( _nAllocSize > 100000 ) CKVSSUtil::BookeepLargeArrayAlloc(__FILE__, __LINE__, _nSize, _nAllocSize); \
	for ( int _nth = 0, _nUnit = 1000 ; _nth <= 10000000 ; ) \
{ \
	var = new type [_nSize]; \
	if ( var != NULL ) break; \
	CKVSSUtil::BookeepFailedArrayAlloc(__FILE__, __LINE__, _nSize, _nAllocSize, _nth); \
	_nth += _nUnit; \
	if ( _nth == _nUnit * 10 ) _nUnit = _nth; \
} \
} \
}
#endif 