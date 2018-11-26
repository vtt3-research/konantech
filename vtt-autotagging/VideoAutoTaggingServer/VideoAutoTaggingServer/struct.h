#pragma once

#include <vector>

typedef struct cms_info
{
	CString strMamUri;
	CString strExUri;
	CString strReceiveUri;
	CString strPassword;
	CString strUser;
	CString strFFmpegPath;
	CString strInterval;
	CString strRecognizeApiUri;
	CString strServiceType;
	CString strSceneSensitivity;
	
	int nChannel;
	int nExpool;
	int nPool;

	bool bAutoConnect;

} CMS_INFO;

typedef struct mamex
{
	string strStartExTrName;
	string strProgressTrName;
	string strEndExTrName;

}MAMEX;

typedef struct jobinfo
{
	CString strJobInfo;
	CString strSrcStorage;
	CString strSrcFilepath;
	CString strSrcFullPath;

	int nSrcAssetID;

	double dbVideoframerate;

	CString strDstStorage;
	CString strDstFilePath;
	CString strDstFullPath;

}JOB_INFO;

typedef struct kvpair
{
	int m_nAuxJobID;
	int m_nWorkfolwID;
	int m_nWorkerID;
	int m_nKeyValue;
	int m_nStatus;
	CString m_strCurrentJobName;

}KVPAIR;