#include "stdafx.h"
#include "Utility.h"

#include <atlstr.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <direct.h>
#include <tchar.h>

#define MAX_UTF8_BUFFER_SIZE 50000

#define BAD     -1
#define DECODE64(c)  (isascii(c) ? base64val[c] : BAD)

static const char base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char base64val[128];

#pragma warning (disable : 4996)

namespace Utility
{
	void Tokenize(const string& str,
		vector<string>& tokens,
		const string& delimiters)
	{
		// 맨 첫 글자가 구분자인 경우 무시
		string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// 구분자가 아닌 첫 글자를 찾는다
		string::size_type pos = str.find_first_of(delimiters, lastPos);

		while (string::npos != pos || string::npos != lastPos)
		{
			// token을 찾았으니 vector에 추가한다
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// 구분자를 뛰어넘는다.  "not_of"에 주의하라
			lastPos = str.find_first_not_of(delimiters, pos);
			// 다음 구분자가 아닌 글자를 찾는다
			pos = str.find_first_of(delimiters, lastPos);
		}
	}

	string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
		if (from.empty())
			return str;

		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}

	string string_format(const string fmt, ...) {
		int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
		string str;
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
			va_end(ap);
			if (n > -1 && n < size) {  // Everything worked
				str.resize(n);
				return str;
			}
			if (n > -1)  // Needed size returned
				size = n + 1;   // For null char
			else
				size *= 2;      // Guess at a larger size (OS specific)
		}
		return str;
	}

	void encode_base64(unsigned char *out, unsigned char *in, int inlen)
	{
		for (; inlen >= 3; inlen -= 3)                                      //3바이트를 6비트 단위로 끊어서 4바이트로 만듬.
		{
			*out++ = base64digits[in[0] >> 2];                              //0번째 앞6자리
			*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];    //0번째 뒤2자리 | 1번째 앞4자리
			*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];    //1번째 뒤4자리 | 2번째 앞2자리
			*out++ = base64digits[in[2] & 0x3f];                            //2번째 뒤6자리
			in += 3;
		}

		if (inlen > 0)
		{
			unsigned char fragment;

			*out++ = base64digits[in[0] >> 2];
			fragment = (in[0] << 4) & 0x30;

			if (inlen > 1)  fragment |= in[1] >> 4;

			*out++ = base64digits[fragment];
			*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
			*out++ = '=';
		}
		*out = '\0';
	}

	int decode_base64(unsigned char *out, unsigned char *in)
	{
		int len = 0;
		unsigned char digit1, digit2, digit3, digit4;

		if (in[0] == '+' && in[1] == ' ')   in += 2;
		if (*in == 'r') return(0);

		do {
			digit1 = in[0];

			if (DECODE64(digit1) == BAD)    return(-1);

			digit2 = in[1];

			if (DECODE64(digit2) == BAD)    return(-1);

			digit3 = in[2];

			if (digit3 != '=' && DECODE64(digit3) == BAD)   return(-1);

			digit4 = in[3];

			if (digit4 != '=' && DECODE64(digit4) == BAD)   return(-1);

			in += 4;
			*out++ = (DECODE64(digit1) << 2) | (DECODE64(digit2) >> 4);
			++len;

			if (digit3 != '=')
			{
				*out++ = ((DECODE64(digit2) << 4) & 0xf0) | (DECODE64(digit3) >> 2);
				++len;

				if (digit4 != '=')
				{
					*out++ = ((DECODE64(digit3) << 6) & 0xc0) | DECODE64(digit4);
					++len;
				}
			}

		} while	(*in && *in != 'r' && digit4 != '=');

		return (len);
	}

	BOOL IsFile(LPCTSTR lpszPathName)
	{
		struct _tstati64 stati64;
		int nRet = _tstati64(lpszPathName, &stati64);
		if (nRet == 0)
		{
			if (stati64.st_mode & _S_IFREG)
			{
				if (!(stati64.st_mode & _S_IFDIR))
					return TRUE;
			}
		}

		return FALSE;
	}

	char* CConvertUTF8Operation::ConvertStringToUTF8(const CString strValue, int* pnByteCount)
	{
		char* pcRet = NULL;

		size_t srcLen = strValue.GetLength();
		size_t bufSize = 0;

#ifdef _UNICODE
		int nByteLenth = ((int)srcLen + 1) * 3;

		char* pBuffer = new char[nByteLenth + 1];

		memset(pBuffer, 0, (sizeof(char)* (nByteLenth + 1)));

		int nByteCounts = static_cast<int>(sizeof(char)* (nByteLenth));
		int nCount = WideCharToMultiByte(CP_UTF8, 0, strValue, -1, pBuffer, nByteCounts, NULL, NULL);

		bufSize = nCount;
		pcRet = new char[bufSize];
		memset(pcRet, 0, bufSize);
		memcpy_s(pcRet, bufSize, pBuffer, bufSize);

		delete[]pBuffer;
#else
		int nSizeUTF16 = ::MultiByteToWideChar(CP_ACP, 0, strValue, (int)srcLen, NULL, 0);
		BSTR bstrString = ::SysAllocStringLen(NULL, nSizeUTF16);
		::MultiByteToWideChar(CP_ACP, 0, strValue, (int)srcLen, bstrString, nSizeUTF16);
		int nSizeUTF8 = ::WideCharToMultiByte(CP_UTF8, 0, bstrString, nSizeUTF16, NULL, NULL, NULL, NULL);
		BYTE* pStrUTF8 = new BYTE[nSizeUTF8 + 1];
		memset(pStrUTF8, 0, nSizeUTF8 + 1);
		::WideCharToMultiByte(CP_UTF8, 0, bstrString, nSizeUTF16, (char*)pStrUTF8, nSizeUTF8, NULL, NULL);
		::SysFreeString(bstrString);

		bufSize = nSizeUTF8;
		pcRet = (char*)pStrUTF8;
#endif

		if (pnByteCount)
		{
			*pnByteCount = (int)bufSize;
		}

		return pcRet;
	}

	void CConvertUTF8Operation::ConvertUTF8ToString(char* pUTF8Buffer, CString& strDstValue)
	{
		CStringA strATemp;
		strATemp.Format("%s", pUTF8Buffer);
		int nSizeUTF8 = strATemp.GetLength() + 1;
		int nSizeUTF16 = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8Buffer, nSizeUTF8, NULL, 0);

