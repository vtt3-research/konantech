// KRestSogangService.h : KRestSogangService 헤더 파일입니다.
//
#pragma once

#include <map>
#include <vector>
#include <queue>

using namespace std;

#ifdef RESTSKSERVICE_EXPORTS
#define RESTSKSERVICE_API __declspec(dllexport)
#else
#define RESTSKSERVICE_API __declspec(dllimport)
#endif

#define RESTSKSERVICE_ERROR_SUCCESS			 (0)
#define RESTSKSERVICE_ERROR_SIZE			 (400001)
#define RESTSKSERVICE_ERROR_INVALID_JSON	 (400002)
#define RESTSKSERVICE_ERROR_HTTP_STATUS		 (400003)
#define RESTSKSERVICE_ERROR_INVALID_JOB		 (400004)
#define RESTSKSERVICE_ERROR_FILE_IO			 (400005)
#define RESTSKSERVICE_ERROR_NOSUCHAJOB		 (400006)
#define RESTSKSERVICE_ERROR_ALREADYALLOCJOB	 (400007)
#define RESTSKSERVICE_ERROR_RESULTPROCESS	 (400008)
#define RESTSKSERVICE_ERROR_FORCED_ABORT	 (400009)

enum eSKAnnotationJobState
{
	eJobStateError = -1,
	eJobStateStartGalleryFeatureGen,
	eJobStateEndGalleryFeatureGen,
	eJobStateStartFrameFeatureGen,
	eJobStateEndFrameFeatureGen,
	eJobStateStartCharacterIdentify,
	eJobStateEndCharacterIdentify,
	eJobStateStartObjectEtcIdentify,
	eJobStateEndObjectEtcIdentify,
	eJobStateStartVideoIdentify,
	eJobStateEndVideoIdentify
};

enum eSKAnnotationResultProcState
{
	eAnnotationResultProcError = -1,
	eAnnotationResultProcStateStart,
	eAnnotationResultProcStateEnd
};

class CRestClient;

class RESTSKSERVICE_API CSKAnnotationJobInfo
{
public:
	CSKAnnotationJobInfo();
	virtual ~CSKAnnotationJobInfo();

	CSKAnnotationJobInfo(const CSKAnnotationJobInfo&);
	CSKAnnotationJobInfo operator=(const CSKAnnotationJobInfo &);

	virtual string GetJsonJobInfo();

	string GetAnnotationType() { return m_stdstrAnnotationType; };

public:
	__int64 m_nJobID;

	string* m_pstdstrJobInfo;

protected:
	string m_stdstrAnnotationType;
};

class RESTSKSERVICE_API CCharacterInfo
{
public:
	CCharacterInfo();
	virtual ~CCharacterInfo();

	CCharacterInfo(const CCharacterInfo&);
	CCharacterInfo operator=(const CCharacterInfo &);

public:
	__int64 m_llCharID;

	string m_stdstrCharName;
	string m_stdstrCharRole;
	string m_stdstrCharSrcImagesDirPath;
	string m_stdstrCharFeatureOutFilePath;
};

class RESTSKSERVICE_API CSceneInfo
{
public:
	CSceneInfo();
	virtual ~CSceneInfo();

	CSceneInfo(const CSceneInfo&);
	CSceneInfo operator=(const CSceneInfo &);

public:
	__int64 m_llSceneID;
	
	int m_nStartIndex;
	int m_nEndIndex;
};

class RESTSKSERVICE_API CGenerateCharacterFeatureJobInfo : public CSKAnnotationJobInfo
{
public:
	CGenerateCharacterFeatureJobInfo();
	virtual ~CGenerateCharacterFeatureJobInfo();

	CGenerateCharacterFeatureJobInfo(const CGenerateCharacterFeatureJobInfo&);
	CGenerateCharacterFeatureJobInfo operator=(const CGenerateCharacterFeatureJobInfo &);

	virtual string GetJsonJobInfo();

public:
	vector<CCharacterInfo> m_vecCharacterInfo;
};

