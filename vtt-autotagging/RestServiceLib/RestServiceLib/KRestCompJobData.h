// KRestCompJobData.h : KRestCompJobData 헤더 파일입니다.
//

#pragma once

#include <vector>
#include <map>

#ifdef RESTCOMPJOBMANAGER_EXPORTS
#define RESTCOMPJOBMANAGER_API __declspec(dllexport)
#else
#define RESTCOMPJOBMANAGER_API __declspec(dllimport)
#endif

using namespace std;

#define FILE_EXIST_THEN_FAIL                   (0x00)
#define FILE_EXIST_THEN_SUCCESS                (0x01)
#define FILE_EXIST_THEN_OVERWRITE              (0x02)

enum eStorageType
{
	eStorageTypeDisk = 0,
	eStorageTypeFTP,
	eStorageTypeDTL
};

enum eMediaType
{
	eShot = 0,
	eQualityCheck,
	eVideo,
	eProxyVideo,
	eAudio,
	eProxyAudio,
	eImage,
	eThumbnailImage,
	eDocument
};

enum eJobType
{
	eArchive = 0,
	eRestore,
	ePartialRestore,
	eDelete,
	eTranscode,
	eCatalog,
	eTransfer,
	eImageConvert,
	eVolumeInfo
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestCompJobFileInfo
{
public:
	CKRestCompJobFileInfo();
	CKRestCompJobFileInfo(const CKRestCompJobFileInfo&);
	CKRestCompJobFileInfo operator=(const CKRestCompJobFileInfo &);

	virtual ~CKRestCompJobFileInfo();

	virtual void Clear();

public:
	eStorageType m_eStorageType;

	string m_stdstrStorage;
	string m_stdstrPath;
	//string m_stdstrRename;
	//string m_stdstrDisplayPath;

	string m_stdstrHostAddress;
	string m_stdstrUser;
	string m_stdstrPassword;

	//string m_stdstrServerType;

	int m_nPort;
	//int m_nAllowExistFile;

	//bool m_bFolder;
	//bool m_bDelete;
	//bool m_bZip;

	//vector<string> m_vecExceptFilePath;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestCompJobAssetData
{
public:
	CKRestCompJobAssetData();
	CKRestCompJobAssetData(const CKRestCompJobAssetData&);
	CKRestCompJobAssetData operator=(const CKRestCompJobAssetData &);

	virtual ~CKRestCompJobAssetData();

	virtual void Clear();

public:
	CKRestCompJobFileInfo m_FileInfo;

	//string m_stdstrVideoWrapperType;
	//string m_stdstrMobType;

	string m_stdstrObjectID;

	long long m_nAssetID;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestCAJobSrcData : public CKRestCompJobAssetData
{
public:
	CKRestCAJobSrcData();
	CKRestCAJobSrcData(const CKRestCAJobSrcData&);
	CKRestCAJobSrcData operator=(const CKRestCAJobSrcData &);

	virtual ~CKRestCAJobSrcData();

	virtual void Clear();

public:
	double m_dbFramerate;
	string m_stdstrTotalDuration;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestTCJobSrcData : public CKRestCompJobAssetData
{
public:
	CKRestTCJobSrcData();
	CKRestTCJobSrcData(const CKRestTCJobSrcData&);
	CKRestTCJobSrcData operator=(const CKRestTCJobSrcData &);

	virtual ~CKRestTCJobSrcData();

	virtual void Clear();

public:
	eMediaType m_eMediaType;

	string m_stdstrVideoWrapperType;
};

class RESTCOMPJOBMANAGER_API CKRestTCJobDestData : public CKRestCompJobAssetData
{
public:
	CKRestTCJobDestData();
	CKRestTCJobDestData(const CKRestTCJobDestData&);
	CKRestTCJobDestData operator=(const CKRestTCJobDestData &);

	virtual ~CKRestTCJobDestData();

	virtual void Clear();

public:
	eMediaType m_eMediaType;

	string m_stdstrMobType;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestTMJobSrcData : public CKRestCompJobAssetData
{
public:
	CKRestTMJobSrcData();
	CKRestTMJobSrcData(const CKRestTMJobSrcData&);
	CKRestTMJobSrcData operator=(const CKRestTMJobSrcData &);

	virtual ~CKRestTMJobSrcData();

	virtual void Clear();

public:
	string m_stdstrRename;
	string m_stdstrServerType;

	bool m_bFolder;
	bool m_bDelete;
	bool m_bZip;

	vector<string> m_vecExceptFilePath;
};

class RESTCOMPJOBMANAGER_API CKRestTMJobDestData : public CKRestCompJobAssetData
{
public:
	CKRestTMJobDestData();
	CKRestTMJobDestData(const CKRestTMJobDestData&);
	CKRestTMJobDestData operator=(const CKRestTMJobDestData &);

	virtual ~CKRestTMJobDestData();

	virtual void Clear();

public:
	string m_stdstrDisplayPath;
	string m_stdstrServerType;

	int m_nAllowExistFile;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestArchiveJobSrcData : public CKRestCompJobAssetData
{
public:
	CKRestArchiveJobSrcData();
	CKRestArchiveJobSrcData(const CKRestArchiveJobSrcData&);
	CKRestArchiveJobSrcData operator=(const CKRestArchiveJobSrcData &);

	virtual ~CKRestArchiveJobSrcData();

	virtual void Clear();

public:
	bool m_bFolder;
	bool m_bDelete;
	bool m_bZip;
};

class RESTCOMPJOBMANAGER_API CKRestArchiveJobDestData : public CKRestCompJobAssetData
{
public:
	CKRestArchiveJobDestData();
	CKRestArchiveJobDestData(const CKRestArchiveJobDestData&);
	CKRestArchiveJobDestData operator=(const CKRestArchiveJobDestData &);

	virtual ~CKRestArchiveJobDestData();

	virtual void Clear();

public:
	string m_stdstrDTLGroup;
	string m_stdstrDTLCategory;

	int m_nAllowExistFile;
};

class RESTCOMPJOBMANAGER_API CKRestRestoreJobSrcData : public CKRestCompJobAssetData
{
public:
	CKRestRestoreJobSrcData();
	CKRestRestoreJobSrcData(const CKRestRestoreJobSrcData&);
	CKRestRestoreJobSrcData operator=(const CKRestRestoreJobSrcData &);

	virtual ~CKRestRestoreJobSrcData();

	virtual void Clear();

public:
	string m_stdstrDTLGroup;
	string m_stdstrDTLCategory;

	string m_stdstrCompressiontype;
	string m_stdstrWrappertype;
};

class RESTCOMPJOBMANAGER_API CKRestRestoreJobDestData : public CKRestCompJobAssetData
{
public:
	CKRestRestoreJobDestData();
	CKRestRestoreJobDestData(const CKRestRestoreJobDestData&);
	CKRestRestoreJobDestData operator=(const CKRestRestoreJobDestData &);

	virtual ~CKRestRestoreJobDestData();

	virtual void Clear();

public:
	int m_nStartFrameIndex;
	int m_nEndFrameIndex;
};

class RESTCOMPJOBMANAGER_API CKRestDeleteInstanceJobData : public CKRestArchiveJobSrcData
{
public:
	CKRestDeleteInstanceJobData();
	CKRestDeleteInstanceJobData(const CKRestDeleteInstanceJobData&);
	CKRestDeleteInstanceJobData operator=(const CKRestDeleteInstanceJobData &);

	virtual ~CKRestDeleteInstanceJobData();

	virtual void Clear();

public:
	string m_stdstrDTLGroup;
	string m_stdstrDTLCategory;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestCompJobData
{
public:
	CKRestCompJobData();
	CKRestCompJobData(const CKRestCompJobData&);
	CKRestCompJobData operator=(const CKRestCompJobData &);

	virtual ~CKRestCompJobData();

	virtual void Clear();
	
public:
	int m_nChannel;
	int m_nMamExPool;
	int m_nError;

	long long m_nCheckInID;
	long long m_nWorkflowID;
	long long m_nJobID;

	string m_stdstrWorkflowName;
	string m_stdstrJobName;
	string m_stdstrJobType;
	
	string m_stdstrMamExStart;
	string m_stdstrMamExProgress;
	string m_stdstrMamExEnd;

	string m_stdstrJobJson;
	string m_stdstrQuery;
	string m_stdstrErrMsg;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestCAJobData : public CKRestCompJobData
{
public:
	CKRestCAJobData();
	CKRestCAJobData(const CKRestCAJobData&);
	CKRestCAJobData operator=(const CKRestCAJobData &);

	virtual ~CKRestCAJobData();

	virtual void Clear();
public:

	CKRestCAJobSrcData m_SrcData;

	string m_stdstrShotImagePath;
	string m_stdstrShotImageStorage;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestTCJobData : public CKRestCompJobData
{
public:
	CKRestTCJobData();
	CKRestTCJobData(const CKRestTCJobData&);
	CKRestTCJobData operator=(const CKRestTCJobData &);

	virtual ~CKRestTCJobData();

	virtual void Clear();
public:

	CKRestTCJobSrcData m_SrcData;

	vector<CKRestTCJobDestData> m_vecDestData;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestTMJobData : public CKRestCompJobData
{
public:
	CKRestTMJobData();
	CKRestTMJobData(const CKRestTMJobData&);
	CKRestTMJobData operator=(const CKRestTMJobData &);

	virtual ~CKRestTMJobData();

	virtual void Clear();

public:

	vector<pair<CKRestTMJobSrcData, vector<CKRestTMJobDestData>>> m_vecSrcDest;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestAMJobData : public CKRestCompJobData
{
public:
	CKRestAMJobData();
	CKRestAMJobData(const CKRestAMJobData&);
	CKRestAMJobData operator=(const CKRestAMJobData &);

	virtual ~CKRestAMJobData();

	virtual void Clear();

public:
	eJobType m_eAMJobType;
	string m_stdstrDTLName;

	vector<pair<vector<CKRestArchiveJobSrcData>, CKRestArchiveJobDestData>> m_vecArchiveSrcDest;
	vector<pair<vector<CKRestRestoreJobSrcData>, CKRestRestoreJobDestData>> m_vecRestoreSrcDest;
	vector<CKRestDeleteInstanceJobData> m_vceDeleteData;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestExData
{
public:
	CKRestExData();
	CKRestExData(const CKRestExData&);
	CKRestExData operator=(const CKRestExData &);

	virtual ~CKRestExData();

	virtual void Clear();

	virtual string GetExTransactionName();
	virtual string GetExTransactionData();

	string GetErrorMessage();

public:
	eMediaType m_eMediaType;

	string m_stdstrWorkingInfoJson;

	long long m_nAssetid;

protected:
	string m_stdstrErrMsg;
	int m_nError;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestExUpdateAudioInfoData : public CKRestExData
{
public:
	CKRestExUpdateAudioInfoData();
	CKRestExUpdateAudioInfoData(const CKRestExUpdateAudioInfoData&);
	CKRestExUpdateAudioInfoData operator=(const CKRestExUpdateAudioInfoData &);

	virtual ~CKRestExUpdateAudioInfoData();

	virtual void Clear();

	virtual string GetExTransactionName();
	virtual string GetExTransactionData();

public:
	long long m_nFileSize;

	int m_nAudioSampleRate;
	int m_nAudioBitPerSample;
	int m_nAudioChannel;
	int m_nAudioBitrate;

	string m_stdstrMobType;
	string m_stdstrAudioFileFormat;
	string m_stdstrAudioCompressiontype;
};

class RESTCOMPJOBMANAGER_API CKRestExUpdateVideoInfoData : public CKRestExData
{
public:
	CKRestExUpdateVideoInfoData();
	CKRestExUpdateVideoInfoData(const CKRestExUpdateVideoInfoData&);
	CKRestExUpdateVideoInfoData operator=(const CKRestExUpdateVideoInfoData &);

	virtual ~CKRestExUpdateVideoInfoData();

	virtual void Clear();

	virtual string GetExTransactionName();
	virtual string GetExTransactionData();

public:
	long long m_nVideoBitrate;
	long long m_nFileSize;

	double m_dbFramerate;

	int m_nWidth;
	int m_nHeight;
	int m_nStartFrameIndex;
	int m_nStreamDurationFrameCount;

	int m_nAudioSampleRate;
	int m_nAudioBitPerSample;
	int m_nAudioChannel;
	int m_nAudioBitrate;

	string m_stdstrMobType;
	string m_stdstrVideoFileFormat;
	string m_stdstrVideoCompressiontype;
	string m_stdstrAspectratio;
	string m_stdstrChromaSubSampling;
	string m_stdstrStartTimecode;
	string m_stdstrEndTimecode;
	string m_stdstrStreamDurationTimecode;
	string m_stdstrSOMTimecode;
	string m_stdstrEOMTimecode;

	string m_stdstrAudioCompressiontype;
};

class RESTCOMPJOBMANAGER_API CKRestExUpdateTranscodingData : public CKRestExData
{
public:
	CKRestExUpdateTranscodingData();
	CKRestExUpdateTranscodingData(const CKRestExUpdateTranscodingData&);
	CKRestExUpdateTranscodingData operator=(const CKRestExUpdateTranscodingData &);

	virtual ~CKRestExUpdateTranscodingData();

	virtual void Clear();

	virtual string GetExTransactionName();
	virtual string GetExTransactionData();

public:
	vector<CKRestExUpdateVideoInfoData> m_vecVideoInfoData;
	vector<CKRestExUpdateAudioInfoData> m_vecAudioInfoData;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestExUpdateQCData : public CKRestExData
{
public:
	CKRestExUpdateQCData();
	CKRestExUpdateQCData(const CKRestExUpdateQCData&);
	CKRestExUpdateQCData operator=(const CKRestExUpdateQCData &);

	virtual ~CKRestExUpdateQCData();

	virtual void Clear();

	virtual string GetExTransactionName();
	virtual string GetExTransactionData();

public:
	string m_stdstrStartTimecode;
	string m_stdstrEndTimecode;

	int m_nQCType;
	int m_nStartFrameIndex;
	int m_nEndFrameIndex;
};

///////////////////////////////////////////////////////////////////////////////////////

class RESTCOMPJOBMANAGER_API CKRestExUpdateShotData : public CKRestExUpdateQCData
{
public:
	CKRestExUpdateShotData();
	CKRestExUpdateShotData(const CKRestExUpdateShotData&);
	CKRestExUpdateShotData operator=(const CKRestExUpdateShotData &);

	virtual ~CKRestExUpdateShotData();

	virtual void Clear();

	virtual string GetExTransactionName();
	virtual string GetExTransactionData();

public:
	CKRestCompJobFileInfo m_FileInfo;

	vector<string> m_vecFaceRect;
	vector<pair<string, string>> m_vecDescriptionRectPair;
	vector<double> m_vecAccuracy;

	string m_stdstrSequenceType;
	string m_stdstrKeyPosTimecode;
	string m_stdstrTotalDuration;
	string m_stdstrTaggingData;

	int m_nImageWidth;
	int m_nImageHeight;
	int m_nKeyPosFrameIndex;

	double m_dFrameRate;

	bool m_bWithImageData;

private:
	char* GetBase64EncodedImageData();
};

///////////////////////////////////////////////////////////////////////////////////////