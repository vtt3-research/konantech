#include "stdafx.h"
#include "ConvertUTF8Operation.h"

#define MAX_UTF8_BUFFER_SIZE 50000

char* CConvertUTF8Operation::ConvertStringToUTF8(const CString strValue, int* pnByteCount)
{
	char* pcRet = NULL;

	size_t srcLen = strValue.GetLength();
	size_t bufSize = 0;

#ifdef _UNICODE
	int nByteLenth = ((int)srcLen + 1) * 3;

	char* pBuffer = new char[nByteLenth + 1];

	memset(pBuffer, 0, (sizeof(char) * (nByteLenth + 1)));

	int nByteCounts = static_cast<int>(sizeof(char) * (nByteLenth));
	int nCount = WideCharToMultiByte(CP_UTF8, 0, strValue, -1, pBuffer, nByteCounts, NULL, NULL);

	bufSize = nCount;
	pcRet = new char[bufSize];
	memset(pcRet, 0, bufSize);
	memcpy_s(pcRet, bufSize, pBuffer, bufSize);

	delete []pBuffer;
#else
	int nSizeUTF16 = ::MultiByteToWideChar (CP_ACP, 0, strValue, (int)srcLen, NULL, 0);
	BSTR bstrString = ::SysAllocStringLen (NULL, nSizeUTF16);
	::MultiByteToWideChar (CP_ACP, 0, strValue, (int)srcLen, bstrString, nSizeUTF16);
	int nSizeUTF8 = ::WideCharToMultiByte (CP_UTF8, 0, bstrString, nSizeUTF16, NULL, NULL, NULL, NULL);
	BYTE* pStrUTF8 = new BYTE[nSizeUTF8+1];
	memset(pStrUTF8, 0, nSizeUTF8+1);
	::WideCharToMultiByte (CP_UTF8, 0, bstrString, nSizeUTF16, (char*)pStrUTF8, nSizeUTF8, NULL, NULL);
	::SysFreeString (bstrString);

	bufSize = nSizeUTF8;
	pcRet = (char*)pStrUTF8;

	//string convert_from_mbcs_to_utf8(const string& text)
	//{
	//	return convert_encoding(CP_ACP, CP_UTF8, text);
	//}

	//string convert_from_utf8_to_mbcs(const string& text)
	//{
	//	return convert_encoding(CP_UTF8, CP_ACP, text);
	//}

	//string convert_encoding(UINT in_encoding, UINT out_encoding, const string& in_encoding_text)
	//{
	//	// 인코딩을 in_encoding에서 UTF-16으로 변환합니다.
	//	int utf16_character_count = MultiByteToWideChar(in_encoding, 0, in_encoding_text.c_str(),
	//		in_encoding_text.size(), NULL, 0);
	//	if (utf16_character_count == 0)
	//		return "";
	//	wstring utf16_text(utf16_character_count, L'\0');
	//	MultiByteToWideChar(in_encoding, 0, in_encoding_text.c_str(), in_encoding_text.size(),
	//		&utf16_text[0], utf16_character_count);

	//	// 인코딩을 UTF-16에서 out_encoding으로 변환합니다.
	//	int out_encoding_character_count = WideCharToMultiByte(out_encoding, 0, &utf16_text[0],
	//		utf16_text.size(), NULL, 0, NULL, NULL);
	//	if (out_encoding_character_count == 0)
	//		return "";
	//	string out_encoding_text(out_encoding_character_count, '\0');
	//	WideCharToMultiByte(out_encoding, 0, &utf16_text[0], utf16_text.size(),
	//		&out_encoding_text[0], out_encoding_character_count, NULL, NULL);

	//	return out_encoding_text;
	//}
#endif

	if(pnByteCount)
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
	int nSizeUTF16 = ::MultiByteToWideChar (CP_UTF8, 0, (LPCSTR)pUTF8Buffer, nSizeUTF8, NULL, 0);

#ifdef _UNICODE
	::MultiByteToWideChar (CP_UTF8, 0, (LPCSTR)pUTF8Buffer, nSizeUTF8, strDstValue.GetBuffer(nSizeUTF16), nSizeUTF16);
	strDstValue.ReleaseBuffer();
#else
	BSTR bstrString = ::SysAllocStringLen (NULL, nSizeUTF16);
	::MultiByteToWideChar (CP_UTF8, 0, (LPCSTR)pUTF8Buffer, nSizeUTF8, bstrString, nSizeUTF16);
	int nStringLen = ::WideCharToMultiByte (CP_ACP, 0, bstrString, nSizeUTF16, NULL, 0, NULL, NULL);
	::WideCharToMultiByte (CP_ACP, 0, bstrString, nSizeUTF16, strDstValue.GetBuffer (nStringLen), nStringLen, NULL, NULL);
	strDstValue.ReleaseBuffer ();
	::SysFreeString (bstrString);
#endif
}

