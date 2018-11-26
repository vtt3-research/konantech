#include "StdAfx.h"
#include "FFMPEGProcessOperation.h"

#include "timecode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <mutex>

std::mutex g_mutexProcess;
FILE* g_fpOpenedProcess = NULL;

CString GetProcessPrefixPath()
{
	CString strModuleFilePath;
	_TCHAR FilePath[MAX_PATH];
	GetModuleFileName (NULL, FilePath, MAX_PATH);
	strModuleFilePath.Format(_T("%s"), FilePath);
	int nEnd = strModuleFilePath.ReverseFind(_T('\\'));
	if (nEnd != -1)
	{
		strModuleFilePath = strModuleFilePath.Mid(0, nEnd+1); 
	}

	return strModuleFilePath;
}

CFFMPEGProcessOperation::CFFMPEGProcessOperation(void)
{
	m_nErrorCode = 0;
	m_strLastCmd = _T("");
	m_nProgressIndex = 0;
	m_nInputCount = 0;
	m_nTotalFrameCount = 0;
	m_nProgress = 0;

	m_pfReportProgress = NULL;
	m_lpReportProgressUserData = NULL;

	m_dbSrcVideoFrameRate = 29.97;

	//CString strPrefixPath = GetProcessPrefixPath();
	//if(CFFMPEGProcessOperation::GetFFMPEGExeFilePath().IsEmpty())
	//{
	//	CString strPath = strPrefixPath + _T("/FFMPEG/") + FILE_CONSOLEAPP_FFMPEGEXE;
	//	CFFMPEGProcessOperation::SetFFMPEGExeFilePath(strPath);
	//}
}

void CFFMPEGProcessOperation::initExePath(int channel){

	CString strPrefixPath = GetProcessPrefixPath();
	//if(CFFMPEGProcessOperation::GetFFMPEGExeFilePath().IsEmpty())
	//{
	CString strChannel;
	strChannel.Format(_T("%02d"), channel);

	CString strPath = strPrefixPath + _T("/FFMPEG/") + FILE_CONSOLEAPP_FFMPEGEXE;

	int nIndex = 0;
	nIndex = strPath.Find(_T(".exe"));
	if(nIndex > 0)
	{
		strPath.Insert(nIndex, strChannel);
	}

	CFFMPEGProcessOperation::SetFFMPEGExeFilePath(strPath);

}

void CFFMPEGProcessOperation::ProcessEnd(){

	m_Redirector.Close();
}
CFFMPEGProcessOperation::CFFMPEGProcessOperation(CString path)
{
	m_nErrorCode = 0;
	m_strLastCmd = _T("");
	m_nProgressIndex = 0;
	m_nInputCount = 0;
	m_nTotalFrameCount = 0;
	m_nProgress = 0;

	m_pfReportProgress = NULL;
	m_lpReportProgressUserData = NULL;

	m_dbSrcVideoFrameRate = 29.97;

	//if(CFFMPEGProcessOperation::GetFFMPEGExeFilePath().IsEmpty())
	//{
		CString strPath = path + FILE_CONSOLEAPP_MAGICEXE;
		CFFMPEGProcessOperation::SetFFMPEGExeFilePath(strPath);
	//}
}

CFFMPEGProcessOperation::CFFMPEGProcessOperation(int nChannel)
{
	m_nErrorCode = 0;
	m_strLastCmd = _T("");
	m_nProgressIndex =0;
	m_nInputCount = 0;
	m_nTotalFrameCount = 0;
	m_nProgress = 0;

	m_pfReportProgress = NULL;
	m_lpReportProgressUserData = NULL;

	m_dbSrcVideoFrameRate = 29.97;

	CString strPrefixPath = GetProcessPrefixPath();
	//if(CFFMPEGProcessOperation::GetFFMPEGExeFilePath().IsEmpty())
	//{
		CString strChannel;
		strChannel.Format(_T("%02d"), nChannel);
		
		CString strPath = strPrefixPath + _T("/FFMPEG/") + FILE_CONSOLEAPP_FFMPEGEXE;

		int nIndex = 0;
		nIndex = strPath.Find(_T(".exe"));
		if(nIndex > 0)
		{
			strPath.Insert(nIndex, strChannel);
		}

		CFFMPEGProcessOperation::SetFFMPEGExeFilePath(strPath);
	//}
}

