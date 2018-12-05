//////////////////////////////////////////////////////////////////////
//
// Redirector - to redirect the input / output of a console
//
// Developer: Jeff Lee
// Dec 10, 2001
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REDIR_H__4FB57DC3_29A3_11D5_BB60_006097553C52__INCLUDED_)
#define AFX_REDIR_H__4FB57DC3_29A3_11D5_BB60_006097553C52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"

class CRedirector : public CObject  
{
public:
	CRedirector();
	virtual ~CRedirector();

private:
	HANDLE m_hThread;		// thread to receive the output of the child process
	HANDLE m_hEvtStop;		// event to notify the redir thread to exit
	DWORD m_dwThreadId;		// id of the redir thread
	DWORD m_dwWaitTime;		// wait time to check the status of the child process

	CString mStdOutBuffer;

	DWORD m_dwExitCode;

protected:
	HANDLE m_hStdinWrite;	// write end of child's stdin pipe
	HANDLE m_hStdoutRead;	// read end of child's stdout pipe
	HANDLE m_hChildProcess;

	DWORD m_dwChildProcessID;

	BOOL LaunchChild(LPCTSTR pszCmdLine, LPTSTR pszParam,
		HANDLE hStdOut, HANDLE hStdIn, HANDLE hStdErr);
	int RedirectStdout();
	void DestroyHandle(HANDLE& rhObject);

	static DWORD WINAPI OutputThread(LPVOID lpvThreadParam);

	std::mutex m_mutexOutputBuffer;

protected:
	// overrides:
	virtual void WriteStdOut(LPCSTR pszOutput);
	virtual void WriteStdError(LPCSTR pszError);

public:
	BOOL Open(LPCTSTR pszCmdLine, LPTSTR pszParam);
	
	void Abort(UINT nExitCode);
	static void Abort(DWORD dwProcessID, UINT nExitCode);

	virtual void Close();

	BOOL Printf(LPCTSTR pszFormat, ...);

	void SetWaitTime(DWORD dwWaitTime) { m_dwWaitTime = dwWaitTime; }

	CString GetResult(BOOL bCurrent = FALSE);
	BOOL IsRunningChildProcess();
	//void SetStdOutput(CString *stdoutBuffer);

	HANDLE GetProcess() {return m_hChildProcess;};

	DWORD GetProcessID() { return m_dwChildProcessID; };
	DWORD GetExitCode() {return m_dwExitCode;};
	
	void BufferEmpty();
};

#endif // !defined(AFX_REDIR_H__4FB57DC3_29A3_11D5_BB60_006097553C52__INCLUDED_)
