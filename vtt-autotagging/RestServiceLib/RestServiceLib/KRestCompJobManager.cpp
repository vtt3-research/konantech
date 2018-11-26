
#include "stdafx.h"
#include "KRestCompJobManager.h"
#include "KRestCompJob.h"

#include "RestClient.h"
#include "RestServer.h"

#include "Utility.h"

#include <mutex>

#pragma warning (disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define THREAD_STACK_SIZE					(1024*1024*4)

const pair<string, string> g_pairCompCheckIn("POST", "/v2/compservers");
const pair<string, string> g_pairCompCheckOut("DELETE", "/v2/compservers/{compserverid}");
const pair<string, string> g_pairCompReportResult("PUT", "/v2/compservers/{compserverid}/reportResult");
const pair<string, string> g_pairCompReportProgress("PUT", "/v2/compservers/{compserverid}/reportProgress");
const pair<string, string> g_pairCompReceivePing("PUT", "/v2/compservers/{compserverid}/ping");
const pair<string, string> g_pairCompReceiveJob("POST", "/v2/compservers/{compserverid}/jobs");
const pair<string, string> g_pairCompReceiveJobCancel("DELETE", "/v2/compservers/{compserverid}/jobs/{jobid}");
const pair<string, string> g_pairMamEx("PUT", "/v2/mamEx/{trname}/{pool}");

mutex g_mutexJobSync;

int GetStatusCode(int nError)
{
	int nStatusCode = 200;

	switch (nError)
	{
	case RESTCOMPJOB_ERROR_SUCCESS:
		nStatusCode = 200; // Ok
		break;
	case RESTCOMPJOB_ERROR_INVALID_COMPSERV:
	case RESTCOMPJOB_ERROR_INVALID_PARAM:
	case RESTCOMPJOB_ERROR_INVALID_JOB:
		nStatusCode = 404; // NotFound
		break;
	case RESTCOMPJOB_ERROR_INVALID_JSON:
		nStatusCode = 400; // BadRequest
		break;
	case RESTCOMPJOB_ERROR_CANCEL_JOB_THREAD:
	case RESTCOMPJOB_ERROR_BEGIN_JOB_THREAD:
		nStatusCode = 500; // InternalError
		break;
	default:
		nStatusCode = nError;
	}

	return nStatusCode;
}

int ReceivePingCB(map<string, string>& mapPathVarValue, string stdstrQuery, string stdstrBodyJson, void* pData, string& stdstrResultJon)
{
	if (pData)
	{
		int nError = 0;
		string stdstrMsg;

		JOBABORTCB pFuncJobAbort = NULL;
		void* pDataJobAbort = NULL;

		int nChannel = -1;
		long long nCheckInID = 0;

		CKRestCompJob* pCompJob = NULL;
		CKRestCompJobManager* pManager = NULL;

		if (mapPathVarValue.empty())
			return GetStatusCode(RESTCOMPJOB_ERROR_INVALID_PARAM);

		map<string, string>::iterator it = mapPathVarValue.begin();
		string stdstrCheckInID = it->second;
		stdstrCheckInID = Utility::ReplaceAll(stdstrCheckInID, "/", "");

		nCheckInID = atoll(stdstrCheckInID.c_str());

		pManager = (CKRestCompJobManager*)pData;

		{
			Utility::CAutoLock Lock(&g_mutexJobSync);
			pCompJob = pManager->GetCompJob(nCheckInID);
		}

		stdstrResultJon = "{\"result\":\"success\"}";

		if (!pCompJob)
		{
			nError = RESTCOMPJOB_ERROR_INVALID_COMPSERV;
			stdstrMsg = Utility::string_format("Not found compserver(ID:%I64d)", nCheckInID);
			stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
			return GetStatusCode(nError);
		}
	}

	return GetStatusCode(RESTCOMPJOB_ERROR_SUCCESS);
}

unsigned __stdcall AllocJobStartThreadProc(void *pData)
{
	unsigned nRet = 0;

	if (pData)
	{
		int nError = 0;
		string stdstrMsg;

		CKRestCompJob* pCompJob = (CKRestCompJob*)pData;
		CKRestCompJobManager* pManager = (CKRestCompJobManager*)(pCompJob->GetParent());
		if (pManager)
		{
			JOBALLOCCB pFuncJobAlloc = NULL;
			void* pDataJobAlloc = NULL;
			{
				Utility::CAutoLock Lock(&g_mutexJobSync);
				pFuncJobAlloc = pCompJob->GetCompJobAllocCB(pDataJobAlloc);
			}

			if (pFuncJobAlloc)
			{
				int nChannel = -1;

				void* pJobData = NULL;
				{
					Utility::CAutoLock Lock(&g_mutexJobSync);
					nChannel = pCompJob->GetChannel();

					if (nError = pManager->StartCompJob(nChannel))
					{
						stdstrMsg = pManager->GetCompJobMessage(nChannel, nError);
						pCompJob->SetErrorMsg(nError, stdstrMsg);
					}
					else
						pJobData = pCompJob->ExtractAllocJobInfo();

					if (!pJobData)
					{
						pJobData = pCompJob->GetJobData();
						((CKRestCompJobData*)pJobData)->m_stdstrJobJson = pCompJob->GetAllocJobInfo();
						((CKRestCompJobData*)pJobData)->m_stdstrErrMsg = pCompJob->GetErrorMsg(((CKRestCompJobData*)pJobData)->m_nError);
					}
				}

				pFuncJobAlloc(pJobData, pDataJobAlloc);
			}
		}
	}

	return nRet;
}

