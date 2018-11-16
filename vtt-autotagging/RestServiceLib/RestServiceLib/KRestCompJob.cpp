
#include "stdafx.h"
#include "KRestCompJob.h"

#include "Utility.h"

#include <cpprest/json.h>

using namespace web;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define COMPJOB_JSON_KEY_CHEKCIN_ID					(L"compserverId")

#define COMPJOB_JSON_KEY_WORKFLOWID					(L"workflowid")
#define COMPJOB_JSON_KEY_WORKFLOWNAME				(L"workflowname")
#define COMPJOB_JSON_KEY_JOBID						(L"jobid")
#define COMPJOB_JSON_KEY_JOBNAME					(L"jobname")
#define COMPJOB_JSON_KEY_JOBTYPE					(L"jobtype")
#define COMPJOB_JSON_KEY_MAMEX						(L"mamex")
#define COMPJOB_JSON_KEY_MAMEX_START				(L"start")
#define COMPJOB_JSON_KEY_MAMEX_PROGRESS				(L"progress")
#define COMPJOB_JSON_KEY_MAMEX_END					(L"end")
#define COMPJOB_JSON_KEY_MAMEX_POOL					(L"pool")
#define COMPJOB_JSON_KEY_JOBINFOS					(L"jobinfos")
#define COMPJOB_JSON_KEY_CONVERTVIDEOSRC			(L"convertvideosrc")
#define COMPJOB_JSON_KEY_STORAGE					(L"storage")
#define COMPJOB_JSON_KEY_ASSETID					(L"assetid")
#define COMPJOB_JSON_KEY_VIDEOWRAPPERTYPE			(L"videowrappertype")
#define COMPJOB_JSON_KEY_FILEPATH					(L"filepath")
#define COMPJOB_JSON_KEY_PROXYVIDEOS				(L"proxyvideos")
#define COMPJOB_JSON_KEY_MOBTYPE					(L"mobtype")
#define COMPJOB_JSON_KEY_RESULT						(L"result")
#define COMPJOB_JSON_KEY_MESSAGE					(L"message")
#define COMPJOB_JSON_KEY_OBJECTID					(L"objectid")
#define COMPJOB_JSON_KEY_ISFOLDER					(L"isfolder")
#define COMPJOB_JSON_KEY_ISDELETE					(L"isdelete")
#define COMPJOB_JSON_KEY_ISZIP						(L"iszip")
#define COMPJOB_JSON_KEY_EXCEPTFILEPATH				(L"exceptfilepath")
#define COMPJOB_JSON_KEY_FILERENAME					(L"filerename")
#define COMPJOB_JSON_KEY_ADDRESS					(L"address")
#define COMPJOB_JSON_KEY_PORT						(L"port")
#define COMPJOB_JSON_KEY_USERNAME					(L"username")
#define COMPJOB_JSON_KEY_PASSWORD					(L"password")
#define COMPJOB_JSON_KEY_DISPLAYDESTFILEPATH		(L"displaydestfilepath")
#define COMPJOB_JSON_KEY_ALLOWEXISTFILE				(L"allowexistfile")
#define COMPJOB_JSON_KEY_SERVERTYPE					(L"servertype")
#define COMPJOB_JSON_KEY_FILELIST					(L"filelist")
#define COMPJOB_JSON_KEY_COMPRESSIONTYPE			(L"compressiontype")
#define COMPJOB_JSON_KEY_WRAPPERTYPE				(L"wrappertype")
#define COMPJOB_JSON_KEY_STARTFRAMEINDEX			(L"startframeindex")
#define COMPJOB_JSON_KEY_ENDFRAMEINDEX				(L"endframeindex")
#define COMPJOB_JSON_KEY_WINVOLUME					(L"volumewin") // win
#define COMPJOB_JSON_KEY_ETCVOLUME					(L"volumeetc") // win 외

#define COMPJOB_JSON_KEY_TRANSFER_DISKTOFTP			(L"transferdisktoftp")
#define COMPJOB_JSON_KEY_TRANSFER_DISKTODISK		(L"transferdisktodisk")
#define COMPJOB_JSON_KEY_TRANSFER_FTPTODISK			(L"transferftptodisk")
#define COMPJOB_JSON_KEY_TRANSFER_FTPTOFTP			(L"transferftptoftp")
#define COMPJOB_JSON_KEY_DELETEFILE_IN_DISK			(L"deletediskinstance")
#define COMPJOB_JSON_KEY_DELETEFILE_IN_FTP			(L"deleteftpinstance")
#define COMPJOB_JSON_KEY_GET_VOLUME_INFO			(L"getvolumeinfo")
#define COMPJOB_JSON_KEY_GET_FILE_INFO				(L"getfileinfo")
#define COMPJOB_JSON_KEY_TRANSFER_DISKSRC			(L"disksrc")
#define COMPJOB_JSON_KEY_TRANSFER_FTPSRC			(L"ftpsrc")
#define COMPJOB_JSON_KEY_TRANSFER_FTPDEST			(L"ftpdest")
#define COMPJOB_JSON_KEY_TRANSFER_DISKDEST			(L"diskdest")

#define COMPJOB_JSON_KEY_CATALOGVIDEOSRC			(L"catalogvideosrc")
#define COMPJOB_JSON_KEY_VIDEOFRAMERATE				(L"videoframerate")
#define COMPJOB_JSON_KEY_CATALOGSHOTIMAGE			(L"shotimages")

#define COMPJOB_JSON_KEY_ARCHIVE_DISKSRC			(L"archivedisksrc")
#define COMPJOB_JSON_KEY_ARCHIVE_DTLDEST			(L"archivedtldest")
#define COMPJOB_JSON_KEY_RESTORE_DISKSRC			(L"restoredisksrc")
#define COMPJOB_JSON_KEY_RESTORE_DTLSRC				(L"restoredtlsrc")
#define COMPJOB_JSON_KEY_RESTORE_DISKDEST			(L"restorediskdest")
#define COMPJOB_JSON_KEY_DELETEINSTANCE_IN_DTL		(L"deletedtlinstance")
#define COMPJOB_JSON_KEY_DTLGROUP					(L"dtlgroup")
#define COMPJOB_JSON_KEY_DTLCATEGORY				(L"dtlcategory")

#define COMPJOB_SUCCESS								(L"success")
#define COMPJOB_FAIL								(L"fail")


CKRestCompJob::CKRestCompJob()
{
	m_bAssigned = false;
	m_nCheckInID = 0;
	m_nJobID = 0;
	m_nError = 0;

	SetCompJobInfo(-1, -1, 0, "", "");

	m_pfJobAllocCB = NULL;
	m_pfJobAbortCB = NULL;
	m_pDataJobAllocCB = NULL;
	m_pDataJobAbortCB = NULL;
}

CKRestCompJob::CKRestCompJob(int nChannel)
{
	m_bAssigned = false;
	m_nCheckInID = 0;
	m_nJobID = 0;
	m_nError = 0;

	SetCompJobInfo(-1, nChannel, 0, "", "");

	m_pfJobAllocCB = NULL;
	m_pfJobAbortCB = NULL;
	m_pDataJobAllocCB = NULL;
	m_pDataJobAbortCB = NULL;
}

CKRestCompJob::CKRestCompJob(int nPool, int nChannel, int nServicePort, string stdstrCompType, string stdstrJobList)
{
	m_bAssigned = false;
	m_nCheckInID = 0;
	m_nJobID = 0;
	m_nError = 0;

	SetCompJobInfo(nPool, nChannel, nServicePort, stdstrCompType, stdstrJobList);

	m_pfJobAllocCB = NULL;
	m_pfJobAbortCB = NULL;
	m_pDataJobAllocCB = NULL;
	m_pDataJobAbortCB = NULL;
}

CKRestCompJob::~CKRestCompJob()
{

}

void CKRestCompJob::SetCompJobInfo(int nPool, int nChannel, int nServicePort, string stdstrCompType, string stdstrJobList)
{
	m_nPool = nPool;
	m_nChannel = nChannel;
	m_nServicePort = nServicePort;

	m_stdstrCompType = stdstrCompType;
	SetCompJobList(stdstrJobList);
}

void CKRestCompJob::SetCompJobList(string stdstrJobList)
{
	m_vecJobList.clear();
	Utility::Tokenize(stdstrJobList, m_vecJobList, " ,");
}

void CKRestCompJob::SetCompJobAllocCB(JOBALLOCCB pFunc, void* pFuncData)
{
	m_pfJobAllocCB = pFunc;
	m_pDataJobAllocCB = pFuncData;
}

void CKRestCompJob::SetCompJobAbortCB(JOBABORTCB pFunc, void* pFuncData)
{
	m_pfJobAbortCB = pFunc;
	m_pDataJobAbortCB = pFuncData;
}

int CKRestCompJob::SetCheckInID(string stdstrResponseData)
{
	int nRet = RESTCOMPJOB_ERROR_SUCCESS;

	utility::string_t stdstrData = utility::conversions::utf8_to_utf16(stdstrResponseData);

	try
	{
		json::value jsonData = json::value::parse(stdstrData);
		m_nCheckInID = jsonData[COMPJOB_JSON_KEY_CHEKCIN_ID].as_integer();
	}
	catch (web::json::json_exception const & e)
	{
		m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();
		m_stdstrErrorMsg.assign(e.what());
	}

	return nRet;
}

string CKRestCompJob::GetCompType()
{
	return m_stdstrCompType;
}

string CKRestCompJob::GetCompJobList()
{
	string stdstrRet;

	vector<string>::iterator it = m_vecJobList.begin();
	for (it; it != m_vecJobList.end(); it++)
	{
		if (it == m_vecJobList.begin())
		{
			stdstrRet = "\"" + *it + "\"";

			continue;
		}

		stdstrRet = stdstrRet.append(",\"") + *it + "\"";
	}

	return stdstrRet;
}

string CKRestCompJob::GetCheckInRequestData()
{
	string stdstrRet;
	string stdstrJobList = GetCompJobList();

	stdstrRet = Utility::string_format(
		"{\
		\"compservername\":\"%s\",\
		\"channel\":%d,\
		\"pool\":%d,\
		\"jobnames\":[%s],\
		\"port\" : %d\
		}", m_stdstrCompType.c_str(), m_nChannel, m_nPool, stdstrJobList.c_str(), m_nServicePort);

	return stdstrRet;
}

string CKRestCompJob::GetCheckOutRequestData()
{
	string stdstrRet;

	stdstrRet = Utility::string_format(
		"{\
		\"compservername\":\"%s\",\
		\"channel\":%d,\
		\"pool\":%d\
		}", m_stdstrCompType, m_nChannel, m_nPool);

	return stdstrRet;
}

string CKRestCompJob::GetErrorMsg(int& nError)
{
	nError = m_nError;

	return m_stdstrErrorMsg;
}

void CKRestCompJob::SetAllocJobInfo(string stdstrJasonJobInfo)
{
	m_nJobID = 0;
	m_stdstrJasonJobInfo = stdstrJasonJobInfo;
}

void* CKRestCompJob::ExtractAllocJobInfo()
{
	void* pRet = NULL;
	return pRet;
}

void CKRestCompJob::ParseJobData()
{

}

void* CKRestCompJob::GetJobData()
{
	void* pRet = NULL;
	return pRet;
}

int CKRestCompJob::GetMamExStartJobData(string& stdstrJson)
{
	int nRet = 0;

	return nRet;
}

