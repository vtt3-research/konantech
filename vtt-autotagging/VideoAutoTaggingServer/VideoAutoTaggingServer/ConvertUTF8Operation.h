#pragma once

#include <atlstr.h>

#define MAX_UTF8_BUFFER_SIZE 50000

class CConvertUTF8Operation
{
public:
	static char* ConvertStringToUTF8(const CString strValue, int* pnByteCount = NULL);
	static void ConvertUTF8ToString(char* pUTF8Buffer, CString& strDstValue);

	static char* ConvertStringToAnsi(const CString strValue, int* pnByteCount = NULL);
	static void ConvertAnsiToString(char* pAnsiBuffer, CString& strDstValue);

	// Check for valid UTF-8 string. Code taken from the examples in RFC 2640
	static BOOL UTF8_Validation(const unsigned char *buf, unsigned int len);
};