int AllocJobCB(map<string, string>& mapPathVarValue, string stdstrQuery, string stdstrBodyJson, void* pData, string& stdstrResultJon)
{
	if (pData)
	{
		int nError = 0;
		string stdstrMsg;

		int nChannel = -1;
		long long nCheckInID = 0;
		CKRestCompJob* pCompJob = NULL;
		CKRestCompJobManager* pManager = NULL;

		if (mapPathVarValue.empty())
			return GetStatusCode(RESTCOMPJOB_ERROR_INVALID_PARAM);

		map<string, string>::iterator it = mapPathVarValue.begin();
		string stdstrCheckInID = it->second;
		stdstrCheckInID = Utility::ReplaceAll(stdstrCheckInID, "/", "");

		nCheckInID = atoll(stdstrCheckInID.c_str());

		pManager = (CKRestCompJobManager*)pData;

		Utility::CAutoLock Lock(&g_mutexJobSync);
		pCompJob = pManager->GetCompJob(nCheckInID);

		if (pCompJob)
		{
			nChannel = pCompJob->GetChannel();

			pCompJob->SetAllocJobInfo(stdstrBodyJson);
			if (nError = pCompJob->GetMamExStartJobData(stdstrResultJon))
			{
				stdstrMsg = pCompJob->GetErrorMsg(nError);
				stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
				return GetStatusCode(nError);
			}

			void* pJobData = pCompJob->GetJobData();
			if (!pJobData)
			{
				stdstrMsg = pCompJob->GetErrorMsg(nError);
				stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
				return GetStatusCode(nError);
			}

			((CKRestCompJobData*)pJobData)->m_stdstrQuery = stdstrQuery;

			stdstrResultJon = "";

			unsigned int uThreadID = 0;
			uintptr_t pThread = _beginthreadex(NULL, THREAD_STACK_SIZE, AllocJobStartThreadProc, (void*)pCompJob, 0, &uThreadID);

			if (!pThread)
			{
				stdstrMsg.assign(strerror(errno));
				stdstrResultJon = CKRestCompJob::GetErrorJson(errno, stdstrMsg);
				return GetStatusCode(RESTCOMPJOB_ERROR_BEGIN_JOB_THREAD);
			}
			
			stdstrResultJon = "{\"result\":\"success\"}";

			//void* pJobData = pCompJob->GetJobData();
			//if (!pJobData)
			//{
			//	stdstrMsg = pCompJob->GetErrorMsg(nError);
			//	stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
			//	return GetStatusCode(nError);
			//}

			//if (nError = pManager->StartCompJob(nChannel))
			//{
			//	stdstrMsg = pManager->GetCompJobMessage(nChannel);
			//	stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
			//	return GetStatusCode(nError);
			//}

			//pJobData = pCompJob->AnalyzeAllocJobInfo();
			//if (!pJobData)
			//{
			//	stdstrMsg = pCompJob->GetErrorMsg(nError);
			//	stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
			//	return GetStatusCode(nError);
			//}

			//pFuncJobAlloc = pCompJob->GetCompJobAllocCB(pDataJobAlloc);

			//if (pFuncJobAlloc && pJobData)
			//{
			//	((CKRestCompJobData*)pJobData)->m_stdstrQuery = stdstrQuery;
			//	pFuncJobAlloc(pManager->GetSessionID(), pJobData, pDataJobAlloc, nError, stdstrMsg);

			//	if (nError)
			//	{
			//		stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
			//		return GetStatusCode(nError);
			//	}
			//}
		}
		else
		{
			stdstrMsg = Utility::string_format("Not found compserver(ID:%I64d)", nCheckInID);
			stdstrResultJon = CKRestCompJob::GetErrorJson(RESTCOMPJOB_ERROR_INVALID_COMPSERV, stdstrMsg);
			return GetStatusCode(nError);
		}
	}

	return GetStatusCode(RESTCOMPJOB_ERROR_SUCCESS);
}