class RESTSKSERVICE_API CGenerateSceneCharIdentificationJobInfo : public CSKAnnotationJobInfo
{
public:
	CGenerateSceneCharIdentificationJobInfo();
	virtual ~CGenerateSceneCharIdentificationJobInfo();

	CGenerateSceneCharIdentificationJobInfo(const CGenerateSceneCharIdentificationJobInfo&);
	CGenerateSceneCharIdentificationJobInfo operator=(const CGenerateSceneCharIdentificationJobInfo &);

	virtual string GetJsonJobInfo();

public:
	__int64 m_llAssetID;

	string m_stdstrTitle;
	string m_stdstrSrcFramesDirPath;
	string m_stdstrWorkingDirPath;
	string m_stdstrFrameThumbDirPath;
	string m_stdstrCharGroupPictureDirPath;
	string m_stdstrCharRepPictureDirPath;
	string m_stdstrShotInfoPath;

	string m_stdstrExtraValue;

	vector<string> m_vecCharFeaturePath;
	vector<CSceneInfo> m_vecSceneInfo;
};

class RESTSKSERVICE_API CGenerateObjectEtcIdentificationJobInfo : public CSKAnnotationJobInfo
{
public:
	CGenerateObjectEtcIdentificationJobInfo();
	virtual ~CGenerateObjectEtcIdentificationJobInfo();

	CGenerateObjectEtcIdentificationJobInfo(const CGenerateObjectEtcIdentificationJobInfo&);
	CGenerateObjectEtcIdentificationJobInfo operator=(const CGenerateObjectEtcIdentificationJobInfo &);

	virtual string GetJsonJobInfo();

public:
	__int64 m_llAssetID;

	string m_stdstrTitle;
	string m_stdstrSrcFramesDirPath;
	string m_stdstrWorkingDirPath;
	string m_stdstrVideoFilePath;
};

class RESTSKSERVICE_API CCharacterIdentifyData : public CCharacterInfo
{
public:
	CCharacterIdentifyData();
	virtual ~CCharacterIdentifyData();

	CCharacterIdentifyData(const CCharacterIdentifyData&);
	CCharacterIdentifyData operator=(const CCharacterIdentifyData &);

public:
	double m_dbConfidence;

	int m_nPositionX;
	int m_nPositionY;
	int m_nPositionWidth;
	int m_nPositionHeight;
};

class RESTSKSERVICE_API CSKAnnotationResult
{
public:
	CSKAnnotationResult();
	virtual ~CSKAnnotationResult();

	CSKAnnotationResult(const CSKAnnotationResult&);
	CSKAnnotationResult operator=(const CSKAnnotationResult &);

	void SetJobInfo(void* pJobInfo) { m_pIdentifyJobInfo = pJobInfo; };
	void AbortOperation();

	virtual void Clear();

	virtual string GetResultData(bool& bAbnormalEnd);
	virtual int LoadIdentifyDataFile();

	virtual void IdentifyDataOperationProc();

	string GetLastErrorMsg(int& nRetError) { nRetError = m_nError; return m_stdstrErrMsg; };

public:
	__int64 m_llAssetID;
	
	bool m_bReportEndData;

	int m_nDataCountPerReport;

	string* m_pstdstrJobInfo;

protected:
	bool m_bAbort;

	int m_nError;
	int m_nIdentifyDataCount;
	
	string m_stdstrErrMsg;

	void* m_pIdentifyResultJsonData;
	void* m_pIdentifyJobInfo;

	string m_stdstrAnnotationType;
};

class RESTSKSERVICE_API CSKFaceAnnotationResult : public CSKAnnotationResult
{
public:
	CSKFaceAnnotationResult();
	virtual ~CSKFaceAnnotationResult();

	CSKFaceAnnotationResult(const CSKFaceAnnotationResult&);
	CSKFaceAnnotationResult operator=(const CSKFaceAnnotationResult &);

