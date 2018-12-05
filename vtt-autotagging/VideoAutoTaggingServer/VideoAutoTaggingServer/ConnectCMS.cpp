#include "stdafx.h"
#include "ConnectCMS.h"
#include "timecode.h"
#include "ConvertUTF8Operation.h"

CKRestCompJobManager* CConnectCMS::g_pCompJobManager = NULL;

CConnectCMS::CConnectCMS()
: m_nJobID(0)
, m_bIsAcceptJob(true)
, m_bConnect(false)
{
	callback_connect = NULL;
	m_lpvoid = NULL;

	callback_abort = NULL;
	m_lpAbortData = NULL;

	callback_disconnect = NULL;
	m_lpDisconnData = NULL;

	callback_job = NULL;
	m_lpAllocJobData = NULL;

}

CConnectCMS::~CConnectCMS()
{

}

BOOL CConnectCMS::OpenCompJobManager(CString strMamUri, CString strMamExUri, CString strReceiveUri, CString strUser, CString strPasswd)
{
	BOOL bRet = TRUE;
	CString strMsg;

	if (!g_pCompJobManager)
		g_pCompJobManager = new CKRestCompJobManager();

	int nError = 0;

	try
	{
		string stdstrMamUri, stdstrUserID, stdstrPwd, stdstrMamExUri, stdstrReceiveUri;

		char* pBuff = CConvertUTF8Operation::ConvertStringToUTF8(strMamUri);
		stdstrMamUri.assign(pBuff);
		delete pBuff;
		pBuff = NULL;

		pBuff = CConvertUTF8Operation::ConvertStringToUTF8(strMamExUri);
		stdstrMamExUri.assign(pBuff);
		delete pBuff;
		pBuff = NULL;

		pBuff = CConvertUTF8Operation::ConvertStringToUTF8(strReceiveUri);
		stdstrReceiveUri.assign(pBuff);
		delete pBuff;
		pBuff = NULL;

		g_pCompJobManager->SetMamConnInfo(stdstrMamUri, stdstrUserID, stdstrPwd, stdstrMamExUri);
		g_pCompJobManager->SetReceiveServiceInfo(stdstrReceiveUri);

		if (nError = g_pCompJobManager->ConnectMamServer())
		{
			bRet = FALSE;

			strMsg.Format(_T("Failed to connect mam server."));
			writeLog(strMsg);
		}
	}
	catch (CKException *pEx)
	{
		writeLog(pEx->what());
		throw pEx;
	}

	return bRet;
}

void CConnectCMS::CloseCompJobManager()
{
	if (g_pCompJobManager)
	{
		g_pCompJobManager->DisconnectMamServer();

		delete g_pCompJobManager;
		g_pCompJobManager = NULL;
	}
}

void CConnectCMS::ConnectCMS()
{
	CString strMsg, strCompServer, strJobList;
	CString strErrMsg;

	try
	{
		char* pBuff = CConvertUTF8Operation::ConvertStringToUTF8(m_strJobList);
		string stdstrCompJobList(pBuff);
		delete pBuff;
		pBuff = NULL;

		pBuff = CConvertUTF8Operation::ConvertStringToUTF8(STR_CATALOGER_TYPE);
		string stdstrCompType(pBuff);
		delete pBuff;
		pBuff = NULL;

		g_pCompJobManager->SetCompJobAllocCB(m_cmsInfo.nChannel, stdstrCompType, CConnectCMS::pFuncJobAlloc, (void*)this);
		g_pCompJobManager->SetCompJobAbortCB(m_cmsInfo.nChannel, stdstrCompType, CConnectCMS::pFuncJobAbort, (void*)this);

		int nError = 0;

		if (nError = g_pCompJobManager->CompJobCheckIn(m_cmsInfo.nPool, m_cmsInfo.nChannel, stdstrCompType, stdstrCompJobList))
		{
			string stdstrReason = g_pCompJobManager->GetCompJobMessage(m_cmsInfo.nChannel, nError);
			CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrReason.c_str(), strErrMsg);

			strMsg.Format(_T("Check In - %s"), strErrMsg);
			writeLog(strMsg);
		}
		else
			m_bConnect = true;
	}
	catch (CKException *pEx)
	{
		writeLog(pEx->what());
		throw pEx;
	}

	if (callback_connect && m_lpvoid)
		callback_connect(m_bConnect, m_lpvoid);
}