string CKRestCompJob::GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg)
{
	string stdstrRet;

	return stdstrRet;
}

string CKRestCompJob::GetMamExProgressData(int nProgress, string stdstrExValue)
{
	string stdstrRet;

	stdstrRet = GetReportProgressData(nProgress, stdstrExValue);

	return stdstrRet;
}

string CKRestCompJob::GetReportProgressData(int nProgress, string stdstrExValue)
{
	string stdstrRet;

	stdstrRet = Utility::string_format(
		"{\
		\"jobid\":%I64d,\
		\"progress\":%d,\
		\"exvalue\":\"%s\"\
		}", m_nJobID, nProgress, stdstrExValue.c_str());

	return stdstrRet;
}

string CKRestCompJob::GetReportResultData(bool bSuccess, int nErrorCode, string stdstrErrMsg)
{
	string stdstrRet;

	string stdstrSuccess("success");
	if (!bSuccess)
	{
		stdstrSuccess.clear();
		stdstrSuccess.assign("fail");
	}

	stdstrRet = Utility::string_format(
		"{\
		\"jobid\":%I64d,\
		\"result\":\"%s\",\
		\"message\":\"%s\"\
		}", m_nJobID, stdstrSuccess.c_str(), stdstrErrMsg.c_str());

	return stdstrRet;
}

JOBALLOCCB CKRestCompJob::GetCompJobAllocCB(void*& pDataJobAlloc)
{
	pDataJobAlloc = m_pDataJobAllocCB;

	return m_pfJobAllocCB;
}

JOBABORTCB CKRestCompJob::GetCompJobAbortCB(void*& pDataJobAbort)
{
	pDataJobAbort = m_pDataJobAbortCB;

	return m_pfJobAbortCB;
}

string CKRestCompJob::GetMamErrorMsg(string stdstrJsonData)
{
	//int ERR_7010_COMPSERVER_ERROR = 7010; // 콤포넌트 오류
	//{
	//	"message": "checkout 오류 입니다 ( 콤포넌트서버가 없습니다 )",
	//		"code" : 7010,
	//		"status" : 400,
	//		"timestamp" : 1520326244636,
	//		"path" : "checkout",
	//		"exception" : "com.konantech.spring.controller.rest.WorkflowController",
	//		"error" : null
	//}

	string stdstrRet;

	utility::string_t strData = utility::conversions::utf8_to_utf16(stdstrJsonData);
	if (!strData.empty())
	{
		try
		{
			json::value jData = json::value::parse(strData);

			if (jData.has_field(L"message"))
			{
				stdstrRet = utility::conversions::utf16_to_utf8(jData[L"message"].as_string());
				stdstrRet = Utility::string_format("message: %s", stdstrRet.c_str());
			}
		}
		catch (web::json::json_exception const & e)
		{
			printf("%s\n", e.what());
			stdstrRet = stdstrJsonData;
		}
	}

	return stdstrRet;
}

string CKRestCompJob::GetMamErrorMsg(string stdstrJsonData, int& nErrorCode)
{
	//int ERR_7010_COMPSERVER_ERROR = 7010; // 콤포넌트 오류
	//{
	//	"message": "checkout 오류 입니다 ( 콤포넌트서버가 없습니다 )",
	//		"code" : 7010,
	//		"status" : 400,
	//		"timestamp" : 1520326244636,
	//		"path" : "checkout",
	//		"exception" : "com.konantech.spring.controller.rest.WorkflowController",
	//		"error" : null
	//}

	nErrorCode = -1;
	string stdstrRet;

	utility::string_t strData = utility::conversions::utf8_to_utf16(stdstrJsonData);
	if (!strData.empty())
	{
		try
		{
			json::value jData = json::value::parse(strData);

			if (jData.has_field(L"code"))
			{
				nErrorCode = jData[L"code"].as_integer();
				stdstrRet = Utility::string_format("error: %d", nErrorCode);

				if (jData.has_field(L"message"))
				{
					stdstrRet = utility::conversions::utf16_to_utf8(jData[L"message"].as_string());
					stdstrRet = Utility::string_format("error: %d - message: %s", nErrorCode, stdstrRet.c_str());
				}
			}
			else if (jData.has_field(L"message"))
			{
				stdstrRet = Utility::string_format("message: %s", stdstrRet.c_str());
			}
		}
		catch (web::json::json_exception const & e)
		{
			printf("%s\n", e.what());
			stdstrRet = stdstrJsonData;
		}
	}

	return stdstrRet;
}

string CKRestCompJob::GetErrorJson(int nError, string stdstrMsg)
{
	string stdstrRet;

	stdstrRet = Utility::string_format("{\
		\"result\":\"fail\",\
		\"error\": %d,\
		\"message\": \"%s\"\
		}", nError, stdstrMsg.c_str());

	return stdstrRet;
}

bool CKRestCompJob::IsAssignedJob()
{
	return m_bAssigned;
}

void CKRestCompJob::SetIsAssignedJob(bool bAssigned)
{
	m_bAssigned = bAssigned;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestTCCompJob::CKRestTCCompJob()
{

}

CKRestTCCompJob::~CKRestTCCompJob()
{

}

void CKRestTCCompJob::SetAllocJobInfo(string stdstrJasonJobInfo)
{
	CKRestCompJob::SetAllocJobInfo(stdstrJasonJobInfo);

	m_TCJobData.Clear();
}

void* CKRestTCCompJob::ExtractAllocJobInfo()
{
	void* pRet = NULL;

	// transcoding job info json ///////////////////////////////////////////////////////////////////
	//{
	//	"jobid":243, 
	//	"jobinfos" : [
	//		{
	//			"convertvideosrc": {
	//				"assetid":36248,
	//				"filepath" : "/proxy/2010/12/23/V201000036248.mp4",
	//				"storage" : "O",
	//				"videowrappertype" : "MXF"
	//			},
	//			"proxyvideos":[{
	//				"storage": "Z",
	//				"assetid" : 1234,
	//				"filepath" : "/VIDEO/2010/12/23/V201000036248.mp4",
	//				"mobtype" : "h264"
	//			}]
	//		}
	//	],
	//	"jobname" : "transcoding",
	//	"jobtype" : "transcode", //transcode, wrapperconvert, filemerge, partialextraction
	//	"mamex" : {
	//		"end":"startCompjob",
	//		"pool" : 0,
	//		"progress" : "progress",
	//		"start" : "startCompjob"
	//	},
	//	"workflowid" : 263,
	//	"workflowname" : "ingest"
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////

	utility::string_t strKey;

	try
	{
		CKRestTCJobDestData DestData;

		if (m_stdstrJasonJobInfo.empty())
		{
			web::json::json_exception e(L"Job info is empty.");

			throw e;
		}

		string stdstrTemp;
		GetMamExStartJobData(stdstrTemp);

		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		strKey = COMPJOB_JSON_KEY_JOBINFOS;
		json::value jsonJobInfo = jsonJobData.at(strKey);

		m_TCJobData.m_stdstrJobJson = m_stdstrJasonJobInfo;

		if (jsonJobInfo.size() > 0)
		{
			for (int i = 0; i < (int)jsonJobInfo.size(); i++)
			{
				strKey = COMPJOB_JSON_KEY_CONVERTVIDEOSRC;
				if (jsonJobInfo[i].has_field(strKey))
				{
					json::value jsonJobSrc = jsonJobInfo[i].at(strKey);
					if (jsonJobSrc.is_null())
					{
						web::json::json_exception e(L"Srource is empty.");

						throw e;
					}

					strKey = COMPJOB_JSON_KEY_ASSETID;
					m_TCJobData.m_SrcData.m_nAssetID = jsonJobSrc[strKey].as_number().to_int64();
					strKey = COMPJOB_JSON_KEY_VIDEOWRAPPERTYPE;
					m_TCJobData.m_SrcData.m_stdstrVideoWrapperType = utility::conversions::utf16_to_utf8(jsonJobSrc[strKey].as_string());
					strKey = COMPJOB_JSON_KEY_STORAGE;
					m_TCJobData.m_SrcData.m_FileInfo.m_stdstrStorage = utility::conversions::utf16_to_utf8(jsonJobSrc[strKey].as_string());
					strKey = COMPJOB_JSON_KEY_FILEPATH;
					m_TCJobData.m_SrcData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(jsonJobSrc[strKey].as_string());

					strKey = COMPJOB_JSON_KEY_PROXYVIDEOS;
					if (!jsonJobInfo[i].has_field(strKey))
					{
						web::json::json_exception e(L"Destnation not found.");

						throw e;
					}

					json::value jsonJobDest = jsonJobInfo[i].at(strKey);
					if (jsonJobDest.is_null())
					{
						web::json::json_exception e(L"Destnation is empty.");

						throw e;
					}

					json::array arrDest = jsonJobDest.as_array();
					int nSize = arrDest.size();
					for (int j = 0; j < nSize; j++)
					{
						json::value jsonDest = arrDest[j];
						strKey = COMPJOB_JSON_KEY_ASSETID;
						DestData.m_nAssetID = jsonDest[strKey].as_number().to_int64();
						strKey = COMPJOB_JSON_KEY_MOBTYPE;
						DestData.m_stdstrMobType = utility::conversions::utf16_to_utf8(jsonDest[strKey].as_string());
						strKey = COMPJOB_JSON_KEY_STORAGE;
						DestData.m_FileInfo.m_stdstrStorage = utility::conversions::utf16_to_utf8(jsonDest[strKey].as_string());
						strKey = COMPJOB_JSON_KEY_FILEPATH;
						DestData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(jsonDest[strKey].as_string());

						m_TCJobData.m_vecDestData.push_back(DestData);

						DestData.Clear();
					}
				}
				else
				{
					web::json::json_exception e(L"Srource not found.");

					throw e;
				}
			}

			pRet = &m_TCJobData;
		}
	}
	catch (web::json::json_exception const & e)
	{
		m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();

		if (strKey.empty())
			m_stdstrErrorMsg.assign(e.what());
		else
			m_stdstrErrorMsg = Utility::string_format("Key: %s - %s", utility::conversions::utf16_to_utf8(strKey).c_str(), e.what());
		
		m_TCJobData.Clear();

		printf("%s\n", m_stdstrErrorMsg.c_str());
		printf("%s\n", m_stdstrJasonJobInfo.c_str());
	}

	return pRet;
}

void CKRestTCCompJob::ParseJobData()
{

}

void* CKRestTCCompJob::GetJobData()
{
	return &m_TCJobData;
}

int CKRestTCCompJob::GetMamExStartJobData(string& stdstrJson)
{
	int nRet = 0;
	utility::string_t strKey;
	
	stdstrJson = "";

	try
	{
		if (m_stdstrJasonJobInfo.empty())
		{
			web::json::json_exception e(L"Job info is empty.");

			throw e;
		}

		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		strKey = COMPJOB_JSON_KEY_MAMEX;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
		{
			json::value jsonJobMamEx = jsonJobData.at(strKey);

			strKey = COMPJOB_JSON_KEY_MAMEX_POOL;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_TCJobData.m_nMamExPool = jsonJobMamEx[strKey].as_integer();

			strKey = COMPJOB_JSON_KEY_MAMEX_START;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_TCJobData.m_stdstrMamExStart = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());

			strKey = COMPJOB_JSON_KEY_MAMEX_PROGRESS;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_TCJobData.m_stdstrMamExProgress = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());

			strKey = COMPJOB_JSON_KEY_MAMEX_END;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_TCJobData.m_stdstrMamExEnd = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());
		}

		strKey = COMPJOB_JSON_KEY_JOBINFOS;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
			json::value jsonJobInfo = jsonJobData.at(strKey);

		strKey = COMPJOB_JSON_KEY_WORKFLOWID;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
			m_TCJobData.m_nWorkflowID = jsonJobData[strKey].as_number().to_int64();
		
		strKey = COMPJOB_JSON_KEY_JOBID;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
			m_TCJobData.m_nJobID = jsonJobData[strKey].as_number().to_int64();

		m_nJobID = m_TCJobData.m_nJobID;

		strKey = COMPJOB_JSON_KEY_WORKFLOWNAME;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
			m_TCJobData.m_stdstrWorkflowName = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());

		strKey = COMPJOB_JSON_KEY_JOBNAME;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
			m_TCJobData.m_stdstrJobName = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());

		strKey = COMPJOB_JSON_KEY_JOBTYPE;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
			m_TCJobData.m_stdstrJobType = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());

		m_TCJobData.m_nChannel = m_nChannel;
		m_TCJobData.m_nCheckInID = m_nCheckInID;
		m_TCJobData.m_stdstrJobJson = m_stdstrJasonJobInfo;

		if (!m_TCJobData.m_stdstrMamExStart.empty())
			stdstrJson = m_stdstrJasonJobInfo;
	}
	catch (web::json::json_exception const & e)
	{
		nRet = m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();

		if (strKey.empty())
			m_stdstrErrorMsg.assign(e.what());
		else
			m_stdstrErrorMsg = Utility::string_format("Key: %s - %s", utility::conversions::utf16_to_utf8(strKey).c_str(), e.what());

		m_TCJobData.Clear();

		printf("%s\n", m_stdstrErrorMsg.c_str());
		printf("%s\n", m_stdstrJasonJobInfo.c_str());
	}

	return nRet;
}

