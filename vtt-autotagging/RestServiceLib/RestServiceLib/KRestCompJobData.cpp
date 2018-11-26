
#include "stdafx.h"
#include "KRestCompJobData.h"

#include "Utility.h"

#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <cpprest/json.h>

#pragma warning (disable : 4996)

using namespace web;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EX_TRANSACTION_NAME_UPDATE_JOBRESULT		("ex_update_jobresult")

///////////////////////////////////////////////////////////////////////////////////////

CKRestCompJobFileInfo::CKRestCompJobFileInfo()
{
	m_eStorageType = eStorageTypeDisk;

	m_nPort = 0;
	//m_nAllowExistFile = FILE_EXIST_THEN_OVERWRITE;

	//m_bFolder = false;
	//m_bDelete = false;
	//m_bZip = false;
}

CKRestCompJobFileInfo::CKRestCompJobFileInfo(const CKRestCompJobFileInfo& operand)
{
	m_eStorageType = operand.m_eStorageType;

	m_stdstrStorage = operand.m_stdstrStorage;
	m_stdstrPath = operand.m_stdstrPath;

	m_stdstrHostAddress = operand.m_stdstrHostAddress;
	m_stdstrUser = operand.m_stdstrUser;
	m_stdstrPassword = operand.m_stdstrPassword;

	//m_stdstrServerType = operand.m_stdstrServerType;

	m_nPort = operand.m_nPort;
	//m_nAllowExistFile = operand.m_nAllowExistFile;

	//m_bFolder = operand.m_bFolder;
	//m_bDelete = operand.m_bDelete;
	//m_bZip = operand.m_bZip;

	//m_vecExceptFilePath = operand.m_vecExceptFilePath;
}

CKRestCompJobFileInfo CKRestCompJobFileInfo::operator=(const CKRestCompJobFileInfo& operand)
{
	m_eStorageType = operand.m_eStorageType;

	m_stdstrStorage = operand.m_stdstrStorage;
	m_stdstrPath = operand.m_stdstrPath;

	m_stdstrHostAddress = operand.m_stdstrHostAddress;
	m_stdstrUser = operand.m_stdstrUser;
	m_stdstrPassword = operand.m_stdstrPassword;

	//m_stdstrServerType = operand.m_stdstrServerType;

	m_nPort = operand.m_nPort;
	//m_nAllowExistFile = operand.m_nAllowExistFile;

	//m_bFolder = operand.m_bFolder;
	//m_bDelete = operand.m_bDelete;
	//m_bZip = operand.m_bZip;

	//m_vecExceptFilePath = operand.m_vecExceptFilePath;

	return *this;
}

CKRestCompJobFileInfo::~CKRestCompJobFileInfo()
{

}

void CKRestCompJobFileInfo::Clear()
{
	m_eStorageType = eStorageTypeDisk;

	m_stdstrStorage.clear();
	m_stdstrPath.clear();

	m_stdstrHostAddress.clear();
	m_stdstrUser.clear();
	m_stdstrPassword.clear();
	//m_stdstrServerType.clear();

	m_nPort = 0;
	//m_nAllowExistFile = FILE_EXIST_THEN_OVERWRITE;

	//m_bFolder = false;
	//m_bDelete = false;
	//m_bZip = false;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestCompJobAssetData::CKRestCompJobAssetData()
{
	m_nAssetID = 0;
}

CKRestCompJobAssetData::CKRestCompJobAssetData(const CKRestCompJobAssetData& operand)
{
	m_FileInfo = operand.m_FileInfo;

	//m_stdstrVideoWrapperType = operand.m_stdstrVideoWrapperType;
	//m_stdstrMobType = operand.m_stdstrMobType;
	m_stdstrObjectID = operand.m_stdstrObjectID;

	m_nAssetID = operand.m_nAssetID;
}

CKRestCompJobAssetData CKRestCompJobAssetData::operator=(const CKRestCompJobAssetData& operand)
{
	m_FileInfo = operand.m_FileInfo;

	//m_stdstrVideoWrapperType = operand.m_stdstrVideoWrapperType;
	//m_stdstrMobType = operand.m_stdstrMobType;
	m_stdstrObjectID = operand.m_stdstrObjectID;

	m_nAssetID = operand.m_nAssetID;

	return *this;
}

CKRestCompJobAssetData::~CKRestCompJobAssetData()
{

}

void CKRestCompJobAssetData::Clear()
{
	m_FileInfo.Clear();

	//m_stdstrVideoWrapperType.clear();
	//m_stdstrMobType.clear();
	m_stdstrObjectID.clear();

	m_nAssetID = 0;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestCAJobSrcData::CKRestCAJobSrcData()
{
	m_dbFramerate = 0.0;
}

CKRestCAJobSrcData::CKRestCAJobSrcData(const CKRestCAJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);
	m_dbFramerate = operand.m_dbFramerate;
	m_stdstrTotalDuration = operand.m_stdstrTotalDuration;
}

CKRestCAJobSrcData CKRestCAJobSrcData::operator=(const CKRestCAJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);
	m_dbFramerate = operand.m_dbFramerate;
	m_stdstrTotalDuration = operand.m_stdstrTotalDuration;

	return *this;
}

CKRestCAJobSrcData::~CKRestCAJobSrcData()
{

}

void CKRestCAJobSrcData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_dbFramerate = 0.0;
	m_stdstrTotalDuration = "";
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestTCJobSrcData::CKRestTCJobSrcData()
{
	m_eMediaType = eVideo;
}

CKRestTCJobSrcData::CKRestTCJobSrcData(const CKRestTCJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_eMediaType = operand.m_eMediaType;
	m_stdstrVideoWrapperType = operand.m_stdstrVideoWrapperType;
}

CKRestTCJobSrcData CKRestTCJobSrcData::operator=(const CKRestTCJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_eMediaType = operand.m_eMediaType;
	m_stdstrVideoWrapperType = operand.m_stdstrVideoWrapperType;

	return *this;
}

CKRestTCJobSrcData::~CKRestTCJobSrcData()
{

}

void CKRestTCJobSrcData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_eMediaType = eVideo;
	m_stdstrVideoWrapperType.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestTCJobDestData::CKRestTCJobDestData()
{
	m_eMediaType = eProxyVideo;
}

CKRestTCJobDestData::CKRestTCJobDestData(const CKRestTCJobDestData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_eMediaType = operand.m_eMediaType;
	m_stdstrMobType = operand.m_stdstrMobType;
}

CKRestTCJobDestData CKRestTCJobDestData::operator=(const CKRestTCJobDestData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_eMediaType = operand.m_eMediaType;
	m_stdstrMobType = operand.m_stdstrMobType;

	return *this;
}

CKRestTCJobDestData::~CKRestTCJobDestData()
{

}

void CKRestTCJobDestData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_eMediaType = eProxyVideo;
	m_stdstrMobType.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestTMJobSrcData::CKRestTMJobSrcData()
{
	m_bFolder = false;
	m_bDelete = false;
	m_bZip = false;
}

CKRestTMJobSrcData::CKRestTMJobSrcData(const CKRestTMJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_stdstrRename = operand.m_stdstrRename;
	m_stdstrServerType = operand.m_stdstrServerType;

	m_bFolder = operand.m_bFolder;
	m_bDelete = operand.m_bDelete;
	m_bZip = operand.m_bZip;

	m_vecExceptFilePath = operand.m_vecExceptFilePath;
}