#ifdef _UNICODE
		::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8Buffer, nSizeUTF8, strDstValue.GetBuffer(nSizeUTF16), nSizeUTF16);
		strDstValue.ReleaseBuffer();
#else
		BSTR bstrString = ::SysAllocStringLen(NULL, nSizeUTF16);
		::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8Buffer, nSizeUTF8, bstrString, nSizeUTF16);
		int nStringLen = ::WideCharToMultiByte(CP_ACP, 0, bstrString, nSizeUTF16, NULL, 0, NULL, NULL);
		::WideCharToMultiByte(CP_ACP, 0, bstrString, nSizeUTF16, strDstValue.GetBuffer(nStringLen), nStringLen, NULL, NULL);
		strDstValue.ReleaseBuffer();
		::SysFreeString(bstrString);
#endif
	}

	char* CConvertUTF8Operation::ConvertStringToAnsi(const CString strValue, int* pnByteCount)
	{
		char* pcRet = NULL;

		size_t srcLen = strValue.GetLength();

#ifdef _UNICODE
		int nByteLenth = ((int)srcLen + 1) * 3;

		char* pBuffer = new char[nByteLenth + 1];

		memset(pBuffer, 0, (sizeof(char)* (nByteLenth + 1)));

		int nByteCounts = static_cast<int>(sizeof(char)* (nByteLenth));

		int nCount = WideCharToMultiByte(CP_ACP, 0, strValue, -1, pBuffer, nByteCounts, NULL, NULL);

		size_t bufSize = nCount;

		pcRet = new char[bufSize];

		memset(pcRet, 0, bufSize);

		memcpy_s(pcRet, bufSize, pBuffer, bufSize);

		delete[]pBuffer;
#else
		size_t bufSize = srcLen;

		pcRet = new char[bufSize + 1];

		memset(pcRet, 0, bufSize + 1);

		memcpy(pcRet, (LPCSTR)strValue, bufSize);
#endif

		if (pnByteCount)
		{
			*pnByteCount = (int)bufSize;
		}

		return pcRet;
	}

	void CConvertUTF8Operation::ConvertAnsiToString(char* pAnsiBuffer, CString& strDstValue)
	{
		CStringA strATemp;
		strATemp.Format("%s", pAnsiBuffer);

		int nSizeAnsi = strATemp.GetLength() + 1;
		int nSizeUTF16 = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pAnsiBuffer, nSizeAnsi, NULL, 0);