	virtual string GetResultData(bool& bAbnormalEnd);
	virtual int LoadIdentifyDataFile();

	virtual void Clear();

	virtual void IdentifyDataOperationProc();

protected:
	string GetXmlResultCharData();
	string GetXmlResultFrameRangeData();

	string GetXmlResultFrameCharsData();
	string GetXmlReportCharsData();

	bool CompareCharIdentification(vector<CCharacterIdentifyData>& vecCharIdentifyDataOld
		, vector<CCharacterIdentifyData>& vecCharIdentifyData);

	int GetFrameCharIdentifyResultData(void* pJsonFrameData, vector<CCharacterIdentifyData>& retFrameCharIdentifyData);

public:
	__int64 m_llSceneID;

	int m_nFramePeriod;
	int m_nReportCount;
	int m_nCurrentResultSceneIdx;

	// pair<start frame index, end frame index>
	map<pair<int, int>, vector<CCharacterIdentifyData>> m_mapFrameRangeCharIdentifyData;
	map<pair<int, int>, vector<CCharacterIdentifyData>> m_mapLastFrameRangeCharIdentifyData;

	// pair<frame index, frame path>
	queue<map<pair<int, string>, vector<CCharacterIdentifyData>>> m_queueFrameCharIdentifyDataQueue;

	bool m_bEndResultData;
	bool m_bWorkingResult;
};

class RESTSKSERVICE_API CSKFFaceAnnotationResult
{
public:
	CSKFFaceAnnotationResult();
	virtual ~CSKFFaceAnnotationResult();

	CSKFFaceAnnotationResult(const CSKFFaceAnnotationResult&);
	CSKFFaceAnnotationResult operator=(const CSKFFaceAnnotationResult &);

	string GetXmlResultData();

	void Clear();

protected:
	//string GetXmlResultCharData(vector<CCharacterIdentifyData>& vecCharIdentifyData);
	string GetXmlResultCharData();
	//string GetXmlResultFrameData();
	string GetXmlResultFrameRangeData();

	bool CompareCharIdentification(vector<CCharacterIdentifyData>& vecCharIdentifyDataOld
		, vector<CCharacterIdentifyData>& vecCharIdentifyData);

public:
	__int64 m_llSceneID;
	__int64 m_llAssetID;

	int m_nFramePeriod;
	int m_nReportCount;

	string* m_pstdstrJobInfo;

	// pair<start frame index, end frame index>
	map<pair<int, int>, vector<CCharacterIdentifyData>> m_mapFrameRangeCharIdentifyData;
	map<pair<int, int>, vector<CCharacterIdentifyData>> m_mapLastFrameRangeCharIdentifyData;

	// pair<frame index, frame path>
	queue<map<pair<int, string>, vector<CCharacterIdentifyData>>> m_queueFrameCharIdentifyDataQueue;

	bool m_bEndResultData;
	bool m_bReportEndData;
};

class RESTSKSERVICE_API CObjectEtcIdentifyData
{
public:
	CObjectEtcIdentifyData();
	virtual ~CObjectEtcIdentifyData();

	CObjectEtcIdentifyData(const CObjectEtcIdentifyData&);
	CObjectEtcIdentifyData operator=(const CObjectEtcIdentifyData &);

public:
	string m_stdstrClass;

	double m_dbScore;
};

class RESTSKSERVICE_API CObjectIdentifyData : public CObjectEtcIdentifyData
{
public:
	CObjectIdentifyData();
	virtual ~CObjectIdentifyData();

	CObjectIdentifyData(const CObjectIdentifyData&);
	CObjectIdentifyData operator=(const CObjectIdentifyData &);

public:
	int m_nPositionX;
	int m_nPositionY;
	int m_nPositionWidth;
	int m_nPositionHeight;
};


class RESTSKSERVICE_API CSKObjectEtcAnnotationResult : public CSKAnnotationResult
{
public:
	CSKObjectEtcAnnotationResult();
	virtual ~CSKObjectEtcAnnotationResult();

