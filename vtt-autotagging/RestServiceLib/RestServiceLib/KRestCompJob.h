
#pragma once

#include "KRestCompJobManager.h"
#include "KRestCompJobData.h"


#include <vector>

using namespace std;

class CKRestCompJob 
{
public:
	CKRestCompJob();
	CKRestCompJob(int nChannel);
	CKRestCompJob(int nPool, int nChannel, int nServicePort, string stdstrCompType, string stdstrJobList);
	virtual ~CKRestCompJob();

	void SetCompJobInfo(int nPool, int nChannel, int nServicePort, string stdstrCompType, string stdstrJobList);
	void SetCompJobList(string stdstrJobList);
	void SetCompJobAllocCB(JOBALLOCCB pFunc, void* pFuncData);
	void SetCompJobAbortCB(JOBABORTCB pFunc, void* pFuncData);

public:
	void SetParent(void* pParent) { m_pParent = pParent; };
	void* GetParent() { return m_pParent; };

	int SetCheckInID(string stdstrResponseData);

	int GetChannel() { return m_nChannel; };
	long long GetCheckInID() { return m_nCheckInID; };

	string GetCompType();
	string GetCompJobList();
	string GetCheckInRequestData();
	string GetCheckOutRequestData();
	string GetAllocJobInfo() { return m_stdstrJasonJobInfo; };

	void SetErrorMsg(int nError, string stdstrMsg) { m_nError = nError; m_stdstrErrorMsg = stdstrMsg; };
	string GetErrorMsg(int& nError);

	virtual void SetAllocJobInfo(string stdstrJasonJobInfo);
	virtual void* ExtractAllocJobInfo();
	virtual void ParseJobData();

	virtual void* GetJobData();

	virtual int GetMamExStartJobData(string& stdstrJson);
	virtual string GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg);
	virtual string GetMamExProgressData(int nProgress, string stdstrExValue);
	virtual string GetReportProgressData(int nProgress, string stdstrExValue);
	virtual string GetReportResultData(bool bSuccess, int nErrorCode, string stdstrErrMsg);

	JOBALLOCCB GetCompJobAllocCB(void*& pDataJobAlloc);
	JOBABORTCB GetCompJobAbortCB(void*& pDataJobAbort);

	static string GetMamErrorMsg(string stdstrJsonData);
	static string GetMamErrorMsg(string stdstrJsonData, int& nErrorCode);
	static string GetErrorJson(int nError, string stdstrMsg);

	bool IsAssignedJob();
	void SetIsAssignedJob(bool bAssigned);

protected:
	bool m_bAssigned;
	
	long long m_nAssignedJobID;

protected:
	int m_nPool;
	int m_nChannel;
	int m_nServicePort;

	int m_nError;
	int m_nMamErrorCode;

	long long m_nCheckInID;
	long long m_nJobID;

	string m_stdstrCompType;
	string m_stdstrJasonJobInfo;
	string m_stdstrErrorMsg;

	vector<string> m_vecJobList;

	JOBALLOCCB m_pfJobAllocCB;
	JOBABORTCB m_pfJobAbortCB;

	void* m_pDataJobAllocCB;
	void* m_pDataJobAbortCB;

	void* m_pParent;
};

class CKRestTCCompJob : public CKRestCompJob
{
public:
	CKRestTCCompJob();
	virtual ~CKRestTCCompJob();

	virtual void SetAllocJobInfo(string stdstrJasonJobInfo);
	virtual void* ExtractAllocJobInfo();
	virtual void ParseJobData();

	virtual void* GetJobData();
	virtual int GetMamExStartJobData(string& stdstrJson);
	virtual string GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg);
	virtual string GetMamExProgressData(int nProgress, string stdstrExValue);

public:
	CKRestTCJobData m_TCJobData;
};

class CKRestTMCompJob : public CKRestCompJob
{
public:
	CKRestTMCompJob();
	virtual ~CKRestTMCompJob();

	virtual void SetAllocJobInfo(string stdstrJasonJobInfo);
	virtual void* ExtractAllocJobInfo();

	virtual void* GetJobData();
	virtual int GetMamExStartJobData(string& stdstrJson);
	virtual string GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg);
	virtual string GetMamExProgressData(int nProgress, string stdstrExValue);

protected:
	void ExtractJobDataDiskToFTP(void* pJsonValue);
	void ExtractJobDataDiskToDISK(void* pJsonValue);
	void ExtractJobDataFTPToFTP(void* pJsonValue);
	void ExtractJobDataDiskSrc(void* pJsonValue, void* pSrcData);
	void ExtractJobDataFTPSrc(void* pJsonValue, void* pSrcData);
	void ExtractJobDataFTPDest(void* pJsonValue, void* pDestData);
	void ExtractJobDataDiskDest(void* pJsonValue, void* pDestData);

public:
	CKRestTMJobData m_TMJobData;

};

class CKRestCACompJob : public CKRestCompJob
{
public:
	CKRestCACompJob();
	virtual ~CKRestCACompJob();

	virtual void SetAllocJobInfo(string stdstrJasonJobInfo);
	virtual void* ExtractAllocJobInfo();
	virtual void ParseJobData();

	virtual void* GetJobData();
	virtual int GetMamExStartJobData(string& stdstrJson);
	virtual string GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg);
	virtual string GetMamExProgressData(int nProgress, string stdstrExValue);

public:
	CKRestCAJobData m_CAJobData;

};

class CKRestAMCompJob : public CKRestCompJob
{
public:
	CKRestAMCompJob();
	virtual ~CKRestAMCompJob();

	virtual void SetAllocJobInfo(string stdstrJasonJobInfo);
	virtual void* ExtractAllocJobInfo();

	virtual void* GetJobData();
	virtual int GetMamExStartJobData(string& stdstrJson);
	virtual string GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg);
	virtual string GetMamExProgressData(int nProgress, string stdstrExValue);

protected:
	void ExtractJobDataArchiveDiskSrc(void* pJsonValue);
	void ExtractJobDataArchiveDTLDest(void* pJsonValue);
	void ExtractJobDataRestoreDiskSrc(void* pJsonValue);
	void ExtractJobDataRestoreDTLSrc(void* pJsonValue);
	void ExtractJobDataPartialRestoreDiskSrc(void* pJsonValue);
	void ExtractJobDataPartialRestoreDTLSrc(void* pJsonValue);
	void ExtractJobDataRestoreDiskDest(void* pJsonValue);
	void ExtractJobDataDeleteDTLInstance(void* pJsonValue);

public:
	CKRestAMJobData m_AMJobData;
};

