
#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

namespace Utility
{
	void Tokenize(const string& str,
		vector<string>& tokens,
		const string& delimiters = " ");

	string ReplaceAll(std::string str, const std::string& from, const std::string& to);

	string string_format(const string fmt, ...);

	// memory free return value after using
	char* base64e(char *src, int length);
	// return result size
	int   base64d(char *src, char *result);

	void encode_base64(unsigned char *out, unsigned char *in, int inlen);

	int decode_base64(unsigned char *out, unsigned char *in);

	BOOL IsFile(LPCTSTR lpszPathName);

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

	class CAutoLock
	{
	public:
		CAutoLock(mutex* pLockMutex) 
		{ 
			m_pMutex = pLockMutex;
			m_pMutex->lock(); 
		};

		~CAutoLock() 
		{ 
			if (m_pMutex)
			{
				m_pMutex->unlock();
				m_pMutex = NULL;
			}
		};

		void Unlock()
		{
			if (m_pMutex)
				m_pMutex->unlock();
		};

	private:
		mutex* m_pMutex;
	};
};