string CKRestTCCompJob::GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg)
{
	string stdstrRet;

	if (!m_TCJobData.m_stdstrMamExEnd.empty())
	{
		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);
		
		jsonJobData[COMPJOB_JSON_KEY_RESULT] = json::value::string(COMPJOB_SUCCESS);

		if (!bSuccess)
			jsonJobData[COMPJOB_JSON_KEY_RESULT] = json::value::string(COMPJOB_FAIL);
		
		jsonJobData[COMPJOB_JSON_KEY_MESSAGE] = json::value::string(utility::conversions::utf8_to_utf16(stdstrErrMsg));

		stdstrRet = utility::conversions::utf16_to_utf8(jsonJobData.serialize());
	}

	return stdstrRet;
}

string CKRestTCCompJob::GetMamExProgressData(int nProgress, string stdstrExValue)
{
	string stdstrRet;

	if (!m_TCJobData.m_stdstrMamExProgress.empty())
	{
		//stdstrRet = Utility::string_format(
		//	"{\
		//	\"jobid\":\"%I64d\",\
		//	\"progress\":\"%d\",\
		//	\"exvalue\":\"%s\"\
		//	}", m_nJobID, nProgress, stdstrExValue.c_str());

		stdstrRet = CKRestCompJob::GetReportProgressData(nProgress, stdstrExValue);
	}

	return stdstrRet;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestTMCompJob::CKRestTMCompJob()
{

}

CKRestTMCompJob::~CKRestTMCompJob()
{

}

void CKRestTMCompJob::SetAllocJobInfo(string stdstrJasonJobInfo)
{
	CKRestCompJob::SetAllocJobInfo(stdstrJasonJobInfo);

	m_TMJobData.Clear();
}

void* CKRestTMCompJob::ExtractAllocJobInfo()
{
	void* pRet = NULL;

	// transfer job info json ///////////////////////////////////////////////////////////////////
	//{
	//	"jobid":243, 
	//	"jobinfos" : [
	//		{
	//			"transferdisktoftp":[
	//				{
	//					"disksrc": {
	//						"assetid": "456938",
	//							"objectid" : "KV0000456938",
	//							"storage" : "Z",
	//							"filepath" : "/LOWRES/236461/2014/KV2014306351.wmv",
	//							"isfolder" : "0",
	//							"isdelete" : "0",
	//							"iszip" : "0",
	//							"filerename" : []
	//					},
	//					"ftpdest":[
	//						{
	//							"address": "172.30.10.21",
	//								"port" : "21",
	//								"username" : "konan2",
	//								"password" : "konan415",
	//								"filepath" : "/2265/Y/236461/L/M3646115101001_N.wmv",
	//								"displaydestfilepath" : [],
	//								"allowexistfile" : "2",
	//								"servertype" : []
	//						},
	//						{
	//							"address": "172.30.10.21",
	//							"port" : "21",
	//							"username" : "konan2",
	//							"password" : "konan415",
	//							"filepath" : "/2265/Y/236461/L/M3646115101001_N.wmv",
	//							"displaydestfilepath" : [],
	//							"allowexistfile" : "2",
	//							"servertype" : []
	//						}
	//					]
	//				},
	//				{
	//					"disksrc": {
	//						"assetid": "456938",
	//							"objectid" : "KV0000456938",
	//							"storage" : "Z",
	//							"filepath" : "/LOWRES/236461/2014/KV2014306351.wmv",
	//							"isfolder" : "0",
	//							"isdelete" : "0",
	//							"iszip" : "0",
	//							"filerename" : []
	//					},
	//					"ftpdest":[
	//						{
	//							"address": "172.30.10.21",
	//								"port" : "21",
	//								"username" : "konan2",
	//								"password" : "konan415",
	//								"filepath" : "/2265/Y/236461/L/M3646115101001_N.wmv",
	//								"displaydestfilepath" : [],
	//								"allowexistfile" : "2",
	//								"servertype" : []
	//						},
	//						{
	//							"address": "172.30.10.21",
	//							"port" : "21",
	//							"username" : "konan2",
	//							"password" : "konan415",
	//							"filepath" : "/2265/Y/236461/L/M3646115101001_N.wmv",
	//							"displaydestfilepath" : [],
	//							"allowexistfile" : "2",
	//							"servertype" : []
	//						}
	//					]
	//				}
	//			],
	//			"transferdisktodisk":[
	//				{
	//					"disksrc": {
	//						"assetid": "100317",
	//							"objectid" : "KD2015100317",
	//							"storage" : "W",
	//							"filepath" : "/DOCUMENT/236461/2015/KD2015100317/week01/es00_default/media/output.asf",
	//							"isfolder" : "0",
	//							"isdelete" : "0",
	//							"iszip" : "0",
	//							"filerename" : []
	//					},
	//					"diskdest":[
	//						{
	//							"assetid": "456938",
	//								"objectid" : "KV0000456938",
	//								"storage" : "Z",
	//								"filepath" : "/LOWRES/236461/2014/KV2014306351.wmv",
	//								"isfolder" : "0",
	//								"isdelete" : "0",
	//								"iszip" : "0",
	//								"filerename" : []
	//						},
	//						{
	//							"assetid": "456938",
	//							"objectid" : "KV0000456938",
	//							"storage" : "Z",
	//							"filepath" : "/LOWRES/236461/2014/KV2014306351.wmv",
	//							"isfolder" : "0",
	//							"isdelete" : "0",
	//							"iszip" : "0",
	//							"filerename" : []
	//						}
	//					]
	//				},
	//				{
	//					"disksrc": {
	//						"assetid": "100317",
	//							"objectid" : "KD2015100317",
	//							"storage" : "W",
	//							"filepath" : "/DOCUMENT/236461/2015/KD2015100317/week01/es00_default/media/output.asf",
	//							"isfolder" : "0",
	//							"isdelete" : "0",
	//							"iszip" : "0",
	//							"filerename" : []
	//					},
	//					"diskdest":[
	//						{
	//							"assetid": "456938",
	//								"objectid" : "KV0000456938",
	//								"storage" : "Z",
	//								"filepath" : "/LOWRES/236461/2014/KV2014306351.wmv",
	//								"isfolder" : "0",
	//								"isdelete" : "0",
	//								"iszip" : "0",
	//								"filerename" : []
	//						},
	//						{
	//							"assetid": "456938",
	//							"objectid" : "KV0000456938",
	//							"storage" : "Z",
	//							"filepath" : "/LOWRES/236461/2014/KV2014306351.wmv",
	//							"isfolder" : "0",
	//							"isdelete" : "0",
	//							"iszip" : "0",
	//							"filerename" : []
	//						}
	//					]
	//				}
	//			]
	//		}
	//	],
	//	"jobname" : "transfering",
	//	"jobtype" : "transfer", //transfer, volumeinfo, delete
	//	"mamex" : {
	//		"end":"startCompjob",
	//		"pool" : 0,
	//		"progress" : "progress",
	//		"start" : "startCompjob"
	//	},
	//	"workflowid" : 263,
	//	"workflowname" : "ingest"
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////

	utility::string_t strKey;

	try
	{
		CKRestTCJobDestData DestData;

		if (m_stdstrJasonJobInfo.empty())
		{
			web::json::json_exception e(L"Job info is empty.");

			throw e;
		}

		string stdstrTemp;
		GetMamExStartJobData(stdstrTemp);

		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		strKey = COMPJOB_JSON_KEY_JOBINFOS;
		json::value jsonJobInfo = jsonJobData.at(strKey);

		m_TMJobData.m_stdstrJobJson = m_stdstrJasonJobInfo;

		if (jsonJobInfo.size() > 0)
		{
			strKey = COMPJOB_JSON_KEY_TRANSFER_DISKTOFTP;
			if (jsonJobInfo.has_field(strKey))
			{
				json::array jsonJobDiskToFTP = jsonJobInfo.at(strKey).as_array();

				strKey = L"";
				ExtractJobDataDiskToFTP(&jsonJobDiskToFTP);
			}

			strKey = COMPJOB_JSON_KEY_TRANSFER_DISKTODISK;
			if (jsonJobInfo.has_field(strKey))
			{
				json::value jsonJobDiskToDisk = jsonJobInfo.at(strKey);

				strKey = L"";
				ExtractJobDataDiskToDISK(&jsonJobDiskToDisk);
			}

			for (int i = 0; i < (int)jsonJobInfo.size(); i++)
			{
				json::value jsonJobSrcDest = jsonJobInfo[i];
				if (jsonJobSrcDest.is_null())
				{
					web::json::json_exception e(L"Srource, destnation is empty.");

					throw e;
				}
			}

			pRet = &m_TMJobData;
		}
	}
	catch (web::json::json_exception const & e)
	{
		m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();

		if (strKey.empty())
			m_stdstrErrorMsg.assign(e.what());
		else
			m_stdstrErrorMsg = Utility::string_format("Key: %s - %s", utility::conversions::utf16_to_utf8(strKey).c_str(), e.what());

		m_TMJobData.Clear();

		printf("%s\n", m_stdstrErrorMsg.c_str());
		printf("%s\n", m_stdstrJasonJobInfo.c_str());
	}

	return pRet;
}

void* CKRestTMCompJob::GetJobData()
{
	return &m_TMJobData;
}

int CKRestTMCompJob::GetMamExStartJobData(string& stdstrJson)
{
	int nRet = 0;
	utility::string_t strKey;

	stdstrJson = "";

	try
	{
		if (m_stdstrJasonJobInfo.empty())
		{
			web::json::json_exception e(L"Job info is empty.");

			throw e;
		}

		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		strKey = COMPJOB_JSON_KEY_MAMEX;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
		{
			json::value jsonJobMamEx = jsonJobData.at(strKey);

			strKey = COMPJOB_JSON_KEY_MAMEX_POOL;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_TMJobData.m_nMamExPool = jsonJobMamEx[strKey].as_integer();

			strKey = COMPJOB_JSON_KEY_MAMEX_START;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_TMJobData.m_stdstrMamExStart = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());

			strKey = COMPJOB_JSON_KEY_MAMEX_PROGRESS;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_TMJobData.m_stdstrMamExProgress = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());

			strKey = COMPJOB_JSON_KEY_MAMEX_END;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_TMJobData.m_stdstrMamExEnd = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());
		}

		strKey = COMPJOB_JSON_KEY_JOBINFOS;
		json::value jsonJobInfo = jsonJobData.at(strKey);

		strKey = COMPJOB_JSON_KEY_WORKFLOWID;
		m_TMJobData.m_nWorkflowID = jsonJobData[strKey].as_number().to_int64();
		strKey = COMPJOB_JSON_KEY_JOBID;
		m_TMJobData.m_nJobID = jsonJobData[strKey].as_number().to_int64();
		m_nJobID = m_TMJobData.m_nJobID;
		strKey = COMPJOB_JSON_KEY_WORKFLOWNAME;
		m_TMJobData.m_stdstrWorkflowName = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());
		strKey = COMPJOB_JSON_KEY_JOBNAME;
		m_TMJobData.m_stdstrJobName = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());
		strKey = COMPJOB_JSON_KEY_JOBTYPE;
		m_TMJobData.m_stdstrJobType = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());

		m_TMJobData.m_nChannel = m_nChannel;
		m_TMJobData.m_nCheckInID = m_nCheckInID;
		m_TMJobData.m_stdstrJobJson = m_stdstrJasonJobInfo;

		if (!m_TMJobData.m_stdstrMamExStart.empty())
			stdstrJson = m_stdstrJasonJobInfo;
	}
	catch (web::json::json_exception const & e)
	{
		nRet = m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();

		if (strKey.empty())
			m_stdstrErrorMsg.assign(e.what());
		else
			m_stdstrErrorMsg = Utility::string_format("Key: %s - %s", utility::conversions::utf16_to_utf8(strKey).c_str(), e.what());

		m_TMJobData.Clear();

		printf("%s\n", m_stdstrErrorMsg.c_str());
		printf("%s\n", m_stdstrJasonJobInfo.c_str());
	}

	return nRet;
}