unsigned __stdcall AllocJobCancelThreadProc(void *pData)
{
	unsigned nRet = 0;

	if (pData)
	{
		int nError = 0;
		string stdstrMsg;

		CKRestCompJob* pCompJob = (CKRestCompJob*)pData;
		CKRestCompJobManager* pManager = (CKRestCompJobManager*)(pCompJob->GetParent());
		if (pManager)
		{
			JOBABORTCB pFuncJobAbort = NULL;
			void* pDataJobAbort = NULL;

			{
				Utility::CAutoLock Lock(&g_mutexJobSync);
				pFuncJobAbort = pCompJob->GetCompJobAllocCB(pDataJobAbort);
			}

			int nChannel = -1;
			void* pJobData = NULL;
			{
				Utility::CAutoLock Lock(&g_mutexJobSync);
				nChannel = pCompJob->GetChannel();

				if (nError = pManager->StartCompJob(nChannel))
				{
					stdstrMsg = pManager->GetCompJobMessage(nChannel, nError);
					pCompJob->SetErrorMsg(nError, stdstrMsg);
				}

				if (!pJobData)
				{
					pJobData = pCompJob->GetJobData();
					((CKRestCompJobData*)pJobData)->m_stdstrJobJson = pCompJob->GetAllocJobInfo();
					((CKRestCompJobData*)pJobData)->m_stdstrErrMsg = pCompJob->GetErrorMsg(((CKRestCompJobData*)pJobData)->m_nError);
				}
			}

			if (pFuncJobAbort)
			{
				pFuncJobAbort(pJobData, pDataJobAbort);
			}
		}
	}

	return nRet;
}

int JobCancelCB(map<string, string>& mapPathVarValue, string stdstrQuery, string stdstrBodyJson, void* pData, string& stdstrResultJon)
{
	if (pData)
	{
		int nError = 0;
		string stdstrMsg;

		JOBABORTCB pFuncJobAbort = NULL;
		void* pDataJobAbort = NULL;

		int nChannel = -1;
		long long nCheckInID = 0;
		long long nJobID = 0;
		CKRestCompJob* pCompJob = NULL;
		CKRestCompJobManager* pManager = NULL;

		if (mapPathVarValue.empty() || (mapPathVarValue.size() < 2))
			return GetStatusCode(RESTCOMPJOB_ERROR_INVALID_PARAM);

		map<string, string>::iterator it = mapPathVarValue.begin();
		string stdstrCheckInID = it->second;
		stdstrCheckInID = Utility::ReplaceAll(stdstrCheckInID, "/", "");

		nCheckInID = atoll(stdstrCheckInID.c_str());

		it++;
		string stdstrJobID = it->second;
		stdstrJobID = Utility::ReplaceAll(stdstrJobID, "/", "");
		nJobID = atoll(stdstrJobID.c_str());

		pManager = (CKRestCompJobManager*)pData;

		Utility::CAutoLock Lock(&g_mutexJobSync);
		pCompJob = pManager->GetCompJob(nCheckInID);

		if (pCompJob)
		{
			nChannel = pCompJob->GetChannel();

			void* pJobData = pCompJob->GetJobData();
			if (!pJobData)
			{
				stdstrMsg = pCompJob->GetErrorMsg(nError);
				stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
				return GetStatusCode(nError);
			}

			if (nJobID != ((CKRestCompJobData*)pJobData)->m_nJobID)
			{
				nError = RESTCOMPJOB_ERROR_INVALID_JOB;
				stdstrMsg = Utility::string_format("Invalid job id - %d", nJobID);
				stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
				return GetStatusCode(nError);
			}

			((CKRestCompJobData*)pJobData)->m_stdstrQuery = stdstrQuery;

			stdstrResultJon = "";

			unsigned int uThreadID = 0;
			uintptr_t pThread = _beginthreadex(NULL, THREAD_STACK_SIZE, AllocJobCancelThreadProc, (void*)pCompJob, 0, &uThreadID);

			if (!pThread)
			{
				stdstrMsg.assign(strerror(errno));
				stdstrResultJon = CKRestCompJob::GetErrorJson(errno, stdstrMsg);
				return GetStatusCode(RESTCOMPJOB_ERROR_CANCEL_JOB_THREAD);
			}

			stdstrResultJon = "{\"result\":\"success\"}";

			//if (nError = pManager->StartCompJob(nChannel))
			//{
			//	stdstrMsg = pManager->GetCompJobMessage(nChannel, nError);
			//	stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
			//	return GetStatusCode(nError);
			//}

			//pFuncJobAbort = pCompJob->GetCompJobAbortCB(pDataJobAbort);

			//if (pFuncJobAbort && pJobData)
			//{
			//	if (((CKRestCompJobData*)pJobData)->m_nJobID != nJobID)
			//		return GetStatusCode(RESTCOMPJOB_ERROR_INVALID_PARAM);
			//	
			//	pFuncJobAbort(pManager->GetSessionID(), pJobData, pDataJobAbort);

			//	if (nError)
			//	{
			//		stdstrResultJon = CKRestCompJob::GetErrorJson(nError, stdstrMsg);
			//		return GetStatusCode(nError);
			//	}
			//}
		}
		else
		{
			stdstrMsg = Utility::string_format("Not found compserver(ID:%I64d)", nCheckInID);
			stdstrResultJon = CKRestCompJob::GetErrorJson(RESTCOMPJOB_ERROR_INVALID_COMPSERV, stdstrMsg);
			return GetStatusCode(nError);
		}
	}

	return GetStatusCode(RESTCOMPJOB_ERROR_SUCCESS);
}