CFFMPEGProcessOperation::~CFFMPEGProcessOperation(void)
{
}

void CFFMPEGProcessOperation::initExePath(CString path, int nTotalFrameCount)
{
	m_nErrorCode = 0;
	m_strLastCmd = _T("");
	m_strDuration = _T("");
	m_nProgressIndex = 0;
	m_nInputCount = 0;
	m_nTotalFrameCount = nTotalFrameCount;
	m_nProgress = 0;

	m_pfReportProgress = NULL;
	m_lpReportProgressUserData = NULL;

	m_vecPTSTime.clear();

	////if(CFFMPEGProcessOperation::GetFFMPEGExeFilePath().IsEmpty())
	////{
	//CString strPath = path + FILE_CONSOLEAPP_FFMPEGEXE/*FILE_CONSOLEAPP_MAGICEXE*/;
	//CFFMPEGProcessOperation::SetFFMPEGExeFilePath(strPath);
	////}

	CString strPrefixPath = GetProcessPrefixPath();
	CString strPath = strPrefixPath + _T("FFMPEG/") + FILE_CONSOLEAPP_FFMPEGEXE;

	CFFMPEGProcessOperation::SetFFMPEGExeFilePath(strPath);
}

BOOL CFFMPEGProcessOperation::StartFFMPEGProcess(CString strCmd, CString& strStartCmd)//Ju.NaRae 20171206
{
	BOOL bRet = TRUE;

	m_nErrorCode = 0;
	m_nProgress = 0;
	//m_nTotalFrameCount = 0;///
	m_nProgressIndex = 0;
	m_nInputCount = 0;
	m_strArrDuration.RemoveAll();

	//strCmd = _T("-y -i \"D:\\video\\sbs\\sample1_16x9.wmv\" -i \"D:\\video\\sbs\\ar.mp4\" -i \"D:\\video\\sbs\\ar.mp4\" -filter_complex \"[0:0] [0:1] [1:0] [1:1] [2:0] [2:1] concat=n=3:v=1:a=1 [v] [a]\" -map [v] -map [a] \"D:\\video\\sbs\\merged.mp4\"");
	//D:\\video\\sbs\\a.wmv
	//-vcodec libx264 -pix_fmt yuv420p -profile:v main -level 3.1 -preset slow -filter_complex "[0:v][1:v] overlay=861:30" "\"
	//strCmd = "-y -i \"d:/video/sbs/a.wmv\" -i \"d:/video/sbs/sbs_logo.png\" -vcodec libx264 -pix_fmt yuv420p -profile:v main -level 3.1 -preset slow -filter_complex \"[0:v][1:v] overlay=861:30\" -vb 2000k -ab 128k \"d:/temp/YTNPXV201500682682.mp4\"";
	
	
	strCmd = m_strFFMPEGExeFilePath + _T(" ") + strCmd;
	
	strStartCmd = strCmd;
	
	//strCmd = _T("chcp 65001 && ") + strCmd;

	m_strLastCmd = strCmd;

	if(!m_Redirector.Open(strCmd, NULL))
	{
		m_nErrorCode = (int)GetLastError();

		return FALSE;
	}

	return TRUE;
}