	CSKObjectEtcAnnotationResult(const CSKObjectEtcAnnotationResult&);
	CSKObjectEtcAnnotationResult operator=(const CSKObjectEtcAnnotationResult &);

	virtual string GetResultData(bool& bAbnormalEnd);
	virtual int LoadIdentifyDataFile();

	virtual void Clear();

	virtual void IdentifyDataOperationProc();

protected:
	string GetXmlResultFrameObjectEtcData();
	string GetXmlReportObjectEtcData();

	string GetXmlReportVideoData();
	string GetXmlReportObjectData();
	string GetXmlReportEventData();
	string GetXmlReportPlaceData();
	string GetXmlReportLandmarkData();

public:
	int m_nReportCount;

	bool m_bEndResultData;
	bool m_bWorkingResult;

	vector<CObjectEtcIdentifyData> m_vecVideoData;

	map<int, vector<CObjectIdentifyData>> m_mapObjectData;
	map<int, vector<CObjectEtcIdentifyData>> m_mapPlaceData;
	map<int, vector<CObjectEtcIdentifyData>> m_mapLandmarkData;
	map<int, vector<CObjectEtcIdentifyData>> m_mapEventData;

	deque<pair<int, int>> m_queueFrameObjectEtcdentifyDataQueue;
};

class RESTSKSERVICE_API CKRestSKAnnotationService
{
public:
	typedef int(*JOBSTATECB)(string stdstrJobState, int nProgress, string stdstrErrMsg);

public:
	CKRestSKAnnotationService(void);
	virtual ~CKRestSKAnnotationService(void);

	void SetServiceInfo(string stdstrServiceUrl);
	void SetJobStateCB(JOBSTATECB pFunc, void* pData); // implement not yet

	void SetJobInfo(CSKAnnotationJobInfo* pJobInfo);

	int AssignFaceFeatureGenJob();
	int AssignSceneCharacterIdentificationJob();
	int AssignObjectEtcIdentificationJob();
	int RemoveAssignedJob();
	int AbortAssignedJob();
	int GetStateAssignedJob(eSKAnnotationJobState& eRetJobState, int& nRetProgress, string& stdstrRetErrMsg);

	int LoadIdentifyDataFile();
	string GetRefinedIdentifyData(eSKAnnotationResultProcState& eRetResultState);

	int LoadSceneCharIdentifyDataFile();
	int LoadSceneCharIdentifyDataFile(string stdstrResultJsonPath);
	string GetSceneCharIdentifyResultData(bool& bRetEndData);

	string GetServiceMessage(int& nRetError);
	string GetStrResponse();

	void AddCharIdentifyDataThreadProc();

	string GetXmlExtraValueData(__int64 llAssetID, string stdstrExtraValue, string stdstrJobInfo);

	void SetResultDataCountPerOneReport(int nCount);

protected:

	int GetFrameCharIdentifyResultData(void* pJsonFrameData, vector<CCharacterIdentifyData>& retFrameCharIdentifyData);

	void CheckResponseData(string stdstrResponse, int& nRetError, string& stdstrRetErrMsg);
	void GetJobStateInfo(eSKAnnotationJobState& eRetJobState, int& nRetProgress, string& stdstrRetErrMsg);

private:
	int m_nCurrentResultSceneIdx;
	int m_nDataCountPerReport;
	int m_nGotCharIdentifyDataCount;
	int m_nError;
	int m_nReportCount;

	bool m_bWorkingResult;
	bool m_bAbort;

	string m_stdstrServiceUrl;
	string m_stdstrErrMsg;
	string m_stdstrResponse;

	JOBSTATECB m_pFuncStateCB;
	void* m_pDataStateCB;

	void* m_pCharIdentifyResultJsonData;

	CSKAnnotationJobInfo* m_pJobInfo;
	CSKFFaceAnnotationResult* m_pFaceAnnotationResult;

	CSKAnnotationResult* m_pAnnotationResult;
};
