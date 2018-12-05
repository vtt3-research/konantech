#pragma once
#include "struct.h"
#include "Def.h"
#include "KException.h"

//#include "include/KSIMCommonDefs.h"
#include "include/KRestCompJobData.h"
#include "include/KRestCompJobManager.h"

#include "include/json/json.h"

#define EX_TRANSACTION_NAME_UPDATE_JOBRESULT		("ex_update_jobresult")
#define	STR_CATALOGER_TYPE			_T("cataloger")

#define CATALOGING_JOB_RESULT_COMPLETE	(0)
#define CATALOGING_JOB_RESULT_ABORT		(1)
#define CATALOGING_JOB_RESULT_ERROR		(2)
#define CATALOGING_JOB_RESULT_RETRY		(3)

typedef void(*CALLBACK_CONNECT) (bool, LPVOID);
typedef void(*CALLBACK_ABORT) (int, LPVOID);
typedef void(*CALLBACK_DISCONNECT) (LPVOID);
typedef void(*CALLBACK_ALLOCJOB) (bool, LPVOID);

class CConnectCMS
{

public:

	CConnectCMS();
	~CConnectCMS();

public:

	static BOOL OpenCompJobManager(CString strMamUri, CString strMamExUri, CString strReceiveUri, CString strUser, CString strPasswd);
	static void CloseCompJobManager();
	static void pFuncJobAlloc(void* pJobData, void* pDataCB);
	static void pFuncJobAbort(void* pJobData, void* pDataCB);

	bool RegisterWorkerToMam(CMS_INFO cms_info, CString strJobList, bool bStandAlone);

	void ConnectCMS();
	bool DisconnectCMS();
	bool DisconnectCMS(CMS_INFO cmsInfo, __int64 nID);

	bool UpdateShotInfoEx(int lWidth, int lHeight, double dbFPS, CString strDuration, void* pShotInfo);
	
	void Callback_Connect(CALLBACK_CONNECT handler, LPVOID lpvoid);
	void Callback_Disconnect(CALLBACK_DISCONNECT handler, LPVOID lpvoid);
	void CallbackJobAlloc(CALLBACK_ALLOCJOB handler, LPVOID lpvoid);
	void CallbackJobAbort(CALLBACK_ABORT handler, LPVOID lpvoid);

	void JobAllocProc(void* pJobData);
	void ReportPregress(int nProgress);
	void ReportResult(int nResult, CString strComment);

	JOB_INFO GetJobInfo() { return m_JobInfo; }
	KVPAIR GetKvPair() { return m_Kvpair; }
	MAMEX GetMamEx() { return m_mamex; }

protected:
	
	void jobAbort(void* pJobData);
	void jobInfoClear();

	static void writeLog(CString strMsg);

private:

	static CKRestCompJobManager* g_pCompJobManager;

	CALLBACK_ALLOCJOB callback_job;
	LPVOID m_lpAllocJobData;

	CALLBACK_CONNECT callback_connect;
	LPVOID m_lpvoid;

	CALLBACK_ABORT callback_abort;
	LPVOID m_lpAbortData;

	CALLBACK_DISCONNECT callback_disconnect;
	LPVOID m_lpDisconnData;

	int m_nJobID;
	
	bool m_bIsAcceptJob;
	bool m_bConnect;
	
	CMS_INFO m_cmsInfo;
	JOB_INFO m_JobInfo;
	KVPAIR m_Kvpair;
	MAMEX m_mamex;

	CString m_strJobList;
};

