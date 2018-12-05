// KRestCompJobManager.h : KRestCompJobManager 헤더 파일입니다.
//
#pragma once

#include <map>

using namespace std;

#ifdef RESTCOMPJOBMANAGER_EXPORTS
#define RESTCOMPJOBMANAGER_API __declspec(dllexport)
#else
#define RESTCOMPJOBMANAGER_API __declspec(dllimport)
#endif

#define COMPONENT_TYPE_TRANSFERMANAGER				("transfermanager")
#define COMPONENT_TYPE_TRANSCODER					("transcoder")
#define COMPONENT_TYPE_CATALOGER					("cataloger")
#define COMPONENT_TYPE_ARCHIVEMANAGER				("archivemanager")

#define RESTCOMPJOB_ERROR_SUCCESS					 (0)
#define RESTCOMPJOB_ERROR_INVALID_COMPSERV			 (100001)
#define RESTCOMPJOB_ERROR_INVALID_PARAM				 (100002)
#define RESTCOMPJOB_ERROR_INVALID_JSON				 (100003)
#define RESTCOMPJOB_ERROR_BEGIN_JOB_THREAD			 (100004)
#define RESTCOMPJOB_ERROR_CANCEL_JOB_THREAD			 (100005)
#define RESTCOMPJOB_ERROR_INVALID_JOB				 (100006)

typedef void(*JOBALLOCCB)(void* pJobData, void* pDataCB);
typedef void(*JOBABORTCB)(void* pJobData, void* pDataCB);

class CRestClient;
class CRestServer;
class CKRestCompJob;
class CKRestExData;

class RESTCOMPJOBMANAGER_API CKRestCompJobManager
{
public:
	CKRestCompJobManager(void);
	virtual ~CKRestCompJobManager(void);

	void SetMamConnInfo(string stdstrMamUri, string stdstrUserID, string stdstrPwd, string stdstrMamExUri);
	void SetReceiveServiceInfo(string stdstrReceiveUri);
	void SetCompJobAllocCB(int nChannel, string stdstrCompType, JOBALLOCCB pFunc, void* pFuncData);
	void SetCompJobAbortCB(int nChannel, string stdstrCompType, JOBABORTCB pFunc, void* pFuncData);

	int ConnectMamServer();
	int DisconnectMamServer();

	int CompJobCheckIn(int nPool, int nChannel, string stdstrCompType, string stdstrJobList);
	int CompJobCheckOut(int nChannel);
	int CompJobCheckOut(int nChannel, long long nID);

	int StartCompJob(int nChannel);
	int EndCompJob(int nChannel, bool bSuccess, int nErrorCode, string stdstrErrMsg);
	int ProgressCompJob(int nChannel, int nProgress, string stdstrExValue);
	int ReportJobProgress(int nChannel, int nProgress, string stdstrExValue);
	int ReportJobResult(int nChannel, bool bSuccess, int nErrorCode, string stdstrErrMsg);
	int RunTransactionEx(int nChannel, CKRestExData* pExData);
	int RunTransactionEx(int nChannel, string stdstrTransactionName, string stdstrData);

	bool IsMamConnected();

	long long GetSessionID();
	CKRestCompJob* GetCompJob(int nChannel);
	CKRestCompJob* GetCompJob(long long nCheckInID);
	string GetCompJobInfo(int nChannel);

	string GetCompJobMessage(int nChannel, int& nRetError);

	// test
	//int RunReceiveService();
	//int StopReceiveService();
protected:
	int ConnectMam();
	int DisconnectMam();

	int RunReceiveService();
	int StopReceiveService();

	void ReleaseCompJob();

	CKRestCompJob* CreateComJob(string stdstrCompType);

private:
	long long m_nSessionID;

	int m_nReceivePort;

	string m_stdstrMamUri;
	string m_stdstrMamExUri;
	string m_stdstrUserID;
	string m_stdstrPwd;
	string m_stdstrReceiveUri;

	time_t m_timeLastReceivedPing;

private:
	map<int, CKRestCompJob*> m_mapCompJob;
	map<long long, CKRestCompJob*> m_mapCheckInCompJob;
	map<int, string> m_mapCompJobMessage;

	CRestServer* m_pReceiveServer;
};