bool CConnectCMS::RegisterWorkerToMam(CMS_INFO cms_info, CString strJobList, bool bStandAlone)
{
	int nErr = 0;

	CString strMsg;

	if (bStandAlone)
	{
		strMsg.Format(_T("StandAlone"));
		writeLog(strMsg);
		return false;
	}

	m_cmsInfo = cms_info;
	m_strJobList = strJobList;

	ConnectCMS();
	
	return true;
}

bool CConnectCMS::DisconnectCMS()
{
	CString strMsg, strCompServer, strJobList;
	CString strErrMsg;

	try
	{
		int nError = 0;

		//이미 체크아웃 되있으면 넘어가게 수정.
		if (m_bConnect)
		{
			if (nError = g_pCompJobManager->CompJobCheckOut(m_cmsInfo.nChannel))
			{
				string stdstrReason = g_pCompJobManager->GetCompJobMessage(m_cmsInfo.nChannel, nError);
				CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrReason.c_str(), strErrMsg);

				strMsg.Format(_T("Check out - %s"), strErrMsg);
				writeLog(strMsg);

				if (nError == 7010)
				{
					strMsg.Format(_T("Error code 7010"));
					writeLog(strMsg);
				}
			}
			else
				m_bConnect = FALSE;
		}
	}
	catch (CKException *pEx)
	{
		writeLog(pEx->what());

		throw pEx;
	}

	return true;
}

bool CConnectCMS::DisconnectCMS(CMS_INFO cmsInfo, __int64 nID)
{
	CString strMsg, strCompServer, strJobList;
	CString strErrMsg;

	try
	{
		int nError = 0;

		m_bConnect = false;
		if(CConnectCMS::OpenCompJobManager(cmsInfo.strMamUri, cmsInfo.strExUri, cmsInfo.strReceiveUri, cmsInfo.strUser, cmsInfo.strPassword) != 0)
			m_bConnect = true;

		if (m_bConnect)
		{
			if (nError = g_pCompJobManager->CompJobCheckOut(cmsInfo.nChannel, nID))
			{
				string stdstrReason = g_pCompJobManager->GetCompJobMessage(m_cmsInfo.nChannel, nError);
				CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrReason.c_str(), strErrMsg);

				strMsg.Format(_T("Check out - %s"), strErrMsg);
				writeLog(strMsg);

				if (nError == 7010)
				{
					strMsg.Format(_T("Error code 7010"));
					writeLog(strMsg);
				}
			}
			else
				m_bConnect = FALSE;

			CConnectCMS::CloseCompJobManager();
		}
	}
	catch (CKException *pEx)
	{
		writeLog(pEx->what());

		throw pEx;
	}

	return true;
}

void CConnectCMS::pFuncJobAlloc(void* pJobData, void* pDataCB)
{
	CConnectCMS *pConnectCMS = (CConnectCMS *)pDataCB;

	if (pConnectCMS && pJobData)
		pConnectCMS->JobAllocProc(pJobData);
}

void CConnectCMS::JobAllocProc(void* pJobData)
{
	CKRestCAJobData* pJobInfo = (CKRestCAJobData*)pJobData;

	if (pJobInfo)
	{
		CString strMsg;

		m_nJobID = (int)pJobInfo->m_nJobID;

		if (pJobInfo->m_nError)
		{
			strMsg.Format(_T("Job assignment failed"));
			writeLog(strMsg);

			CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_stdstrJobJson.c_str(), strMsg);
			writeLog(strMsg);
			CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_stdstrErrMsg.c_str(), strMsg);
			writeLog(strMsg);

			ReportResult(CATALOGING_JOB_RESULT_ERROR, m_JobInfo.strJobInfo);

			callback_job(false, m_lpAllocJobData);
			return;
		}

		CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_stdstrJobJson.c_str(), m_JobInfo.strJobInfo);
		CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_SrcData.m_FileInfo.m_stdstrStorage.c_str()
			, m_JobInfo.strSrcStorage);
		CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_SrcData.m_FileInfo.m_stdstrPath.c_str()
			, m_JobInfo.strSrcFilepath);

		m_JobInfo.nSrcAssetID = (int)pJobInfo->m_SrcData.m_nAssetID;
		m_Kvpair.m_nWorkfolwID = (int)pJobInfo->m_nWorkflowID;
		m_JobInfo.dbVideoframerate = pJobInfo->m_SrcData.m_dbFramerate;

		CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_stdstrShotImageStorage.c_str()
			, m_JobInfo.strDstStorage);
		CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_stdstrShotImagePath.c_str()
			, m_JobInfo.strDstFilePath);

		callback_job(true, m_lpAllocJobData);
	}

}