CKRestTMJobSrcData CKRestTMJobSrcData::operator=(const CKRestTMJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_stdstrRename = operand.m_stdstrRename;
	m_stdstrServerType = operand.m_stdstrServerType;

	m_bFolder = operand.m_bFolder;
	m_bDelete = operand.m_bDelete;
	m_bZip = operand.m_bZip;

	m_vecExceptFilePath = operand.m_vecExceptFilePath;

	return *this;
}

CKRestTMJobSrcData::~CKRestTMJobSrcData()
{

}

void CKRestTMJobSrcData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_stdstrRename.clear();
	m_stdstrServerType.clear();

	m_bFolder = false;
	m_bDelete = false;
	m_bZip = false;

	m_vecExceptFilePath.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestTMJobDestData::CKRestTMJobDestData()
{
	m_nAllowExistFile = FILE_EXIST_THEN_OVERWRITE;
}

CKRestTMJobDestData::CKRestTMJobDestData(const CKRestTMJobDestData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_stdstrDisplayPath = operand.m_stdstrDisplayPath;
	m_stdstrServerType = operand.m_stdstrServerType;

	m_nAllowExistFile = operand.m_nAllowExistFile;
}

CKRestTMJobDestData CKRestTMJobDestData::operator=(const CKRestTMJobDestData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_stdstrDisplayPath = operand.m_stdstrDisplayPath;
	m_stdstrServerType = operand.m_stdstrServerType;

	m_nAllowExistFile = operand.m_nAllowExistFile;

	return *this;
}

CKRestTMJobDestData::~CKRestTMJobDestData()
{

}

void CKRestTMJobDestData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_stdstrDisplayPath.clear();
	m_stdstrServerType.clear();

	m_nAllowExistFile = FILE_EXIST_THEN_OVERWRITE;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestArchiveJobSrcData::CKRestArchiveJobSrcData()
{
	m_bFolder = false;
	m_bDelete = false;
	m_bZip = false;
}

CKRestArchiveJobSrcData::CKRestArchiveJobSrcData(const CKRestArchiveJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_bFolder = operand.m_bFolder;
	m_bDelete = operand.m_bDelete;
	m_bZip = operand.m_bZip;
}

CKRestArchiveJobSrcData CKRestArchiveJobSrcData::operator=(const CKRestArchiveJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_bFolder = operand.m_bFolder;
	m_bDelete = operand.m_bDelete;
	m_bZip = operand.m_bZip;

	return *this;
}

CKRestArchiveJobSrcData::~CKRestArchiveJobSrcData()
{

}

void CKRestArchiveJobSrcData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_bFolder = false;
	m_bDelete = false;
	m_bZip = false;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestArchiveJobDestData::CKRestArchiveJobDestData()
{
	m_nAllowExistFile = FILE_EXIST_THEN_OVERWRITE;
}

CKRestArchiveJobDestData::CKRestArchiveJobDestData(const CKRestArchiveJobDestData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_stdstrDTLGroup = operand.m_stdstrDTLGroup;
	m_stdstrDTLCategory = operand.m_stdstrDTLCategory;

	m_nAllowExistFile = operand.m_nAllowExistFile;
}

CKRestArchiveJobDestData CKRestArchiveJobDestData::operator=(const CKRestArchiveJobDestData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_stdstrDTLGroup = operand.m_stdstrDTLGroup;
	m_stdstrDTLCategory = operand.m_stdstrDTLCategory;

	m_nAllowExistFile = operand.m_nAllowExistFile;

	return *this;
}

CKRestArchiveJobDestData::~CKRestArchiveJobDestData()
{

}

void CKRestArchiveJobDestData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_stdstrDTLGroup.clear();
	m_stdstrDTLCategory.clear();

	m_nAllowExistFile = FILE_EXIST_THEN_OVERWRITE;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestRestoreJobSrcData::CKRestRestoreJobSrcData()
{

}

CKRestRestoreJobSrcData::CKRestRestoreJobSrcData(const CKRestRestoreJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_stdstrDTLGroup = operand.m_stdstrDTLGroup;
	m_stdstrDTLCategory = operand.m_stdstrDTLCategory;

	m_stdstrCompressiontype = operand.m_stdstrCompressiontype;
	m_stdstrWrappertype = operand.m_stdstrWrappertype;
}

CKRestRestoreJobSrcData CKRestRestoreJobSrcData::operator=(const CKRestRestoreJobSrcData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_stdstrDTLGroup = operand.m_stdstrDTLGroup;
	m_stdstrDTLCategory = operand.m_stdstrDTLCategory;

	m_stdstrCompressiontype = operand.m_stdstrCompressiontype;
	m_stdstrWrappertype = operand.m_stdstrWrappertype;

	return *this;
}

CKRestRestoreJobSrcData::~CKRestRestoreJobSrcData()
{

}

void CKRestRestoreJobSrcData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_stdstrDTLGroup.clear();
	m_stdstrDTLCategory.clear();

	m_stdstrCompressiontype.clear();
	m_stdstrWrappertype.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestRestoreJobDestData::CKRestRestoreJobDestData()
{
	m_nStartFrameIndex = -1;
	m_nEndFrameIndex = -1;
}

CKRestRestoreJobDestData::CKRestRestoreJobDestData(const CKRestRestoreJobDestData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_nStartFrameIndex = operand.m_nStartFrameIndex;
	m_nEndFrameIndex = operand.m_nEndFrameIndex;
}

CKRestRestoreJobDestData CKRestRestoreJobDestData::operator=(const CKRestRestoreJobDestData& operand)
{
	CKRestCompJobAssetData::operator=(operand);

	m_nStartFrameIndex = operand.m_nStartFrameIndex;
	m_nEndFrameIndex = operand.m_nEndFrameIndex;

	return *this;
}

CKRestRestoreJobDestData::~CKRestRestoreJobDestData()
{

}