char* CConvertUTF8Operation::ConvertStringToAnsi(const CString strValue, int* pnByteCount)
{
	char* pcRet = NULL;

	size_t srcLen = strValue.GetLength();

#ifdef _UNICODE
	int nByteLenth = ((int)srcLen + 1) * 3;

	char* pBuffer = new char[nByteLenth + 1];

	memset(pBuffer, 0, (sizeof(char) * (nByteLenth + 1)));

	int nByteCounts = static_cast<int>(sizeof(char) * (nByteLenth));

	int nCount = WideCharToMultiByte(CP_ACP, 0, strValue, -1, pBuffer, nByteCounts, NULL, NULL);

	size_t bufSize = nCount;

	pcRet = new char[bufSize];

	memset(pcRet, 0, bufSize);

	memcpy_s(pcRet, bufSize, pBuffer, bufSize);

	delete []pBuffer;
#else
	size_t bufSize = srcLen;

	pcRet = new char[bufSize + 1];

	memset(pcRet, 0, bufSize + 1);

	memcpy(pcRet, (LPCSTR)strValue, bufSize);
#endif

	if(pnByteCount)
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
	int nSizeUTF16 = ::MultiByteToWideChar (CP_ACP, 0, (LPCSTR)pAnsiBuffer, nSizeAnsi, NULL, 0);

#ifdef _UNICODE
	::MultiByteToWideChar (CP_ACP, 0, (LPCSTR)pAnsiBuffer, nSizeAnsi, strDstValue.GetBuffer(nSizeUTF16), nSizeUTF16);
	strDstValue.ReleaseBuffer();
#else
	BSTR bstrString = ::SysAllocStringLen (NULL, nSizeUTF16);
	::MultiByteToWideChar (CP_ACP, 0, (LPCSTR)pAnsiBuffer, nSizeAnsi, bstrString, nSizeUTF16);
	int nStringLen = ::WideCharToMultiByte (CP_ACP, 0, bstrString, nSizeUTF16, NULL, 0, NULL, NULL);
	::WideCharToMultiByte (CP_ACP, 0, bstrString, nSizeUTF16, strDstValue.GetBuffer (nStringLen), nStringLen, NULL, NULL);
	strDstValue.ReleaseBuffer ();
	::SysFreeString (bstrString);
#endif
}

// Check for valid UTF-8 string. Code taken from the examples in RFC 2640
BOOL CConvertUTF8Operation::UTF8_Validation(const unsigned char *buf, unsigned int len)
{
	const unsigned char *endbuf = buf + len;
	unsigned char byte2mask=0x00, c;
	int trailing = 0;  // trailing (continuation) bytes to follow

	while (buf != endbuf)
	{
		c = *buf++;
		if (trailing)
			if ((c&0xC0) == 0x80)  // Does trailing byte follow UTF-8 format?
			{if (byte2mask)        // Need to check 2nd byte for proper range?
			if (c&byte2mask)     // Are appropriate bits set?
				byte2mask=0x00;
			else
				return 0;
		trailing--; }
			else
				return 0;
		else
			if ((c&0x80) == 0x00)  continue;      // valid 1 byte UTF-8
			else if ((c&0xE0) == 0xC0)            // valid 2 byte UTF-8
				if (c&0x1E)                     // Is UTF-8 byte in
					// proper range?
					trailing =1;
				else
					return 0;
			else if ((c&0xF0) == 0xE0)           // valid 3 byte UTF-8
			{if (!(c&0x0F))                // Is UTF-8 byte in
			// proper range?
			byte2mask=0x20;              // If not set mask
			// to check next byte
			trailing = 2;}
			else if ((c&0xF8) == 0xF0)           // valid 4 byte UTF-8
			{if (!(c&0x07))                // Is UTF-8 byte in
			// proper range?

			byte2mask=0x30;              // If not set mask
			// to check next byte
			trailing = 3;}
			else if ((c&0xFC) == 0xF8)           // valid 5 byte UTF-8
			{if (!(c&0x03))                // Is UTF-8 byte in
			// proper range?
			byte2mask=0x38;              // If not set mask
			// to check next byte
			trailing = 4;}
			else if ((c&0xFE) == 0xFC)           // valid 6 byte UTF-8
			{if (!(c&0x01))                // Is UTF-8 byte in
			// proper range?
			byte2mask=0x3C;              // If not set mask
			// to check next byte
			trailing = 5;}
			else  return 0;
	}
	return trailing == 0;
}