void CConnectCMS::ReportResult(int nResult, CString strComment)
{
	int nError = 0;
	bool bResult = false;

	CString strMsg, strResult;
	string stdstrReportMsg;

	try
	{
		if (!strComment.IsEmpty())
		{
			int nIndex = -1;
			nIndex = strComment.Find(_T("\""));
			if (nIndex >= 0)
			{
				strComment.Replace(_T("\""), _T("\\\""));
			}

			char* pComment = CConvertUTF8Operation::ConvertStringToUTF8(strComment);
			if (pComment)
			{
				stdstrReportMsg.assign(pComment);
				delete[]pComment;
				pComment = NULL;
			}
		}

		EnumCompWorkReportType enumReporttype;
		if (nResult == CATALOGING_JOB_RESULT_COMPLETE)
		{
			enumReporttype = ENUM_COMPWORK_REPORTTYPE_SUCCESS;
			strResult.Format(_T("SUCCESS"));
			bResult = true;
		}
		else if (nResult == CATALOGING_JOB_RESULT_ABORT)
		{
			enumReporttype = ENUM_COMPWORK_REPORTTYPE_FAILURE;
			strResult.Format(_T("FAILURE"));
			bResult = false;
		}
		else if (nResult == CATALOGING_JOB_RESULT_RETRY)
		{
			enumReporttype = ENUM_COMPWORK_REPORTTYPE_RETRY;
			strResult.Format(_T("RETRY"));
			bResult = false;
		}
		else
		{
			enumReporttype = ENUM_COMPWORK_REPORTTYPE_FAILURE;
			strResult.Format(_T("FAILURE"));
			bResult = false;
		}

		if (g_pCompJobManager)
		{
			strMsg.Format(_T("Report cataloging job result (JobID : %d, %s)"), m_nJobID, strResult);
			writeLog(strMsg);

			if (nError = g_pCompJobManager->ReportJobResult(m_cmsInfo.nChannel, bResult, (int)enumReporttype, stdstrReportMsg))
			{
				string stdstrReason = g_pCompJobManager->GetCompJobMessage(m_cmsInfo.nChannel, nError);
				CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrReason.c_str(), strMsg);

				writeLog(strMsg);
			}
		}

		strMsg.Format(_T("Reporting success (JobID : %d, %s)"), m_nJobID, strResult);
		writeLog(strMsg);
	}
	catch (CKException *pEx)
	{
		writeLog(pEx->what());

		delete pEx;
	}
}

void CConnectCMS::ReportPregress(int nProgress)
{
	int nError = 0;
	CString strMsg;

	if (g_pCompJobManager)
	{
		try
		{
			if (nError = g_pCompJobManager->ReportJobProgress(m_cmsInfo.nChannel, nProgress, ""))
			{
				string stdstrReason = g_pCompJobManager->GetCompJobMessage(m_cmsInfo.nChannel, nError);
				CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrReason.c_str(), strMsg);

				writeLog(strMsg);
			}
			else
			{
				strMsg.Format(_T("progress : %d"), nProgress);
				writeLog(strMsg);
			}
		}
		catch (CKException *pEx)
		{
			writeLog(pEx->what());
			strMsg.Format(_T("Failed to report cataloging Progress (JobID : %d)"), m_nJobID);
			writeLog(strMsg);

			delete pEx;
		}
	}
}

void CConnectCMS::pFuncJobAbort(void* pJobData, void* pDataCB)
{
	CConnectCMS *pConnectCMS = (CConnectCMS *)pDataCB;

	if (pConnectCMS && pJobData)
		pConnectCMS->jobAbort(pJobData);
}

void CConnectCMS::jobAbort(void* pJobData)
{
	CKRestCAJobData* pJobInfo = (CKRestCAJobData*)pJobData;

	if (pJobData)
	{
		CString strMsg;

		if (pJobInfo->m_nError)
		{
			strMsg.Format(_T("Job abort failed"));
			writeLog(strMsg);

			CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_stdstrJobJson.c_str(), strMsg);
			writeLog(strMsg);
			CConvertUTF8Operation::ConvertUTF8ToString((char*)pJobInfo->m_stdstrErrMsg.c_str(), strMsg);
			writeLog(strMsg);

			return;
		}

		strMsg.Format(_T("Assigned a job abort [JobID = %d]"), (int)pJobInfo->m_nJobID);
		writeLog(strMsg);
	}

	callback_abort((int)pJobInfo->m_nJobID, m_lpAbortData);
}