CKRestCompJobManager::CKRestCompJobManager(void)
{
	m_nSessionID = 0;
	m_nReceivePort = 0;

	//m_pRequestClient = new CRestClient();
	m_pReceiveServer = new CRestServer();
}

CKRestCompJobManager::~CKRestCompJobManager(void)
{
	//if (m_pRequestClient)
	//	delete m_pRequestClient;

	if (m_pReceiveServer)
		delete m_pReceiveServer;

	ReleaseCompJob();
}

void CKRestCompJobManager::SetMamConnInfo(string stdstrMamUri, string stdstrUserID, string stdstrPwd, string stdstrMamExUri)
{
	m_stdstrMamUri = stdstrMamUri;
	m_stdstrMamExUri = stdstrMamExUri;
	m_stdstrUserID = stdstrUserID;
	m_stdstrPwd = stdstrPwd;
}

void CKRestCompJobManager::SetReceiveServiceInfo(string stdstrReceiveUri)
{
	m_stdstrReceiveUri = stdstrReceiveUri;
	if (m_pReceiveServer)
	{
		m_pReceiveServer->SetServiceUri(m_stdstrReceiveUri);
		m_nReceivePort = m_pReceiveServer->GetServicePort();
	}
}

void CKRestCompJobManager::SetCompJobAllocCB(int nChannel, string stdstrCompType, JOBALLOCCB pFunc, void* pFuncData)
{
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];

	if (!pCompJob)
	{
		pCompJob = CreateComJob(stdstrCompType);

		if (pCompJob)
			m_mapCompJob[nChannel] = pCompJob;
	}

	pCompJob->SetCompJobAllocCB(pFunc, pFuncData);
}

void CKRestCompJobManager::SetCompJobAbortCB(int nChannel, string stdstrCompType, JOBABORTCB pFunc, void* pFuncData)
{
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];

	if (!pCompJob)
	{
		pCompJob = CreateComJob(stdstrCompType);

		if (pCompJob)
			m_mapCompJob[nChannel] = pCompJob;
	}

	pCompJob->SetCompJobAbortCB(pFunc, pFuncData);
}

int CKRestCompJobManager::ConnectMamServer()
{
	int nRet = 0;

	do
	{
		if (nRet = RunReceiveService())
			break;

		if (nRet = ConnectMam())
			break;

	} while (0);

	return nRet;
}

int CKRestCompJobManager::DisconnectMamServer()
{
	int nRet = 0;

	do
	{
		if (nRet = DisconnectMam())
			break;

		if (nRet = StopReceiveService())
			break;

	} while (0);

	return nRet;
}

int CKRestCompJobManager::CompJobCheckIn(int nPool, int nChannel, string stdstrCompType, string stdstrJobList)
{
	int nRet = RESTCOMPJOB_ERROR_SUCCESS;

	Utility::CAutoLock Lock(&g_mutexJobSync);
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (!pCompJob)
	{
		pCompJob = CreateComJob(stdstrCompType);

		if (pCompJob)
			m_mapCompJob[nChannel] = pCompJob;
		//return RESTCOMPJOB_ERROR_INVALID_PARAM;
	}

	pCompJob->SetCompJobInfo(nPool, nChannel, m_nReceivePort, stdstrCompType, stdstrJobList);

	string stdstrQuery = "";
	string stdstrBody = pCompJob->GetCheckInRequestData();

	CRestClient requestClient(m_stdstrMamUri);
	nRet = requestClient.Request(g_pairCompCheckIn.first, g_pairCompCheckIn.second, stdstrQuery, stdstrBody);
	printf("check in - %s\n", g_pairCompCheckIn.second.c_str());

	if (nRet == 200)
	{
		// status ok
		string stdstrResponseData = requestClient.GetResponseData();
		pCompJob->SetCheckInID(stdstrResponseData);

		long long nCheckInID = pCompJob->GetCheckInID();
		m_mapCheckInCompJob[nCheckInID] = pCompJob;

		printf("check in - ID:%I64d\n", nCheckInID);

		nRet = RESTCOMPJOB_ERROR_SUCCESS;
	}
	else
	{
		int nErrorCode = 0;
		string stdstrErrorMsg = requestClient.GetReason();
		if (stdstrErrorMsg.empty())
		{
			//stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData
			stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData(), nErrorCode);
			nRet = nErrorCode;
		}

		pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);

		//m_mapCompJobMessage[nChannel] = requestClient.GetReason();
		//string stdstrMamErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData());

		//if (m_mapCompJobMessage[nChannel].empty())
		//	m_mapCompJobMessage[nChannel] = stdstrMamErrorMsg;
	}

	return nRet;
}