BOOL CFFMPEGProcessOperation::StartFFMPEGProcess(CString strCmd)
{
	BOOL bRet = TRUE;

	m_nErrorCode = 0;
	m_nProgress = 0;
	//m_nTotalFrameCount = 0;///
	m_nProgressIndex = 0;
	m_nInputCount = 0;
	m_strArrDuration.RemoveAll();

	//strCmd = _T("-y -i \"D:\\video\\sbs\\sample1_16x9.wmv\" -i \"D:\\video\\sbs\\ar.mp4\" -i \"D:\\video\\sbs\\ar.mp4\" -filter_complex \"[0:0] [0:1] [1:0] [1:1] [2:0] [2:1] concat=n=3:v=1:a=1 [v] [a]\" -map [v] -map [a] \"D:\\video\\sbs\\merged.mp4\"");
	//D:\\video\\sbs\\a.wmv
	//-vcodec libx264 -pix_fmt yuv420p -profile:v main -level 3.1 -preset slow -filter_complex "[0:v][1:v] overlay=861:30" "\"
	//strCmd = "-y -i \"d:/video/sbs/a.wmv\" -i \"d:/video/sbs/sbs_logo.png\" -vcodec libx264 -pix_fmt yuv420p -profile:v main -level 3.1 -preset slow -filter_complex \"[0:v][1:v] overlay=861:30\" -vb 2000k -ab 128k \"d:/temp/YTNPXV201500682682.mp4\"";
	
	
	strCmd = m_strFFMPEGExeFilePath + _T(" ") + strCmd;
	
	//strCmd = _T("chcp 65001 && ") + strCmd;

	m_strLastCmd = strCmd;

	if(!m_Redirector.Open(strCmd, NULL))
	{
		m_nErrorCode = (int)GetLastError();

		return FALSE;
	}

	return TRUE;
}

void CFFMPEGProcessOperation::AbortFFMPEGProcess()
{
	m_Redirector.Abort(EXITCODE_FFMPEGEXE_FORCE_ABORT);
	m_nErrorCode = EXITCODE_FFMPEGEXE_FORCE_ABORT;
}

void CFFMPEGProcessOperation::AbortFFMPEGProcess(DWORD dwProcessID)
{
	CRedirector::Abort(dwProcessID, EXITCODE_FFMPEGEXE_FORCE_ABORT);
	m_nErrorCode = EXITCODE_FFMPEGEXE_FORCE_ABORT;
}

BOOL CFFMPEGProcessOperation::IsRunningFFMPEGProcessShot(int* nProgress)
{
	BOOL bRet = m_Redirector.IsRunningChildProcess();

	if (!bRet)
	{
		m_Redirector.Close();
	}
	else
	{
		CString strFindPTSTime = _T("pts_time:");
		CString strResult = m_Redirector.GetResult(TRUE);
		CString strPTSTime = _T("");
		CString strFindDuration = _T("Duration: ");
		CString strTime = _T("");

		double dbPTSTime = 0.0f;
		double dbDuration = 0.0f;
		//double dbTime = 0.0f;

		int nIndex = 0;

		nIndex = strResult.Find(strFindDuration);

		if (nIndex >= 0)
		{
			strResult = strResult.Mid(nIndex + strFindDuration.GetLength());
			m_strDuration = strResult.Left(strResult.Find(','));
			m_strDuration.Replace(_T(" "), _T(""));
		}

		strTime = m_strDuration.Left(2);
		dbDuration = atof(strTime) * 3600;

		strTime = m_strDuration.Mid(3, 2);
		dbDuration += atof(strTime) * 60;

		strTime = m_strDuration.Mid(6, 2);
		dbDuration += atof(strTime);

		nIndex = 0;

		while (1)
		{
			nIndex = strResult.Find(strFindPTSTime);

			if (nIndex >= 0)
			{
				strResult = strResult.Mid(nIndex + strFindPTSTime.GetLength());
				strPTSTime = strResult.Left(strResult.Find('p'));
				strPTSTime.Replace(_T(" "), _T(""));
				dbPTSTime = atof(strPTSTime);
			}
			else
			{
				*nProgress = (int)((dbPTSTime / dbDuration) * 100);
				break;
			}
		}
	}

	return bRet;
}

BOOL CFFMPEGProcessOperation::IsRunningFFMPEGProcessFrame(int* nProgress)
{
	BOOL bRet = m_Redirector.IsRunningChildProcess();

	if (!bRet)
	{
		m_Redirector.Close();
	}
	else
	{
		if (m_nTotalFrameCount > 0)
		{
			LONGLONG nFramCount = GetProcessingFrameCount();
			*nProgress = (int)((nFramCount * 1000) / m_nTotalFrameCount);
		}
	}

	return bRet;
}