#ifdef _UNICODE
		::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pAnsiBuffer, nSizeAnsi, strDstValue.GetBuffer(nSizeUTF16), nSizeUTF16);
		strDstValue.ReleaseBuffer();
#else
		BSTR bstrString = ::SysAllocStringLen(NULL, nSizeUTF16);
		::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pAnsiBuffer, nSizeAnsi, bstrString, nSizeUTF16);
		int nStringLen = ::WideCharToMultiByte(CP_ACP, 0, bstrString, nSizeUTF16, NULL, 0, NULL, NULL);
		::WideCharToMultiByte(CP_ACP, 0, bstrString, nSizeUTF16, strDstValue.GetBuffer(nStringLen), nStringLen, NULL, NULL);
		strDstValue.ReleaseBuffer();
		::SysFreeString(bstrString);
#endif
	}

	// Check for valid UTF-8 string. Code taken from the examples in RFC 2640
	BOOL CConvertUTF8Operation::UTF8_Validation(const unsigned char *buf, unsigned int len)
	{
		const unsigned char *endbuf = buf + len;
		unsigned char byte2mask = 0x00, c;
		int trailing = 0;  // trailing (continuation) bytes to follow

		while (buf != endbuf)
		{
			c = *buf++;
			if (trailing)
			if ((c & 0xC0) == 0x80)  // Does trailing byte follow UTF-8 format?
			{
				if (byte2mask)        // Need to check 2nd byte for proper range?
				if (c&byte2mask)     // Are appropriate bits set?
					byte2mask = 0x00;
				else
					return 0;
				trailing--;
			}
			else
				return 0;
			else
			if ((c & 0x80) == 0x00)  continue;      // valid 1 byte UTF-8
			else if ((c & 0xE0) == 0xC0)            // valid 2 byte UTF-8
			if (c & 0x1E)                     // Is UTF-8 byte in
				// proper range?
				trailing = 1;
			else
				return 0;
			else if ((c & 0xF0) == 0xE0)           // valid 3 byte UTF-8
			{
				if (!(c & 0x0F))                // Is UTF-8 byte in
					// proper range?
					byte2mask = 0x20;              // If not set mask
				// to check next byte
				trailing = 2;
			}
			else if ((c & 0xF8) == 0xF0)           // valid 4 byte UTF-8
			{
				if (!(c & 0x07))                // Is UTF-8 byte in
					// proper range?

					byte2mask = 0x30;              // If not set mask
				// to check next byte
				trailing = 3;
			}
			else if ((c & 0xFC) == 0xF8)           // valid 5 byte UTF-8
			{
				if (!(c & 0x03))                // Is UTF-8 byte in
					// proper range?
					byte2mask = 0x38;              // If not set mask
				// to check next byte
				trailing = 4;
			}
			else if ((c & 0xFE) == 0xFC)           // valid 6 byte UTF-8
			{
				if (!(c & 0x01))                // Is UTF-8 byte in
					// proper range?
					byte2mask = 0x3C;              // If not set mask
				// to check next byte
				trailing = 5;
			}
			else  return 0;
		}
		return trailing == 0;
	}
};