void CKRestRestoreJobDestData::Clear()
{
	CKRestCompJobAssetData::Clear();

	m_nStartFrameIndex = -1;
	m_nEndFrameIndex = -1;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestDeleteInstanceJobData::CKRestDeleteInstanceJobData()
{

}

CKRestDeleteInstanceJobData::CKRestDeleteInstanceJobData(const CKRestDeleteInstanceJobData& operand)
{
	CKRestArchiveJobSrcData::operator=(operand);

	m_stdstrDTLGroup = operand.m_stdstrDTLGroup;
	m_stdstrDTLCategory = operand.m_stdstrDTLCategory;
}

CKRestDeleteInstanceJobData CKRestDeleteInstanceJobData::operator=(const CKRestDeleteInstanceJobData& operand)
{
	CKRestArchiveJobSrcData::operator=(operand);

	m_stdstrDTLGroup = operand.m_stdstrDTLGroup;
	m_stdstrDTLCategory = operand.m_stdstrDTLCategory;

	return *this;
}

CKRestDeleteInstanceJobData::~CKRestDeleteInstanceJobData()
{

}

void CKRestDeleteInstanceJobData::Clear()
{
	CKRestArchiveJobSrcData::Clear();

	m_stdstrDTLGroup.clear();
	m_stdstrDTLCategory.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestCompJobData::CKRestCompJobData()
{
	m_nChannel = -1;
	m_nMamExPool = 0;
	m_nError = 0;

	m_nCheckInID = 0;
	m_nWorkflowID = 0;
	m_nJobID = 0;
}

CKRestCompJobData::CKRestCompJobData(const CKRestCompJobData& operand)
{
	m_nChannel = operand.m_nChannel;
	m_nMamExPool = operand.m_nMamExPool;
	m_nError = operand.m_nError;

	m_nCheckInID = operand.m_nCheckInID;
	m_nWorkflowID = operand.m_nWorkflowID;
	m_nJobID = operand.m_nJobID;

	m_stdstrWorkflowName = operand.m_stdstrWorkflowName;
	m_stdstrJobName = operand.m_stdstrJobName;
	m_stdstrJobType = operand.m_stdstrJobType;

	m_stdstrMamExStart = operand.m_stdstrMamExStart;
	m_stdstrMamExProgress = operand.m_stdstrMamExProgress;
	m_stdstrMamExEnd = operand.m_stdstrMamExEnd;

	m_stdstrJobJson = operand.m_stdstrJobJson;
	m_stdstrQuery = operand.m_stdstrQuery;
	m_stdstrErrMsg = operand.m_stdstrErrMsg;
}

CKRestCompJobData CKRestCompJobData::operator=(const CKRestCompJobData& operand)
{
	m_nChannel = operand.m_nChannel;
	m_nMamExPool = operand.m_nMamExPool;
	m_nError = operand.m_nError;

	m_nCheckInID = operand.m_nCheckInID;
	m_nWorkflowID = operand.m_nWorkflowID;
	m_nJobID = operand.m_nJobID;

	m_stdstrWorkflowName = operand.m_stdstrWorkflowName;
	m_stdstrJobName = operand.m_stdstrJobName;
	m_stdstrJobType = operand.m_stdstrJobType;

	m_stdstrMamExStart = operand.m_stdstrMamExStart;
	m_stdstrMamExProgress = operand.m_stdstrMamExProgress;
	m_stdstrMamExEnd = operand.m_stdstrMamExEnd;

	m_stdstrJobJson = operand.m_stdstrJobJson;
	m_stdstrQuery = operand.m_stdstrQuery;
	m_stdstrErrMsg = operand.m_stdstrErrMsg;

	return *this;
}

CKRestCompJobData::~CKRestCompJobData()
{

}

void CKRestCompJobData::Clear()
{
	m_nChannel = -1;
	m_nMamExPool = 0;
	m_nError = 0;

	m_nCheckInID = 0;
	m_nWorkflowID = 0;
	m_nJobID = 0;

	m_stdstrWorkflowName.clear();
	m_stdstrJobName.clear();
	m_stdstrJobType.clear();

	m_stdstrMamExStart.clear();
	m_stdstrMamExProgress.clear();
	m_stdstrMamExEnd.clear();

	m_stdstrJobJson.clear();
	m_stdstrQuery.clear();
	m_stdstrErrMsg.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestCAJobData::CKRestCAJobData()
{

}

CKRestCAJobData::CKRestCAJobData(const CKRestCAJobData& operand)
{
	CKRestCompJobData::operator=(operand);

	m_SrcData = operand.m_SrcData;

	m_stdstrShotImagePath = operand.m_stdstrShotImagePath;
	m_stdstrShotImageStorage = operand.m_stdstrShotImageStorage;
}

CKRestCAJobData CKRestCAJobData::operator=(const CKRestCAJobData& operand)
{
	CKRestCompJobData::operator=(operand);

	m_SrcData = operand.m_SrcData;

	m_stdstrShotImagePath = operand.m_stdstrShotImagePath;
	m_stdstrShotImageStorage = operand.m_stdstrShotImageStorage;

	return *this;
}

CKRestCAJobData::~CKRestCAJobData()
{

}

void CKRestCAJobData::Clear()
{
	CKRestCompJobData::Clear();

	m_SrcData.Clear();

	m_stdstrShotImagePath.clear();
	m_stdstrShotImageStorage.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestTCJobData::CKRestTCJobData()
{

}

CKRestTCJobData::CKRestTCJobData(const CKRestTCJobData& operand)
{
	CKRestCompJobData::operator=(operand);

	m_SrcData = operand.m_SrcData;

	m_vecDestData = operand.m_vecDestData;
}

CKRestTCJobData CKRestTCJobData::operator=(const CKRestTCJobData& operand)
{
	CKRestCompJobData::operator=(operand);

	m_SrcData = operand.m_SrcData;

	m_vecDestData = operand.m_vecDestData;

	return *this;
}

CKRestTCJobData::~CKRestTCJobData()
{

}

void CKRestTCJobData::Clear()
{
	CKRestCompJobData::Clear();

	m_SrcData.Clear();
	m_vecDestData.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestTMJobData::CKRestTMJobData()
{
	
}

CKRestTMJobData::CKRestTMJobData(const CKRestTMJobData& operand)
{
	CKRestCompJobData::operator=(operand);

	m_vecSrcDest = operand.m_vecSrcDest;
}

CKRestTMJobData CKRestTMJobData::operator=(const CKRestTMJobData& operand)
{
	CKRestCompJobData::operator=(operand);

	m_vecSrcDest = operand.m_vecSrcDest;

	return *this;
}

CKRestTMJobData::~CKRestTMJobData()
{

}

void CKRestTMJobData::Clear()
{
	CKRestCompJobData::Clear();

	m_vecSrcDest.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestAMJobData::CKRestAMJobData()
{
	m_eAMJobType = eArchive;
}

CKRestAMJobData::CKRestAMJobData(const CKRestAMJobData& operand)
{
	CKRestCompJobData::operator=(operand);

	m_stdstrDTLName = operand.m_stdstrDTLName;

	m_vecArchiveSrcDest = operand.m_vecArchiveSrcDest;
	m_vecRestoreSrcDest = operand.m_vecRestoreSrcDest;
	m_vceDeleteData = operand.m_vceDeleteData;
}

CKRestAMJobData CKRestAMJobData::operator=(const CKRestAMJobData& operand)
{
	CKRestCompJobData::operator=(operand);

	m_stdstrDTLName = operand.m_stdstrDTLName;

	m_vecArchiveSrcDest = operand.m_vecArchiveSrcDest;
	m_vecRestoreSrcDest = operand.m_vecRestoreSrcDest;
	m_vceDeleteData = operand.m_vceDeleteData;

	return *this;
}

CKRestAMJobData::~CKRestAMJobData()
{

}

void CKRestAMJobData::Clear()
{
	CKRestCompJobData::Clear();

	m_eAMJobType = eArchive;
	m_stdstrDTLName.clear();

	m_vecArchiveSrcDest.clear();
	m_vecRestoreSrcDest.clear();
	m_vceDeleteData.clear();
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestExData::CKRestExData()
{
	m_eMediaType = eShot;
	m_nAssetid = -1;
}

CKRestExData::CKRestExData(const CKRestExData& operand)
{
	m_stdstrWorkingInfoJson = operand.m_stdstrWorkingInfoJson;
	m_eMediaType = operand.m_eMediaType;
	m_nAssetid = operand.m_nAssetid;
}

CKRestExData CKRestExData::operator=(const CKRestExData& operand)
{
	m_stdstrWorkingInfoJson = operand.m_stdstrWorkingInfoJson;
	m_eMediaType = operand.m_eMediaType;
	m_nAssetid = operand.m_nAssetid;

	return *this;
}

CKRestExData::~CKRestExData()
{

}

void CKRestExData::Clear()
{
	m_nAssetid = -1;
	m_nError = 0;

	m_stdstrWorkingInfoJson = "";
	m_stdstrErrMsg = "";
}

string CKRestExData::GetExTransactionName()
{
	string stdstrRet;

	return stdstrRet;
}

string CKRestExData::GetExTransactionData()
{
	string stdstrRet;

	return stdstrRet;
}

string CKRestExData::GetErrorMessage()
{
	return m_stdstrErrMsg;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestExUpdateAudioInfoData::CKRestExUpdateAudioInfoData()
{
	m_eMediaType = eAudio;

	m_nFileSize = 0;

	m_nAudioSampleRate = 0;
	m_nAudioBitPerSample = 0;
	m_nAudioChannel = 0;
	m_nAudioBitrate = 0;
}

CKRestExUpdateAudioInfoData::CKRestExUpdateAudioInfoData(const CKRestExUpdateAudioInfoData& operand)
{
	CKRestExData::operator=(operand);
	
	m_nFileSize = operand.m_nFileSize;

	m_nAudioSampleRate = operand.m_nAudioSampleRate;
	m_nAudioBitPerSample = operand.m_nAudioBitPerSample;
	m_nAudioChannel = operand.m_nAudioChannel;
	m_nAudioBitrate = operand.m_nAudioBitrate;

	m_stdstrAudioFileFormat = operand.m_stdstrAudioFileFormat;
	m_stdstrAudioCompressiontype = operand.m_stdstrAudioCompressiontype;
}

CKRestExUpdateAudioInfoData CKRestExUpdateAudioInfoData::operator=(const CKRestExUpdateAudioInfoData& operand)
{
	CKRestExData::operator=(operand);
	
	m_nFileSize = operand.m_nFileSize;

	m_nAudioSampleRate = operand.m_nAudioSampleRate;
	m_nAudioBitPerSample = operand.m_nAudioBitPerSample;
	m_nAudioChannel = operand.m_nAudioChannel;
	m_nAudioBitrate = operand.m_nAudioBitrate;

	m_stdstrAudioFileFormat = operand.m_stdstrAudioFileFormat;
	m_stdstrAudioCompressiontype = operand.m_stdstrAudioCompressiontype;

	return *this;
}

CKRestExUpdateAudioInfoData::~CKRestExUpdateAudioInfoData()
{

}

void CKRestExUpdateAudioInfoData::Clear()
{
	CKRestExData::Clear();

	m_eMediaType = eAudio;

	m_nFileSize = 0;

	m_nAudioSampleRate = 0;
	m_nAudioBitPerSample = 0;
	m_nAudioChannel = 0;
	m_nAudioBitrate = 0;

	m_stdstrAudioFileFormat.clear();
	m_stdstrAudioCompressiontype.clear();
}

string CKRestExUpdateAudioInfoData::GetExTransactionName()
{
	string stdstrRet;

	stdstrRet = EX_TRANSACTION_NAME_UPDATE_JOBRESULT;

	return stdstrRet;
}


string CKRestExUpdateAudioInfoData::GetExTransactionData()
{
	// {\
	//		\"type\":\"audio\",\
	//		\"audioinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"audiofileformat\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiosamplerate\":\"%d\",\
	//			\"audiobitpersample\":\"%d\",\
	//			\"audiochannel\":\"%d\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"startframeindex\":\"%d\",\
	//			\"streamduration\":\"%d\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"streamdurationtimecode\":\"%s\",\
	//			\"som\":\"%s\"\
	//			\"eom\":\"%s\",\
	//			\"filesize\":\"%I64d\"\
	//		}\
	//	}
	// or
	// {\
	//		\"type\":\"proxyaudio\",\
	//		\"audioinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"mobtype\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"audiofileformat\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiosamplerate\":\"%d\",\
	//			\"audiobitpersample\":\"%d\",\
	//			\"audiochannel\":\"%d\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"startframeindex\":\"%d\",\
	//			\"streamduration\":\"%d\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"streamdurationtimecode\":\"%s\",\
	//			\"som\":\"%s\"\
	//			\"eom\":\"%s\",\
	//			\"filesize\":\"%I64d\"\
	//		}\
	//	}

	string stdstrRet;

	//jsonQCInfo[L"assetid"] = json::value::number(m_nAssetid);
	//jsonQCInfo[L"qualitychecktype"] = json::value::number(m_nQCType);
	//jsonQCInfo[L"starttimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrStartTimecode));
	//jsonQCInfo[L"endtimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrEndTimecode));
	//jsonQCInfo[L"startframeindex"] = json::value::number(m_nStartFrameIndex);
	//jsonQCInfo[L"endframeindex"] = json::value::number(m_nEndFrameIndex);

	json::value jsonUpdateData;
	json::value jsonAudioInfo;

	jsonUpdateData[L"type"] = json::value::string(utility::conversions::utf8_to_utf16("audio"));
	if (m_eMediaType == eProxyAudio)
	{
		jsonUpdateData[L"type"] = json::value::string(utility::conversions::utf8_to_utf16("proxyaudio"));
		jsonAudioInfo[L"mobtype"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrMobType));
	}

	jsonAudioInfo[L"assetid"] = json::value::number(m_nAssetid);
	jsonAudioInfo[L"audiofileformat"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrAudioFileFormat));
	jsonAudioInfo[L"audiocompressiontype"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrAudioCompressiontype));
	jsonAudioInfo[L"audiosamplerate"] = json::value::number(m_nAudioSampleRate);
	jsonAudioInfo[L"audiobitpersample"] = json::value::number(m_nAudioBitPerSample);
	jsonAudioInfo[L"audiochannel"] = json::value::number(m_nAudioChannel);
	jsonAudioInfo[L"audiobitrate"] = json::value::number(m_nAudioBitrate);
	//jsonAudioInfo[L"startframeindex"] = json::value::number(m_nStartFrameIndex);
	//jsonAudioInfo[L"streamduration"] = json::value::number(m_nStreamDurationFrameCount);
	//jsonAudioInfo[L"starttimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrStartTimecode));
	//jsonAudioInfo[L"endtimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrEndTimecode));
	//jsonAudioInfo[L"streamdurationtimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrStreamDurationTimecode));
	//jsonAudioInfo[L"som"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrSOMTimecode));
	//jsonAudioInfo[L"eom"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrEOMTimecode));
	jsonAudioInfo[L"filesize"] = json::value::number(m_nFileSize);

	jsonUpdateData[L"audioinfo"] = jsonAudioInfo;

	stdstrRet = utility::conversions::utf16_to_utf8(jsonUpdateData.serialize());

	return stdstrRet;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestExUpdateVideoInfoData::CKRestExUpdateVideoInfoData()
{
	m_eMediaType = eVideo;

	m_nVideoBitrate = 0;
	m_nFileSize = 0;

	m_dbFramerate = 0.0;

	m_nWidth = 0;
	m_nHeight = 0;
	m_nStartFrameIndex = 0;
	m_nStreamDurationFrameCount = 0;

	m_nAudioSampleRate = 0;
	m_nAudioBitPerSample = 0;
	m_nAudioChannel = 0;
	m_nAudioBitrate = 0;
}

CKRestExUpdateVideoInfoData::CKRestExUpdateVideoInfoData(const CKRestExUpdateVideoInfoData& operand)
{
	CKRestExData::operator=(operand);

	m_nVideoBitrate = operand.m_nVideoBitrate;
	m_nFileSize = operand.m_nFileSize;

	m_dbFramerate = operand.m_dbFramerate;

	m_nWidth = operand.m_nWidth;
	m_nHeight = operand.m_nHeight;
	m_nStartFrameIndex = operand.m_nStartFrameIndex;
	m_nStreamDurationFrameCount = operand.m_nStreamDurationFrameCount;

	m_nAudioSampleRate = operand.m_nAudioSampleRate;
	m_nAudioBitPerSample = operand.m_nAudioBitPerSample;
	m_nAudioChannel = operand.m_nAudioChannel;
	m_nAudioBitrate = operand.m_nAudioBitrate;

	m_stdstrMobType = operand.m_stdstrMobType;
	m_stdstrVideoFileFormat = operand.m_stdstrVideoFileFormat;
	m_stdstrVideoCompressiontype = operand.m_stdstrVideoCompressiontype;
	m_stdstrAspectratio = operand.m_stdstrAspectratio;
	m_stdstrChromaSubSampling = operand.m_stdstrChromaSubSampling;
	m_stdstrStartTimecode = operand.m_stdstrStartTimecode;
	m_stdstrEndTimecode = operand.m_stdstrEndTimecode;
	m_stdstrStreamDurationTimecode = operand.m_stdstrStreamDurationTimecode;
	m_stdstrSOMTimecode = operand.m_stdstrSOMTimecode;
	m_stdstrEOMTimecode = operand.m_stdstrEOMTimecode;

	m_stdstrAudioCompressiontype = operand.m_stdstrAudioCompressiontype;
}

CKRestExUpdateVideoInfoData CKRestExUpdateVideoInfoData::operator=(const CKRestExUpdateVideoInfoData& operand)
{
	CKRestExData::operator=(operand);

	m_nVideoBitrate = operand.m_nVideoBitrate;
	m_nFileSize = operand.m_nFileSize;

	m_dbFramerate = operand.m_dbFramerate;

	m_nWidth = operand.m_nWidth;
	m_nHeight = operand.m_nHeight;
	m_nStartFrameIndex = operand.m_nStartFrameIndex;
	m_nStreamDurationFrameCount = operand.m_nStreamDurationFrameCount;

	m_nAudioSampleRate = operand.m_nAudioSampleRate;
	m_nAudioBitPerSample = operand.m_nAudioBitPerSample;
	m_nAudioChannel = operand.m_nAudioChannel;
	m_nAudioBitrate = operand.m_nAudioBitrate;

	m_stdstrMobType = operand.m_stdstrMobType;
	m_stdstrVideoFileFormat = operand.m_stdstrVideoFileFormat;
	m_stdstrVideoCompressiontype = operand.m_stdstrVideoCompressiontype;
	m_stdstrAspectratio = operand.m_stdstrAspectratio;
	m_stdstrChromaSubSampling = operand.m_stdstrChromaSubSampling;
	m_stdstrStartTimecode = operand.m_stdstrStartTimecode;
	m_stdstrEndTimecode = operand.m_stdstrEndTimecode;
	m_stdstrStreamDurationTimecode = operand.m_stdstrStreamDurationTimecode;
	m_stdstrSOMTimecode = operand.m_stdstrSOMTimecode;
	m_stdstrEOMTimecode = operand.m_stdstrEOMTimecode;

	m_stdstrAudioCompressiontype = operand.m_stdstrAudioCompressiontype;

	return *this;
}

CKRestExUpdateVideoInfoData::~CKRestExUpdateVideoInfoData()
{

}

void CKRestExUpdateVideoInfoData::Clear()
{
	CKRestExData::Clear();

	m_eMediaType = eVideo;

	m_nVideoBitrate = 0;
	m_nFileSize = 0;

	m_dbFramerate = 0.0;

	m_nWidth = 0;
	m_nHeight = 0;
	m_nStartFrameIndex = 0;
	m_nStreamDurationFrameCount = 0;

	m_nAudioSampleRate = 0;
	m_nAudioBitPerSample = 0;
	m_nAudioChannel = 0;
	m_nAudioBitrate = 0;

	m_stdstrMobType.clear();
	m_stdstrVideoFileFormat.clear();
	m_stdstrVideoCompressiontype.clear();
	m_stdstrAspectratio.clear();
	m_stdstrChromaSubSampling.clear();
	m_stdstrStartTimecode.clear();
	m_stdstrEndTimecode.clear();
	m_stdstrStreamDurationTimecode.clear();
	m_stdstrSOMTimecode.clear();
	m_stdstrEOMTimecode.clear();

	m_stdstrAudioCompressiontype.clear();
}

string CKRestExUpdateVideoInfoData::GetExTransactionName()
{
	string stdstrRet;

	stdstrRet = EX_TRANSACTION_NAME_UPDATE_JOBRESULT;

	return stdstrRet;
}

string CKRestExUpdateVideoInfoData::GetExTransactionData()
{
	//	{\
	//		\"type\":\"video\",\
	//		\"videoinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"videocompressiontype\":\"%s\",\
	//			\"aspectratio\":\"%s\",\
	//			\"videobitrate\":\"%I64d\",\
	//			\"videoframrate\":\"%f\",\
	//			\"width\":\"%d\",\
	//			\"height\":\"%d\",\
	//			\"videowrappertype\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiosamplerate\":\"%d\",\
	//			\"audiobitpersample\":\"%d\",\
	//			\"audiochannel\":\"%d\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"chromsubsampling\":\"%s\",\
	//			\"startframeindex\":\"%d\",\
	//			\"streamduration\":\"%d\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"streamdurationtimecode\":\"%s\",\
	//			\"som\":\"%s\"\
	//			\"eom\":\"%s\",\
	//			\"filesize\":\"%I64d\"\
	//		}\
	//	}
	// or
	// {\
	//		\"type\":\"proxyvideo\",\
	//		\"videoinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"mobtype\":\"%s\",\
	//			\"videocompressiontype\":\"%s\",\
	//			\"aspectratio\":\"%s\",\
	//			\"videobitrate\":\"%I64d\",\
	//			\"videoframrate\":\"%f\",\
	//			\"width\":\"%d\",\
	//			\"height\":\"%d\",\
	//			\"videowrappertype\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiosamplerate\":\"%d\",\
	//			\"audiobitpersample\":\"%d\",\
	//			\"audiochannel\":\"%d\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"chromsubsampling\":\"%s\",\
	//			\"startframeindex\":\"%d\",\
	//			\"streamduration\":\"%d\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"streamdurationtimecode\":\"%s\",\
	//			\"som\":\"%s\"\
	//			\"eom\":\"%s\",\
	//			\"filesize\":\"%I64d\"\	
	//		}\
	//	}

	string stdstrRet;

	json::value jsonUpdateData;
	json::value jsonVideoInfo;

	jsonUpdateData[L"type"] = json::value::string(utility::conversions::utf8_to_utf16("video"));
	if (m_eMediaType == eProxyVideo)
	{
		jsonUpdateData[L"type"] = json::value::string(utility::conversions::utf8_to_utf16("proxyvideo"));
		jsonVideoInfo[L"mobtype"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrMobType));
	}

	jsonVideoInfo[L"assetid"] = json::value::number(m_nAssetid);
	jsonVideoInfo[L"videocompressiontype"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrStartTimecode));
	jsonVideoInfo[L"aspectratio"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrAspectratio));
	jsonVideoInfo[L"videobitrate"] = json::value::number(m_nVideoBitrate);
	jsonVideoInfo[L"videoframrate"] = json::value::value(m_dbFramerate);
	jsonVideoInfo[L"width"] = json::value::number(m_nWidth);
	jsonVideoInfo[L"height"] = json::value::number(m_nHeight);
	jsonVideoInfo[L"videowrappertype"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrVideoFileFormat));
	jsonVideoInfo[L"audiocompressiontype"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrAudioCompressiontype));
	jsonVideoInfo[L"audiosamplerate"] = json::value::number(m_nAudioSampleRate);
	jsonVideoInfo[L"audiobitpersample"] = json::value::number(m_nAudioBitPerSample);
	jsonVideoInfo[L"audiochannel"] = json::value::number(m_nAudioChannel);
	jsonVideoInfo[L"audiobitrate"] = json::value::number(m_nAudioBitrate);
	jsonVideoInfo[L"chromsubsampling"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrChromaSubSampling));
	jsonVideoInfo[L"startframeindex"] = json::value::number(m_nStartFrameIndex);
	jsonVideoInfo[L"streamduration"] = json::value::number(m_nStreamDurationFrameCount);
	jsonVideoInfo[L"starttimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrStartTimecode));
	jsonVideoInfo[L"endtimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrEndTimecode));
	jsonVideoInfo[L"streamdurationtimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrStreamDurationTimecode));
	jsonVideoInfo[L"som"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrSOMTimecode));
	jsonVideoInfo[L"eom"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrEOMTimecode));
	jsonVideoInfo[L"filesize"] = json::value::number(m_nFileSize);

	jsonUpdateData[L"videoinfo"] = jsonVideoInfo;

	stdstrRet = utility::conversions::utf16_to_utf8(jsonUpdateData.serialize());

	return stdstrRet;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestExUpdateTranscodingData::CKRestExUpdateTranscodingData()
{

}

CKRestExUpdateTranscodingData::CKRestExUpdateTranscodingData(const CKRestExUpdateTranscodingData& operand)
{
	CKRestExData::operator=(operand);

	m_vecVideoInfoData = operand.m_vecVideoInfoData;
	m_vecAudioInfoData = operand.m_vecAudioInfoData;
}

CKRestExUpdateTranscodingData CKRestExUpdateTranscodingData::operator=(const CKRestExUpdateTranscodingData& operand)
{
	CKRestExData::operator=(operand);

	m_vecVideoInfoData = operand.m_vecVideoInfoData;
	m_vecAudioInfoData = operand.m_vecAudioInfoData;

	return *this;
}

CKRestExUpdateTranscodingData::~CKRestExUpdateTranscodingData()
{

}

void CKRestExUpdateTranscodingData::Clear()
{
	CKRestExData::Clear();

	m_vecVideoInfoData.clear();
	m_vecAudioInfoData.clear();
}

string CKRestExUpdateTranscodingData::GetExTransactionName()
{
	string stdstrRet;

	stdstrRet = EX_TRANSACTION_NAME_UPDATE_JOBRESULT;

	return stdstrRet;
}

string CKRestExUpdateTranscodingData::GetExTransactionData()
{
	//"{\
	//	\"assetinfo\":[
	//	{\
	//		\"type\":\"video\",\
	//		\"videoinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"videocompressiontype\":\"%s\",\
	//			\"aspectratio\":\"%s\",\
	//			\"videobitrate\":\"%I64d\",\
	//			\"videoframrate\":\"%f\",\
	//			\"width\":\"%d\",\
	//			\"height\":\"%d\",\
	//			\"videowrappertype\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiosamplerate\":\"%d\",\
	//			\"audiobitpersample\":\"%d\",\
	//			\"audiochannel\":\"%d\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"chromsubsampling\":\"%s\",\
	//			\"startframeindex\":\"%d\",\
	//			\"streamduration\":\"%d\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"streamdurationtimecode\":\"%s\",\
	//			\"som\":\"%s\"\
	//			\"eom\":\"%s\",\
	//			\"filesize\":\"%I64d\"\
	//		}\
	//	},\
	// {\
	//		\"type\":\"proxyvideo\",\
	//		\"videoinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"mobtype\":\"%s\",\
	//			\"videocompressiontype\":\"%s\",\
	//			\"aspectratio\":\"%s\",\
	//			\"videobitrate\":\"%I64d\",\
	//			\"videoframrate\":\"%f\",\
	//			\"width\":\"%d\",\
	//			\"height\":\"%d\",\
	//			\"videowrappertype\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiosamplerate\":\"%d\",\
	//			\"audiobitpersample\":\"%d\",\
	//			\"audiochannel\":\"%d\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"chromsubsampling\":\"%s\",\
	//			\"startframeindex\":\"%d\",\
	//			\"streamduration\":\"%d\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"streamdurationtimecode\":\"%s\",\
	//			\"som\":\"%s\"\
	//			\"eom\":\"%s\",\
	//			\"filesize\":\"%I64d\"\	
	//		}\
	//	},\
	// {\
	//		\"type\":\"proxyaudio\",\
	//		\"audioinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"mobtype\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"audiofileformat\":\"%s\",\
	//			\"audiocompressiontype\":\"%s\",\
	//			\"audiosamplerate\":\"%d\",\
	//			\"audiobitpersample\":\"%d\",\
	//			\"audiochannel\":\"%d\",\
	//			\"audiobitrate\":\"%d\",\
	//			\"startframeindex\":\"%d\",\
	//			\"streamduration\":\"%d\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"streamdurationtimecode\":\"%s\",\
	//			\"som\":\"%s\"\
	//			\"eom\":\"%s\",\
	//			\"filesize\":\"%I64d\"\
	//		}\
	//	}\
	//	]\
	//}"

	string stdstrRet;

	json::value jsonUpdateData;
	json::value jsonAssetInfo;
	json::value jsonVideoInfo;
	json::value jsonAudioInfo;

	CKRestExUpdateVideoInfoData VideoData;
	CKRestExUpdateAudioInfoData AudioData;

	vector<json::value> vecData;

	int nCount = 0;

	if (!m_vecVideoInfoData.empty())
	{
		nCount = m_vecVideoInfoData.size();
		for (int i = 0; i < nCount; i++)
		{
			VideoData = m_vecVideoInfoData[i];

			stdstrRet = VideoData.GetExTransactionData();
			jsonVideoInfo = json::value::string(utility::conversions::utf8_to_utf16(stdstrRet));

			vecData.push_back(jsonVideoInfo);
		}
	}

	if (!m_vecAudioInfoData.empty())
	{
		nCount = m_vecAudioInfoData.size();
		for (int i = 0; i < nCount; i++)
		{
			AudioData = m_vecAudioInfoData[i];

			stdstrRet = AudioData.GetExTransactionData();
			jsonAudioInfo = json::value::string(utility::conversions::utf8_to_utf16(stdstrRet));

			vecData.push_back(jsonAudioInfo);
		}
	}

	stdstrRet.clear();

	if (!vecData.empty())
	{
		jsonAssetInfo = json::value::array(vecData);
		jsonUpdateData[L"assetinfo"] = jsonAssetInfo;

		stdstrRet = utility::conversions::utf16_to_utf8(jsonUpdateData.serialize());
	}

	return stdstrRet;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestExUpdateQCData::CKRestExUpdateQCData()
{
	m_eMediaType = eQualityCheck;

	m_nQCType = 1;
	m_nStartFrameIndex = -1;
	m_nEndFrameIndex = -1;
}

CKRestExUpdateQCData::CKRestExUpdateQCData(const CKRestExUpdateQCData& operand)
{
	CKRestExData::operator=(operand);

	m_stdstrStartTimecode = operand.m_stdstrStartTimecode;
	m_stdstrEndTimecode = operand.m_stdstrEndTimecode;

	m_nQCType = operand.m_nQCType;
	m_nStartFrameIndex = operand.m_nStartFrameIndex;
	m_nEndFrameIndex = operand.m_nEndFrameIndex;
}

CKRestExUpdateQCData CKRestExUpdateQCData::operator=(const CKRestExUpdateQCData& operand)
{
	CKRestExData::operator=(operand);

	m_stdstrStartTimecode = operand.m_stdstrStartTimecode;
	m_stdstrEndTimecode = operand.m_stdstrEndTimecode;

	m_nQCType = operand.m_nQCType;
	m_nStartFrameIndex = operand.m_nStartFrameIndex;
	m_nEndFrameIndex = operand.m_nEndFrameIndex;

	return *this;
}

CKRestExUpdateQCData::~CKRestExUpdateQCData()
{

}

void CKRestExUpdateQCData::Clear()
{
	CKRestExData::Clear();

	m_eMediaType = eQualityCheck;

	m_stdstrStartTimecode = "";
	m_stdstrEndTimecode = "";

	m_nQCType = 1;
	m_nStartFrameIndex = -1;
	m_nEndFrameIndex = -1;
}

string CKRestExUpdateQCData::GetExTransactionName()
{
	string stdstrRet;

	stdstrRet = EX_TRANSACTION_NAME_UPDATE_JOBRESULT;

	return stdstrRet;
}


string CKRestExUpdateQCData::GetExTransactionData()
{
	//"{\
	//	\"assetinfo\":{\
	//		\"type\":\"qualitycheck\",
	//		\"qualitycheckinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"qualitychecktype\":\"%d\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"startframeindex\":\"%d\",\
	//			\"endframeindex\":\"%d\"\
	//		}\
	//	}\
	//}"

	string stdstrRet;

	json::value jsonUpdateData;
	json::value jsonAssetInfo;
	json::value jsonQCInfo;

	jsonQCInfo[L"assetid"] = json::value::number(m_nAssetid);
	jsonQCInfo[L"qualitychecktype"] = json::value::number(m_nQCType);
	jsonQCInfo[L"starttimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrStartTimecode));
	jsonQCInfo[L"endtimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrEndTimecode));
	jsonQCInfo[L"startframeindex"] = json::value::number(m_nStartFrameIndex);
	jsonQCInfo[L"endframeindex"] = json::value::number(m_nEndFrameIndex);

	jsonAssetInfo[L"type"] = json::value::string(utility::conversions::utf8_to_utf16("qualitycheck"));
	jsonAssetInfo[L"qualitycheck"] = jsonQCInfo;
	jsonUpdateData[L"assetinfo"] = jsonAssetInfo;

	stdstrRet = utility::conversions::utf16_to_utf8(jsonUpdateData.serialize());

	return stdstrRet;
}

///////////////////////////////////////////////////////////////////////////////////////

CKRestExUpdateShotData::CKRestExUpdateShotData()
{
	m_eMediaType = eShot;

	m_nImageWidth = 0;
	m_nImageHeight = 0;

	m_nKeyPosFrameIndex = -1;
	m_bWithImageData = false;
	m_dFrameRate = -0.0;
}

CKRestExUpdateShotData::CKRestExUpdateShotData(const CKRestExUpdateShotData& operand)
{
	CKRestExUpdateQCData::operator=(operand);

	m_FileInfo = operand.m_FileInfo;
	m_vecFaceRect = operand.m_vecFaceRect;
	m_vecDescriptionRectPair = operand.m_vecDescriptionRectPair;
	m_vecAccuracy = operand.m_vecAccuracy;

	m_stdstrSequenceType = operand.m_stdstrSequenceType;
	m_stdstrKeyPosTimecode = operand.m_stdstrKeyPosTimecode;
	m_stdstrTotalDuration = operand.m_stdstrTotalDuration;
	m_dFrameRate = operand.m_dFrameRate;

	m_nImageWidth = operand.m_nImageWidth;
	m_nImageHeight = operand.m_nImageHeight;

	m_nKeyPosFrameIndex = operand.m_nKeyPosFrameIndex;

	m_bWithImageData = operand.m_bWithImageData;
}

CKRestExUpdateShotData CKRestExUpdateShotData::operator=(const CKRestExUpdateShotData& operand)
{
	CKRestExUpdateQCData::operator=(operand);

	m_FileInfo = operand.m_FileInfo;
	m_vecFaceRect = operand.m_vecFaceRect;
	m_vecDescriptionRectPair = operand.m_vecDescriptionRectPair;
	m_vecAccuracy = operand.m_vecAccuracy;

	m_stdstrSequenceType = operand.m_stdstrSequenceType;
	m_stdstrKeyPosTimecode = operand.m_stdstrKeyPosTimecode;
	m_stdstrTotalDuration = operand.m_stdstrTotalDuration;
	m_dFrameRate = operand.m_dFrameRate;

	m_nImageWidth = operand.m_nImageWidth;
	m_nImageHeight = operand.m_nImageHeight;

	m_nKeyPosFrameIndex = operand.m_nKeyPosFrameIndex;

	m_bWithImageData = operand.m_bWithImageData;

	return *this;
}

CKRestExUpdateShotData::~CKRestExUpdateShotData()
{

}

void CKRestExUpdateShotData::Clear()
{
	CKRestExUpdateQCData::Clear();
	
	m_eMediaType = eShot;
	
	m_FileInfo.Clear();
	m_vecFaceRect.clear();
	m_vecDescriptionRectPair.clear();
	m_vecAccuracy.clear();

	m_stdstrSequenceType = "";
	m_stdstrKeyPosTimecode = "";

	m_nKeyPosFrameIndex = -1;

	m_nImageWidth = 0;
	m_nImageHeight = 0;

	m_bWithImageData = false;
	m_dFrameRate = -0.0;
}

string CKRestExUpdateShotData::GetExTransactionName()
{
	string stdstrRet;

	stdstrRet = EX_TRANSACTION_NAME_UPDATE_JOBRESULT;

	return stdstrRet;
}

string CKRestExUpdateShotData::GetExTransactionData()
{
	//"{\
	//	\"assetinfo\":{\
	//		\"type\":\"shot\",
	//		\"shotinfo\":{\
	//			\"assetid\":%I64d,\
	//			\"sequencetype\":\"%s\",\
	//			\"starttimecode\":\"%s\",\
	//			\"endtimecode\":\"%s\",\
	//			\"keypositiontimecode\":\"%s\",\
	//			\"startframeindex\":\"%d\",\
	//			\"endframeindex\":\"%d\",\
	//			\"keypositionframeindex\":\"%d\",\
	//			\"facerect\":[\"%s\", \"%s\", \"%s\"],\
	//			\"storage\":\"%s\",\
	//			\"filepath\":\"%s\",\
	// image datat add or not ///////////////////////////////
	//			\"imagedata\":\"%s\"\
	//		}\
	//	}\
	//}"

	string stdstrRet;

	json::value jsonUpdateData;
	json::value jsonAssetInfo;
	json::value jsonShotInfo;
	json::value jsonMediaInfo;

	jsonShotInfo[L"assetid"] = json::value::number(m_nAssetid);
	jsonShotInfo[L"sequencetype"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrSequenceType));
	jsonShotInfo[L"starttimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrStartTimecode));
	jsonShotInfo[L"endtimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrEndTimecode));
	jsonShotInfo[L"keypositiontimecode"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrKeyPosTimecode));
	jsonShotInfo[L"startframeindex"] = json::value::number(m_nStartFrameIndex);
	jsonShotInfo[L"endframeindex"] = json::value::number(m_nEndFrameIndex);
	jsonShotInfo[L"keypositionframeindex"] = json::value::number(m_nKeyPosFrameIndex);
	jsonShotInfo[L"storage"] = json::value::string(utility::conversions::utf8_to_utf16(m_FileInfo.m_stdstrStorage));
	jsonShotInfo[L"filepath"] = json::value::string(utility::conversions::utf8_to_utf16(m_FileInfo.m_stdstrPath));
	jsonShotInfo[L"width"] = json::value::value(m_nImageWidth);
	jsonShotInfo[L"height"] = json::value::value(m_nImageHeight);

	if (m_bWithImageData)
	{
		char* pContent = NULL;
		pContent = GetBase64EncodedImageData();

		if (!pContent)
		{
			stdstrRet.clear();
			return stdstrRet;
		}

		stdstrRet = Utility::string_format("%s", pContent);
		jsonShotInfo[L"imagedata"] = json::value::string(utility::conversions::utf8_to_utf16(pContent));

		if (pContent)
		{
			delete[]pContent;
			pContent = NULL;
		}
	}

	int nSize = 0;
	if ((nSize = m_vecFaceRect.size()) > 0)
	{
		json::value jsonFaceRect;
		vector<json::value> vecFaceRectValue;

		for (int i = 0; i < nSize; i++)
		{
			jsonFaceRect = json::value::string(utility::conversions::utf8_to_utf16(m_vecFaceRect[i]));
			vecFaceRectValue.push_back(jsonFaceRect);
		}

		json::value jsonFaceRects = json::value::array(vecFaceRectValue);
		jsonShotInfo[L"facerect"] = jsonFaceRects;
	}

	if ((nSize = m_vecDescriptionRectPair.size()) > 0)
	{
		int nAccuracySize = m_vecAccuracy.size();

		json::value jsonObject;
		vector<json::value> vecObjectValue;

		for (int i = 0; i < nSize; i++)
		{
			pair<string, string> pairDescriptionRect = m_vecDescriptionRectPair[i];
			jsonObject[L"label"] = json::value::string(utility::conversions::utf8_to_utf16(pairDescriptionRect.first));

			if (!pairDescriptionRect.second.empty())
				jsonObject[L"rect"] = json::value::string(utility::conversions::utf8_to_utf16(pairDescriptionRect.second));

			if (nSize == nAccuracySize)
				jsonObject[L"accuracy"] = json::value::value(m_vecAccuracy[i]);

			vecObjectValue.push_back(jsonObject);
		}

		json::value jsonObjects = json::value::array(vecObjectValue);
		jsonShotInfo[L"object"] = jsonObjects;
	}

	if (!m_stdstrTaggingData.empty())
	{
		bool bJsonParced = false;

		json::value jsonObjects;

		try
		{
			jsonObjects = json::value::parse(utility::conversions::utf8_to_utf16(m_stdstrTaggingData));

			bJsonParced = true;
		}
		catch (web::json::json_exception &e)
		{
			printf("json parse(%s) - %s\n", m_stdstrTaggingData.c_str(), e.what());
		}

		if(bJsonParced)
			jsonShotInfo[L"object"] = jsonObjects;
	}

	if (m_dFrameRate > 0)
		jsonMediaInfo[L"framerate"] = json::value::value(m_dFrameRate);

	if (!m_stdstrTotalDuration.empty())
		jsonMediaInfo[L"duration"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrTotalDuration));

	jsonAssetInfo[L"type"] = json::value::string(utility::conversions::utf8_to_utf16("shot"));
	jsonAssetInfo[L"shotinfo"] = jsonShotInfo;

	if (!jsonMediaInfo.is_null())
		jsonAssetInfo[L"mediainfo"] = jsonMediaInfo;

	jsonUpdateData[L"assetinfo"] = jsonAssetInfo;

	stdstrRet = utility::conversions::utf16_to_utf8(jsonUpdateData.serialize());

	return stdstrRet;
}

char* CKRestExUpdateShotData::GetBase64EncodedImageData()
{
	char* pRetBuff = NULL;
	char* pImageBuff = NULL;

	int hReadFile = -1;

	bool bError = false;

	CString strFilePath;
	Utility::CConvertUTF8Operation::ConvertUTF8ToString((char*)m_FileInfo.m_stdstrPath.c_str(), strFilePath);

	struct _stati64 stat;

	do
	{
		int nRet = _tstati64(strFilePath, &stat);
		if (nRet == -1)
		{
			m_nError = ::GetLastError();
			m_stdstrErrMsg = Utility::string_format("_tstati64 (%s)", m_FileInfo.m_stdstrPath.c_str());

			bError = true;

			break;
		}

		//if (stat.st_size == 0)
		//{
		//	m_nError = RESTGOOGLECLOUDSERVICE_ERROR_SIZE;
		//	m_stdstrErrMsg = Utility::string_format("_tstati64 (%s)", m_FileInfo.m_stdstrPath.c_str());

		//	bError = true;

		//	break;
		//}

		pImageBuff = new char[(int)stat.st_size];

		hReadFile = _topen(strFilePath, _O_BINARY, _S_IREAD);
		if (hReadFile == -1)
		{
			m_nError = ::GetLastError();
			m_stdstrErrMsg = Utility::string_format("_open (%s)", m_FileInfo.m_stdstrPath.c_str());

			bError = true;

			break;
		}

		int nRead = _read(hReadFile, pImageBuff, (int)stat.st_size);
		if (nRead <= 0)
		{
			m_nError = ::GetLastError();
			m_stdstrErrMsg = Utility::string_format("_read (%s)", m_FileInfo.m_stdstrPath.c_str());

			bError = true;

			break;
		}
	} while (0);

	if (hReadFile > 0)
	{
		_close(hReadFile);
		hReadFile = -1;
	}

	if (!bError)
	{
		int nResultLength = ((int)stat.st_size + 2 - (((int)stat.st_size + 2) % 3)) / 3 * 4;
		pRetBuff = new char[nResultLength + 10];
		memset(pRetBuff, 0, nResultLength);
		Utility::encode_base64((unsigned char*)pRetBuff, (unsigned char*)pImageBuff, (int)stat.st_size);
	}
	else
		printf("%s\n", m_stdstrErrMsg.c_str());

	if (pImageBuff)
	{
		delete[]pImageBuff;
		pImageBuff = NULL;
	}

	return pRetBuff;
}

///////////////////////////////////////////////////////////////////////////////////////