BOOL CFFMPEGProcessOperation::IsRunningFFMPEGProcess(int* progress)
{
	BOOL bRet = m_Redirector.IsRunningChildProcess();

	if (!bRet)
	{
		m_Redirector.Close();
	}
	else
	{
		/*if(m_nTotalFrameCount == 0)
			m_nTotalFrameCount = GetTotalFrameCount();
		else */if(m_nTotalFrameCount > 0)
		{
			LONGLONG nFramCount = GetProcessingFrameCount();
			int nProgress = (int)(nFramCount * 100 / m_nTotalFrameCount);

			if(m_nProgress != nProgress)
			{
				m_nProgress = nProgress;
				*progress = m_nProgress;
				if(m_nProgress > 100)
					m_nProgress = 99;
					*progress = m_nProgress;

				if(m_pfReportProgress && m_lpReportProgressUserData)
				{
					m_pfReportProgress(nProgress, m_lpReportProgressUserData);
				}
			}
		}
	}

	return bRet;
}

int CFFMPEGProcessOperation::IsCompleteFFMPEGProcess(CString& strResult, BOOL bPTS)
{
	if (bPTS)
		ExtractPTSTime();
	
	strResult = m_Redirector.GetResult();
	m_nErrorCode = (int)m_Redirector.GetExitCode();

	if(m_nErrorCode == 0 && m_pfReportProgress && m_lpReportProgressUserData)
	{
		if(m_nProgress != 100)
		{
			m_nProgress = 100;
			m_pfReportProgress(100, m_lpReportProgressUserData);
		}
	}

	return m_nErrorCode;
}

void CFFMPEGProcessOperation::ExtractPTSTime()
{
	CString strFindPTSTime = _T("pts_time:");
	CString strResult = m_Redirector.GetResult(TRUE);
	CString strPTSTime = _T("");
	CString strFindDuration = _T("Duration: ");

	int nIndex = 0;

	nIndex = strResult.Find(strFindDuration);

	if (nIndex >= 0)
	{
		strResult = strResult.Mid(nIndex + strFindDuration.GetLength());
		m_strDuration = strResult.Left(strResult.Find(','));
		m_strDuration.Replace(_T(" "), _T(""));
	}

	nIndex = 0;

	while (1)
	{
		nIndex = strResult.Find(strFindPTSTime);

		if (nIndex >= 0)
		{
			strResult = strResult.Mid(nIndex + strFindPTSTime.GetLength());
			strPTSTime = strResult.Left(strResult.Find('p'));
			strPTSTime.Replace(_T(" "), _T(""));
			m_vecPTSTime.push_back(atof(strPTSTime));
		}
		else
			break;
	}
}

double CFFMPEGProcessOperation::GetPTSTime(int nNum)
{
	double dbPTSTime = 0.0f;

	if (nNum + 1 > (int)m_vecPTSTime.size())
	{
		m_vecPTSTime.clear();
		return -1;
	}
	else
	{
		dbPTSTime = m_vecPTSTime[nNum];
	}

	return dbPTSTime;
}