int CKRestCompJobManager::CompJobCheckOut(int nChannel)
{
	int nRet = RESTCOMPJOB_ERROR_SUCCESS;

	Utility::CAutoLock Lock(&g_mutexJobSync);
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (!pCompJob)
	{
		return RESTCOMPJOB_ERROR_INVALID_PARAM;
	}

	string stdstrQuery = "";
	string stdstrBody = "";

	string stdstrCheckInID = Utility::string_format("%d", pCompJob->GetCheckInID());
	//stdstrCheckInID = "1";
	
	vector<string> vecVars;
	vecVars.push_back(stdstrCheckInID);

	CRestClient requestClient(m_stdstrMamUri);
	requestClient.SetResponseCB(g_pairCompCheckOut.first, g_pairCompCheckOut.second, NULL, NULL);

	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairCompCheckOut, vecVars);

	nRet = requestClient.Request(g_pairCompCheckOut.first, stdstrPath, stdstrQuery, stdstrBody);
	printf("check out - %s\n", stdstrPath.c_str());

	if (nRet == 200)
	{
		// status ok
		if (pCompJob)
		{
			pCompJob->SetCompJobAllocCB(NULL, NULL);
			pCompJob->SetCompJobAbortCB(NULL, NULL);
		}

		long long nCheckInID = pCompJob->GetCheckInID();
		m_mapCheckInCompJob.erase(nCheckInID);

		nRet = RESTCOMPJOB_ERROR_SUCCESS;
	}
	else
	{
		int nErrorCode = 0;
		string stdstrErrorMsg = requestClient.GetReason();
		if (stdstrErrorMsg.empty())
		{
			//stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData
			stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData(), nErrorCode);
			nRet = nErrorCode;
		}

		pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
	}

	return nRet;
}

int CKRestCompJobManager::CompJobCheckOut(int nChannel, long long nID)
{
	int nRet = RESTCOMPJOB_ERROR_SUCCESS;

	Utility::CAutoLock Lock(&g_mutexJobSync);

	string stdstrQuery = "";
	string stdstrBody = "";

	string stdstrCheckInID = Utility::string_format("%d", nID);
	//stdstrCheckInID = "1";

	vector<string> vecVars;
	vecVars.push_back(stdstrCheckInID);

	CRestClient requestClient(m_stdstrMamUri);
	requestClient.SetResponseCB(g_pairCompCheckOut.first, g_pairCompCheckOut.second, NULL, NULL);

	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairCompCheckOut, vecVars);

	nRet = requestClient.Request(g_pairCompCheckOut.first, stdstrPath, stdstrQuery, stdstrBody);
	printf("check out - %s\n", stdstrPath.c_str());

	if (nRet == 200)
	{
		nRet = RESTCOMPJOB_ERROR_SUCCESS;
	}
	else
	{
		int nErrorCode = 0;
		string stdstrErrorMsg = requestClient.GetReason();
		if (stdstrErrorMsg.empty())
		{
			//stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData
			stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData(), nErrorCode);
			nRet = nErrorCode;
		}
	}

	return nRet;
}

int CKRestCompJobManager::StartCompJob(int nChannel)
{
	int nRet = RESTCOMPJOB_ERROR_SUCCESS;

	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (pCompJob)
	{
		CKRestCompJobData* pCompJobData = (CKRestCompJobData*)pCompJob->GetJobData();
		if (pCompJobData)
		{
			string stdstrQuery = "";
			string stdstrBody = "";
			//if (nRet = pCompJob->GetMamExStartJobData(stdstrBody))
			//{
			//	m_mapCompJobMessage[nChannel] = pCompJob->GetErrorMsg(nRet);
			//}

			nRet = pCompJob->GetMamExStartJobData(stdstrBody);

			if (!stdstrBody.empty())
			{
				string stdstrMamExPool = Utility::string_format("%d", pCompJobData->m_nMamExPool);

				vector<string> vecVars;
				vecVars.push_back(pCompJobData->m_stdstrMamExStart);
				vecVars.push_back(stdstrMamExPool);

				CRestClient requestClient(m_stdstrMamExUri);
				requestClient.SetResponseCB(g_pairMamEx.first, g_pairMamEx.second, NULL, NULL);

				string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairMamEx, vecVars);

				int nRet = requestClient.Request(g_pairMamEx.first, stdstrPath, stdstrQuery, stdstrBody);
				printf("start compjob - %s\n", stdstrPath.c_str());

				if (nRet == 200)
				{
					// status ok
					pCompJob->SetAllocJobInfo(requestClient.GetResponseData());
					nRet = RESTCOMPJOB_ERROR_SUCCESS;
				}
				else
				{
					string stdstrErrorMsg = requestClient.GetReason();
					if (stdstrErrorMsg.empty())
						stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData());

					pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
				}

				return nRet;
			}
		}
	}

	return nRet;
}