string CKRestTMCompJob::GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg)
{
	string stdstrRet;

	if (!m_TMJobData.m_stdstrMamExEnd.empty())
	{
		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		jsonJobData[COMPJOB_JSON_KEY_RESULT] = json::value::string(COMPJOB_SUCCESS);

		if (!bSuccess)
			jsonJobData[COMPJOB_JSON_KEY_RESULT] = json::value::string(COMPJOB_FAIL);

		jsonJobData[COMPJOB_JSON_KEY_MESSAGE] = json::value::string(utility::conversions::utf8_to_utf16(stdstrErrMsg));

		stdstrRet = utility::conversions::utf16_to_utf8(jsonJobData.serialize());
	}

	return stdstrRet;
}

string CKRestTMCompJob::GetMamExProgressData(int nProgress, string stdstrExValue)
{
	string stdstrRet;

	if (!m_TMJobData.m_stdstrMamExProgress.empty())
	{
		//stdstrRet = Utility::string_format(
		//	"{\
		//	\"jobid\":\"%I64d\",\
		//	\"progress\":\"%d\",\
		//	\"exvalue\":\"%s\"\
		//	}", m_nJobID, nProgress, stdstrExValue.c_str());

		stdstrRet = CKRestCompJob::GetReportProgressData(nProgress, stdstrExValue);
	}

	return stdstrRet;
}