LONGLONG CFFMPEGProcessOperation::GetProcessingFrameCount()
{
	LONGLONG nFrameCount = 0;


	CString strFindFps = _T(" fps=");
	CString strFindFrame = _T("frame= ");
	CString strFindTime = _T("time= ");
	CString strResult = m_Redirector.GetResult(TRUE);
	m_Redirector.BufferEmpty();

	int nIndex = 0;
	nIndex = strResult.Find(strFindFrame);
	if (nIndex < 0)
	{
		strFindFrame = _T("frame=");
		nIndex = strResult.Find(strFindFrame);
	}

	if (nIndex >= 0)
	{
		strResult = strResult.Mid(nIndex + strFindFrame.GetLength());

		while (1)
		{
			nIndex = strResult.Find(strFindFps);
			if (nIndex >= 0)
			{
				CString strProgressFrameCount = strResult.Mid(0, nIndex);
				strProgressFrameCount.Replace(_T(" "), _T(""));

				nFrameCount = _ttol((LPCTSTR)strProgressFrameCount);

				nIndex = strResult.Find(strFindFrame);
				if (nIndex >= 0)
				{
					strResult = strResult.Mid(nIndex + strFindFrame.GetLength());
				}
				else
					break;
			}
			else
				break;
		}
	}
	else
	{
		//size = N / A time = 00:03 : 49.75 bitrate = N / A speed = 28.6x

		CTimecodeFrameCountOperation TimecodeOperation;
		TimecodeOperation.SetFrameRate(m_dbSrcVideoFrameRate);

		nIndex = strResult.Find(strFindTime);
		if (nIndex < 0)
		{
			strFindTime = _T("time=");
			nIndex = strResult.Find(strFindTime);
			if (nIndex >= 0)
			{
				strResult = strResult.Mid(nIndex);

				strFindTime = _T(" bitrate");
				nIndex = strResult.Find(strFindTime);

				if (nIndex >= 0)
				{
					int nHour = 0, nMin = 0, nSec = 0;
					strResult = strResult.Mid(0, nIndex);
					strResult.Replace(_T("time="), _T(""));

					nIndex = strResult.Find(_T("."));
					if (nIndex >= 0)
					{
						CString strTemp;
						strResult = strResult.Mid(0, nIndex);

						nIndex = strResult.Find(_T(":"));
						if (nIndex >= 0)
						{
							strTemp = strResult.Mid(0, nIndex);
							nHour = _ttoi((LPCTSTR)strTemp);

							strResult = strResult.Mid(nIndex + 1);
						}

						nIndex = strResult.Find(_T(":"));
						if (nIndex >= 0)
						{
							strTemp = strResult.Mid(0, nIndex);
							nMin = _ttoi((LPCTSTR)strTemp);

							strResult = strResult.Mid(nIndex + 1);
						}

						nSec = _ttoi((LPCTSTR)strResult);

						nFrameCount = TimecodeOperation.TimecodeToFrame(nHour, nMin, nSec, 0);
					}
				}
			}
		}
	}

	return nFrameCount;
}

LONGLONG CFFMPEGProcessOperation::GetTotalFrameCount()
{
	LONGLONG nTotalFramCount = 0;

	CString strFind;
	CString strResult = m_Redirector.GetResult(TRUE);
	m_Redirector.BufferEmpty();
	
	int nIndex = 0;
	
	m_nTotalFrameCount = 0;
	m_nInputCount = 0;
	m_strArrDuration.RemoveAll();

	nIndex = strResult.Find(_T("frame="));
	if(nIndex > 0)
	{

		CString strReMainResult = strResult;
		CString strDuration;

		while(1)
		{
			strFind.Format(_T("Input #%ld"), m_nInputCount);
			nIndex = strReMainResult.Find(strFind);

			if(nIndex > 0)
			{
				m_nInputCount++;

				strReMainResult = strReMainResult.Mid(nIndex);
				strReMainResult.Replace(strFind, _T(""));

				strFind = _T("Duration: ");
				nIndex = strReMainResult.Find(strFind);
				if(nIndex > 0)
				{
					strReMainResult = strReMainResult.Mid(nIndex);
					strReMainResult = strReMainResult.Mid(strFind.GetLength());

					strFind = _T(",");
					nIndex = strReMainResult.Find(strFind);
					if(nIndex > 0)
					{
						strDuration = strReMainResult.Mid(0, nIndex);
						m_strArrDuration.Add(strDuration);
					}
				}
			}
			else
				break;
		}
	}
	else
		m_nInputCount = 0;
	

	for(int i = 0; i < m_nInputCount; i++)
	{
		CString strDuration = m_strArrDuration[i];

		CString strFind = _T(".");
		int nIndex = 0;

		nIndex = strDuration.Find(strFind);
		if(nIndex > 0)
		{
			CString strTimecode = strDuration.Mid(0, nIndex);
			CString strMillisecond = strDuration.Mid(nIndex + 1);

			int nMillisecond = _ttoi((LPCTSTR)strMillisecond);
			if(strMillisecond.GetLength() == 2)
				nMillisecond = _ttoi((LPCTSTR)strMillisecond) * 10;

			CString strFrame;
			strFrame.Format(_T(":%02d"), nMillisecond / 33);
			strTimecode = strTimecode + strFrame;
			
			strFind = _T(":");
			
			int nHour = 0, nMin = 0, nSec = 0, nFrame = 0;
			nIndex = strTimecode.Find(strFind);
			if(nIndex > 0)
			{
				nHour = _ttoi((LPCTSTR)strTimecode.Mid(0, nIndex));
				strTimecode = strTimecode.Mid(nIndex + 1);
			}

			nIndex = strTimecode.Find(strFind);
			if(nIndex > 0)
			{
				nMin = _ttoi((LPCTSTR)strTimecode.Mid(0, nIndex));
				strTimecode = strTimecode.Mid(nIndex + 1);
			}

			nIndex = strTimecode.Find(strFind);
			if(nIndex > 0)
			{
				nSec = _ttoi((LPCTSTR)strTimecode.Mid(0, nIndex));
				nFrame = _ttoi((LPCTSTR)strTimecode.Mid(nIndex + 1));
			}

			int nFrameCount = TimecodeToFrame(nHour, nMin, nSec, nFrame);
			nTotalFramCount = nTotalFramCount + nFrameCount;
		}
	}

	return nTotalFramCount;
}