int CKRestCompJobManager::EndCompJob(int nChannel, bool bSuccess, int nErrorCode, string stdstrErrMsg)
{
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (pCompJob)
	{
		CKRestCompJobData* pCompJobData = (CKRestCompJobData*)pCompJob->GetJobData();
		if (pCompJobData)
		{
			string stdstrQuery = "";
			string stdstrBody = pCompJob->GetMamExEndJobData(bSuccess, nErrorCode, stdstrErrMsg);
			if (!stdstrBody.empty())
			{
				string stdstrMamExPool = Utility::string_format("%d", pCompJobData->m_nMamExPool);

				vector<string> vecVars;
				vecVars.push_back(pCompJobData->m_stdstrMamExEnd);
				vecVars.push_back(stdstrMamExPool);

				CRestClient requestClient(m_stdstrMamExUri);
				requestClient.SetResponseCB(g_pairMamEx.first, g_pairMamEx.second, NULL, NULL);

				string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairMamEx, vecVars);

				int nRet = requestClient.Request(g_pairMamEx.first, stdstrPath, stdstrQuery, stdstrBody);
				printf("end compjob - %s\n", stdstrPath.c_str());

				if (nRet == 200)
				{
					// status ok
					nRet = RESTCOMPJOB_ERROR_SUCCESS;
				}
				else
				{
					string stdstrErrorMsg = requestClient.GetReason();
					if (stdstrErrorMsg.empty())
						stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData());

					pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
				}

				return nRet;
			}
		}
	}

	return RESTCOMPJOB_ERROR_SUCCESS;
}

int CKRestCompJobManager::ProgressCompJob(int nChannel, int nProgress, string stdstrExValue)
{
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (pCompJob)
	{
		CKRestCompJobData* pCompJobData = (CKRestCompJobData*)pCompJob->GetJobData();
		if (pCompJobData)
		{
			string stdstrQuery = "";
			string stdstrBody = pCompJob->GetMamExProgressData(nProgress, stdstrExValue);
			if (!stdstrBody.empty())
			{
				string stdstrMamExPool = Utility::string_format("%d", pCompJobData->m_nMamExPool);

				vector<string> vecVars;
				vecVars.push_back(pCompJobData->m_stdstrMamExProgress);
				vecVars.push_back(stdstrMamExPool);

				CRestClient requestClient(m_stdstrMamExUri);
				requestClient.SetResponseCB(g_pairMamEx.first, g_pairMamEx.second, NULL, NULL);

				string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairMamEx, vecVars);

				int nRet = requestClient.Request(g_pairMamEx.first, stdstrPath, stdstrQuery, stdstrBody);
				printf("progress compjob - %s(%s)\n", stdstrPath.c_str(), stdstrBody.c_str());

				if (nRet == 200)
				{
					// status ok
					nRet = RESTCOMPJOB_ERROR_SUCCESS;
				}
				else
				{
					string stdstrErrorMsg = requestClient.GetReason();
					if (stdstrErrorMsg.empty())
						stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData());

					pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
				}

				return nRet;
			}
		}
	}

	return RESTCOMPJOB_ERROR_SUCCESS;
}


int CKRestCompJobManager::ReportJobProgress(int nChannel, int nProgress, string stdstrExValue)
{
	int nRet = 0;

	Utility::CAutoLock Lock(&g_mutexJobSync);
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (!pCompJob)
	{
		return RESTCOMPJOB_ERROR_INVALID_PARAM;
	}

	if (nRet = ProgressCompJob(nChannel, nProgress, stdstrExValue))
		return nRet;

	string stdstrQuery = "";
	string stdstrBody = pCompJob->GetReportProgressData(nProgress, stdstrExValue);

	string stdstrCheckInID = Utility::string_format("%d", pCompJob->GetCheckInID());

	vector<string> vecVars;
	vecVars.push_back(stdstrCheckInID);

	CRestClient requestClient(m_stdstrMamUri);
	requestClient.SetResponseCB(g_pairCompReportProgress.first, g_pairCompReportProgress.second, NULL, NULL);

	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairCompReportProgress, vecVars);

	nRet = requestClient.Request(g_pairCompReportProgress.first, stdstrPath, stdstrQuery, stdstrBody);
	printf("report progress - %s(%s)\n", stdstrPath.c_str(), stdstrBody.c_str());

	if (nRet == 200)
	{
		// status ok
		nRet = RESTCOMPJOB_ERROR_SUCCESS;
	}
	else
	{
		string stdstrErrorMsg = requestClient.GetReason();
		if (stdstrErrorMsg.empty())
			stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData());

		pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
	}

	return nRet;
}

