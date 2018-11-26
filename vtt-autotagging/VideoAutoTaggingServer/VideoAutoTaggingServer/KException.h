// KException.h: interface for the CKException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEXCEPTION_H__A84FDD5A_4C37_4C1F_858E_96780F5C92F7__INCLUDED_)
#define AFX_KEXCEPTION_H__A84FDD5A_4C37_4C1F_858E_96780F5C92F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <exception>
#include <time.h>

#pragma warning( disable : 4996 )

using namespace::std;

enum EExKind {eSystem, eWSA, eFTP};

#ifdef _DEBUG
	#define EXMSGOUT()	\
		TRACE (_T("[ERROR] %s\n"), pEx->what ());	\
		TRACE (_T("[TRACE] %s\n"), pEx->GetTraceMsg ());
#else
	#define EXMSGOUT()	\
		{													\
			TCHAR tLogFileName[MAX_PATH], tbuff[32];	\
			_tstrtime_s (tbuff);	\
			SYSTEMTIME localTime;							\
			::GetLocalTime(&localTime);						\
			_stprintf_s(tLogFileName, MAX_PATH, _T("KException_%04d%02d%02d.log"), localTime.wYear, localTime.wMonth, localTime.wDay); \
			FILE *pLogFile=NULL;	\
			if ((_tfopen_s (&pLogFile, tLogFileName, _T("a"))) == 0) {	\
				_ftprintf (pLogFile, _T("[%s ERROR] %s(Ret: %d)\n"), tbuff, pEx->what (), pEx->GetError ());	\
				_ftprintf (pLogFile, _T("[%s TRACE] %s \n"), tbuff, pEx->GetTraceMsg ());	\
				fclose (pLogFile);	\
			}	\
		}
#endif

// General Case
#define DECL_EX_HANDLING_VAR()	\
	int nKExError=0;							\
	CString strKExError, strKExTrace;

#define SETTRACEMSG() \
	strKExTrace.Format (_T("File: %s - Ln: %d"), __SOURCE__, __LINE__);

#define ADDTRACEMSG(KException) \
	strKExTrace.Format (_T("File: %s - Ln: %d"), __SOURCE__, __LINE__);\
	KException->AddTraceMsg (strKExTrace);

#define THROW_EX(errormsg, errorcode, auxvalue)		\
{															\
	SETTRACEMSG ()											\
	nKExError = errorcode;									\
	strKExError.Format (_T("%s"), errormsg);	\
	CKException::Throw (nKExError, strKExError, strKExTrace, auxvalue);	\
}

#define THROW_FN_EX(fname, errorcode, auxvalue)								\
{																\
	SETTRACEMSG ()											\
	nKExError = (int)errorcode;								\
	strKExError.Format (_T("%s (Ret: %d)"), fname, nKExError);	\
	CKException::Throw (nKExError, strKExError, strKExTrace, auxvalue);					\
}	

#define THROW_SYSTEM_EX(fname, n)					\
{														\
	SETTRACEMSG ()									\
	nKExError = ::GetLastError ();				\
	LPTSTR lpBuffer = NULL;						\
	::FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,		\
		NULL, 0, MAKELANGID (LANG_NEUTRAL, LANG_NEUTRAL), lpBuffer, UCHAR_MAX, NULL);	\
		strKExError.Format (_T("%s - %s(ErrCode: %d)"), fname, lpBuffer, nKExError);			\
	::LocalFree (lpBuffer);											\
	CKException::Throw (nKExError, strKExError, strKExTrace, n);	\
}

#define THROW_WSA_EX(fname,n)				\
{														\
	SETTRACEMSG ()									\
	nKExError = WSAGetLastError ();				\
	strKExError.Format (_T("%s - ErrorCode: %d)"), fname, nKExError);				\
	CKException::Throw (nKExError, strKExError, strKExTrace, n);	\
}

#define THROW_CRUNTIME_EX(fname, n)					\
{														\
	SETTRACEMSG ()											\
	nKExError = errno;									\
	strKExError.Format (_T("%s - %s(errno: %d)"), fname, strerror(nKExError), nKExError);	\
	CKException::Throw (nKExError, strKExError, strKExTrace, n);	\
}

class CKException : public exception
{
public:
	CKException() : m_nAuxValue (0) {}
	virtual ~CKException() {}

	static void Throw (int nError, LPCTSTR lpszErrorMsg, LPCTSTR lpszTraceMsg = NULL, int nAuxValue = 0)
	{
		CKException* pEx = new CKException;
		pEx->SetError (nError);
		pEx->SetErrorMsg (lpszErrorMsg);
		pEx->SetAuxValue (nAuxValue);
		pEx->AddTraceMsg (lpszTraceMsg);
		
		throw pEx;
	}
	
	// Returns the error message
	LPCTSTR what() { return (LPCTSTR)m_strErrorMsg; }
	void SetErrorMsg (LPCTSTR lpszErrorMsg) { m_strErrorMsg = lpszErrorMsg; }	
	LPCTSTR GetTraceMsg () { return (LPCTSTR)m_strTraceMsg; }
	void AddTraceMsg (LPCTSTR lpszTraceMsg)
	{
		m_strTraceMsg += lpszTraceMsg;
		m_strTraceMsg += _T ("\n");
	}
	int GetAuxValue () { return m_nAuxValue; }
	void SetAuxValue (int nAuxValue) { m_nAuxValue = nAuxValue; }
	int GetError () { return m_nError; }
	void SetError (int nError) { m_nError = nError; }
	
protected:
	int m_nAuxValue;
	int m_nError;
	CString m_strTraceMsg;
	CString m_strErrorMsg;
};

#endif // !defined(AFX_KEXCEPTION_H__A84FDD5A_4C37_4C1F_858E_96780F5C92F7__INCLUDED_)