/*
LONGLONG CFFMPEGProcessOperation::GetProcessingFrameCount()
{
	LONGLONG nFrameCount = 0;

	if(m_nProgressIndex > 0)
	{
		CString strFindFps = _T(" fps=");
		CString strFindFrame = _T("frame= ");
		CString strResult = m_Redirector.GetResult(TRUE);
		m_Redirector.BufferEmpty();


		CString strProgressResult = strResult.Mid(m_nProgressIndex);

		int nIndex = 0;
		nIndex = strProgressResult.Find(strFindFrame);
		if(nIndex >= 0)
		{
			strProgressResult = strProgressResult.Mid(nIndex + strFindFrame.GetLength());
			m_nProgressIndex = m_nProgressIndex + nIndex + strFindFrame.GetLength();

			CString strtemp;
			strtemp = strResult.Mid(m_nProgressIndex);

			while(1)
			{
				nIndex = strProgressResult.Find(strFindFps);
				if(nIndex >= 0)
				{
					CString strProgressFrameCount = strProgressResult.Mid(0, nIndex);
					strProgressFrameCount.Replace(_T(" "), _T(""));

					nFrameCount = _ttol((LPCTSTR)strProgressFrameCount);

					nIndex = strProgressResult.Find(strFindFrame);
					if(nIndex >= 0)
					{
						strProgressResult = strProgressResult.Mid(nIndex + strFindFrame.GetLength());
						m_nProgressIndex = m_nProgressIndex + nIndex + strFindFrame.GetLength();

						CString strtemp;
						strtemp = strResult.Mid(m_nProgressIndex);
					}
					else
						break;
				}
				else
					break;
			}
		}
	}

	return nFrameCount;
}

LONGLONG CFFMPEGProcessOperation::GetProcessingFrameCount()
{
	LONGLONG nFrameCount = 0;

	if(m_nProgressIndex > 0)
	{
		CString strFindFps = _T(" fps=");
		CString strFindFrame = _T("frame= ");
		CString strResult = m_Redirector.GetResult(TRUE);
		m_Redirector.BufferEmpty();
		
		
		CString strProgressResult = strResult.Mid(m_nProgressIndex);

		int nIndex = 0;
		nIndex = strProgressResult.Find(strFindFrame);
		if(nIndex >= 0)
		{
			strProgressResult = strProgressResult.Mid(nIndex + strFindFrame.GetLength());
			m_nProgressIndex = m_nProgressIndex + nIndex + strFindFrame.GetLength();

			CString strtemp;
			strtemp = strResult.Mid(m_nProgressIndex);

			while(1)
			{
				nIndex = strProgressResult.Find(strFindFps);
				if(nIndex >= 0)
				{
					CString strProgressFrameCount = strProgressResult.Mid(0, nIndex);
					strProgressFrameCount.Replace(_T(" "), _T(""));

					nFrameCount = _ttol((LPCTSTR)strProgressFrameCount);

					nIndex = strProgressResult.Find(strFindFrame);
					if(nIndex >= 0)
					{
						strProgressResult = strProgressResult.Mid(nIndex + strFindFrame.GetLength());
						m_nProgressIndex = m_nProgressIndex + nIndex + strFindFrame.GetLength();

						CString strtemp;
						strtemp = strResult.Mid(m_nProgressIndex);
					}
					else
						break;
				}
				else
					break;
			}
		}
	}

	return nFrameCount;
}
LONGLONG CFFMPEGProcessOperation::GetTotalFrameCount()
{
	LONGLONG nTotalFramCount = 0;

	CString strFind;
	CString strResult = m_Redirector.GetResult(TRUE);
	//m_Redirector.BufferEmpty();

	int nIndex = 0;
	if(m_nProgressIndex == 0)
	{
		m_nTotalFrameCount = 0;
		m_nInputCount = 0;
		m_strArrDuration.RemoveAll();

		nIndex = strResult.Find(_T("frame="));
		if(nIndex > 0)
		{
			m_nProgressIndex = nIndex;

			CString strReMainResult = strResult;
			CString strDuration;

			while(1)
			{
				strFind.Format(_T("Input #%ld"), m_nInputCount);
				nIndex = strReMainResult.Find(strFind);

				if(nIndex > 0)
				{
					m_nInputCount++;

					strReMainResult = strReMainResult.Mid(nIndex);
					strReMainResult.Replace(strFind, _T(""));

					strFind = _T("Duration: ");
					nIndex = strReMainResult.Find(strFind);
					if(nIndex > 0)
					{
						strReMainResult = strReMainResult.Mid(nIndex);
						strReMainResult = strReMainResult.Mid(strFind.GetLength());

						strFind = _T(",");
						nIndex = strReMainResult.Find(strFind);
						if(nIndex > 0)
						{
							strDuration = strReMainResult.Mid(0, nIndex);
							m_strArrDuration.Add(strDuration);
						}
					}
				}
				else
					break;
			}
		}
		else
			m_nInputCount = 0;
	}

	for(int i = 0; i < m_nInputCount; i++)
	{
		CString strDuration = m_strArrDuration[i];

		CString strFind = _T(".");
		int nIndex = 0;

		nIndex = strDuration.Find(strFind);
		if(nIndex > 0)
		{
			CString strTimecode = strDuration.Mid(0, nIndex);
			CString strMillisecond = strDuration.Mid(nIndex + 1);

			int nMillisecond = _ttoi((LPCTSTR)strMillisecond);
			if(strMillisecond.GetLength() == 2)
				nMillisecond = _ttoi((LPCTSTR)strMillisecond) * 10;

			CString strFrame;
			strFrame.Format(_T(":%02d"), nMillisecond / 33);
			strTimecode = strTimecode + strFrame;

			strFind = _T(":");

			int nHour = 0, nMin = 0, nSec = 0, nFrame = 0;
			nIndex = strTimecode.Find(strFind);
			if(nIndex > 0)
			{
				nHour = _ttoi((LPCTSTR)strTimecode.Mid(0, nIndex));
				strTimecode = strTimecode.Mid(nIndex + 1);
			}

			nIndex = strTimecode.Find(strFind);
			if(nIndex > 0)
			{
				nMin = _ttoi((LPCTSTR)strTimecode.Mid(0, nIndex));
				strTimecode = strTimecode.Mid(nIndex + 1);
			}

			nIndex = strTimecode.Find(strFind);
			if(nIndex > 0)
			{
				nSec = _ttoi((LPCTSTR)strTimecode.Mid(0, nIndex));
				nFrame = _ttoi((LPCTSTR)strTimecode.Mid(nIndex + 1));
			}

			int nFrameCount = TimecodeToFrame(nHour, nMin, nSec, nFrame);
			nTotalFramCount = nTotalFramCount + nFrameCount;
		}
	}

	return nTotalFramCount;
}
*/

void CFFMPEGProcessOperation::SetCallback_Progress(int (*pfCB)(int, LPVOID), LPVOID lpUserData)
{
	m_pfReportProgress = pfCB;
	m_lpReportProgressUserData = lpUserData;
}