void CKRestTMCompJob::ExtractJobDataDiskToFTP(void* pJsonValue)
{
	if (pJsonValue)
	{
		json::array* pArrJobDiskToFTP = (json::array*)pJsonValue;
		int nCount = pArrJobDiskToFTP->size();

		if (nCount == 0)
		{
			web::json::json_exception e(L"Transfer job info is empty(disk to FTP).");

			throw e;
		}

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestTMJobSrcData SrcData;
		CKRestTMJobDestData DestData;
		vector<CKRestTMJobDestData> vecDestData;
		pair<CKRestTMJobSrcData, vector<CKRestTMJobDestData>> pairSrcDestData;

		json::value jsonJobData;

		for (int i = 0; i < nCount; i++)
		{
			jsonJobData = pArrJobDiskToFTP->at(i);

			strKey = COMPJOB_JSON_KEY_TRANSFER_DISKSRC;
			if (jsonJobData.has_field(strKey))
			{
				json::value jsonJobDiskSrc = jsonJobData.at(strKey);
				ExtractJobDataDiskSrc(&jsonJobDiskSrc, &SrcData);
			}
			else
			{
				strMsg = L"Key: " + strKey + L" - Transfer source info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			strKey = COMPJOB_JSON_KEY_TRANSFER_FTPDEST;
			if (jsonJobData.has_field(strKey))
			{
				json::array jsonArrJobFTPDest = jsonJobData.at(strKey).as_array();
				int nDestCount = jsonArrJobFTPDest.size();

				if (nDestCount == 0)
				{
					strMsg = L"Key: " + strKey + L" - Transfer job info is empty(FTP dest).";
					web::json::json_exception e(strMsg.c_str());

					throw e;
				}

				json::value jsonJobFTPDest;
				for (int j = 0; j < nDestCount; j++)
				{
					jsonJobFTPDest = jsonJobData.at(j);
					
					ExtractJobDataFTPDest(&jsonJobFTPDest, &DestData);
					
					vecDestData.push_back(DestData);
				}
			}
			else
			{
				strMsg = L"Key: " + strKey + L" - Transfer destination info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			pairSrcDestData.first = SrcData;
			pairSrcDestData.second = vecDestData;

			m_TMJobData.m_vecSrcDest.push_back(pairSrcDestData);
		}
	}
}

void CKRestTMCompJob::ExtractJobDataDiskToDISK(void* pJsonValue)
{
	if (pJsonValue)
	{
		json::array* pArrJobDiskToFTP = (json::array*)pJsonValue;
		int nCount = pArrJobDiskToFTP->size();

		if (nCount == 0)
		{
			web::json::json_exception e(L"Transfer job info is empty(disk to FTP).");

			throw e;
		}

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestTMJobSrcData SrcData;
		CKRestTMJobDestData DestData;
		vector<CKRestTMJobDestData> vecDestData;
		pair<CKRestTMJobSrcData, vector<CKRestTMJobDestData>> pairSrcDestData;

		json::value jsonJobData;

		for (int i = 0; i < nCount; i++)
		{
			jsonJobData = pArrJobDiskToFTP->at(i);

			strKey = COMPJOB_JSON_KEY_TRANSFER_DISKSRC;
			if (jsonJobData.has_field(strKey))
			{
				json::value jsonJobDiskSrc = jsonJobData.at(strKey);
				ExtractJobDataDiskSrc(&jsonJobDiskSrc, &SrcData);
			}
			else
			{
				strMsg = L"Key: " + strKey + L" - Transfer source info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			strKey = COMPJOB_JSON_KEY_TRANSFER_DISKDEST;
			if (jsonJobData.has_field(strKey))
			{
				json::array jsonArrJobDiskDest = jsonJobData.at(strKey).as_array();
				int nDestCount = jsonArrJobDiskDest.size();

				if (nDestCount == 0)
				{
					strMsg = L"Key: " + strKey + L" - Transfer job info is empty(FTP dest).";
					web::json::json_exception e(strMsg.c_str());

					throw e;
				}

				json::value jsonJobDiskDest;
				for (int j = 0; j < nDestCount; j++)
				{
					jsonJobDiskDest = jsonJobData.at(j);

					ExtractJobDataDiskDest(&jsonJobDiskDest, &DestData);

					vecDestData.push_back(DestData);
				}
			}
			else
			{
				strMsg = L"Key: " + strKey + L" - Transfer destination info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			pairSrcDestData.first = SrcData;
			pairSrcDestData.second = vecDestData;

			m_TMJobData.m_vecSrcDest.push_back(pairSrcDestData);
		}
	}
}

void CKRestTMCompJob::ExtractJobDataFTPToFTP(void* pJsonValue)
{
	if (pJsonValue)
	{
		json::array* pArrJobFTPToFTP = (json::array*)pJsonValue;
		int nCount = pArrJobFTPToFTP->size();

		if (nCount == 0)
		{
			web::json::json_exception e(L"Transfer job info is empty(FTP to FTP).");

			throw e;
		}

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestTMJobSrcData SrcData;
		CKRestTMJobDestData DestData;
		vector<CKRestTMJobDestData> vecDestData;
		pair<CKRestTMJobSrcData, vector<CKRestTMJobDestData>> pairSrcDestData;

		json::value jsonJobData;

		for (int i = 0; i < nCount; i++)
		{
			jsonJobData = pArrJobFTPToFTP->at(i);

			strKey = COMPJOB_JSON_KEY_TRANSFER_FTPSRC;
			if (jsonJobData.has_field(strKey))
			{
				json::value jsonJobFTPSrc = jsonJobData.at(strKey);
				ExtractJobDataFTPSrc(&jsonJobFTPSrc, &SrcData);
			}
			else
			{
				strMsg = L"Key: " + strKey + L" - Transfer source info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			strKey = COMPJOB_JSON_KEY_TRANSFER_FTPDEST;
			if (jsonJobData.has_field(strKey))
			{
				json::array jsonArrJobFTPDest = jsonJobData.at(strKey).as_array();
				int nDestCount = jsonArrJobFTPDest.size();

				if (nDestCount == 0)
				{
					strMsg = L"Key: " + strKey + L" - Transfer job info is empty(FTP dest).";
					web::json::json_exception e(strMsg.c_str());

					throw e;
				}

				json::value jsonJobFTPDest;
				for (int j = 0; j < nDestCount; j++)
				{
					jsonJobFTPDest = jsonJobData.at(j);

					ExtractJobDataFTPDest(&jsonJobFTPDest, &DestData);

					vecDestData.push_back(DestData);
				}
			}
			else
			{
				strMsg = L"Key: " + strKey + L" - Transfer destination info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			pairSrcDestData.first = SrcData;
			pairSrcDestData.second = vecDestData;

			m_TMJobData.m_vecSrcDest.push_back(pairSrcDestData);
		}
	}
}

void CKRestTMCompJob::ExtractJobDataDiskSrc(void* pJsonValue, void* pSrcData)
{
	if (pJsonValue && pSrcData)
	{
		json::value* pJobDiskSrc = (json::value*)pJsonValue;
		if (pJobDiskSrc->is_null())
		{
			web::json::json_exception e(L"Transfer job info is empty(disk src)");

			throw e;
		}

		utility::string_t strMsg;
		utility::string_t strKey;

		try
		{
			CKRestTMJobSrcData* pData = (CKRestTMJobSrcData*)pSrcData;

			strKey = COMPJOB_JSON_KEY_ASSETID;
			pData->m_nAssetID = pJobDiskSrc->at(strKey).as_number().to_int64();

			strKey = COMPJOB_JSON_KEY_OBJECTID;
			pData->m_stdstrObjectID = utility::conversions::utf16_to_utf8(pJobDiskSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_ISFOLDER;
			if (pJobDiskSrc->at(strKey).as_integer() > 0)
				pData->m_bFolder = true;

			strKey = COMPJOB_JSON_KEY_ISDELETE;
			if (pJobDiskSrc->at(strKey).as_integer() > 0)
				pData->m_bDelete = true;

			strKey = COMPJOB_JSON_KEY_ISZIP;
			if (pJobDiskSrc->at(strKey).as_integer() > 0)
				pData->m_bZip = true;

			strKey = COMPJOB_JSON_KEY_FILEPATH;
			pData->m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(pJobDiskSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_FILERENAME;
			pData->m_stdstrRename = utility::conversions::utf16_to_utf8(pJobDiskSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_STORAGE;
			pData->m_FileInfo.m_stdstrStorage = utility::conversions::utf16_to_utf8(pJobDiskSrc->at(strKey).as_string());

			pData->m_FileInfo.m_eStorageType = eStorageTypeDisk;
		}
		catch (web::json::json_exception const & e)
		{
			strMsg = L"Key: " + strKey + L" - " + utility::conversions::utf8_to_utf16(e.what());
			web::json::json_exception e2(strMsg.c_str());

			throw e2;
		}
	}
}

void CKRestTMCompJob::ExtractJobDataFTPSrc(void* pJsonValue, void* pSrcData)
{
	if (pJsonValue && pSrcData)
	{
		json::value* pJobFTPSrc = (json::value*)pJsonValue;
		if (pJobFTPSrc->is_null())
		{
			web::json::json_exception e(L"Transfer job info is empty(FTP src)");

			throw e;
		}

		utility::string_t strMsg;
		utility::string_t strKey;

		try
		{
			CKRestTMJobSrcData* pData = (CKRestTMJobSrcData*)pSrcData;

			strKey = COMPJOB_JSON_KEY_ASSETID;
			pData->m_nAssetID = pJobFTPSrc->at(strKey).as_number().to_int64();

			strKey = COMPJOB_JSON_KEY_OBJECTID;
			pData->m_stdstrObjectID = utility::conversions::utf16_to_utf8(pJobFTPSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_ISFOLDER;
			if (pJobFTPSrc->at(strKey).as_integer() > 0)
				pData->m_bFolder = true;

			strKey = COMPJOB_JSON_KEY_ISDELETE;
			if (pJobFTPSrc->at(strKey).as_integer() > 0)
				pData->m_bDelete = true;

			strKey = COMPJOB_JSON_KEY_ISZIP;
			if (pJobFTPSrc->at(strKey).as_integer() > 0)
				pData->m_bZip = true;

			strKey = COMPJOB_JSON_KEY_FILEPATH;
			pData->m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(pJobFTPSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_FILERENAME;
			pData->m_stdstrRename = utility::conversions::utf16_to_utf8(pJobFTPSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_PORT;
			pData->m_FileInfo.m_nPort = pJobFTPSrc->at(strKey).as_integer();

			strKey = COMPJOB_JSON_KEY_ADDRESS;
			pData->m_FileInfo.m_stdstrHostAddress = utility::conversions::utf16_to_utf8(pJobFTPSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_USERNAME;
			pData->m_FileInfo.m_stdstrUser = utility::conversions::utf16_to_utf8(pJobFTPSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_PASSWORD;
			pData->m_FileInfo.m_stdstrPassword = utility::conversions::utf16_to_utf8(pJobFTPSrc->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_SERVERTYPE;
			pData->m_stdstrServerType = utility::conversions::utf16_to_utf8(pJobFTPSrc->at(strKey).as_string());

			pData->m_FileInfo.m_eStorageType = eStorageTypeFTP;
		}
		catch (web::json::json_exception const & e)
		{
			strMsg = L"Key: " + strKey + L" - " + utility::conversions::utf8_to_utf16(e.what());
			web::json::json_exception e2(strMsg.c_str());

			throw e2;
		}
	}
}

void CKRestTMCompJob::ExtractJobDataFTPDest(void* pJsonValue, void* pDestData)
{
	if (pJsonValue && pDestData)
	{
		json::value* pJobFTPDest = (json::value*)pJsonValue;
		if (pJobFTPDest->is_null())
		{
			web::json::json_exception e(L"Transfer job info is empty(FTP dest)");

			throw e;
		}

		utility::string_t strMsg;
		utility::string_t strKey;

		try
		{
			CKRestTMJobDestData* pData = (CKRestTMJobDestData*)pDestData;

			strKey = COMPJOB_JSON_KEY_PORT;
			pData->m_FileInfo.m_nPort = pJobFTPDest->at(strKey).as_integer();

			strKey = COMPJOB_JSON_KEY_ALLOWEXISTFILE;
			pData->m_nAllowExistFile = pJobFTPDest->at(strKey).as_integer();

			strKey = COMPJOB_JSON_KEY_ADDRESS;
			pData->m_FileInfo.m_stdstrHostAddress = utility::conversions::utf16_to_utf8(pJobFTPDest->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_USERNAME;
			pData->m_FileInfo.m_stdstrUser = utility::conversions::utf16_to_utf8(pJobFTPDest->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_PASSWORD;
			pData->m_FileInfo.m_stdstrPassword = utility::conversions::utf16_to_utf8(pJobFTPDest->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_DISPLAYDESTFILEPATH;
			pData->m_stdstrDisplayPath = utility::conversions::utf16_to_utf8(pJobFTPDest->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_SERVERTYPE;
			pData->m_stdstrServerType = utility::conversions::utf16_to_utf8(pJobFTPDest->at(strKey).as_string());

			pData->m_FileInfo.m_eStorageType = eStorageTypeFTP;
		}
		catch (web::json::json_exception const & e)
		{
			strMsg = L"Key: " + strKey + L" - " + utility::conversions::utf8_to_utf16(e.what());
			web::json::json_exception e2(strMsg.c_str());

			throw e2;
		}
	}
}

void CKRestTMCompJob::ExtractJobDataDiskDest(void* pJsonValue, void* pDestData)
{
	if (pJsonValue && pDestData)
	{
		json::value* pJobDiskDest = (json::value*)pJsonValue;
		if (pJobDiskDest->is_null())
		{
			web::json::json_exception e(L"Transfer job info is empty(FTP dest)");

			throw e;
		}

		utility::string_t strMsg;
		utility::string_t strKey;

		try
		{
			CKRestTMJobDestData* pData = (CKRestTMJobDestData*)pDestData;

			strKey = COMPJOB_JSON_KEY_ALLOWEXISTFILE;
			pData->m_nAllowExistFile = pJobDiskDest->at(strKey).as_integer();

			strKey = COMPJOB_JSON_KEY_FILEPATH;
			pData->m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(pJobDiskDest->at(strKey).as_string());

			strKey = COMPJOB_JSON_KEY_STORAGE;
			pData->m_FileInfo.m_stdstrStorage = utility::conversions::utf16_to_utf8(pJobDiskDest->at(strKey).as_string());

			pData->m_FileInfo.m_eStorageType = eStorageTypeDisk;
		}
		catch (web::json::json_exception const & e)
		{
			strMsg = L"Key: " + strKey + L" - " + utility::conversions::utf8_to_utf16(e.what());
			web::json::json_exception e2(strMsg.c_str());

			throw e2;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestCACompJob::CKRestCACompJob()
{

}

CKRestCACompJob::~CKRestCACompJob()
{

}

void CKRestCACompJob::SetAllocJobInfo(string stdstrJasonJobInfo)
{
	CKRestCompJob::SetAllocJobInfo(stdstrJasonJobInfo);

	m_CAJobData.Clear();
}

void* CKRestCACompJob::ExtractAllocJobInfo()
{
	// cataloging job info json ///////////////////////////////////////////////////////////////////
	// request json (mam to cataloger)
	//{
	//	"workflowid" : 263,
	//	"workflowname" : "ingest",
	//	"jobid":243, 
	//	"jobname" : "ca_cataloging",
	//	"jobtype" : "catalog",
	//	"mamex" : {
	//		"end":"startCompjob",
	//		"pool" : 0,
	//		"progress" : "progress",
	//		"start" : "startCompjob"
	//	},
	//	"jobinfos" : [
	//		{
	//			"shotimages": {
	//				"filepath":"proxyshot/2018/02/13/1/OV201800000001/",
	//				"volumeetc" : "/Volumes/konan/darc4data/",
	//				"volumewin" : "X:/darc4data/"
	//			},
	//			"catalogvideosrc": {
	//				"assetid":36248,
	//				"filepath" : "/proxy/2010/12/23/V201000036248.mp4",
	//				"volumeetc" : "/Volumes/konan/darc4data/",
	//				"volumewin" : "X:/darc4data/",
	//				"videoframerate" : "29.97"
	//			}
	//		}
	//	]
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////

	void* pRet = NULL;

	utility::string_t strKey;

	try
	{
		if (m_stdstrJasonJobInfo.empty())
		{
			web::json::json_exception e(L"Job data is empty.");

			throw e;
		}

		string stdstrTemp;
		GetMamExStartJobData(stdstrTemp);

		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		m_CAJobData.m_stdstrJobJson = m_stdstrJasonJobInfo;

		strKey = COMPJOB_JSON_KEY_JOBINFOS;
		if (!jsonJobData.has_field(strKey) && jsonJobData[strKey].is_null())
		{
			web::json::json_exception e(L"Job info not found.");

			throw e;
		}

		json::array jsonJobSrcArr = jsonJobData.at(strKey).as_array();
		
		int nCount = (int)jsonJobSrcArr.size();
		if (nCount <= 0)
		{
			web::json::json_exception e(L"Job info is empty.");
		}

		if (jsonJobSrcArr[0].is_null())
		{
			web::json::json_exception e(L"Job info is empty.");
		}

		json::value jsonJobInfo = jsonJobSrcArr[0];

		strKey = COMPJOB_JSON_KEY_CATALOGVIDEOSRC;
		if (!jsonJobInfo.has_field(strKey))
		{
			web::json::json_exception e(L"Srource not found.");

			throw e;
		}

		json::value jsonJobSrc = jsonJobInfo.at(strKey);

		if (jsonJobSrc.is_null())
		{
			web::json::json_exception e(L"Srource is empty.");

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_ASSETID;
		if (jsonJobSrc.has_field(strKey) && !jsonJobSrc.at(strKey).is_null())
			m_CAJobData.m_SrcData.m_nAssetID = jsonJobSrc[strKey].as_number().to_int64();

		//strKey = COMPJOB_JSON_KEY_STORAGE;
		strKey = COMPJOB_JSON_KEY_WINVOLUME;
		if (jsonJobSrc.has_field(strKey) && !jsonJobSrc.at(strKey).is_null())
			m_CAJobData.m_SrcData.m_FileInfo.m_stdstrStorage = utility::conversions::utf16_to_utf8(jsonJobSrc[strKey].as_string());

		strKey = COMPJOB_JSON_KEY_FILEPATH;
		if (jsonJobSrc.has_field(strKey) && !jsonJobSrc.at(strKey).is_null())
			m_CAJobData.m_SrcData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(jsonJobSrc[strKey].as_string());

		strKey = COMPJOB_JSON_KEY_VIDEOFRAMERATE;
		if (jsonJobSrc.has_field(strKey) && !jsonJobSrc.at(strKey).is_null())
			m_CAJobData.m_SrcData.m_dbFramerate = (double)jsonJobSrc[strKey].as_double();

		strKey = COMPJOB_JSON_KEY_CATALOGSHOTIMAGE;
		if (!jsonJobInfo.has_field(strKey))
		{
			web::json::json_exception e(L"Shot image path not found.");

			throw e;
		}

		json::value jsonJobShotImageDest = jsonJobInfo.at(strKey);

		if (jsonJobShotImageDest.is_null())
		{
			web::json::json_exception e(L"Shot image path is empty.");

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_WINVOLUME;
		if (jsonJobShotImageDest.has_field(strKey) && !jsonJobShotImageDest.at(strKey).is_null())
			m_CAJobData.m_stdstrShotImageStorage = utility::conversions::utf16_to_utf8(jsonJobShotImageDest[strKey].as_string());
		
		strKey = COMPJOB_JSON_KEY_FILEPATH;
		if (jsonJobShotImageDest.has_field(strKey) && !jsonJobShotImageDest.at(strKey).is_null())
			m_CAJobData.m_stdstrShotImagePath = utility::conversions::utf16_to_utf8(jsonJobShotImageDest[strKey].as_string());

		pRet = &m_CAJobData;
	}
	catch (web::json::json_exception const & e)
	{
		m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();

		if (strKey.empty())
			m_stdstrErrorMsg.assign(e.what());
		else
			m_stdstrErrorMsg = Utility::string_format("Key: %s - %s", utility::conversions::utf16_to_utf8(strKey).c_str(), e.what());

		m_CAJobData.Clear();

		printf("%s\n", m_stdstrErrorMsg.c_str());
		printf("%s\n", m_stdstrJasonJobInfo.c_str());
	}

	return pRet;
}

void CKRestCACompJob::ParseJobData()
{

}

void* CKRestCACompJob::GetJobData()
{
	return &m_CAJobData;
}

int CKRestCACompJob::GetMamExStartJobData(string& stdstrJson)
{
	int nRet = 0;
	utility::string_t strKey;

	stdstrJson = "";

	try
	{
		if (m_stdstrJasonJobInfo.empty())
		{
			web::json::json_exception e(L"Job info is empty.");

			throw e;
		}

		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		strKey = COMPJOB_JSON_KEY_MAMEX;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
		{
			json::value jsonJobMamEx = jsonJobData.at(strKey);

			strKey = COMPJOB_JSON_KEY_MAMEX_POOL;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_CAJobData.m_nMamExPool = jsonJobMamEx[strKey].as_integer();

			strKey = COMPJOB_JSON_KEY_MAMEX_START;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_CAJobData.m_stdstrMamExStart = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());

			strKey = COMPJOB_JSON_KEY_MAMEX_PROGRESS;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_CAJobData.m_stdstrMamExProgress = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());

			strKey = COMPJOB_JSON_KEY_MAMEX_END;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_CAJobData.m_stdstrMamExEnd = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());
		}

		strKey = COMPJOB_JSON_KEY_JOBINFOS;
		json::value jsonJobInfo = jsonJobData.at(strKey);

		strKey = COMPJOB_JSON_KEY_WORKFLOWID;
		m_CAJobData.m_nWorkflowID = jsonJobData[strKey].as_number().to_int64();
		strKey = COMPJOB_JSON_KEY_JOBID;
		m_CAJobData.m_nJobID = jsonJobData[strKey].as_number().to_int64();
		m_nJobID = m_CAJobData.m_nJobID;
		strKey = COMPJOB_JSON_KEY_WORKFLOWNAME;
		m_CAJobData.m_stdstrWorkflowName = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());
		strKey = COMPJOB_JSON_KEY_JOBNAME;
		m_CAJobData.m_stdstrJobName = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());
		strKey = COMPJOB_JSON_KEY_JOBTYPE;
		m_CAJobData.m_stdstrJobType = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());

		m_CAJobData.m_nChannel = m_nChannel;
		m_CAJobData.m_nCheckInID = m_nCheckInID;
		m_CAJobData.m_stdstrJobJson = m_stdstrJasonJobInfo;

		if (!m_CAJobData.m_stdstrMamExStart.empty())
			stdstrJson = m_stdstrJasonJobInfo;
	}
	catch (web::json::json_exception const & e)
	{
		nRet = m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();

		if (strKey.empty())
			m_stdstrErrorMsg.assign(e.what());
		else
			m_stdstrErrorMsg = Utility::string_format("Key: %s - %s", utility::conversions::utf16_to_utf8(strKey).c_str(), e.what());

		m_CAJobData.Clear();

		printf("%s\n", m_stdstrErrorMsg.c_str());
		printf("%s\n", m_stdstrJasonJobInfo.c_str());
	}

	return nRet;
}

string CKRestCACompJob::GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg)
{
	string stdstrRet;

	if (!m_CAJobData.m_stdstrMamExEnd.empty())
	{
		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		jsonJobData[COMPJOB_JSON_KEY_RESULT] = json::value::string(COMPJOB_SUCCESS);

		if (!bSuccess)
			jsonJobData[COMPJOB_JSON_KEY_RESULT] = json::value::string(COMPJOB_FAIL);

		jsonJobData[COMPJOB_JSON_KEY_MESSAGE] = json::value::string(utility::conversions::utf8_to_utf16(stdstrErrMsg));

		stdstrRet = utility::conversions::utf16_to_utf8(jsonJobData.serialize());
	}

	return stdstrRet;
}

string CKRestCACompJob::GetMamExProgressData(int nProgress, string stdstrExValue)
{
	string stdstrRet;

	if (!m_CAJobData.m_stdstrMamExProgress.empty())
	{
		//stdstrRet = Utility::string_format(
		//	"{\
				//	\"jobid\":\"%I64d\",\
				//	\"progress\":\"%d\",\
				//	\"exvalue\":\"%s\"\
				//	}", m_nJobID, nProgress, stdstrExValue.c_str());

		stdstrRet = CKRestCompJob::GetReportProgressData(nProgress, stdstrExValue);
	}

	return stdstrRet;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestAMCompJob::CKRestAMCompJob()
{

}

CKRestAMCompJob::~CKRestAMCompJob()
{

}

void CKRestAMCompJob::SetAllocJobInfo(string stdstrJasonJobInfo)
{
	CKRestCompJob::SetAllocJobInfo(stdstrJasonJobInfo);

	m_AMJobData.Clear();
}

void* CKRestAMCompJob::ExtractAllocJobInfo()
{
	// archive job info json ///////////////////////////////////////////////////////////////////
	// request json (mam to archive manager)
	//{
	//	"workflowid" : 263,
	//	"workflowname" : "ingest",
	//	"jobid":243, 
	//	"jobname" : "ac_archive",
	//	"jobtype" : "archive",       // archive, restore, partialrestore, delete, volumeinfo
	//	"mamex" : {
	//		"start":"startCompjob",
	//		"pool" : 0,
	//		"progress" : "progress",
	//		"end" : "endCompjob"
	//	},
	//	"jobinfos" : {
	//		"archivedisksrc": {
	//			"assetid":36248,
	//			"objectid":"V201711111",
	//			"filelist" : [
	//				{
	//					"storage" : "X",
	//					"filepath" :"/2010/12/23/V201000036248.mxf",
	//					"isdelete" : 0, // 0, 1
	//					"isfloder" : 0 // 0, 1
	//					"iszip" : 0 // 0, 1
	//				},
	//				{
	//					"storage" : "X",
	//					"filepath" :"/2010/12/23/V201000036248.mxf",
	//					"isdelete" : 0, // 0, 1
	//					"isfloder" : 0 // 0, 1
	//					"iszip" : 0 // 0, 1
	//				}
	//			]
	//		},
	//		"archivedtldest": {
	//			"objectid":"V201711111",
	//			"dtlgroup":"CMS",
	//			"dtlcategory":"CMS",
	//			"filepath" : "/2010/12/23/"
	//		}
	//	}
	//  restore ///////////////////////////////////////////////////////////////////
	//	"jobinfos" : {
	//		"restoredtlsrc": {
	//			"assetid":36248,
	//			"objectid":"V201711111",
	//			"dtlgroup":"CMS",
	//			"dtlcategory":"CMS",
	//			"filelist" : [
	//				"/2010/12/23/V201000036248.mxf",
	//				"/2010/12/23/V201000036248.mxf"
	//			]
	//		},
	//		"restorediskdest": {
	//			"storage" : "X",
	//			"filepath" : "/2010/12/23/"
	//		}
	//	}
	//  partial restore ///////////////////////////////////////////////////////////////////
	//	"jobinfos" : {
	//		"restoredtlsrc": {
	//			"assetid":36248,
	//			"objectid":"V201711111",
	//			"dtlgroup":"CMS",
	//			"dtlcategory":"CMS",
	//			"compressiontype" : "XDCAMHD",
	//			"wrappertype" : "MXF",
	//			"filepath" : "/2010/12/23/V201000036248.mxf"
	//		},
	//		or
	//		"restoredisksrc": {
	//			"assetid":36248,
	//			"objectid":"V201711111",
	//			"compressiontype" : "XDCAMHD",
	//			"wrappertype" : "MXF",
	//			"storage" : "X",
	//			"filepath" : "/2010/12/23/V201000036248.mxf"
	//		},
	//		"restorediskdest": {
	//			"assetid":36248,
	//			"startframeindex" : "MXF",
	//			"endframeindex" : "MXF",
	//			"storage" : "X",
	//			"filepath" : "/2010/12/23/V201000036248.mxf"
	//		}
	//	}
	//}
	//  delete ///////////////////////////////////////////////////////////////////
	//	"jobinfos" : {
	//		"deletedtlinstance": {
	//			"assetid":36248,
	//			"objectid":"V201711111",
	//			"dtlgroup":"CMS",
	//			"dtlcategory":"CMS"
	//		}
	//	}
	//}
	//  get volume info ///////////////////////////////////////////////////////////////////
	//	"jobinfos" : {
	//		"getvolumeinfo":"dtl1"
	//	}
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////

	void* pRet = NULL;

	utility::string_t strKey;

	try
	{
		if (m_stdstrJasonJobInfo.empty())
		{
			web::json::json_exception e(L"Job info is empty.");

			throw e;
		}
		
		string stdstrTemp;
		GetMamExStartJobData(stdstrTemp);
		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		strKey = COMPJOB_JSON_KEY_JOBINFOS;
		json::value jsonJobInfo = jsonJobData.at(strKey);

		m_AMJobData.m_stdstrJobJson = m_stdstrJasonJobInfo;

		if (jsonJobInfo.size() > 0)
		{
			strKey = COMPJOB_JSON_KEY_ARCHIVE_DISKSRC;
			if (jsonJobInfo.has_field(strKey))
			{
				m_AMJobData.m_eAMJobType = eArchive;

				json::value jsonJobSrc = jsonJobInfo.at(strKey);
				ExtractJobDataArchiveDiskSrc(&jsonJobSrc);
			}

			strKey = COMPJOB_JSON_KEY_ARCHIVE_DTLDEST;
			if (jsonJobInfo.has_field(strKey))
			{
				json::value jsonJobDest = jsonJobInfo.at(strKey);
				ExtractJobDataArchiveDTLDest(&jsonJobDest);
			}

			strKey = COMPJOB_JSON_KEY_RESTORE_DISKSRC;
			if (jsonJobInfo.has_field(strKey))
			{
				m_AMJobData.m_eAMJobType = eRestore;

				json::value jsonJobSrc = jsonJobInfo.at(strKey);

				if (m_AMJobData.m_stdstrJobType.compare("restore"))
					ExtractJobDataRestoreDiskSrc(&jsonJobSrc);

				if (m_AMJobData.m_stdstrJobType.compare("partialrestore"))
				{
					m_AMJobData.m_eAMJobType = ePartialRestore;

					ExtractJobDataPartialRestoreDiskSrc(&jsonJobSrc);
				}
			}

			strKey = COMPJOB_JSON_KEY_RESTORE_DTLSRC;
			if (jsonJobInfo.has_field(strKey))
			{
				m_AMJobData.m_eAMJobType = eRestore;

				json::value jsonJobSrc = jsonJobInfo.at(strKey);

				if (m_AMJobData.m_stdstrJobType.compare("restore"))
					ExtractJobDataRestoreDTLSrc(&jsonJobSrc);

				if (m_AMJobData.m_stdstrJobType.compare("partialrestore"))
				{
					m_AMJobData.m_eAMJobType = ePartialRestore;

					ExtractJobDataPartialRestoreDTLSrc(&jsonJobSrc);
				}
			}

			strKey = COMPJOB_JSON_KEY_RESTORE_DISKDEST;
			if (jsonJobInfo.has_field(strKey))
			{
				json::value jsonJobSrc = jsonJobInfo.at(strKey);
				ExtractJobDataRestoreDTLSrc(&jsonJobSrc);
			}

			strKey = COMPJOB_JSON_KEY_DELETEINSTANCE_IN_DTL;
			if (jsonJobInfo.has_field(strKey))
			{
				m_AMJobData.m_eAMJobType = eDelete;

				json::value jsonJobDelete = jsonJobInfo.at(strKey);
				ExtractJobDataDeleteDTLInstance(&jsonJobDelete);
			}

			strKey = COMPJOB_JSON_KEY_GET_VOLUME_INFO;
			if (jsonJobInfo.has_field(strKey))
			{
				m_AMJobData.m_eAMJobType = eVolumeInfo;

				m_AMJobData.m_stdstrDTLName = utility::conversions::utf16_to_utf8(jsonJobInfo.at(strKey).as_string());
			}

			pRet = &m_AMJobData;
		}
	}
	catch (web::json::json_exception const & e)
	{
		m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();

		if (strKey.empty())
			m_stdstrErrorMsg.assign(e.what());
		else
			m_stdstrErrorMsg = Utility::string_format("Key: %s - %s", utility::conversions::utf16_to_utf8(strKey).c_str(), e.what());

		m_AMJobData.Clear();

		printf("%s\n", m_stdstrErrorMsg.c_str());
		printf("%s\n", m_stdstrJasonJobInfo.c_str());
	}

	return pRet;
}

void* CKRestAMCompJob::GetJobData()
{
	return &m_AMJobData;
}

int CKRestAMCompJob::GetMamExStartJobData(string& stdstrJson)
{
	int nRet = 0;
	utility::string_t strKey;

	stdstrJson = "";

	try
	{
		if (m_stdstrJasonJobInfo.empty())
		{
			web::json::json_exception e(L"Job info is empty.");

			throw e;
		}

		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		strKey = COMPJOB_JSON_KEY_MAMEX;
		if (jsonJobData.has_field(strKey) && !jsonJobData.at(strKey).is_null())
		{
			json::value jsonJobMamEx = jsonJobData.at(strKey);

			strKey = COMPJOB_JSON_KEY_MAMEX_POOL;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_AMJobData.m_nMamExPool = jsonJobMamEx[strKey].as_integer();

			strKey = COMPJOB_JSON_KEY_MAMEX_START;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_AMJobData.m_stdstrMamExStart = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());

			strKey = COMPJOB_JSON_KEY_MAMEX_PROGRESS;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_AMJobData.m_stdstrMamExProgress = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());

			strKey = COMPJOB_JSON_KEY_MAMEX_END;
			if (jsonJobMamEx.has_field(strKey) && !jsonJobMamEx.at(strKey).is_null())
				m_AMJobData.m_stdstrMamExEnd = utility::conversions::utf16_to_utf8(jsonJobMamEx[strKey].as_string());
		}

		strKey = COMPJOB_JSON_KEY_JOBINFOS;
		json::value jsonJobInfo = jsonJobData.at(strKey);

		strKey = COMPJOB_JSON_KEY_WORKFLOWID;
		m_AMJobData.m_nWorkflowID = jsonJobData[strKey].as_number().to_int64();
		strKey = COMPJOB_JSON_KEY_JOBID;
		m_AMJobData.m_nJobID = jsonJobData[strKey].as_number().to_int64();
		m_nJobID = m_AMJobData.m_nJobID;
		strKey = COMPJOB_JSON_KEY_WORKFLOWNAME;
		m_AMJobData.m_stdstrWorkflowName = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());
		strKey = COMPJOB_JSON_KEY_JOBNAME;
		m_AMJobData.m_stdstrJobName = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());
		strKey = COMPJOB_JSON_KEY_JOBTYPE;
		m_AMJobData.m_stdstrJobType = utility::conversions::utf16_to_utf8(jsonJobData[strKey].as_string());

		m_AMJobData.m_nChannel = m_nChannel;
		m_AMJobData.m_nCheckInID = m_nCheckInID;
		m_AMJobData.m_stdstrJobJson = m_stdstrJasonJobInfo;

		if (!m_AMJobData.m_stdstrMamExStart.empty())
			stdstrJson = m_stdstrJasonJobInfo;
	}
	catch (web::json::json_exception const & e)
	{
		nRet = m_nError = RESTCOMPJOB_ERROR_INVALID_JSON;

		m_stdstrErrorMsg.clear();

		if (strKey.empty())
			m_stdstrErrorMsg.assign(e.what());
		else
			m_stdstrErrorMsg = Utility::string_format("Key: %s - %s", utility::conversions::utf16_to_utf8(strKey).c_str(), e.what());

		m_AMJobData.Clear();

		printf("%s\n", m_stdstrErrorMsg.c_str());
		printf("%s\n", m_stdstrJasonJobInfo.c_str());
	}

	return nRet;
}

string CKRestAMCompJob::GetMamExEndJobData(bool bSuccess, int nErrorCode, string stdstrErrMsg)
{
	string stdstrRet;

	if (!m_AMJobData.m_stdstrMamExEnd.empty())
	{
		utility::string_t strJobData = utility::conversions::utf8_to_utf16(m_stdstrJasonJobInfo);
		json::value jsonJobData = json::value::parse(strJobData);

		jsonJobData[COMPJOB_JSON_KEY_RESULT] = json::value::string(COMPJOB_SUCCESS);

		if (!bSuccess)
			jsonJobData[COMPJOB_JSON_KEY_RESULT] = json::value::string(COMPJOB_FAIL);

		jsonJobData[COMPJOB_JSON_KEY_MESSAGE] = json::value::string(utility::conversions::utf8_to_utf16(stdstrErrMsg));

		stdstrRet = utility::conversions::utf16_to_utf8(jsonJobData.serialize());
	}

	return stdstrRet;
}

string CKRestAMCompJob::GetMamExProgressData(int nProgress, string stdstrExValue)
{
	string stdstrRet;

	if (!m_AMJobData.m_stdstrMamExProgress.empty())
	{
		//stdstrRet = Utility::string_format(
		//	"{\
						//	\"jobid\":\"%I64d\",\
						//	\"progress\":\"%d\",\
						//	\"exvalue\":\"%s\"\
						//	}", m_nJobID, nProgress, stdstrExValue.c_str());

		stdstrRet = CKRestCompJob::GetReportProgressData(nProgress, stdstrExValue);
	}

	return stdstrRet;
}

void CKRestAMCompJob::ExtractJobDataArchiveDiskSrc(void* pJsonValue)
{
	if (pJsonValue)
	{
		if (((json::value*)pJsonValue)->is_null())
		{
			web::json::json_exception e(L"Archive job info is empty(disk SRC).");

			throw e;
		}

		json::value JsonSrcData = *((json::value*)pJsonValue);

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestArchiveJobSrcData SrcData;
		vector<CKRestArchiveJobSrcData> vecSrcData;
		pair<vector<CKRestArchiveJobSrcData>, CKRestArchiveJobDestData> pairSrcDestData;

		if (m_AMJobData.m_vecArchiveSrcDest.empty())
			m_AMJobData.m_vecArchiveSrcDest.push_back(pairSrcDestData);

		strKey = COMPJOB_JSON_KEY_ASSETID;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_nAssetID = JsonSrcData.at(strKey).as_number().to_int64();
		else
		{
			strMsg = L"Key: " + strKey + L" - Archive job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_OBJECTID;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrObjectID = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Archive job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_FILELIST;
		if (JsonSrcData.has_field(strKey))
		{
			json::array jsonArrSrcFiles = JsonSrcData.at(strKey).as_array();
			int nCount = jsonArrSrcFiles.size();

			if (nCount == 0)
			{
				strMsg = L"Key: " + strKey + L" - Archive job source info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			json::value jsonJobFile;
			for (int i = 0; i < nCount; i++)
			{
				SrcData.m_bFolder = false;
				SrcData.m_bDelete = false;
				SrcData.m_bZip = false;

				jsonJobFile = jsonArrSrcFiles.at(i);

				strKey = COMPJOB_JSON_KEY_ISFOLDER;
				if (jsonJobFile.has_field(strKey))
				{
					if (jsonJobFile.at(strKey).as_integer() > 0)
						SrcData.m_bFolder = true;
				}

				strKey = COMPJOB_JSON_KEY_ISDELETE;
				if (jsonJobFile.has_field(strKey))
				{
					if (jsonJobFile.at(strKey).as_integer() > 0)
						SrcData.m_bDelete = true;
				}

				strKey = COMPJOB_JSON_KEY_ISZIP;
				if (jsonJobFile.has_field(strKey))
				{
					if (jsonJobFile.at(strKey).as_integer() > 0)
						SrcData.m_bZip = true;
				}

				strKey = COMPJOB_JSON_KEY_STORAGE;
				if (jsonJobFile.has_field(strKey))
					SrcData.m_FileInfo.m_stdstrStorage = utility::conversions::utf16_to_utf8(jsonJobFile.at(strKey).as_string());
				else
				{
					strMsg = L"Key: " + strKey + L" - Archive job source info is not found.";
					web::json::json_exception e(strMsg.c_str());

					throw e;
				}

				strKey = COMPJOB_JSON_KEY_FILEPATH;
				if (jsonJobFile.has_field(strKey))
					SrcData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(jsonJobFile.at(strKey).as_string());
				else
				{
					strMsg = L"Key: " + strKey + L" - Archive job source info is not found.";
					web::json::json_exception e(strMsg.c_str());

					throw e;
				}

				vecSrcData.push_back(SrcData);
			}

			m_AMJobData.m_vecArchiveSrcDest[0].first = vecSrcData;
		}
		else
		{
			strMsg = L"Key: " + strKey + L" - Archive job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}
	}
}

void CKRestAMCompJob::ExtractJobDataArchiveDTLDest(void* pJsonValue)
{
	if (pJsonValue)
	{
		if (((json::value*)pJsonValue)->is_null())
		{
			web::json::json_exception e(L"Archive job info is empty(DTL DEST).");

			throw e;
		}

		json::value JsonDestData = *((json::value*)pJsonValue);

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestArchiveJobDestData DestData;
		pair<vector<CKRestArchiveJobSrcData>, CKRestArchiveJobDestData> pairSrcDestData;

		if (m_AMJobData.m_vecArchiveSrcDest.empty())
			m_AMJobData.m_vecArchiveSrcDest.push_back(pairSrcDestData);

		strKey = COMPJOB_JSON_KEY_OBJECTID;
		if (JsonDestData.has_field(strKey))
			DestData.m_stdstrObjectID = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Archive job destination info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLGROUP;
		if (JsonDestData.has_field(strKey))
			DestData.m_stdstrDTLGroup = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Archive job destination info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLCATEGORY;
		if (JsonDestData.has_field(strKey))
			DestData.m_stdstrDTLCategory = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Archive job destination info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_FILEPATH;
		if (JsonDestData.has_field(strKey))
			DestData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Archive job destination info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		m_AMJobData.m_vecArchiveSrcDest[0].second = DestData;
	}
}

void CKRestAMCompJob::ExtractJobDataRestoreDiskSrc(void* pJsonValue)
{
	{
		if (pJsonValue)
		{
			if (((json::value*)pJsonValue)->is_null())
			{
				web::json::json_exception e(L"Restore job info is empty(Disk SRC).");

				throw e;
			}

			json::value JsonDestData = *((json::value*)pJsonValue);

			utility::string_t strMsg;
			utility::string_t strKey;

			CKRestArchiveJobDestData DestData;
			pair<vector<CKRestArchiveJobSrcData>, CKRestArchiveJobDestData> pairSrcDestData;

			if (m_AMJobData.m_vecArchiveSrcDest.empty())
				m_AMJobData.m_vecArchiveSrcDest.push_back(pairSrcDestData);

			strKey = COMPJOB_JSON_KEY_OBJECTID;
			if (JsonDestData.has_field(strKey))
				DestData.m_stdstrObjectID = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
			else
			{
				strMsg = L"Key: " + strKey + L" - Archive job destination info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			strKey = COMPJOB_JSON_KEY_DTLGROUP;
			if (JsonDestData.has_field(strKey))
				DestData.m_stdstrDTLGroup = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
			else
			{
				strMsg = L"Key: " + strKey + L" - Archive job destination info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			strKey = COMPJOB_JSON_KEY_DTLCATEGORY;
			if (JsonDestData.has_field(strKey))
				DestData.m_stdstrDTLCategory = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
			else
			{
				strMsg = L"Key: " + strKey + L" - Archive job destination info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			strKey = COMPJOB_JSON_KEY_FILEPATH;
			if (JsonDestData.has_field(strKey))
				DestData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
			else
			{
				strMsg = L"Key: " + strKey + L" - Archive job destination info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			m_AMJobData.m_vecArchiveSrcDest[0].second = DestData;
		}
	}
}

void CKRestAMCompJob::ExtractJobDataRestoreDTLSrc(void* pJsonValue)
{
	if (pJsonValue)
	{
		if (((json::value*)pJsonValue)->is_null())
		{
			web::json::json_exception e(L"Restore job info is empty(DTL SRC).");

			throw e;
		}

		json::value JsonSrcData = *((json::value*)pJsonValue);

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestRestoreJobSrcData SrcData;
		vector<CKRestRestoreJobSrcData> vecSrcData;
		pair<vector<CKRestRestoreJobSrcData>, CKRestRestoreJobDestData> pairSrcDestData;

		if (m_AMJobData.m_vecRestoreSrcDest.empty())
			m_AMJobData.m_vecRestoreSrcDest.push_back(pairSrcDestData);

		strKey = COMPJOB_JSON_KEY_ASSETID;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_nAssetID = JsonSrcData.at(strKey).as_number().to_int64();
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_OBJECTID;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrObjectID = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLGROUP;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrDTLGroup = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLCATEGORY;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrDTLCategory = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_FILELIST;
		if (JsonSrcData.has_field(strKey))
		{
			json::array jsonArrSrcFiles = JsonSrcData.at(strKey).as_array();
			int nCount = jsonArrSrcFiles.size();

			if (nCount == 0)
			{
				strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
				web::json::json_exception e(strMsg.c_str());

				throw e;
			}

			json::value jsonJobFile;
			for (int i = 0; i < nCount; i++)
			{
				jsonJobFile = jsonArrSrcFiles.at(i);

				strKey = COMPJOB_JSON_KEY_STORAGE;
				if (jsonJobFile.has_field(strKey))
					SrcData.m_FileInfo.m_stdstrStorage = utility::conversions::utf16_to_utf8(jsonArrSrcFiles.at(i).at(strKey).as_string());
				else
				{
					strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
					web::json::json_exception e(strMsg.c_str());

					throw e;
				}

				strKey = COMPJOB_JSON_KEY_FILEPATH;
				if (jsonJobFile.has_field(strKey))
					SrcData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(jsonArrSrcFiles.at(i).at(strKey).as_string());
				else
				{
					strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
					web::json::json_exception e(strMsg.c_str());

					throw e;
				}

				vecSrcData.push_back(SrcData);
			}

			m_AMJobData.m_vecRestoreSrcDest[0].first = vecSrcData;
		}
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}
	}
}

void CKRestAMCompJob::ExtractJobDataPartialRestoreDiskSrc(void* pJsonValue)
{
	if (pJsonValue)
	{
		if (((json::value*)pJsonValue)->is_null())
		{
			web::json::json_exception e(L"Restore job info is empty(Disk SRC).");

			throw e;
		}

		json::value JsonSrcData = *((json::value*)pJsonValue);

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestRestoreJobSrcData SrcData;
		vector<CKRestRestoreJobSrcData> vecSrcData;
		pair<vector<CKRestRestoreJobSrcData>, CKRestRestoreJobDestData> pairSrcDestData;

		if (m_AMJobData.m_vecRestoreSrcDest.empty())
			m_AMJobData.m_vecRestoreSrcDest.push_back(pairSrcDestData);

		strKey = COMPJOB_JSON_KEY_ASSETID;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_nAssetID = JsonSrcData.at(strKey).as_number().to_int64();
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_OBJECTID;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrObjectID = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLGROUP;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrDTLGroup = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLCATEGORY;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrDTLCategory = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_COMPRESSIONTYPE;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrCompressiontype = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_WRAPPERTYPE;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrWrappertype = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_FILEPATH;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		vecSrcData.push_back(SrcData);
		m_AMJobData.m_vecRestoreSrcDest[0].first = vecSrcData;
	}
}

void CKRestAMCompJob::ExtractJobDataPartialRestoreDTLSrc(void* pJsonValue)
{
	if (pJsonValue)
	{
		if (((json::value*)pJsonValue)->is_null())
		{
			web::json::json_exception e(L"Restore job info is empty(DTL SRC).");

			throw e;
		}

		json::value JsonSrcData = *((json::value*)pJsonValue);

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestRestoreJobSrcData SrcData;
		vector<CKRestRestoreJobSrcData> vecSrcData;
		pair<vector<CKRestRestoreJobSrcData>, CKRestRestoreJobDestData> pairSrcDestData;

		if (m_AMJobData.m_vecRestoreSrcDest.empty())
			m_AMJobData.m_vecRestoreSrcDest.push_back(pairSrcDestData);

		strKey = COMPJOB_JSON_KEY_ASSETID;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_nAssetID = JsonSrcData.at(strKey).as_number().to_int64();
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_OBJECTID;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrObjectID = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLGROUP;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrDTLGroup = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLCATEGORY;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrDTLCategory = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_COMPRESSIONTYPE;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrCompressiontype = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_WRAPPERTYPE;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_stdstrWrappertype = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_FILEPATH;
		if (JsonSrcData.has_field(strKey))
			SrcData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(JsonSrcData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job source info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		vecSrcData.push_back(SrcData);
		m_AMJobData.m_vecRestoreSrcDest[0].first = vecSrcData;
	}
}

void CKRestAMCompJob::ExtractJobDataRestoreDiskDest(void* pJsonValue)
{
	if (pJsonValue)
	{
		if (((json::value*)pJsonValue)->is_null())
		{
			web::json::json_exception e(L"Restore job info is empty(Disk DEST).");

			throw e;
		}

		json::value JsonDestData = *((json::value*)pJsonValue);

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestRestoreJobDestData DestData;
		pair<vector<CKRestRestoreJobSrcData>, CKRestRestoreJobDestData> pairSrcDestData;

		if (m_AMJobData.m_vecRestoreSrcDest.empty())
			m_AMJobData.m_vecRestoreSrcDest.push_back(pairSrcDestData);

		strKey = COMPJOB_JSON_KEY_ASSETID;
		if (JsonDestData.has_field(strKey))
			DestData.m_nAssetID = JsonDestData.at(strKey).as_number().to_int64();

		strKey = COMPJOB_JSON_KEY_STARTFRAMEINDEX;
		if (JsonDestData.has_field(strKey))
			DestData.m_nStartFrameIndex = JsonDestData.at(strKey).as_integer();

		strKey = COMPJOB_JSON_KEY_ENDFRAMEINDEX;
		if (JsonDestData.has_field(strKey))
			DestData.m_nEndFrameIndex = JsonDestData.at(strKey).as_integer();

		strKey = COMPJOB_JSON_KEY_STORAGE;
		if (JsonDestData.has_field(strKey))
			DestData.m_FileInfo.m_stdstrStorage = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job destination info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_FILEPATH;
		if (JsonDestData.has_field(strKey))
			DestData.m_FileInfo.m_stdstrPath = utility::conversions::utf16_to_utf8(JsonDestData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Restore job destination info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		m_AMJobData.m_vecRestoreSrcDest[0].second = DestData;
	}
}

void CKRestAMCompJob::ExtractJobDataDeleteDTLInstance(void* pJsonValue)
{
	if (pJsonValue)
	{
		if (((json::value*)pJsonValue)->is_null())
		{
			web::json::json_exception e(L"Delete job info is empty(DTL Instance).");

			throw e;
		}

		json::value JsonDeleteData = *((json::value*)pJsonValue);

		utility::string_t strMsg;
		utility::string_t strKey;

		CKRestDeleteInstanceJobData DeleteData;
		
		DeleteData.m_bFolder = false;

		strKey = COMPJOB_JSON_KEY_ASSETID;
		if (JsonDeleteData.has_field(strKey))
			DeleteData.m_nAssetID = JsonDeleteData.at(strKey).as_number().to_int64();
		else
		{
			strMsg = L"Key: " + strKey + L" - Delete job info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}
		
		strKey = COMPJOB_JSON_KEY_OBJECTID;
		if (JsonDeleteData.has_field(strKey))
			DeleteData.m_stdstrObjectID = utility::conversions::utf16_to_utf8(JsonDeleteData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Delete job info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLGROUP;
		if (JsonDeleteData.has_field(strKey))
			DeleteData.m_stdstrDTLGroup = utility::conversions::utf16_to_utf8(JsonDeleteData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Delete job info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		strKey = COMPJOB_JSON_KEY_DTLCATEGORY;
		if (JsonDeleteData.has_field(strKey))
			DeleteData.m_stdstrDTLCategory = utility::conversions::utf16_to_utf8(JsonDeleteData.at(strKey).as_string());
		else
		{
			strMsg = L"Key: " + strKey + L" - Delete job info is not found.";
			web::json::json_exception e(strMsg.c_str());

			throw e;
		}

		m_AMJobData.m_vceDeleteData.push_back(DeleteData);
	}
}