void CConnectCMS::jobInfoClear()
{
	m_JobInfo.dbVideoframerate = 0.0f;
	m_JobInfo.nSrcAssetID = 0;

	m_JobInfo.strDstFilePath = _T("");
	m_JobInfo.strDstFullPath = _T("");
	m_JobInfo.strDstStorage = _T("");
	m_JobInfo.strJobInfo = _T("");
	m_JobInfo.strSrcFilepath = _T("");
	m_JobInfo.strSrcFullPath = _T("");
	m_JobInfo.strSrcStorage = _T("");

	m_Kvpair.m_nAuxJobID = 0;
	m_Kvpair.m_nKeyValue = 0;
	m_Kvpair.m_nStatus = 0;
	m_Kvpair.m_nWorkerID = 0;
	m_Kvpair.m_nWorkfolwID = 0;
	m_Kvpair.m_strCurrentJobName = _T("");

	m_mamex.strEndExTrName = _T("");
	m_mamex.strProgressTrName = _T("");
	m_mamex.strStartExTrName = _T("");
}

//callback_connect(strXml, m_lpvoid);
void CConnectCMS::Callback_Connect(CALLBACK_CONNECT handler, LPVOID lpvoid)
{
	callback_connect = handler;
	m_lpvoid = lpvoid;
}

void CConnectCMS::Callback_Disconnect(CALLBACK_DISCONNECT handler, LPVOID lpvoid)
{
	callback_disconnect = handler;
	m_lpDisconnData = lpvoid;
}

void CConnectCMS::CallbackJobAlloc(CALLBACK_ALLOCJOB handler, LPVOID lpvoid)
{
	callback_job = handler;
	m_lpAllocJobData = lpvoid;
}

void CConnectCMS::CallbackJobAbort(CALLBACK_ABORT handler, LPVOID lpvoid)
{
	callback_abort = handler;
	m_lpAbortData = lpvoid;
}

void CConnectCMS::writeLog(CString strMsg)
{
	CString strCurrentTime = CUtil::GetCurTime();

	CString		strLog;

	strLog.Format("[%s] %s", strCurrentTime, strMsg);

	g_mutexLog.lock();

	try
	{
		LOG1("%s\n", (LPCSTR)strLog);

		_tprintf(_T("%s\n"), strLog);

		g_mutexLog.unlock();
	}
	catch (...)
	{
		g_mutexLog.unlock();
	}
}

bool CConnectCMS::UpdateShotInfoEx(int lWidth, int lHeight, double dbFPS, CString strDuration, void* pShotInfo)
{
	bool bUpdate = false;

	if (g_pCompJobManager && pShotInfo)
	{
		CString strMsg;

		DECL_EX_HANDLING_VAR();

		CKRestExUpdateShotData* pData = (CKRestExUpdateShotData*)pShotInfo;
		pData->m_nImageWidth = lWidth;
		pData->m_nImageHeight = lHeight;
		pData->m_dFrameRate = dbFPS;
		pData->m_stdstrTotalDuration.clear();

		char* pBuff = CConvertUTF8Operation::ConvertStringToUTF8(strDuration);

		if (pBuff)
		{
			pData->m_stdstrTotalDuration.assign(pBuff);
			delete[]pBuff;
			pBuff = NULL;
		}

		try
		{
			int nError = g_pCompJobManager->RunTransactionEx(m_cmsInfo.nChannel, pData);

			if (pData)
			{
				delete pData;
				pData = NULL;
			}

			string stdstrMsg = g_pCompJobManager->GetCompJobMessage(m_cmsInfo.nChannel, nError);
			CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrMsg.c_str(), strMsg);

			strMsg = _T("[Response of updating a shot data] : ") + strMsg;
			writeLog(strMsg);

			if (nError)
			{
				strMsg.Format(_T("Error communicating with Ex server - Errcode : %d"), nError);
				writeLog(strMsg);
			}
			else
				bUpdate = true;
		}
		catch (CKException *pEx)
		{
			writeLog(pEx->what());
			delete pEx;
		}
	}

	return bUpdate;
}