int CKRestCompJobManager::ReportJobResult(int nChannel, bool bSuccess, int nErrorCode, string stdstrErrMsg)
{
	int nRet = 0;

	Utility::CAutoLock Lock(&g_mutexJobSync);
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (!pCompJob)
	{
		return RESTCOMPJOB_ERROR_INVALID_PARAM;
	}

	nRet = EndCompJob(nChannel, bSuccess, nErrorCode, stdstrErrMsg);
	if (bSuccess && nRet)
	{
		if (stdstrErrMsg.empty())
			stdstrErrMsg = pCompJob->GetErrorMsg(nErrorCode);
		else
			stdstrErrMsg = stdstrErrMsg + "\n" +  pCompJob->GetErrorMsg(nErrorCode);

		bSuccess = false;
	}

	string stdstrQuery = "";
	string stdstrBody = pCompJob->GetReportResultData(bSuccess, nErrorCode, stdstrErrMsg);

	string stdstrCheckInID = Utility::string_format("%d", pCompJob->GetCheckInID());

	vector<string> vecVars;
	vecVars.push_back(stdstrCheckInID);

	CRestClient requestClient(m_stdstrMamUri);
	requestClient.SetResponseCB(g_pairCompReportResult.first, g_pairCompReportResult.second, NULL, NULL);

	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairCompReportResult, vecVars);

	nRet = requestClient.Request(g_pairCompReportResult.first, stdstrPath, stdstrQuery, stdstrBody);
	printf("report result - %s(%s)\n", stdstrPath.c_str(), stdstrBody.c_str());

	if (nRet == 200)
	{
		// status ok
		nRet = RESTCOMPJOB_ERROR_SUCCESS;
	}
	else
	{
		string stdstrErrorMsg = requestClient.GetReason();
		if (stdstrErrorMsg.empty())
			stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData());

		pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
	}

	void* pCompJobData = pCompJob->GetJobData();
	((CKRestCompJobData*)pCompJobData)->Clear();

	return nRet;
}

int CKRestCompJobManager::RunTransactionEx(int nChannel, CKRestExData* pExData)
{
	int nRet = 0;

	Utility::CAutoLock Lock(&g_mutexJobSync);
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (!pCompJob)
	{
		return RESTCOMPJOB_ERROR_INVALID_PARAM;
	}

	CKRestCompJobData* pCompJobData = (CKRestCompJobData*)pCompJob->GetJobData();

	string stdstrQuery = "";
	string stdstrBody = pExData->GetExTransactionData();
	string stdstrTrName = pExData->GetExTransactionName();
	string stdstrMamExPool = Utility::string_format("%d", pCompJobData->m_nMamExPool);
	stdstrMamExPool = "0";

	vector<string> vecVars;
	vecVars.push_back(stdstrTrName);
	vecVars.push_back(stdstrMamExPool);

	CRestClient requestClient(m_stdstrMamExUri);
	requestClient.SetResponseCB(g_pairMamEx.first, g_pairMamEx.second, NULL, NULL);

	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairMamEx, vecVars);

	nRet = requestClient.Request(g_pairMamEx.first, stdstrPath, stdstrQuery, stdstrBody);
	printf("run ex - %s(%s)\n", stdstrPath.c_str(), stdstrBody.c_str());

	if (nRet == 200)
	{
		// status ok
		nRet = RESTCOMPJOB_ERROR_SUCCESS;

		string stdstrErrorMsg = requestClient.GetResponseData();
		pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
	}
	else
	{
		string stdstrErrorMsg = requestClient.GetReason();
		if (stdstrErrorMsg.empty())
			stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData());

		pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
	}

	return nRet;
}

int CKRestCompJobManager::RunTransactionEx(int nChannel, string stdstrTransactionName, string stdstrData)
{
	int nRet = 0;

	Utility::CAutoLock Lock(&g_mutexJobSync);
	CKRestCompJob* pCompJob = m_mapCompJob[nChannel];
	if (!pCompJob)
	{
		return RESTCOMPJOB_ERROR_INVALID_PARAM;
	}

	CKRestCompJobData* pCompJobData = (CKRestCompJobData*)pCompJob->GetJobData();

	string stdstrQuery = "";
	string stdstrBody = stdstrData;
	string stdstrTrName = stdstrTransactionName;
	string stdstrMamExPool = Utility::string_format("%d", pCompJobData->m_nMamExPool);

	vector<string> vecVars;
	vecVars.push_back(stdstrTrName);
	vecVars.push_back(stdstrMamExPool);

	CRestClient requestClient(m_stdstrMamExUri);
	requestClient.SetResponseCB(g_pairMamEx.first, g_pairMamEx.second, NULL, NULL);

	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairMamEx, vecVars);

	nRet = requestClient.Request(g_pairMamEx.first, stdstrPath, stdstrQuery, stdstrBody);
	printf("run ex - %s(%s)\n", stdstrPath.c_str(), stdstrBody.c_str());

	if (nRet == 200)
	{
		// status ok
		nRet = RESTCOMPJOB_ERROR_SUCCESS;

		string stdstrErrorMsg = requestClient.GetResponseData();
		pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
	}
	else
	{
		string stdstrErrorMsg = requestClient.GetReason();
		if (stdstrErrorMsg.empty())
			stdstrErrorMsg = CKRestCompJob::GetMamErrorMsg(requestClient.GetResponseData());

		pCompJob->SetErrorMsg(nRet, stdstrErrorMsg);
	}

	return nRet;
}

