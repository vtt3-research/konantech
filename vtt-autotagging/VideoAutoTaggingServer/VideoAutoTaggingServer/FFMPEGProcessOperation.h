#pragma once

#include "Redirector.h"

#define EXITCODE_FFMPEGEXE_NOERROR											(0)
#define EXITCODE_FFMPEGEXE_ERROR											(1)
#define EXITCODE_FFMPEGEXE_FORCE_ABORT										(1000001)

#define FILE_CONSOLEAPP_FFMPEGEXE											(_T("ffmpeg.exe"))
#define FILE_CONSOLEAPP_MAGICEXE											(_T("convert"))

class CFFMPEGProcessOperation
{
public:
	CFFMPEGProcessOperation(void);
	CFFMPEGProcessOperation(CString path);
	CFFMPEGProcessOperation(int nChannel);

	void SetSrcVideoFrameRate(double dbFrameRate) { m_dbSrcVideoFrameRate = dbFrameRate; };

	void initExePath(int nChannel);
	void initExePath(CString path, int nTotalFrameCount);

	void ProcessEnd();

	virtual ~CFFMPEGProcessOperation(void);

	void SetFFMPEGExeFilePath(CString strFilePath) {m_strFFMPEGExeFilePath = strFilePath;};
	CString GetFFMPEGExeFilePath() {return m_strFFMPEGExeFilePath;};

	//Ju.NaRae 20171206
	BOOL StartFFMPEGProcess(CString strCmd, CString& strStartCmd);
	BOOL StartFFMPEGProcess(CString strCmd);
	void AbortFFMPEGProcess();
	void AbortFFMPEGProcess(DWORD dwProcessID);

	//Ju.NaRae 20171206
	BOOL IsRunningFFMPEGProcess(int* nProgress);
	int IsCompleteFFMPEGProcess(CString& srtResult, BOOL bPTS = FALSE);

	BOOL IsRunningFFMPEGProcessShot(int* nProgress);
	BOOL IsRunningFFMPEGProcessFrame(int* nProgress);
	int GetProcessingPTSTime();

	CString GetLastCmd() {return m_strLastCmd;};
	int GetErrorCode() {return m_nErrorCode;};

	LONGLONG GetProcessingFrameCount();
	LONGLONG GetTotalFrameCount();

	//Ju.NaRae 20180611
	void ExtractPTSTime();
	double GetPTSTime(int nNum);

	LONGLONG GetProgress() { return m_nProgress; };

	void SetCallback_Progress(int (*pfCB)(int, LPVOID), LPVOID lpUserData);

	CString GetResult(){ return m_Redirector.GetResult(TRUE); };
	LONGLONG m_nTotalFrameCount;

	CString GetDuration() { return m_strDuration; }

protected:
	CRedirector m_Redirector;

	int m_nErrorCode;
	int m_nInputCount;

	int m_nProgressIndex;
	
	int m_nProgress;

	CStringArray m_strArrDuration;

	CString m_strLastCmd;
	CString m_strFFMPEGExeFilePath;
	CString m_strDuration;

	int		(*m_pfReportProgress)(int nProgress, LPVOID lpUserData);
	LPVOID	m_lpReportProgressUserData;

	vector<double> m_vecPTSTime;

	double m_dbSrcVideoFrameRate;
};