bool CKRestCompJobManager::IsMamConnected()
{
	if (m_nSessionID > 0)
		return true;

	return false;
}

long long CKRestCompJobManager::GetSessionID()
{
	return m_nSessionID;
}

CKRestCompJob* CKRestCompJobManager::GetCompJob(int nChannel)
{
	return m_mapCompJob[nChannel];
}

CKRestCompJob* CKRestCompJobManager::GetCompJob(long long nCheckInID)
{
	return m_mapCheckInCompJob[nCheckInID];
}

string CKRestCompJobManager::GetCompJobInfo(int nChannel)
{
	string stdstrRet;

	CKRestCompJobData* pData = (CKRestCompJobData*)m_mapCompJob[nChannel]->GetJobData();
	stdstrRet = pData->m_stdstrJobJson;

	return stdstrRet;
}

string CKRestCompJobManager::GetCompJobMessage(int nChannel, int& nRetError)
{
	string stdstrRet;

	CKRestCompJob* pCompJob = NULL;
	pCompJob = m_mapCompJob[nChannel];
	if (pCompJob)
		stdstrRet = pCompJob->GetErrorMsg(nRetError);

	return stdstrRet;
}

int CKRestCompJobManager::ConnectMam()
{
	int nRet = 0;

	//string stdstrQuery = "";
	//string stdstrBody = "";

	//CRestClient requestClient(m_stdstrMamUri);
	//nRet = requestClient.Request(g_pairCompCheckOut.first, g_pairCompCheckOut.second, stdstrQuery, stdstrBody);

	return nRet;
}

int CKRestCompJobManager::DisconnectMam()
{
	int nRet = 0;

	//string stdstrQuery = "";
	//string stdstrBody = "";

	//CRestClient requestClient(m_stdstrMamUri);
	//nRet = requestClient.Request(g_pairCompCheckOut.first, g_pairCompCheckOut.second, stdstrQuery, stdstrBody);

	return nRet;
}

int CKRestCompJobManager::RunReceiveService()
{
	int nRet = 0;
	
	if (m_pReceiveServer)
	{
		m_pReceiveServer->SetRequestCB(g_pairCompReceivePing.first, g_pairCompReceivePing.second, ReceivePingCB, this);
		m_pReceiveServer->SetRequestCB(g_pairCompReceiveJob.first, g_pairCompReceiveJob.second, AllocJobCB, this);
		m_pReceiveServer->SetRequestCB(g_pairCompReceiveJobCancel.first, g_pairCompReceiveJobCancel.second, JobCancelCB, this);

		nRet = m_pReceiveServer->Start();
	}

	return nRet;
}

int CKRestCompJobManager::StopReceiveService()
{
	int nRet = 0;

	if (m_pReceiveServer)
		nRet = m_pReceiveServer->Stop();

	return nRet;
}

void CKRestCompJobManager::ReleaseCompJob()
{
	int nCount = (int)m_mapCompJob.size();

	if (nCount)
	{
		CKRestCompJob* pCompJob = NULL;

		map<int, CKRestCompJob*>::iterator it = m_mapCompJob.begin();
		for (it; it != m_mapCompJob.end(); it++)
		{
			pCompJob = NULL;
			pCompJob = it->second;

			if (pCompJob)
			{
				delete pCompJob;
				it->second = NULL;
			}
		}
		
		m_mapCompJob.clear();
	}
}

CKRestCompJob* CKRestCompJobManager::CreateComJob(string stdstrCompType)
{
	CKRestCompJob* pCompJob = NULL;
	
	if (!stdstrCompType.compare(COMPONENT_TYPE_TRANSFERMANAGER))
		pCompJob = new CKRestTMCompJob();

	if (!stdstrCompType.compare(COMPONENT_TYPE_TRANSCODER))
		pCompJob = new CKRestTCCompJob();

	if (!stdstrCompType.compare(COMPONENT_TYPE_CATALOGER))
		pCompJob = new CKRestCACompJob();

	if (!stdstrCompType.compare(COMPONENT_TYPE_ARCHIVEMANAGER))
		pCompJob = new CKRestAMCompJob();

	pCompJob->SetParent((void*)this);

	return pCompJob;
}