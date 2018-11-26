
#include "stdafx.h"
#include "KRestSKAnnotationService.h"
#include "RestClient.h"
#include "Utility.h"

#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <mutex>
#include <algorithm>

#include <cpprest/json.h>

using namespace utility;
using namespace web;

#pragma warning (disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define THREAD_STACK_SIZE					(1024*1024*4)

// SK 얼굴 인식 엔진 10프레임 주기로 프레임 이미지 입력
#define RESTSKSERVICE_FACE_IDENTIFY_INPTU_FRAME_PERIOD		 (10)
#define RESTSKSERVICE_IDENTIFY_RESULT_BATCH_COUNT			 (10)

#define RESTSKSERVICE_STATE_ERROR						 ("error")
#define RESTSKSERVICE_STATE_DOGALLERYFEATUREGEN			 ("doGalleryFeatureGen")
#define RESTSKSERVICE_STATE_DONEGALLERYFEATUREGEN		 ("doneGalleryFeatureGen")
#define RESTSKSERVICE_STATE_DOFRAMEFEATUREGEN			 ("doFrameFeatureGen")
#define RESTSKSERVICE_STATE_DONEFRAMEFEATUREGEN			 ("doneFrameFeatureGen")
#define RESTSKSERVICE_STATE_DOCHARACTERIDENTIFY			 ("doCharacterIdentify")
#define RESTSKSERVICE_STATE_DONECHARACTERIDENTIFY		 ("doneCharacterIdentify")
#define RESTSKSERVICE_STATE_DOOBJECTETCIDENTIFY			 ("doObjectEtcIdentify")
#define RESTSKSERVICE_STATE_DONEOBJECTETCIDENTIFY		 ("doneObjectEtcIdentify")
#define RESTSKSERVICE_STATE_DOVIDEOIDENTIFY				 ("doVideoIdentify")
#define RESTSKSERVICE_STATE_DONEVIDEOIDENTIFY			 ("doneVideoIdentify")

#define RESTSKSERVICE_ANNOTATE_GENERATE_CHARACTER_FEATURE	 ("GenCharacterFeature")
#define RESTSKSERVICE_ANNOTATE_IDENTIFY_CHARACTER			 ("IdentifyCharacter")
#define RESTSKSERVICE_ANNOTATE_IDENTIFY_OBJECT_ETC			 ("IdentifyObjectEtc")

#define REPORTED_ANNOTATATION_DATATYPE_NONE					 (0)
#define REPORTED_ANNOTATATION_DATATYPE_OBJECT				 (1)
#define REPORTED_ANNOTATATION_DATATYPE_EVENT				 (2)
#define REPORTED_ANNOTATATION_DATATYPE_PLACE				 (3)
#define REPORTED_ANNOTATATION_DATATYPE_LANDMARK				 (4)

const pair<string, string> g_pairGenCharFeature("POST", "/faceannotation/dofacefeaturegen");
const pair<string, string> g_pairScenCharIdentification("POST", "/faceannotation/identification");
const pair<string, string> g_pairObjectEtcIdentification("POST", "/objectetcannotation/identification");
const pair<string, string> g_pairGetJobState("GET", "/annotation/getjobstate/{jobID}");
const pair<string, string> g_pairRemoveJob("DELETE", "/annotation/removejob/{jobID}");
const pair<string, string> g_pairAbortJob("POST", "/annotation/abortjob/{jobID}");

mutex g_mutexCharIdentifyData;
mutex g_mutexObjectEtcIdentifyData;

CSKAnnotationJobInfo::CSKAnnotationJobInfo()
{
	m_nJobID = -1;

	m_pstdstrJobInfo = new string;
}

CSKAnnotationJobInfo::~CSKAnnotationJobInfo()
{
	if (m_pstdstrJobInfo)
	{
		delete m_pstdstrJobInfo;
		m_pstdstrJobInfo = NULL;
	}

}

CSKAnnotationJobInfo::CSKAnnotationJobInfo(const CSKAnnotationJobInfo& operand)
{
	m_nJobID = operand.m_nJobID;
	m_stdstrAnnotationType = operand.m_stdstrAnnotationType;

	if (operand.m_pstdstrJobInfo)
	{
		if (m_pstdstrJobInfo)
			*m_pstdstrJobInfo = *operand.m_pstdstrJobInfo;
		else
		{
			m_pstdstrJobInfo = new string;
			*m_pstdstrJobInfo = *operand.m_pstdstrJobInfo;
		}
	}
}

CSKAnnotationJobInfo CSKAnnotationJobInfo::operator=(const CSKAnnotationJobInfo& operand)
{
	m_nJobID = operand.m_nJobID;
	m_stdstrAnnotationType = operand.m_stdstrAnnotationType;

	if (operand.m_pstdstrJobInfo)
	{
		if (m_pstdstrJobInfo)
			*m_pstdstrJobInfo = *operand.m_pstdstrJobInfo;
		else
		{
			m_pstdstrJobInfo = new string;
			*m_pstdstrJobInfo = *operand.m_pstdstrJobInfo;
		}
	}

	return *this;
}

string CSKAnnotationJobInfo::GetJsonJobInfo()
{
	string stdstrRet;

	return stdstrRet;
}

CCharacterInfo::CCharacterInfo()
{
	m_llCharID = -1;

	m_stdstrCharName = "";
	m_stdstrCharRole = "";
	m_stdstrCharSrcImagesDirPath = "";
	m_stdstrCharFeatureOutFilePath = "";
}

CCharacterInfo::~CCharacterInfo()
{

}

CCharacterInfo::CCharacterInfo(const CCharacterInfo& operand)
{
	m_llCharID = operand.m_llCharID;

	m_stdstrCharName = operand.m_stdstrCharName;
	m_stdstrCharRole = operand.m_stdstrCharRole;
	m_stdstrCharSrcImagesDirPath = operand.m_stdstrCharSrcImagesDirPath;
	m_stdstrCharFeatureOutFilePath = operand.m_stdstrCharFeatureOutFilePath;
}

CCharacterInfo CCharacterInfo::operator=(const CCharacterInfo & operand)
{
	m_llCharID = operand.m_llCharID;

	m_stdstrCharName = operand.m_stdstrCharName;
	m_stdstrCharRole = operand.m_stdstrCharRole;
	m_stdstrCharSrcImagesDirPath = operand.m_stdstrCharSrcImagesDirPath;
	m_stdstrCharFeatureOutFilePath = operand.m_stdstrCharFeatureOutFilePath;

	return *this;
}


CSceneInfo::CSceneInfo()
{
	m_llSceneID = -1;

	m_nStartIndex = -1;
	m_nEndIndex = -1;
}

CSceneInfo::~CSceneInfo()
{

}

CSceneInfo::CSceneInfo(const CSceneInfo& operand)
{
	m_llSceneID = operand.m_llSceneID;

	m_nStartIndex = operand.m_nStartIndex;
	m_nEndIndex = operand.m_nEndIndex;
}

CSceneInfo CSceneInfo::operator=(const CSceneInfo & operand)
{
	m_llSceneID = operand.m_llSceneID;

	m_nStartIndex = operand.m_nStartIndex;
	m_nEndIndex = operand.m_nEndIndex;

	return *this;
}

CGenerateCharacterFeatureJobInfo::CGenerateCharacterFeatureJobInfo()
{
	m_stdstrAnnotationType = RESTSKSERVICE_ANNOTATE_GENERATE_CHARACTER_FEATURE;
}

CGenerateCharacterFeatureJobInfo::~CGenerateCharacterFeatureJobInfo()
{

}

CGenerateCharacterFeatureJobInfo::CGenerateCharacterFeatureJobInfo(const CGenerateCharacterFeatureJobInfo& operand)
{
	CSKAnnotationJobInfo::operator=(operand);

	m_vecCharacterInfo = operand.m_vecCharacterInfo;
}

CGenerateCharacterFeatureJobInfo CGenerateCharacterFeatureJobInfo::operator=(const CGenerateCharacterFeatureJobInfo & operand)
{
	CSKAnnotationJobInfo::operator=(operand);

	m_vecCharacterInfo = operand.m_vecCharacterInfo;

	return *this;
}


string CGenerateCharacterFeatureJobInfo::GetJsonJobInfo()
{
	////////////// face feature generation job request json ////////////////////////////////////////
	/*
	{
		"jobinfo": {
			"jobid": 1111,
				"character" : [
					{
						"id": 1234,
						"name" : "WoodyHarrelson",
						"role" : "WoodyHarrelson",
						"imagesdirpath" : "./testdata/galleries/NowYouSeeMe/WoodyHarrelson",
						"featurepath" : "./testdata/resultroot/WoodyHarrelson.pikle"
					},
					{
						"id": 1235,
						"name" : "MorganFreeman",
						"role" : "MorganFreeman",
						"imagesdirpath" : "./testdata/galleries/NowYouSeeMe/MorganFreeman",
						"featurepath" : "./testdata/resultroot/MorganFreeman.pikle"
					}
				]
		}
	}
	*/
	////////////// face feature generation job request json ////////////////////////////////////////

	string stdstrRet;
	
	CCharacterInfo CharacterInfo;

	json::value jsonJob;
	json::value jsonJobInfo;
	json::value arrCharacterInfo = json::value::array();
	json::value jsonCharacterInfo;

	int nCount = (int)m_vecCharacterInfo.size();

	for (int i = 0; i < nCount; i++)
	{
		CharacterInfo = m_vecCharacterInfo[i];

		jsonCharacterInfo[L"id"] = json::value::number(CharacterInfo.m_llCharID);
		jsonCharacterInfo[L"name"] = json::value::string(utility::conversions::utf8_to_utf16(CharacterInfo.m_stdstrCharName));
		jsonCharacterInfo[L"role"] = json::value::string(utility::conversions::utf8_to_utf16(CharacterInfo.m_stdstrCharRole));

		string stdstrCharSrcImagesDirPath, stdstrCharFeatureOutFilePath;
		stdstrCharSrcImagesDirPath = Utility::ReplaceAll(CharacterInfo.m_stdstrCharSrcImagesDirPath, "\\", "/");
		stdstrCharFeatureOutFilePath = Utility::ReplaceAll(CharacterInfo.m_stdstrCharFeatureOutFilePath, "\\", "/");

		stdstrCharSrcImagesDirPath = Utility::ReplaceAll(stdstrCharSrcImagesDirPath, "//", "/");
		stdstrCharFeatureOutFilePath = Utility::ReplaceAll(stdstrCharFeatureOutFilePath, "//", "/");

		jsonCharacterInfo[L"imagesdirpath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrCharSrcImagesDirPath));
		jsonCharacterInfo[L"featurepath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrCharFeatureOutFilePath));

		arrCharacterInfo[i] = jsonCharacterInfo;
	}

	jsonJobInfo[L"jobid"] = json::value::number(m_nJobID);
	jsonJobInfo[L"character"] = arrCharacterInfo;
	
	jsonJob[L"jobinfo"] = jsonJobInfo;

	stdstrRet = utility::conversions::utf16_to_utf8(jsonJob.serialize());

	printf("CGenerateCharacterFeatureJobInfo::GetJsonJobInfo() - %s\n", stdstrRet.c_str());

	return stdstrRet;
}


CGenerateSceneCharIdentificationJobInfo::CGenerateSceneCharIdentificationJobInfo()
{
	m_stdstrTitle = "";
	m_stdstrSrcFramesDirPath = "";
	//m_stdstrOut1stFrmFeatureDirPath = "";
	//m_stdstrOut2ndFrmFeatureDirPath = "";
	//m_stdstrOutSceneAnnotationDirPath = "";
	m_stdstrWorkingDirPath = "";
	m_stdstrFrameThumbDirPath = "";
	m_stdstrCharGroupPictureDirPath = "";
	m_stdstrCharRepPictureDirPath = "";
	m_stdstrShotInfoPath = "";
	m_stdstrExtraValue = "";

	m_llAssetID = 0;

	m_stdstrAnnotationType = RESTSKSERVICE_ANNOTATE_IDENTIFY_CHARACTER;
}

CGenerateSceneCharIdentificationJobInfo::~CGenerateSceneCharIdentificationJobInfo()
{
	
}

CGenerateSceneCharIdentificationJobInfo::CGenerateSceneCharIdentificationJobInfo(const CGenerateSceneCharIdentificationJobInfo& operand)
{
	CSKAnnotationJobInfo::operator=(operand);

	m_stdstrTitle = operand.m_stdstrTitle;
	m_stdstrSrcFramesDirPath = operand.m_stdstrSrcFramesDirPath;
	//m_stdstrOut1stFrmFeatureDirPath = operand.m_stdstrOut1stFrmFeatureDirPath;
	//m_stdstrOut2ndFrmFeatureDirPath = operand.m_stdstrOut2ndFrmFeatureDirPath;
	//m_stdstrOutSceneAnnotationDirPath = operand.m_stdstrOutSceneAnnotationDirPath;
	m_stdstrWorkingDirPath = operand.m_stdstrWorkingDirPath;
	m_stdstrFrameThumbDirPath = operand.m_stdstrFrameThumbDirPath;
	m_stdstrCharGroupPictureDirPath = operand.m_stdstrCharGroupPictureDirPath;
	m_stdstrCharRepPictureDirPath = operand.m_stdstrCharRepPictureDirPath;
	m_stdstrShotInfoPath = operand.m_stdstrShotInfoPath;
	m_stdstrExtraValue = operand.m_stdstrExtraValue;

	m_vecCharFeaturePath = operand.m_vecCharFeaturePath;
	m_vecSceneInfo = operand.m_vecSceneInfo;

	m_llAssetID = operand.m_llAssetID;
}

CGenerateSceneCharIdentificationJobInfo CGenerateSceneCharIdentificationJobInfo::operator=(const CGenerateSceneCharIdentificationJobInfo & operand)
{
	CSKAnnotationJobInfo::operator=(operand);

	m_stdstrTitle = operand.m_stdstrTitle;
	m_stdstrSrcFramesDirPath = operand.m_stdstrSrcFramesDirPath;
	//m_stdstrOut1stFrmFeatureDirPath = operand.m_stdstrOut1stFrmFeatureDirPath;
	//m_stdstrOut2ndFrmFeatureDirPath = operand.m_stdstrOut2ndFrmFeatureDirPath;
	//m_stdstrOutSceneAnnotationDirPath = operand.m_stdstrOutSceneAnnotationDirPath;
	m_stdstrWorkingDirPath = operand.m_stdstrWorkingDirPath;
	m_stdstrFrameThumbDirPath = operand.m_stdstrFrameThumbDirPath;
	m_stdstrCharGroupPictureDirPath = operand.m_stdstrCharGroupPictureDirPath;
	m_stdstrCharRepPictureDirPath = operand.m_stdstrCharRepPictureDirPath;
	m_stdstrShotInfoPath = operand.m_stdstrShotInfoPath;
	m_stdstrExtraValue = operand.m_stdstrExtraValue;

	m_vecCharFeaturePath = operand.m_vecCharFeaturePath;
	m_vecSceneInfo = operand.m_vecSceneInfo;

	m_llAssetID = operand.m_llAssetID;

	return *this;
}


string CGenerateSceneCharIdentificationJobInfo::GetJsonJobInfo()
{
	////////////// face identification job request json ////////////////////////////////////////
	/*
	{
		"jobinfo": {
			"jobid": 1111,
			"title": "NowYouSeeMe",
			"characterfeaturepath": ["./testdata/resultroot/WoodyHarrelson.pikle", 
			"./testdata/resultroot/MorganFreeman.pikle"
			],
			"framesdirpath": "./testdata/frames",
			"scenes": [
				{
					"id": 1,
					"startindex": 0,
					"endindex": 9
				},
				{
					"id": 2,
					"startindex": 10,
					"endindex": 19
				}
			],
			"workingdirpath": "./testdata/resultroot",
			"framethumbdirpath": "./testdata/resultroot/NowYouSeeMe/thumb"
		}
	}
	*/
	////////////// face identification job request json ////////////////////////////////////////

	string stdstrRet;

	json::value jsonJob;
	json::value jsonJobInfo;
	json::value arrCharacterFeature = json::value::array();
	json::value arrSceneInfo = json::value::array();

	int i = 0;
	int nCount = (int)m_vecCharFeaturePath.size();
	string stdstrCharFeaturePath;

	for (i = 0; i < nCount; i++)
	{
		stdstrCharFeaturePath = Utility::ReplaceAll(m_vecCharFeaturePath[i], "\\", "/");
		stdstrCharFeaturePath = Utility::ReplaceAll(stdstrCharFeaturePath, "//", "/");

		arrCharacterFeature[i] = json::value::string(utility::conversions::utf8_to_utf16(stdstrCharFeaturePath));
	}

	nCount = (int)m_vecSceneInfo.size();

	for (i = 0; i < nCount; i++)
	{
		json::value jsonSecne;

		jsonSecne[L"id"] = json::value::number(m_vecSceneInfo[i].m_llSceneID);
		jsonSecne[L"startindex"] = json::value::number(m_vecSceneInfo[i].m_nStartIndex);
		jsonSecne[L"endindex"] = json::value::number(m_vecSceneInfo[i].m_nEndIndex);

		arrSceneInfo[i] = jsonSecne;
	}

	jsonJobInfo[L"jobid"] = json::value::number(m_nJobID);
	jsonJobInfo[L"title"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrTitle));
	
	if (!m_stdstrExtraValue.empty())
	{
		json::value jsonExtraValue = json::value::parse(utility::conversions::utf8_to_utf16(m_stdstrExtraValue));
		jsonJobInfo[L"extravalue"] = jsonExtraValue;
	}

	string stdstrSrcFramesDirPath, stdstrWorkingDirPath, stdstrFrameThumbDirPath;
	stdstrSrcFramesDirPath = Utility::ReplaceAll(m_stdstrSrcFramesDirPath, "\\", "/");
	stdstrWorkingDirPath = Utility::ReplaceAll(m_stdstrWorkingDirPath, "\\", "/");
	stdstrFrameThumbDirPath = Utility::ReplaceAll(m_stdstrFrameThumbDirPath, "\\", "/");

	stdstrSrcFramesDirPath = Utility::ReplaceAll(stdstrSrcFramesDirPath, "//", "/");
	stdstrWorkingDirPath = Utility::ReplaceAll(stdstrWorkingDirPath, "//", "/");
	stdstrFrameThumbDirPath = Utility::ReplaceAll(stdstrFrameThumbDirPath, "//", "/");

	string stdstrCharGroupPictureDirPath, stdstrCharRepPictureDirPath, stdstrShotInfoPath;
	stdstrCharGroupPictureDirPath = Utility::ReplaceAll(m_stdstrCharGroupPictureDirPath, "\\", "/");
	stdstrCharRepPictureDirPath = Utility::ReplaceAll(m_stdstrCharRepPictureDirPath, "\\", "/");
	stdstrShotInfoPath = Utility::ReplaceAll(m_stdstrShotInfoPath, "\\", "/");

	stdstrCharGroupPictureDirPath = Utility::ReplaceAll(stdstrCharGroupPictureDirPath, "//", "/");
	stdstrCharRepPictureDirPath = Utility::ReplaceAll(stdstrCharRepPictureDirPath, "//", "/");
	stdstrShotInfoPath = Utility::ReplaceAll(stdstrShotInfoPath, "//", "/");

	jsonJobInfo[L"framesdirpath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrSrcFramesDirPath));
	jsonJobInfo[L"workingdirpath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrWorkingDirPath));
	jsonJobInfo[L"framethumbdirpath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrFrameThumbDirPath));
	jsonJobInfo[L"charactergrouppicturedirpath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrCharGroupPictureDirPath));
	jsonJobInfo[L"characterreppicturedirpath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrCharRepPictureDirPath));
	jsonJobInfo[L"characterfeaturepath"] = arrCharacterFeature;
	jsonJobInfo[L"scenes"] = arrSceneInfo;

	jsonJob[L"jobinfo"] = jsonJobInfo;

	stdstrRet = utility::conversions::utf16_to_utf8(jsonJob.serialize());

	printf("CGenerateSceneCharIdentificationJobInfo::GetJsonJobInfo() - %s\n", stdstrRet.c_str());

	return stdstrRet;
}

CGenerateObjectEtcIdentificationJobInfo::CGenerateObjectEtcIdentificationJobInfo()
{
	m_stdstrTitle = "";
	m_stdstrSrcFramesDirPath = "";
	//m_stdstrOut1stFrmFeatureDirPath = "";
	//m_stdstrOut2ndFrmFeatureDirPath = "";
	//m_stdstrOutSceneAnnotationDirPath = "";
	m_stdstrWorkingDirPath = "";
	m_stdstrVideoFilePath = "";

	m_llAssetID = 0;

	m_stdstrAnnotationType = RESTSKSERVICE_ANNOTATE_IDENTIFY_OBJECT_ETC;
}

CGenerateObjectEtcIdentificationJobInfo::~CGenerateObjectEtcIdentificationJobInfo()
{

}

CGenerateObjectEtcIdentificationJobInfo::CGenerateObjectEtcIdentificationJobInfo(const CGenerateObjectEtcIdentificationJobInfo& operand)
{
	CSKAnnotationJobInfo::operator=(operand);

	m_stdstrTitle = operand.m_stdstrTitle;
	m_stdstrSrcFramesDirPath = operand.m_stdstrSrcFramesDirPath;
	m_stdstrWorkingDirPath = operand.m_stdstrWorkingDirPath;
	m_stdstrVideoFilePath = operand.m_stdstrVideoFilePath;

	m_llAssetID = operand.m_llAssetID;
}

CGenerateObjectEtcIdentificationJobInfo CGenerateObjectEtcIdentificationJobInfo::operator=(const CGenerateObjectEtcIdentificationJobInfo & operand)
{
	CSKAnnotationJobInfo::operator=(operand);

	m_stdstrTitle = operand.m_stdstrTitle;
	m_stdstrSrcFramesDirPath = operand.m_stdstrSrcFramesDirPath;
	m_stdstrWorkingDirPath = operand.m_stdstrWorkingDirPath;
	m_stdstrVideoFilePath = operand.m_stdstrVideoFilePath;

	m_llAssetID = operand.m_llAssetID;

	return *this;
}


string CGenerateObjectEtcIdentificationJobInfo::GetJsonJobInfo()
{
	////////////// object, evnet, place, video identification job request json ////////////////////////////////////////

	//{
	//	"jobinfo": {
	//		"jobid": 1111,
	//		"title": "NowYouSeeMe",
	//		"framesdirpath": "./testdata/frames",
	//		"workingdirpath": "./testdata/resultroot",
	//		"videofilepath": "./testdata/resultroot/NowYouSeeMe/aaaa.ts"
	//	}
	//}

	////////////// object, evnet, place, video identification job request json ////////////////////////////////////////

	string stdstrRet;

	json::value jsonJob;
	json::value jsonJobInfo;
	json::value arrCharacterFeature = json::value::array();
	json::value arrSceneInfo = json::value::array();

	jsonJobInfo[L"jobid"] = json::value::number(m_nJobID);
	jsonJobInfo[L"title"] = json::value::string(utility::conversions::utf8_to_utf16(m_stdstrTitle));

	string stdstrSrcFramesDirPath, stdstrWorkingDirPath, stdstrVideoFilePath;
	stdstrSrcFramesDirPath = Utility::ReplaceAll(m_stdstrSrcFramesDirPath, "\\", "/");
	stdstrWorkingDirPath = Utility::ReplaceAll(m_stdstrWorkingDirPath, "\\", "/");
	stdstrVideoFilePath = Utility::ReplaceAll(m_stdstrVideoFilePath, "\\", "/");

	stdstrSrcFramesDirPath = Utility::ReplaceAll(stdstrSrcFramesDirPath, "//", "/");
	stdstrWorkingDirPath = Utility::ReplaceAll(stdstrWorkingDirPath, "//", "/");
	stdstrVideoFilePath = Utility::ReplaceAll(stdstrVideoFilePath, "//", "/");

	jsonJobInfo[L"framesdirpath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrSrcFramesDirPath));
	jsonJobInfo[L"workingdirpath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrWorkingDirPath));
	jsonJobInfo[L"videofilepath"] = json::value::string(utility::conversions::utf8_to_utf16(stdstrVideoFilePath));

	jsonJob[L"jobinfo"] = jsonJobInfo;

	stdstrRet = utility::conversions::utf16_to_utf8(jsonJob.serialize());

	printf("CGenerateObjectEtcIdentificationJobInfo::GetJsonJobInfo() - %s\n", stdstrRet.c_str());

	return stdstrRet;
}

CCharacterIdentifyData::CCharacterIdentifyData()
{
	m_dbConfidence = 0;

	m_nPositionX = 0;
	m_nPositionY = 0;
	m_nPositionWidth = 0;
	m_nPositionHeight = 0;
}

CCharacterIdentifyData::~CCharacterIdentifyData()
{
	m_dbConfidence = 0;

	m_nPositionX = 0;
	m_nPositionY = 0;
	m_nPositionWidth = 0;
	m_nPositionHeight = 0;
}

CCharacterIdentifyData::CCharacterIdentifyData(const CCharacterIdentifyData& operand)
{
	CCharacterInfo::operator=(operand);

	m_dbConfidence = operand.m_dbConfidence;
	m_nPositionX = operand.m_nPositionX;
	m_nPositionY = operand.m_nPositionY;
	m_nPositionWidth = operand.m_nPositionWidth;
	m_nPositionHeight = operand.m_nPositionHeight;
}

CCharacterIdentifyData CCharacterIdentifyData::operator=(const CCharacterIdentifyData & operand)
{
	CCharacterInfo::operator=(operand);

	m_dbConfidence = operand.m_dbConfidence;
	m_nPositionX = operand.m_nPositionX;
	m_nPositionY = operand.m_nPositionY;
	m_nPositionWidth = operand.m_nPositionWidth;
	m_nPositionHeight = operand.m_nPositionHeight;

	return *this;
}

CSKAnnotationResult::CSKAnnotationResult()
{
	m_pstdstrJobInfo = new string;

	m_llAssetID = 0;

	m_nError = 0;
	m_nIdentifyDataCount;

	m_pIdentifyResultJsonData = NULL;
	m_pIdentifyJobInfo = NULL;

	m_nDataCountPerReport = RESTSKSERVICE_IDENTIFY_RESULT_BATCH_COUNT;

	m_bAbort = false;
	m_bReportEndData = false;
}

CSKAnnotationResult::~CSKAnnotationResult()
{
	m_llAssetID = 0;

	if (m_pstdstrJobInfo)
	{
		m_pstdstrJobInfo->clear();

		delete m_pstdstrJobInfo;
		m_pstdstrJobInfo = NULL;
	}

	m_nError = 0;
	m_nIdentifyDataCount = 0;

	if (m_pIdentifyResultJsonData)
	{
		delete (json::value*)m_pIdentifyResultJsonData;
		m_pIdentifyResultJsonData = NULL;
	}

	m_pIdentifyJobInfo = NULL;

	m_bAbort = false;
	m_bReportEndData = false;
}

CSKAnnotationResult::CSKAnnotationResult(const CSKAnnotationResult& operand)
{
	m_llAssetID = operand.m_llAssetID;

	if (m_pstdstrJobInfo && operand.m_pstdstrJobInfo)
	{
		*m_pstdstrJobInfo = *operand.m_pstdstrJobInfo;
	}

	m_nError = operand.m_nError;
	m_nIdentifyDataCount = operand.m_nIdentifyDataCount;

	m_stdstrErrMsg = operand.m_stdstrErrMsg;

	if (m_pIdentifyResultJsonData && operand.m_pIdentifyResultJsonData)
	{
		delete (json::value*)m_pIdentifyResultJsonData;
		m_pIdentifyResultJsonData = (void*)(new json::value);
		*((json::value*)m_pIdentifyResultJsonData) = *((json::value*)operand.m_pIdentifyResultJsonData);
	}

	m_stdstrAnnotationType = operand.m_stdstrAnnotationType;

	m_bAbort = operand.m_bAbort;
	m_bReportEndData = operand.m_bReportEndData;
}

CSKAnnotationResult CSKAnnotationResult::operator=(const CSKAnnotationResult & operand)
{
	m_llAssetID = operand.m_llAssetID;
	
	if (m_pstdstrJobInfo && operand.m_pstdstrJobInfo)
	{
		*m_pstdstrJobInfo = *operand.m_pstdstrJobInfo;
	}

	m_nError = operand.m_nError;
	m_nIdentifyDataCount = operand.m_nIdentifyDataCount;

	m_stdstrErrMsg = operand.m_stdstrErrMsg;

	if (m_pIdentifyResultJsonData && operand.m_pIdentifyResultJsonData)
	{
		delete (json::value*)m_pIdentifyResultJsonData;
		m_pIdentifyResultJsonData = (void*)(new json::value);
		*((json::value*)m_pIdentifyResultJsonData) = *((json::value*)operand.m_pIdentifyResultJsonData);
	}

	m_stdstrAnnotationType = operand.m_stdstrAnnotationType;

	m_bAbort = operand.m_bAbort;
	m_bReportEndData = operand.m_bReportEndData;

	return *this;
}

void CSKAnnotationResult::AbortOperation()
{ 
	m_bAbort = true;
	
	m_nError = RESTSKSERVICE_ERROR_FORCED_ABORT;
	m_stdstrErrMsg = "Annotation job aborted.";
};

string CSKAnnotationResult::GetResultData(bool& bAbnormalEnd)
{
	string stdstrRet;

	bAbnormalEnd = m_bAbort;
	
	return stdstrRet;
}

void CSKAnnotationResult::Clear()
{
	m_llAssetID = 0;

	if (m_pstdstrJobInfo)
	{
		m_pstdstrJobInfo->clear();
	}

	m_nError = 0;
	m_nIdentifyDataCount = 0;

	if (m_pIdentifyResultJsonData)
	{
		delete (json::value*)m_pIdentifyResultJsonData;
		m_pIdentifyResultJsonData = NULL;
	}

	m_pIdentifyJobInfo = NULL;

	m_bAbort = false;
	m_bReportEndData = false;
}

int CSKAnnotationResult::LoadIdentifyDataFile()
{
	int nRet = 0;

	return nRet;
}

void CSKAnnotationResult::IdentifyDataOperationProc()
{

}

unsigned __stdcall AddIdentifyDataProc(void *pData)
{
	unsigned nRet = 0;

	if (pData)
	{
		CSKAnnotationResult* pAnnotationResult = (CSKAnnotationResult*)pData;
		pAnnotationResult->IdentifyDataOperationProc();
	}

	return nRet;
}

CSKFaceAnnotationResult::CSKFaceAnnotationResult()
{
	m_llSceneID = 0;

	m_bEndResultData = false;

	m_nFramePeriod = RESTSKSERVICE_FACE_IDENTIFY_INPTU_FRAME_PERIOD;
	m_nReportCount = RESTSKSERVICE_IDENTIFY_RESULT_BATCH_COUNT;
	m_nCurrentResultSceneIdx = -1;
}

CSKFaceAnnotationResult::~CSKFaceAnnotationResult()
{
	m_bEndResultData = true;

	m_mapFrameRangeCharIdentifyData.clear();
	m_mapLastFrameRangeCharIdentifyData.clear();

	g_mutexCharIdentifyData.lock();
	//std::queue<map<pair<int, string>, vector<CCharacterIdentifyData>>> empty;
	//std::swap(m_queueFrameCharIdentifyDataQueue, empty);

	int nCount = m_queueFrameCharIdentifyDataQueue.size();
	for (int i = 0; i < nCount; i++)
	{
		m_queueFrameCharIdentifyDataQueue.pop();
	}

	g_mutexCharIdentifyData.unlock();
}

CSKFaceAnnotationResult::CSKFaceAnnotationResult(const CSKFaceAnnotationResult& operand)
{
	CSKAnnotationResult::operator=(operand);

	m_llSceneID = operand.m_llSceneID;

	m_mapFrameRangeCharIdentifyData = operand.m_mapFrameRangeCharIdentifyData;
	m_mapLastFrameRangeCharIdentifyData = operand.m_mapLastFrameRangeCharIdentifyData;
	m_queueFrameCharIdentifyDataQueue = operand.m_queueFrameCharIdentifyDataQueue;

	m_bEndResultData = operand.m_bEndResultData;
	m_nCurrentResultSceneIdx = operand.m_nCurrentResultSceneIdx;
}

CSKFaceAnnotationResult CSKFaceAnnotationResult::operator=(const CSKFaceAnnotationResult & operand)
{
	CSKAnnotationResult::operator=(operand);

	m_llSceneID = operand.m_llSceneID;

	m_mapFrameRangeCharIdentifyData = operand.m_mapFrameRangeCharIdentifyData;
	m_mapLastFrameRangeCharIdentifyData = operand.m_mapLastFrameRangeCharIdentifyData;
	m_queueFrameCharIdentifyDataQueue = operand.m_queueFrameCharIdentifyDataQueue;

	m_bEndResultData = operand.m_bEndResultData;
	m_nCurrentResultSceneIdx = operand.m_nCurrentResultSceneIdx;

	return *this;
}

string CSKFaceAnnotationResult::GetResultData(bool& bAbnormalEnd)
{
	/* Old
	<mamex_request>
	  <ex_update_jobresult>
		<workinfoxml>      
		</workinfoxml>
		<assetinfo type="recognizefaceinfo">
		  <recognizefaceinfo>
			<assetid>81184</assetid>        
			<startframeindex>74578</startframeindex>
			<endframeindex>74609</endframeindex>        
			<people>
			  <person>MP0000000549</person>
			  <person>MP0000000567</person>
			</people>
		  </recognizefaceinfo>
		</assetinfo>
	  </ex_update_jobresult>
	</mamex_request>
	*/

	/* New
	<mamex_request>
	  <ex_update_jobresult>
		<workinfoxml>      
		</workinfoxml>
		<assetinfo type="recognizefaceinfo">
		  <recognizefaceinfo>
			<assetid>81184</assetid>        
			<frameindex>74578</frameindex>   
			<people>
			  <person>
				<score>0.9587138690283279</score>
				<rect>710, 22, 255, 331</rect>
				<id>MP0000000549</id>
			  </person>
			  <person>
				<score>0.9587138690283279</score>
				<rect>710, 22, 255, 331</rect>
				<id>MP0000000567</id>
			  </person>
			</people>
		  </recognizefaceinfo>
		</assetinfo>
	  </ex_update_jobresult>
	</mamex_request>
	*/

	string stdstrRet;

	/* Old */
	//stdstrRet = GetXmlResultFrameRangeData();

	/* New */
	stdstrRet = GetXmlReportCharsData();
	bAbnormalEnd = m_bAbort;
	
	return stdstrRet;
}

int CSKFaceAnnotationResult::LoadIdentifyDataFile()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	m_nReportCount = m_nDataCountPerReport;

	try {
		Clear();

		m_nIdentifyDataCount = 0;
		m_nCurrentResultSceneIdx = 0;

		CGenerateSceneCharIdentificationJobInfo* pScenCharIdentifyJob = NULL;
		pScenCharIdentifyJob = (CGenerateSceneCharIdentificationJobInfo*)m_pIdentifyJobInfo;
		if (pScenCharIdentifyJob)
		{
			if (m_pIdentifyResultJsonData)
			{
				delete (json::value*)m_pIdentifyResultJsonData;
				m_pIdentifyResultJsonData = NULL;
			}

			m_pIdentifyResultJsonData = (void*)(new json::value);
			json::value* pJsonData = (json::value*)m_pIdentifyResultJsonData;

			string stdstrJsonPath = pScenCharIdentifyJob->m_stdstrWorkingDirPath + "/" + pScenCharIdentifyJob->m_stdstrTitle + ".json";

			CString strJsonPath;
			Utility::CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrJsonPath.c_str(), strJsonPath);

			//string_t        strImportFile = utility::conversions::utf8_to_utf16(stdstrJsonPath);     // extract filename
			//ifstream_t      fstream(strImportFile);                     // filestream of working file
			//stringstream_t    strStream;                                  // string stream for holding JSON read from file
			string			strImportFile((char*)strJsonPath.GetBuffer());// extract filename
			ifstream		fstream(strImportFile);                     // filestream of working file
			stringstream    strStream;                                  // string stream for holding JSON read from file

			strJsonPath.ReleaseBuffer();

			//json::value     jsonData;                                   // JSON read from input file

			if (fstream)
			{
				strStream << fstream.rdbuf();                           // stream results of reading from file stream into string stream
				fstream.close();                                        // close the filestream

				*pJsonData = json::value::parse(strStream);               // parse the resultant string stream.

				unsigned int uThreadID = 0;
				uintptr_t pThread = _beginthreadex(NULL, THREAD_STACK_SIZE, AddIdentifyDataProc, (void*)this, 0, &uThreadID);

				if (!pThread)
				{
					nRet = m_nError = RESTSKSERVICE_ERROR_RESULTPROCESS;

					string_t strMsg = L"Failed to create thread - character identification";
					utility::conversions::utf16_to_utf8(strMsg);

					m_stdstrErrMsg.clear();
					m_stdstrErrMsg.assign(utility::conversions::utf16_to_utf8(strMsg));
				}
				else
					Sleep(1500);
			}
			else
			{
				nRet = m_nError = RESTSKSERVICE_ERROR_FILE_IO;

				string_t strMsg = L"Failed to load file stream. - " + utility::conversions::utf8_to_utf16(stdstrJsonPath);
				//utility::conversions::utf16_to_utf8(strMsg);

				if (!Utility::IsFile(strJsonPath))
					strMsg = L"No exist file stream - " + utility::conversions::utf8_to_utf16(stdstrJsonPath);

				utility::conversions::utf16_to_utf8(strMsg);

				m_stdstrErrMsg.clear();
				m_stdstrErrMsg.assign(utility::conversions::utf16_to_utf8(strMsg));
			}
		}
	}
	catch (web::json::json_exception e)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}

	return nRet;
}

void CSKFaceAnnotationResult::Clear()
{
	m_llSceneID = 0;
	m_llAssetID = 0;

	m_bEndResultData = false;
	m_bReportEndData = false;

	m_mapFrameRangeCharIdentifyData.clear();
	m_mapLastFrameRangeCharIdentifyData.clear();

	g_mutexCharIdentifyData.lock();
	//std::queue<map<pair<int, string>, vector<CCharacterIdentifyData>>> empty;
	//std::swap(m_queueFrameCharIdentifyDataQueue, empty);

	int nCount = m_queueFrameCharIdentifyDataQueue.size();
	for (int i = 0; i < nCount; i++)
	{
		m_queueFrameCharIdentifyDataQueue.pop();
	}

	g_mutexCharIdentifyData.unlock();

	if (m_pstdstrJobInfo)
	{
		m_pstdstrJobInfo->clear();
	}
}

void CSKFaceAnnotationResult::IdentifyDataOperationProc()
{
	m_bWorkingResult = true;

	////////////// face identification result json ////////////////////////////////////////
	/*
	{
		"jobid": "1111",
		"title": "NowYouSeeMe",
		"scenes" : [{
			"id": 1,
			"frames":[{
				"frameindex": 0,
				"framepath" : "/frames/NowYouSeeMe_/0.jpg",
				"characters" : [{
					"conf": 0.9722898937539345,
					"id" : "",
					"rect" : {
						"y": 22,
						"x" : 710,
						"w" : 255,
						"h" : 331
					},
					"name": "WoodyHarrelson"
				}]
			},
			{
				"frameindex": 10,
				"framepath" : "/frames/NowYouSeeMe_/10.jpg",
				"characters" : [{
					"conf": 0.9587138690283279,
					"id" : "",
					"rect" : {
						"y": 17,
						"x" : 714,
						"w" : 267,
						"h" : 344
					},
					"name": "WoodyHarrelson"
				}]
			}]
		}]
	}
	*/
	////////////// face identification result json ////////////////////////////////////////

	try 
	{
		if (m_pIdentifyJobInfo)
			m_llAssetID = ((CGenerateSceneCharIdentificationJobInfo*)m_pIdentifyJobInfo)->m_llAssetID;

		if (m_pstdstrJobInfo)
		{
			*m_pstdstrJobInfo = *(((CGenerateSceneCharIdentificationJobInfo*)m_pIdentifyJobInfo)->m_pstdstrJobInfo);
		}

		json::value* pJsonData = (json::value*)m_pIdentifyResultJsonData;

		if (pJsonData)
		{
			utility::string_t strKey;
			strKey = L"scenes";

			if (pJsonData->has_field(strKey))
			{
				json::array arrScenResult = pJsonData->at(strKey).as_array();
				int nSecenCount = (int)arrScenResult.size();

				__int64 llSecneID = 0;

				int i = m_nCurrentResultSceneIdx;
				for (i; i < nSecenCount; i++)
				{
					if (m_bAbort)
						break;

					strKey = L"id";
					if (arrScenResult[i].has_field(strKey))
					{
						llSecneID = arrScenResult[i].at(strKey).as_number().to_int64();
					}

					int nFrameIdex = 0;
					string stdstrFramePath = "";

					strKey = L"frames";
					if (arrScenResult[i].has_field(strKey))
					{
						vector<CCharacterIdentifyData> vecCharacterIdentifyData;

						json::array arrFrameResult = arrScenResult[i].at(strKey).as_array();

						int nFrameCount = (int)arrFrameResult.size();
						
						int j = 0;
						for (j; j < nFrameCount; j++)
						{
							if (m_bAbort)
								break;

							strKey = L"frameindex";
							if (arrFrameResult[j].has_field(strKey))
								nFrameIdex = arrFrameResult[j].at(strKey).as_integer();

							strKey = L"framepath";
							if (arrFrameResult[j].has_field(strKey))
								stdstrFramePath = utility::conversions::utf16_to_utf8(arrFrameResult[j].at(strKey).as_string());

							pair<int, string> pairFrameIndexNPath;
							pairFrameIndexNPath.first = nFrameIdex;
							pairFrameIndexNPath.second = stdstrFramePath;

							if (m_nError = GetFrameCharIdentifyResultData(&arrFrameResult[j], vecCharacterIdentifyData))
							{
								web::json::json_exception e(L"Failed to get frame character identification result data");
								throw e;
							}

							if ((int)vecCharacterIdentifyData.size() > 0)
							{
								m_llSceneID = llSecneID;
									
								map<pair<int, string>, vector<CCharacterIdentifyData>> mapFrameCharIdentifyData;
								mapFrameCharIdentifyData[pairFrameIndexNPath] = vecCharacterIdentifyData;

								while (1)
								{
									if (m_bAbort)
										break;

									g_mutexCharIdentifyData.lock();

									if ((int)m_queueFrameCharIdentifyDataQueue.size() < m_nDataCountPerReport)
									{
										m_queueFrameCharIdentifyDataQueue.push(mapFrameCharIdentifyData);

										g_mutexCharIdentifyData.unlock();

										break;
									}

									g_mutexCharIdentifyData.unlock();

									Sleep(500);
								}
							}
						}
					}
				}
				
				m_bEndResultData = true;
			}
		}
	}
	catch (web::json::json_exception e)
	{
		if (m_nError != RESTSKSERVICE_ERROR_INVALID_JSON)
			m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());

		m_bAbort = true;
	}

	m_bWorkingResult = false;
}

string CSKFaceAnnotationResult::GetXmlResultCharData()
{
	string stdstrRet;

	int nCount = (int)m_mapFrameRangeCharIdentifyData.size();
	if (nCount > 0)
	{
		string stdstrCharacterIdentify;
		map<pair<int, int>, vector<CCharacterIdentifyData>>::iterator it = m_mapFrameRangeCharIdentifyData.begin();
		
		for (it; it != m_mapFrameRangeCharIdentifyData.end(); it++)
		{
			pair<int, int> pairFrameRangeIndex = it->first;
			vector<CCharacterIdentifyData> vecCharIdentifyData = it->second;

			string stdstrCharacterIdentify;
			int nVecCount = (int)vecCharIdentifyData.size();
			if (nVecCount > 0)
			{
				for (int i = 0; i < nVecCount; i++)
				{
					CCharacterIdentifyData CharIdentifyData = vecCharIdentifyData[i];
					stdstrCharacterIdentify = stdstrCharacterIdentify + Utility::string_format("<person>%s</person>"
						, CharIdentifyData.m_stdstrCharName.c_str());
				}
			}

			stdstrRet = stdstrRet + Utility::string_format("<recognizefaceinfo><assetid>%I64d</assetid><startframeindex>%d</startframeindex>\
						<endframeindex>%d</endframeindex><people>%s</people></recognizefaceinfo>", m_llAssetID, pairFrameRangeIndex.first
						, pairFrameRangeIndex.second, stdstrCharacterIdentify.c_str());
		}

		m_mapFrameRangeCharIdentifyData.clear();
	}

	return stdstrRet;
}

string CSKFaceAnnotationResult::GetXmlResultFrameCharsData()
{
	string stdstrRet;

	int nCount = (int)m_mapFrameRangeCharIdentifyData.size();
	if (nCount > 0)
	{
		string stdstrCharacterIdentify;
		map<pair<int, int>, vector<CCharacterIdentifyData>>::iterator it = m_mapFrameRangeCharIdentifyData.begin();

		for (it; it != m_mapFrameRangeCharIdentifyData.end(); it++)
		{
			pair<int, int> pairFrameRangeIndex = it->first;
			vector<CCharacterIdentifyData> vecCharIdentifyData = it->second;

			string stdstrCharacterIdentify;
			int nVecCount = (int)vecCharIdentifyData.size();
			if (nVecCount > 0)
			{
				//<person>
				//	<score>0.9587138690283279</score>
				//	<rect>710, 22, 255, 331</rect>
				//	<id>MP0000000549</id>
				//</person>
				for (int i = 0; i < nVecCount; i++)
				{
					CCharacterIdentifyData CharIdentifyData = vecCharIdentifyData[i];
					stdstrCharacterIdentify = stdstrCharacterIdentify + Utility::string_format("<person><score>%f</score><rect>%d, %d, %d, %d</rect>\
						<id>%s</id></person>", CharIdentifyData.m_dbConfidence, CharIdentifyData.m_nPositionX, CharIdentifyData.m_nPositionY
						, CharIdentifyData.m_nPositionWidth, CharIdentifyData.m_nPositionHeight, CharIdentifyData.m_stdstrCharName.c_str());
				}
			}

			stdstrRet = stdstrRet + Utility::string_format("<recognizefaceinfo><assetid>%I64d</assetid><frameindex>%d</frameindex>\
														  <people>%s</people></recognizefaceinfo>", m_llAssetID, pairFrameRangeIndex.first
														  , stdstrCharacterIdentify.c_str());
		}

		m_mapFrameRangeCharIdentifyData.clear();
	}

	return stdstrRet;
}

string CSKFaceAnnotationResult::GetXmlReportCharsData()
{
	string stdstrRet;
	string stdstrFrameCharsAnnotation;

	int nCount = (int)m_queueFrameCharIdentifyDataQueue.size();
	if (nCount > 0)
	{
		if ((nCount >= m_nReportCount) || (nCount < m_nReportCount && m_bEndResultData))
		{
			g_mutexCharIdentifyData.lock();

			for (int i = 0; i < nCount; i++)
			{
				map<pair<int, string>, vector<CCharacterIdentifyData>> mapFrameCharIdentifyData = m_queueFrameCharIdentifyDataQueue.front();
				m_queueFrameCharIdentifyDataQueue.pop();

				pair<int, string> pairFrameIndexNPath;
				pair<int, int> pairFrameRangeIndex;
				vector<CCharacterIdentifyData> vecCharIdentifyData, vecCharIdentifyDataOld;

				map<pair<int, string>, vector<CCharacterIdentifyData>>::iterator it = mapFrameCharIdentifyData.begin();
				for (it; it != mapFrameCharIdentifyData.end(); it++)
				{
					pairFrameIndexNPath = it->first;
					vecCharIdentifyData = it->second;

					pairFrameRangeIndex.first = pairFrameIndexNPath.first;
					pairFrameRangeIndex.second = pairFrameIndexNPath.first;

					m_mapFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyData;

					if (((int)m_mapFrameRangeCharIdentifyData.size() >= m_nReportCount))
					{
						stdstrFrameCharsAnnotation = GetXmlResultFrameCharsData();

						stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
														   <assetinfo type = \"recognizefaceinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
														   , m_pstdstrJobInfo->c_str(), stdstrFrameCharsAnnotation.c_str());
						break;
					}
				}
			}

			g_mutexCharIdentifyData.unlock();
		}
	}
	else if (m_bEndResultData)
	{
		if ((int)m_mapFrameRangeCharIdentifyData.size() > 0)
		{
			stdstrFrameCharsAnnotation = GetXmlResultFrameCharsData();

			stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
											   <assetinfo type = \"recognizefaceinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
											   , m_pstdstrJobInfo->c_str(), stdstrFrameCharsAnnotation.c_str());
		}

		m_bReportEndData = true;
	}

	return stdstrRet;
}

string CSKFaceAnnotationResult::GetXmlResultFrameRangeData()
{
	string stdstrRet;
	string stdstrFrameRangeAnnotation;

	int nCount = (int)m_queueFrameCharIdentifyDataQueue.size();
	if (nCount > 0)
	{
		if ((nCount >= m_nReportCount) || (nCount < m_nReportCount && m_bEndResultData))
		{
			g_mutexCharIdentifyData.lock();

			for (int i = 0; i < nCount; i++)
			{
				map<pair<int, string>, vector<CCharacterIdentifyData>> mapFrameCharIdentifyData = m_queueFrameCharIdentifyDataQueue.front();
				m_queueFrameCharIdentifyDataQueue.pop();

				pair<int, string> pairFrameIndexNPath;
				pair<int, int> pairFrameRangeIndex;
				vector<CCharacterIdentifyData> vecCharIdentifyData, vecCharIdentifyDataOld;

				map<pair<int, string>, vector<CCharacterIdentifyData>>::iterator it = mapFrameCharIdentifyData.begin();
				for (it; it != mapFrameCharIdentifyData.end(); it++)
				{
					pairFrameIndexNPath = it->first;
					vecCharIdentifyData = it->second;

					if (m_mapLastFrameRangeCharIdentifyData.size() == 0)
					{
						pairFrameRangeIndex.first = pairFrameIndexNPath.first;
						pairFrameRangeIndex.second = pairFrameIndexNPath.first;

						m_mapLastFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyData;

						continue;
					}

					map<pair<int, int>, vector<CCharacterIdentifyData>>::iterator it = m_mapLastFrameRangeCharIdentifyData.begin();
					pairFrameRangeIndex = it->first;
					vecCharIdentifyDataOld = it->second;

					if (pairFrameRangeIndex.second + m_nFramePeriod == pairFrameIndexNPath.first)
					{
						if (!CompareCharIdentification(vecCharIdentifyDataOld, vecCharIdentifyData))
						{
							pairFrameRangeIndex.second = pairFrameIndexNPath.first;
							m_mapLastFrameRangeCharIdentifyData.clear();
							m_mapLastFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyDataOld;

							continue;
						}
					}

					m_mapFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyDataOld;

					pairFrameRangeIndex.first = pairFrameIndexNPath.first;
					pairFrameRangeIndex.second = pairFrameIndexNPath.first;

					m_mapLastFrameRangeCharIdentifyData.clear();
					m_mapLastFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyData;

					if (((int)m_mapFrameRangeCharIdentifyData.size() >= m_nReportCount))
					{
						stdstrFrameRangeAnnotation = GetXmlResultCharData();

						stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
														   								<assetinfo type = \"recognizefaceinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
																						, m_pstdstrJobInfo->c_str(), stdstrFrameRangeAnnotation.c_str());
						break;
					}
				}
			}

			g_mutexCharIdentifyData.unlock();
		}
	}
	else if (m_bEndResultData)
	{
		if (m_mapLastFrameRangeCharIdentifyData.size() > 0)
		{
			map<pair<int, int>, vector<CCharacterIdentifyData>>::iterator it = m_mapLastFrameRangeCharIdentifyData.begin();
			pair<int, int> pairFrameRangeIndex = it->first;
			vector<CCharacterIdentifyData> vecCharIdentifyData = it->second;

			m_mapFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyData;
		}

		if ((int)m_mapFrameRangeCharIdentifyData.size() > 0)
		{
			stdstrFrameRangeAnnotation = GetXmlResultCharData();

			stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
						<assetinfo type = \"recognizefaceinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
						, m_pstdstrJobInfo->c_str(), stdstrFrameRangeAnnotation.c_str());
		}

		m_bReportEndData = true;
	}

	return stdstrRet;
}

bool CSKFaceAnnotationResult::CompareCharIdentification(vector<CCharacterIdentifyData>& vecCharIdentifyDataOld
	, vector<CCharacterIdentifyData>& vecCharIdentifyData)
{
	bool bRet = false;

	int nOldCount = (int)vecCharIdentifyDataOld.size();
	int nCount = (int)vecCharIdentifyData.size();

	if (nOldCount != nCount)
	{
		bRet = true;

		return bRet;
	}

	CCharacterIdentifyData Character, CharacterOld;
	string stdstrCharacter, stdstrCharacterOld;

	for (int i = 0; i < nCount; i++)
	{
		Character = vecCharIdentifyData[i];

		int j = 0;
		for (j = 0; j < nOldCount; j++)
		{
			CharacterOld = vecCharIdentifyDataOld[j];

			if (!Character.m_stdstrCharName.compare(CharacterOld.m_stdstrCharName))
				break;
		}

		if (j >= nOldCount)
		{
			bRet = false;

			break;
		}
	}

	return bRet;
}

int CSKFaceAnnotationResult::GetFrameCharIdentifyResultData(void* pJsonFrameData, vector<CCharacterIdentifyData>& retFrameCharIdentifyData)
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	retFrameCharIdentifyData.clear();

	try {
		json::value* pJsonData = (json::value*)pJsonFrameData;

		if (pJsonData)
		{
			utility::string_t strKey;

			strKey = L"characters";
			if (pJsonData->has_field(strKey))
			{
				CCharacterIdentifyData CharacterIdentifyData;

				json::array arrCharaterResult = pJsonData->at(strKey).as_array();

				int nCharResultCount = (int)arrCharaterResult.size();
				for (int i = 0; i < nCharResultCount; i++)
				{
					strKey = L"id";
					if (arrCharaterResult[i].has_field(strKey))
					{
						utility::string_t strCharID = arrCharaterResult[i].at(strKey).as_string();
						CharacterIdentifyData.m_llCharID = _ttoi64((TCHAR*)strCharID.c_str());
					}

					strKey = L"conf";
					if (arrCharaterResult[i].has_field(strKey))
						CharacterIdentifyData.m_dbConfidence = arrCharaterResult[i].at(strKey).as_double();

					strKey = L"rect";
					if (arrCharaterResult[i].has_field(strKey))
					{
						json::value jsonRectData = arrCharaterResult[i].at(strKey);

						strKey = L"x";
						if (jsonRectData.has_field(strKey))
							CharacterIdentifyData.m_nPositionX = jsonRectData.at(strKey).as_integer();

						strKey = L"y";
						if (jsonRectData.has_field(strKey))
							CharacterIdentifyData.m_nPositionY = jsonRectData.at(strKey).as_integer();

						strKey = L"w";
						if (jsonRectData.has_field(strKey))
							CharacterIdentifyData.m_nPositionWidth = jsonRectData.at(strKey).as_integer();

						strKey = L"h";
						if (jsonRectData.has_field(strKey))
							CharacterIdentifyData.m_nPositionHeight = jsonRectData.at(strKey).as_integer();
					}

					strKey = L"name";
					if (arrCharaterResult[i].has_field(strKey))
						CharacterIdentifyData.m_stdstrCharName = utility::conversions::utf16_to_utf8(arrCharaterResult[i].at(strKey).as_string());

					retFrameCharIdentifyData.push_back(CharacterIdentifyData);
				}
			}
		}
	}
	catch (web::json::json_exception e)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}

	return nRet;
}






CSKFFaceAnnotationResult::CSKFFaceAnnotationResult()
{
	m_llSceneID = 0;

	m_pstdstrJobInfo = new string;

	m_llAssetID = 0;
	m_bEndResultData = false;
	m_bReportEndData = false;

	m_nFramePeriod = RESTSKSERVICE_FACE_IDENTIFY_INPTU_FRAME_PERIOD;
	m_nReportCount = RESTSKSERVICE_IDENTIFY_RESULT_BATCH_COUNT;
}

CSKFFaceAnnotationResult::~CSKFFaceAnnotationResult()
{
	m_bEndResultData = true;

	m_mapFrameRangeCharIdentifyData.clear();
	m_mapLastFrameRangeCharIdentifyData.clear();

	g_mutexCharIdentifyData.lock();
	std::queue<map<pair<int, string>, vector<CCharacterIdentifyData>>> empty;
	std::swap(m_queueFrameCharIdentifyDataQueue, empty);
	g_mutexCharIdentifyData.unlock();

	if (m_pstdstrJobInfo)
	{
		m_pstdstrJobInfo->clear();

		delete m_pstdstrJobInfo;
		m_pstdstrJobInfo = NULL;
	}
}

CSKFFaceAnnotationResult::CSKFFaceAnnotationResult(const CSKFFaceAnnotationResult& operand)
{
	m_llSceneID = operand.m_llSceneID;
	
	m_mapFrameRangeCharIdentifyData = operand.m_mapFrameRangeCharIdentifyData;
	m_mapLastFrameRangeCharIdentifyData = operand.m_mapLastFrameRangeCharIdentifyData;
	m_queueFrameCharIdentifyDataQueue = operand.m_queueFrameCharIdentifyDataQueue;
	
	m_llAssetID = operand.m_llAssetID;
	m_bEndResultData = operand.m_bEndResultData;
	m_bReportEndData = operand.m_bReportEndData;

	if (m_pstdstrJobInfo && operand.m_pstdstrJobInfo)
	{
		*m_pstdstrJobInfo = *operand.m_pstdstrJobInfo;
	}
}

CSKFFaceAnnotationResult CSKFFaceAnnotationResult::operator=(const CSKFFaceAnnotationResult & operand)
{
	m_llSceneID = operand.m_llSceneID;

	m_mapFrameRangeCharIdentifyData = operand.m_mapFrameRangeCharIdentifyData;
	m_mapLastFrameRangeCharIdentifyData = operand.m_mapLastFrameRangeCharIdentifyData;
	m_queueFrameCharIdentifyDataQueue = operand.m_queueFrameCharIdentifyDataQueue;

	m_llAssetID = operand.m_llAssetID;
	m_bEndResultData = operand.m_bEndResultData;
	m_bReportEndData = operand.m_bReportEndData;

	if (m_pstdstrJobInfo && operand.m_pstdstrJobInfo)
	{
		*m_pstdstrJobInfo = *operand.m_pstdstrJobInfo;
	}

	return *this;
}

string CSKFFaceAnnotationResult::GetXmlResultData()
{
	/* New
	<mamex_request>
	  <ex_update_jobresult>
		<workinfoxml>      
		</workinfoxml>
		<assetinfo type="recognizefaceinfo">
		  <recognizefaceinfo>
			<assetid>81184</assetid>        
			<startframeindex>74578</startframeindex>
			<endframeindex>74609</endframeindex>        
			<people>
			  <person>MP0000000549</person>
			  <person>MP0000000567</person>
			</people>
		  </recognizefaceinfo>
		</assetinfo>
	  </ex_update_jobresult>
	</mamex_request>
	*/

	string stdstrRet;// , stdstrFrameAnnotation;

	stdstrRet = GetXmlResultFrameRangeData();

	//if (stdstrFrameAnnotation.empty())
	//	return stdstrRet;

	//stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml><sceneannotationinfo><sceneid>%I64d</sceneid>%s</sceneannotationinfo>\
	//			</ex_update_jobresult></mamex_request>", m_llSceneID, stdstrFrameAnnotation.c_str());

	//stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><sceneannotationinfo><sceneid>%I64d</sceneid>%s</sceneannotationinfo>\
	//			</ex_update_jobresult></mamex_request>", m_llSceneID, stdstrFrameAnnotation.c_str());

	
	return stdstrRet;
}

string CSKFFaceAnnotationResult::GetXmlResultCharData()
{
	string stdstrRet;

	int nCount = (int)m_mapFrameRangeCharIdentifyData.size();
	if (nCount > 0)
	{
		string stdstrCharacterIdentify;
		map<pair<int, int>, vector<CCharacterIdentifyData>>::iterator it = m_mapFrameRangeCharIdentifyData.begin();
		
		for (it; it != m_mapFrameRangeCharIdentifyData.end(); it++)
		{
			pair<int, int> pairFrameRangeIndex = it->first;
			vector<CCharacterIdentifyData> vecCharIdentifyData = it->second;

			string stdstrCharacterIdentify;
			int nVecCount = (int)vecCharIdentifyData.size();
			if (nVecCount > 0)
			{
				for (int i = 0; i < nVecCount; i++)
				{
					CCharacterIdentifyData CharIdentifyData = vecCharIdentifyData[i];
					stdstrCharacterIdentify = stdstrCharacterIdentify + Utility::string_format("<person>%s</person>"
						, CharIdentifyData.m_stdstrCharName.c_str());
				}
			}

			stdstrRet = stdstrRet + Utility::string_format("<recognizefaceinfo><assetid>%I64d</assetid><startframeindex>%d</startframeindex>\
						<endframeindex>%d</endframeindex><people>%s</people></recognizefaceinfo>", m_llAssetID, pairFrameRangeIndex.first
						, pairFrameRangeIndex.second, stdstrCharacterIdentify.c_str());
		}

		m_mapFrameRangeCharIdentifyData.clear();
	}

	//int nCount = (int)vecCharIdentifyData.size();
	//if (nCount > 0)
	//{
	//	string stdstrCharacterIdentify;
	//	for (int i = 0; i < nCount; i++)
	//	{
	//		CCharacterIdentifyData CharIdentifyData = vecCharIdentifyData[i];
	//		stdstrCharacterIdentify = Utility::string_format("<character><conf>%.20f</conf><id>%I64d</id><rect><x>%d</x><y>%d</y><w>%d</w><h>%d</h>\
	//								</rect><name>%s</name></character>", CharIdentifyData.m_dbConfidence, CharIdentifyData.m_llCharID
	//								, CharIdentifyData.m_nPositionX, CharIdentifyData.m_nPositionY, CharIdentifyData.m_nPositionWidth
	//								, CharIdentifyData.m_nPositionHeight, CharIdentifyData.m_stdstrCharName.c_str());

	//		//printf("%.20f", CharIdentifyData.m_dbConfidence);
	//		stdstrRet = stdstrRet + stdstrCharacterIdentify;
	//	}
	//}

	return stdstrRet;
}

bool CSKFFaceAnnotationResult::CompareCharIdentification(vector<CCharacterIdentifyData>& vecCharIdentifyDataOld
	, vector<CCharacterIdentifyData>& vecCharIdentifyData)
{
	bool bRet = false;

	int nOldCount = (int)vecCharIdentifyDataOld.size();
	int nCount = (int)vecCharIdentifyData.size();

	if (nOldCount != nCount)
	{
		bRet = true;

		return bRet;
	}

	CCharacterIdentifyData Character, CharacterOld;
	string stdstrCharacter, stdstrCharacterOld;
	
	for (int i = 0; i < nCount; i++)
	{
		Character = vecCharIdentifyData[i];

		int j = 0;
		for (j = 0; j < nOldCount; j++)
		{
			CharacterOld = vecCharIdentifyDataOld[j];

			if (!Character.m_stdstrCharName.compare(CharacterOld.m_stdstrCharName))
				break;
		}

		if (j >= nOldCount)
		{
			bRet = false;

			break;
		}
	}

	return bRet;
}

string CSKFFaceAnnotationResult::GetXmlResultFrameRangeData()
{
	string stdstrRet;
	string stdstrFrameRangeAnnotation;

	int nCount = (int)m_queueFrameCharIdentifyDataQueue.size();
	if (nCount > 0)
	{
		if ((nCount >= m_nReportCount) || (nCount < m_nReportCount && m_bEndResultData))
		{
			g_mutexCharIdentifyData.lock();

			for (int i = 0; i < nCount; i++)
			{
				map<pair<int, string>, vector<CCharacterIdentifyData>> mapFrameCharIdentifyData = m_queueFrameCharIdentifyDataQueue.front();
				m_queueFrameCharIdentifyDataQueue.pop();

				pair<int, string> pairFrameIndexNPath;
				pair<int, int> pairFrameRangeIndex;
				vector<CCharacterIdentifyData> vecCharIdentifyData, vecCharIdentifyDataOld;

				map<pair<int, string>, vector<CCharacterIdentifyData>>::iterator it = mapFrameCharIdentifyData.begin();
				for (it; it != mapFrameCharIdentifyData.end(); it++)
				{
					pairFrameIndexNPath = it->first;
					vecCharIdentifyData = it->second;

					if (m_mapLastFrameRangeCharIdentifyData.size() == 0)
					{
						pairFrameRangeIndex.first = pairFrameIndexNPath.first;
						pairFrameRangeIndex.second = pairFrameIndexNPath.first;

						m_mapLastFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyData;

						continue;
					}

					map<pair<int, int>, vector<CCharacterIdentifyData>>::iterator it = m_mapLastFrameRangeCharIdentifyData.begin();
					pairFrameRangeIndex = it->first;
					vecCharIdentifyDataOld = it->second;

					if (pairFrameRangeIndex.second + m_nFramePeriod == pairFrameIndexNPath.first)
					{
						if (!CompareCharIdentification(vecCharIdentifyDataOld, vecCharIdentifyData))
						{
							pairFrameRangeIndex.second = pairFrameIndexNPath.first;
							m_mapLastFrameRangeCharIdentifyData.clear();
							m_mapLastFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyDataOld;

							continue;
						}
					}

					m_mapFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyDataOld;

					pairFrameRangeIndex.first = pairFrameIndexNPath.first;
					pairFrameRangeIndex.second = pairFrameIndexNPath.first;

					m_mapLastFrameRangeCharIdentifyData.clear();
					m_mapLastFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyData;

					if (((int)m_mapFrameRangeCharIdentifyData.size() >= m_nReportCount))
					{
						stdstrFrameRangeAnnotation = GetXmlResultCharData();

						stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
														   								<assetinfo type = \"recognizefaceinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
																						, m_pstdstrJobInfo->c_str(), stdstrFrameRangeAnnotation.c_str());
						break;
					}
				}
			}

			g_mutexCharIdentifyData.unlock();
		}
	}
	else if (m_bEndResultData)
	{
		if (m_mapLastFrameRangeCharIdentifyData.size() > 0)
		{
			map<pair<int, int>, vector<CCharacterIdentifyData>>::iterator it = m_mapLastFrameRangeCharIdentifyData.begin();
			pair<int, int> pairFrameRangeIndex = it->first;
			vector<CCharacterIdentifyData> vecCharIdentifyData = it->second;

			m_mapFrameRangeCharIdentifyData[pairFrameRangeIndex] = vecCharIdentifyData;
		}

		if ((int)m_mapFrameRangeCharIdentifyData.size() > 0)
		{
			stdstrFrameRangeAnnotation = GetXmlResultCharData();

			stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
						<assetinfo type = \"recognizefaceinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
						, m_pstdstrJobInfo->c_str(), stdstrFrameRangeAnnotation.c_str());
		}

		m_bReportEndData = true;

		m_llAssetID = 0;
	}

	//int nCount = (int)m_mapFrameCharIdentifyData.size();
	//if (nCount > 0)
	//{
	//	string stdstrFrameAnnotation;
	//	string stdstrCharacterIdentify;
	//	pair<int, string> pairFrameIndexNPath;
	//	vector<CCharacterIdentifyData> vecCharIdentifyData;

	//	map<pair<int, string>, vector<CCharacterIdentifyData>>::iterator it = m_mapFrameCharIdentifyData.begin();
	//	for (it; it != m_mapFrameCharIdentifyData.end(); it++)
	//	{
	//		pairFrameIndexNPath = it->first;
	//		vecCharIdentifyData = it->second;

	//		stdstrCharacterIdentify = GetXmlResultCharData(vecCharIdentifyData);

	//		stdstrFrameAnnotation = Utility::string_format("<frame><index>%d</index><path>%s</path>%s</frame>", pairFrameIndexNPath.first
	//			, pairFrameIndexNPath.second.c_str(), stdstrCharacterIdentify.c_str());

	//		stdstrRet = stdstrRet + stdstrFrameAnnotation;
	//	}
	//}

	return stdstrRet;
}

void CSKFFaceAnnotationResult::Clear()
{
	m_llSceneID = 0;
	m_llAssetID = 0;
	
	m_bEndResultData = false;
	m_bReportEndData = false;

	m_mapFrameRangeCharIdentifyData.clear();
	m_mapLastFrameRangeCharIdentifyData.clear();

	g_mutexCharIdentifyData.lock();
	std::queue<map<pair<int, string>, vector<CCharacterIdentifyData>>> empty;
	std::swap(m_queueFrameCharIdentifyDataQueue, empty);
	g_mutexCharIdentifyData.unlock();

	if (m_pstdstrJobInfo)
	{
		m_pstdstrJobInfo->clear();
	}
}

CObjectEtcIdentifyData::CObjectEtcIdentifyData()
{

}

CObjectEtcIdentifyData::~CObjectEtcIdentifyData()
{

}

CObjectEtcIdentifyData::CObjectEtcIdentifyData(const CObjectEtcIdentifyData& operand)
{
	m_stdstrClass = operand.m_stdstrClass;
	m_dbScore = operand.m_dbScore;
}

CObjectEtcIdentifyData CObjectEtcIdentifyData::operator=(const CObjectEtcIdentifyData& operand)
{
	m_stdstrClass = operand.m_stdstrClass;
	m_dbScore = operand.m_dbScore;

	return *this;
}

CObjectIdentifyData::CObjectIdentifyData()
{

}

CObjectIdentifyData::~CObjectIdentifyData()
{

}

CObjectIdentifyData::CObjectIdentifyData(const CObjectIdentifyData& operand)
{
	CObjectEtcIdentifyData::operator=(operand);

	m_nPositionX = operand.m_nPositionX;
	m_nPositionY = operand.m_nPositionY;
	m_nPositionWidth = operand.m_nPositionWidth;
	m_nPositionHeight = operand.m_nPositionHeight;
}

CObjectIdentifyData CObjectIdentifyData::operator=(const CObjectIdentifyData& operand)
{
	CObjectEtcIdentifyData::operator=(operand);

	m_nPositionX = operand.m_nPositionX;
	m_nPositionY = operand.m_nPositionY;
	m_nPositionWidth = operand.m_nPositionWidth;
	m_nPositionHeight = operand.m_nPositionHeight;

	return *this;
}

CSKObjectEtcAnnotationResult::CSKObjectEtcAnnotationResult()
{
	m_stdstrAnnotationType = RESTSKSERVICE_ANNOTATE_IDENTIFY_OBJECT_ETC;

	m_llAssetID = 0;
	m_bEndResultData = false;
	m_bReportEndData = false;

	//m_nFramePeriod = RESTSKSERVICE_FACE_IDENTIFY_INPTU_FRAME_PERIOD;
	m_nReportCount = RESTSKSERVICE_IDENTIFY_RESULT_BATCH_COUNT;
}

CSKObjectEtcAnnotationResult::~CSKObjectEtcAnnotationResult()
{
	m_bEndResultData = true;

	m_vecVideoData.clear();

	m_mapObjectData.clear();
	m_mapPlaceData.clear();
	m_mapLandmarkData.clear();
	m_mapEventData.clear();

	g_mutexObjectEtcIdentifyData.lock();
	//std::deque<pair<int, int>> empty;
	//std::swap(m_queueFrameObjectEtcdentifyDataQueue, empty);

	int nCount = m_queueFrameObjectEtcdentifyDataQueue.size();
	for (int i = 0; i < nCount; i++)
	{
		m_queueFrameObjectEtcdentifyDataQueue.pop_front();
	}

	g_mutexObjectEtcIdentifyData.unlock();
}

CSKObjectEtcAnnotationResult::CSKObjectEtcAnnotationResult(const CSKObjectEtcAnnotationResult& operand)
{
	CSKAnnotationResult::operator=(operand);

	m_nReportCount = operand.m_nReportCount;

	m_vecVideoData = operand.m_vecVideoData;
	m_mapObjectData = operand.m_mapObjectData;
	m_mapPlaceData = operand.m_mapPlaceData;
	m_mapLandmarkData = operand.m_mapLandmarkData;
	m_mapEventData = operand.m_mapEventData;
	m_queueFrameObjectEtcdentifyDataQueue = operand.m_queueFrameObjectEtcdentifyDataQueue;

	m_bEndResultData = operand.m_bEndResultData;
	m_bReportEndData = operand.m_bReportEndData;
}

CSKObjectEtcAnnotationResult CSKObjectEtcAnnotationResult::operator=(const CSKObjectEtcAnnotationResult & operand)
{
	CSKAnnotationResult::operator=(operand);

	m_nReportCount = operand.m_nReportCount;

	m_vecVideoData = operand.m_vecVideoData;
	m_mapObjectData = operand.m_mapObjectData;
	m_mapPlaceData = operand.m_mapPlaceData;
	m_mapLandmarkData = operand.m_mapLandmarkData;
	m_mapEventData = operand.m_mapEventData;
	m_queueFrameObjectEtcdentifyDataQueue = operand.m_queueFrameObjectEtcdentifyDataQueue;

	m_bEndResultData = operand.m_bEndResultData;
	m_bReportEndData = operand.m_bReportEndData;

	return *this;
}

string CSKObjectEtcAnnotationResult::GetResultData(bool& bAbnormalEnd)
{
	string stdstrRet;

	g_mutexObjectEtcIdentifyData.lock();

	stdstrRet = GetXmlReportVideoData();
	
	if (stdstrRet.empty())
		stdstrRet = GetXmlReportObjectEtcData();

	g_mutexObjectEtcIdentifyData.unlock();

	bAbnormalEnd = m_bAbort;

	return stdstrRet;
}

void CSKObjectEtcAnnotationResult::Clear()
{
	//m_nFramePeriod = 0;
	//m_nReportCount = 0;

	m_bEndResultData = false;
	m_bReportEndData = false;

	m_nError = 0;
	m_nIdentifyDataCount = 0;

	m_vecVideoData.clear();

	m_mapObjectData.clear();
	m_mapPlaceData.clear();
	m_mapLandmarkData.clear();
	m_mapEventData.clear();

	g_mutexObjectEtcIdentifyData.lock();
	//std::deque<pair<int, int>> empty;
	//std::swap(m_queueFrameObjectEtcdentifyDataQueue, empty);

	int nCount = m_queueFrameObjectEtcdentifyDataQueue.size();
	for (int i = 0; i < nCount; i++)
	{
		m_queueFrameObjectEtcdentifyDataQueue.pop_front();
	}

	g_mutexObjectEtcIdentifyData.unlock();

	if (m_pIdentifyResultJsonData)
	{
		delete (json::value*)m_pIdentifyResultJsonData;
		m_pIdentifyResultJsonData = NULL;
	}
}

int CSKObjectEtcAnnotationResult::LoadIdentifyDataFile()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	m_nReportCount = m_nDataCountPerReport;

	try {
		Clear();

		CGenerateObjectEtcIdentificationJobInfo* pObjectEtcIdentifyJob = NULL;
		pObjectEtcIdentifyJob = (CGenerateObjectEtcIdentificationJobInfo*)m_pIdentifyJobInfo;
		if (m_pIdentifyJobInfo)
		{
			if (m_pIdentifyResultJsonData)
			{
				delete (json::value*)m_pIdentifyResultJsonData;
				m_pIdentifyResultJsonData = NULL;
			}

			m_pIdentifyResultJsonData = (void*)(new json::value);
			json::value* pJsonData = (json::value*)m_pIdentifyResultJsonData;

			string stdstrJsonPath = pObjectEtcIdentifyJob->m_stdstrWorkingDirPath + "/" + pObjectEtcIdentifyJob->m_stdstrTitle + "_ObjectEtc.json";
			//string_t stdstrJsonPath_t = L"Z:\\RECOG\\2018\\07\\20\\R201800000050\\한글\\_ObjectEtc.json";
			//string stdstrJsonPath = utility::conversions::utf16_to_utf8(stdstrJsonPath_t);

			CString strJsonPath;
			Utility::CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrJsonPath.c_str(), strJsonPath);
			//stdstrJsonPath.clear();
			//stdstrJsonPath.assign((char*)strJsonPath.GetBuffer());
			//strJsonPath.ReleaseBuffer();

			//string_t        strImportFile = utility::conversions::utf8_to_utf16(stdstrJsonPath);     // extract filename
			//ifstream_t      fstream(strImportFile);                     // filestream of working file
			//stringstream_t    strStream;                                  // string stream for holding JSON read from file
			string			strImportFile((char*)strJsonPath.GetBuffer());// extract filename
			ifstream		fstream(strImportFile);                     // filestream of working file
			stringstream    strStream;                                  // string stream for holding JSON read from file

			strJsonPath.ReleaseBuffer();

			//json::value     jsonData;                                   // JSON read from input file

			if (fstream)
			{
				strStream << fstream.rdbuf();                           // stream results of reading from file stream into string stream
				fstream.close();                                        // close the filestream

				*pJsonData = json::value::parse(strStream);               // parse the resultant string stream.

				unsigned int uThreadID = 0;
				uintptr_t pThread = _beginthreadex(NULL, THREAD_STACK_SIZE, AddIdentifyDataProc, (void*)this, 0, &uThreadID);

				if (!pThread)
				{
					nRet = m_nError = RESTSKSERVICE_ERROR_RESULTPROCESS;

					string_t strMsg = L"Failed to create thread - proccessing result file data";
					utility::conversions::utf16_to_utf8(strMsg);

					m_stdstrErrMsg.clear();
					m_stdstrErrMsg.assign(utility::conversions::utf16_to_utf8(strMsg));
				}
				else
					Sleep(1500);
			}
			else
			{
				nRet = m_nError = RESTSKSERVICE_ERROR_FILE_IO;

				//string_t strMsg = L"Failed to load file stream. - " + strImportFile;
				string_t strMsg = L"Failed to load file stream. - " + utility::conversions::utf8_to_utf16(stdstrJsonPath);
				//utility::conversions::utf16_to_utf8(strMsg);
				
				if (!Utility::IsFile(strJsonPath))
					strMsg = L"No exist file stream - " + utility::conversions::utf8_to_utf16(stdstrJsonPath);

				utility::conversions::utf16_to_utf8(strMsg);

				m_stdstrErrMsg.clear();
				m_stdstrErrMsg.assign(utility::conversions::utf16_to_utf8(strMsg));
			}
		}
	}
	catch (web::json::json_exception e)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}

	return nRet;
}

void CSKObjectEtcAnnotationResult::IdentifyDataOperationProc()
{
	m_bWorkingResult = true;

	////////////// Object etc identification result json ////////////////////////////////////////
	/*
	{
		"videoinfo": {
			"/media/DISK02/media3/SKT29/test/videoclass/밀정.mp4": [{
				"videoClass": "Trailer (promotion)",
				"videoScore": 0.7840122580528259
			}]
		},
		"objectetcinfos": [{
			"objectinfo": {
				"objects": [{
					"topLeftX": 2,
					"topLeftY": 194,
					"objectHeight": 884,
					"objectWidth": 1712,
					"objectScore": 0.92808,
					"objectClass": "book"
				}]
			},
			"placeinfo": {
				"placeClass": "negative",
				"placeScore": 0.9976963400840759
			},
			"eventinfo": {
				"eventClass": "negative",
				"eventScore": 0.0
			},
			"fileid": "/media/DISK02/media3/SKT29/FRAME/2018/06/14/F201800000307/0000000.jpg"
		}]
	}
	*/
	////////////// Object etc identification result json ////////////////////////////////////////

	try
	{
		if (m_pIdentifyJobInfo)
			m_llAssetID = ((CGenerateObjectEtcIdentificationJobInfo*)m_pIdentifyJobInfo)->m_llAssetID;

		if (m_pstdstrJobInfo)
		{
			*m_pstdstrJobInfo = *(((CGenerateObjectEtcIdentificationJobInfo*)m_pIdentifyJobInfo)->m_pstdstrJobInfo);
		}

		json::value* pJsonData = (json::value*)m_pIdentifyResultJsonData;

		if (pJsonData)
		{
			utility::string_t strKey;
			strKey = L"videoinfo";

			if (pJsonData->has_field(strKey))
			{
				json::value jsonVideoInfo = pJsonData->at(strKey);

				int nCount = (int)jsonVideoInfo.size();
				if (nCount > 0)
				{
					json::object jsonVideoInfoObject = jsonVideoInfo.as_object();
					json::object::iterator it = jsonVideoInfoObject.begin();
					for (it; it != jsonVideoInfoObject.end(); ++it)
					{
						json::array jsonArrVideoInfo = it->second.as_array();

						nCount = jsonArrVideoInfo.size();
						for (int i = 0; i < nCount; i++)
						{
							json::value jsonVideo = jsonArrVideoInfo[i];

							strKey = L"videoClass";
							if (jsonVideo.has_field(strKey))
							{
								CObjectEtcIdentifyData VideoIdentifyData;
								VideoIdentifyData.m_stdstrClass.assign(utility::conversions::utf16_to_utf8(jsonVideo.at(strKey).as_string()));

								strKey = L"videoScore";
								if (jsonVideo.has_field(strKey))
								{
									VideoIdentifyData.m_dbScore = jsonVideo.at(strKey).as_double();

									g_mutexObjectEtcIdentifyData.lock();

									m_vecVideoData.push_back(VideoIdentifyData);

									g_mutexObjectEtcIdentifyData.unlock();
								}
							}
						}
					}
				}
			}

			strKey = L"objectetcinfos";

			if (pJsonData->has_field(strKey))
			{
				json::array jsonArrObjectEctInfos = pJsonData->at(strKey).as_array();

				int nArrCount = (int)jsonArrObjectEctInfos.size();
				for (int i = 0; i < nArrCount; i++)
				{
					json::value jsonObjectEtcInfo = jsonArrObjectEctInfos[i];

					strKey = L"fileid";
					if (jsonObjectEtcInfo.has_field(strKey))
					{
						string stdstrFilePath = utility::conversions::utf16_to_utf8(jsonObjectEtcInfo.at(strKey).as_string());
						Utility::ReplaceAll(stdstrFilePath, ".jpg", "");

						int nIndex = stdstrFilePath.rfind("/");
						string stdstrFileTitle = stdstrFilePath.substr(nIndex + 1);

						int nFrameIndex = atoi(stdstrFileTitle.c_str());

						while (1)
						{
							if (m_bAbort)
								break;

							g_mutexObjectEtcIdentifyData.lock();

							strKey = L"objectinfo";
							if (jsonObjectEtcInfo.has_field(strKey))
							{
								json::value jsonObjectInfo = jsonObjectEtcInfo.at(strKey);

								strKey = L"objects";
								if (jsonObjectInfo.has_field(strKey))
								{
									json::array jsonArrObjects = jsonObjectInfo.at(strKey).as_array();

									vector<CObjectIdentifyData> vecObjectIdData;

									int nObjectCount = (int)jsonArrObjects.size();
									if (nObjectCount > 0)
									{
										for (int i = 0; i < nObjectCount; i++)
										{
											json::value jsonObject = jsonArrObjects[i];

											CObjectIdentifyData ObjectData;

											strKey = L"topLeftX";
											if (jsonObject.has_field(strKey))
											{
												ObjectData.m_nPositionX = jsonObject.at(strKey).as_integer();

												strKey = L"topLeftY";
												if (jsonObject.has_field(strKey))
												{
													ObjectData.m_nPositionY = jsonObject.at(strKey).as_integer();
												}

												strKey = L"objectHeight";
												if (jsonObject.has_field(strKey))
												{
													ObjectData.m_nPositionHeight = jsonObject.at(strKey).as_integer();
												}

												strKey = L"objectWidth";
												if (jsonObject.has_field(strKey))
												{
													ObjectData.m_nPositionWidth = jsonObject.at(strKey).as_integer();
												}

												strKey = L"objectScore";
												if (jsonObject.has_field(strKey))
												{
													ObjectData.m_dbScore = jsonObject.at(strKey).as_double();
												}

												strKey = L"objectClass";
												if (jsonObject.has_field(strKey))
												{
													ObjectData.m_stdstrClass = utility::conversions::utf16_to_utf8(jsonObject.at(strKey).as_string());
												}

												vecObjectIdData.push_back(ObjectData);
											}
										}

										if(vecObjectIdData.size() > 0)
											m_mapObjectData[nFrameIndex] = vecObjectIdData;

										m_mapObjectData.find(nFrameIndex);
									}
								}
							}

							strKey = L"placeinfo";
							if (jsonObjectEtcInfo.has_field(strKey))
							{
								json::value jsonPlace = jsonObjectEtcInfo.at(strKey);

								vector<CObjectEtcIdentifyData> vecPlaceIdData;

								CObjectEtcIdentifyData PlaceData;

								strKey = L"placeScore";
								if (jsonPlace.has_field(strKey))
								{
									PlaceData.m_dbScore = jsonPlace.at(strKey).as_double();

									strKey = L"placeClass";
									if (jsonPlace.has_field(strKey))
									{
										string_t str_t = jsonPlace.at(strKey).as_string();
										PlaceData.m_stdstrClass = utility::conversions::utf16_to_utf8(jsonPlace.at(strKey).as_string());
									}

									vecPlaceIdData.push_back(PlaceData);

									m_mapPlaceData[nFrameIndex] = vecPlaceIdData;
								}
							}

							strKey = L"landmarkinfo";
							if (jsonObjectEtcInfo.has_field(strKey))
							{
								json::value jsonLandmark = jsonObjectEtcInfo.at(strKey);

								vector<CObjectEtcIdentifyData> vecLandmarkIdData;

								CObjectEtcIdentifyData LandMarkData;

								strKey = L"landmarkScore";
								if (jsonLandmark.has_field(strKey))
								{
									LandMarkData.m_dbScore = jsonLandmark.at(strKey).as_double();

									strKey = L"landmarkClass";
									if (jsonLandmark.has_field(strKey))
									{
										string_t str_t = jsonLandmark.at(strKey).as_string();
										LandMarkData.m_stdstrClass = utility::conversions::utf16_to_utf8(jsonLandmark.at(strKey).as_string());
									}

									vecLandmarkIdData.push_back(LandMarkData);

									m_mapLandmarkData[nFrameIndex] = vecLandmarkIdData;
								}
							}

							strKey = L"eventinfo";
							if (jsonObjectEtcInfo.has_field(strKey))
							{
								json::value jsonEvent = jsonObjectEtcInfo.at(strKey);

								vector<CObjectEtcIdentifyData> vecEventIdData;

								CObjectEtcIdentifyData EventData;

								strKey = L"eventScore";
								if (jsonEvent.has_field(strKey))
								{
									EventData.m_dbScore = jsonEvent.at(strKey).as_double();

									strKey = L"eventClass";
									if (jsonEvent.has_field(strKey))
									{
										EventData.m_stdstrClass = utility::conversions::utf16_to_utf8(jsonEvent.at(strKey).as_string());
									}

									vecEventIdData.push_back(EventData);

									m_mapEventData[nFrameIndex] = vecEventIdData;
								}
							}

							if ((int)m_queueFrameObjectEtcdentifyDataQueue.size() < m_nDataCountPerReport)
							{
								pair<int, int> pairFrameIndexNReportedDataType(nFrameIndex, REPORTED_ANNOTATATION_DATATYPE_NONE);
								m_queueFrameObjectEtcdentifyDataQueue.push_back(pairFrameIndexNReportedDataType);

								g_mutexObjectEtcIdentifyData.unlock();

								break;
							}

							g_mutexObjectEtcIdentifyData.unlock();

							Sleep(500);
						}
					}
				}
			}

			m_bEndResultData = true;
		}
	}
	catch (web::json::json_exception e)
	{
		if (m_nError != RESTSKSERVICE_ERROR_INVALID_JSON)
			m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());

		m_bAbort = true;
	}

	m_bWorkingResult = false;
}

string CSKObjectEtcAnnotationResult::GetXmlReportVideoData()
{
	//<mamex_request>
	//  <ex_update_jobresult>
	//	<workinfoxml>      
	//	</workinfoxml>
	//	<assetinfo type="recognizevideoinfo">
	//	  <recognizevideoinfo>
	//		<assetid>81184</assetid>        
	//		<video>
	//		  <class>Trailer (promotion)</class>
	//		  <score>0.7840122580528259</score>
	//		</video>
	//		<video>
	//		  <class>Trailer (promotion)</class>
	//		  <score>0.7840122580528259</score>
	//		</video>
	//	  </recognizevideoinfo>
	//	</assetinfo>
	//  </ex_update_jobresult>
	//</mamex_request>

	string stdstrRet;

	int nCount = (int)m_vecVideoData.size();
	if (nCount > 0)
	{
		string stdstrVideoClassInfo;

		vector<CObjectEtcIdentifyData>::iterator it = m_vecVideoData.begin();
		for (it; it != m_vecVideoData.end(); it++)
		{
			stdstrVideoClassInfo = stdstrVideoClassInfo + Utility::string_format("<video><class>%s</class><score>%f</score></video>"
									, it->m_stdstrClass.c_str(), it->m_dbScore);
		}

		m_vecVideoData.clear();

		stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
						<assetinfo type = \"recognizevideoinfo\"><recognizevideoinfo><assetid>%I64d</assetid>%s</recognizevideoinfo>\
						</assetinfo></ex_update_jobresult></mamex_request>"
						, m_pstdstrJobInfo->c_str(), m_llAssetID, stdstrVideoClassInfo.c_str());
	}

	return stdstrRet;
}

string CSKObjectEtcAnnotationResult::GetXmlReportObjectData()
{
	//<mamex_request>
	//  <ex_update_jobresult>
	//	<workinfoxml>      
	//	</workinfoxml>
	//	<assetinfo type="recognizeobjectinfo">
	//	  <recognizeobjectinfo>
	//		<assetid>81184</assetid>        
	//		<frameindex>74578</frameindex>
	//		<object>
	//		  <class>book</class>
	//		  <score>0.7840122580528259</score>
	//		  <rect>10, 20, 100, 200</rect>
	//		</object>
	//		<object>
	//		  <class>book</class>
	//		  <score>0.7840122580528259</score>
	//		  <rect>10, 20, 100, 200</rect>
	//		</object>
	//	  </recognizeobjectinfo>
	//	  <recognizeobjectinfo>
	//		<assetid>81184</assetid>        
	//		<frameindex>74578</frameindex>
	//		<object>
	//		  <class>book</class>
	//		  <score>0.7840122580528259</score>
	//		  <rect>10, 20, 100, 200</rect>
	//		</object>
	//		<object>
	//		  <class>book</class>
	//		  <score>0.7840122580528259</score>
	//		  <rect>10, 20, 100, 200</rect>
	//		</object>
	//	  </recognizeobjectinfo>
	//	</assetinfo>
	//  </ex_update_jobresult>
	//</mamex_request>

	string stdstrRet;
	vector<int> vecFrameIndex;

	int nCount = m_queueFrameObjectEtcdentifyDataQueue.size();//(int)m_mapObjectData.size();
	if (nCount >= m_nReportCount || m_bEndResultData)
	{
		int nProccessedCount = 0;
		string stdstrObjectsInfo;

		//map<int, vector<CObjectIdentifyData>>::iterator mapIt = m_mapObjectData.begin();
		//for (mapIt; mapIt != m_mapObjectData.end(); mapIt++)
		deque<pair<int, int>>::iterator dequeIt = m_queueFrameObjectEtcdentifyDataQueue.begin();
		for (dequeIt; dequeIt != m_queueFrameObjectEtcdentifyDataQueue.end(); dequeIt++)
		{
			string stdstrObject;

			vector<CObjectIdentifyData> vecObjectIdData = m_mapObjectData[dequeIt->first];

			vector<CObjectIdentifyData>::iterator vecIt = vecObjectIdData.begin();
			for (vecIt; vecIt != vecObjectIdData.end(); vecIt++)
			{
				stdstrObject = stdstrObject + Utility::string_format("<object><class>%s</class><score>%f</score>\
										<rect>%d, %d, %d, %d</rect></object>", vecIt->m_stdstrClass.c_str(), vecIt->m_dbScore
										, vecIt->m_nPositionX, vecIt->m_nPositionY, vecIt->m_nPositionWidth, vecIt->m_nPositionHeight);
			}

			if (!stdstrObject.empty())
			{
				stdstrObjectsInfo = stdstrObjectsInfo + Utility::string_format("<recognizeobjectinfo><assetid>%I64d</assetid>\
										<frameindex>%d</frameindex>%s</recognizeobjectinfo>", m_llAssetID, dequeIt->first, stdstrObject.c_str());

			}

			//vecFrameIndex.push_back(mapIt->first);

			//vector<CObjectIdentifyData>::iterator vecIt = mapIt->second.begin();
			//for (vecIt; vecIt != mapIt->second.end(); vecIt++)
			//{
			//	stdstrObject = stdstrObject + Utility::string_format("<object><class>%s</class><score>%f</score>\
			//							<rect>%d, %d, %d, %d</rect></object>", vecIt->m_stdstrClass.c_str(), vecIt->m_dbScore
			//							, vecIt->m_nPositionX, vecIt->m_nPositionY, vecIt->m_nPositionWidth, vecIt->m_nPositionHeight);
			//}

			//stdstrObjectsInfo = stdstrObjectsInfo + Utility::string_format("<recognizeobjectinfo><assetid>%I64d</assetid>\
			//							<frameindex>%d</frameindex>%s</recognizeobjectinfo>", mapIt->first, m_llAssetID, stdstrObject.c_str());


			if (!stdstrObjectsInfo.empty())
				vecFrameIndex.push_back(dequeIt->first);

			dequeIt->second = REPORTED_ANNOTATATION_DATATYPE_OBJECT;

			nProccessedCount++;
			if (nProccessedCount >= m_nReportCount)
				break;
		}

		vector<int>::iterator vecIt = vecFrameIndex.begin();
		for (vecIt; vecIt != vecFrameIndex.end(); vecIt++)
		{
			m_mapObjectData.erase(*vecIt);
		}

		if (!stdstrObjectsInfo.empty())
			stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
						<assetinfo type = \"recognizeobjectinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
						, m_pstdstrJobInfo->c_str(), stdstrObjectsInfo.c_str());
	}

	return stdstrRet;
}

string CSKObjectEtcAnnotationResult::GetXmlReportEventData()
{
	//<mamex_request>
	//  <ex_update_jobresult>
	//	<workinfoxml>      
	//	</workinfoxml>
	//	<assetinfo type="recognizeoeventinfo">
	//	  <recognizeoeventinfo>
	//		<assetid>81184</assetid>        
	//		<frameindex>74578</frameindex>
	//		<event>
	//		  <class>event</class>
	//		  <score>0.7840122580528259</score>
	//		</event>
	//		<event>
	//		  <class>event</class>
	//		  <score>0.7840122580528259</score>
	//		</event>
	//	  </recognizeoeventinfo>
	//	  <recognizeoeventinfo>
	//		<assetid>81184</assetid>        
	//		<frameindex>74578</frameindex>
	//		<event>
	//		  <class>event</class>
	//		  <score>0.7840122580528259</score>
	//		</event>
	//		<event>
	//		  <class>event</class>
	//		  <score>0.7840122580528259</score>
	//		</event>
	//	  </recognizeoeventinfo>
	//	</assetinfo>
	//  </ex_update_jobresult>
	//</mamex_request>

	string stdstrRet;
	vector<int> vecFrameIndex;

	int nCount = m_queueFrameObjectEtcdentifyDataQueue.size();//(int)m_mapEventData.size();
	if (nCount >= m_nReportCount || m_bEndResultData)
	{
		int nProccessedCount = 0;
		string stdstrEventsInfo;

		//map<int, vector<CObjectEtcIdentifyData>>::iterator mapIt = m_mapEventData.begin();
		//for (mapIt; mapIt != m_mapEventData.end(); mapIt++)
		deque<pair<int, int>>::iterator dequeIt = m_queueFrameObjectEtcdentifyDataQueue.begin();
		for (dequeIt; dequeIt != m_queueFrameObjectEtcdentifyDataQueue.end(); dequeIt++)
		{
			if (dequeIt->second != REPORTED_ANNOTATATION_DATATYPE_OBJECT)
				break;

			string stdstrEvent;

			vector<CObjectEtcIdentifyData> vecEventIdData = m_mapEventData[dequeIt->first];

			vector<CObjectEtcIdentifyData>::iterator vecIt = vecEventIdData.begin();
			for (vecIt; vecIt != vecEventIdData.end(); vecIt++)
			{
				stdstrEvent = stdstrEvent + Utility::string_format("<event><class>%s</class><score>%f</score></event>"
					, vecIt->m_stdstrClass.c_str(), vecIt->m_dbScore);
			}

			if (!stdstrEvent.empty())
			{
				stdstrEventsInfo = stdstrEventsInfo + Utility::string_format("<recognizeoeventinfo><assetid>%I64d</assetid>\
								<frameindex>%d</frameindex>%s</recognizeoeventinfo>", m_llAssetID, dequeIt->first, stdstrEvent.c_str());
			}

			if (!stdstrEventsInfo.empty())
				vecFrameIndex.push_back(dequeIt->first);

			dequeIt->second = REPORTED_ANNOTATATION_DATATYPE_EVENT;

			//vecFrameIndex.push_back(mapIt->first);

			//vector<CObjectEtcIdentifyData>::iterator vecIt = mapIt->second.begin();
			//for (vecIt; vecIt != mapIt->second.end(); vecIt++)
			//{
			//	stdstrEvent = stdstrEvent + Utility::string_format("<event><class>%s</class><score>%f</score></event>"
			//								, vecIt->m_stdstrClass.c_str(), vecIt->m_dbScore);
			//}

			//stdstrEventsInfo = stdstrEventsInfo + Utility::string_format("<recognizeoeventinfo><assetid>%I64d</assetid>\
			//					<frameindex>%d</frameindex>%s</recognizeoeventinfo>", mapIt->first, m_llAssetID, stdstrEvent.c_str());

			nProccessedCount++;
			if (nProccessedCount >= m_nReportCount)
				break;
		}

		vector<int>::iterator vecIt = vecFrameIndex.begin();
		for (vecIt; vecIt != vecFrameIndex.end(); vecIt++)
		{
			m_mapEventData.erase(*vecIt);
		}

		if (!stdstrEventsInfo.empty())
			stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
										<assetinfo type = \"recognizeoeventinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
										, m_pstdstrJobInfo->c_str(), stdstrEventsInfo.c_str());
	}

	return stdstrRet;
}

string CSKObjectEtcAnnotationResult::GetXmlReportLandmarkData()
{
	//<mamex_request>
	//  <ex_update_jobresult>
	//	<workinfoxml>      
	//	</workinfoxml>
	//	<assetinfo type="recognizelandmarkinfo">
	//	  <recognizelandmarkinfo>
	//		<assetid>81184</assetid>        
	//		<frameindex>74578</frameindex>
	//		<landmark>
	//		  <class>피사의 탑</class>
	//		  <score>0.7840122580528259</score>
	//		</landmark>
	//		<landmark>
	//		  <class>오페라하우스</class>
	//		  <score>0.7840122580528259</score>
	//		</landmark>
	//	  </recognizelandmarkinfo>
	//	  <recognizelandmarkinfo>
	//		<assetid>81184</assetid>        
	//		<frameindex>74579</frameindex>
	//		<landmark>
	//		  <class>피사의 탑</class>
	//		  <score>0.7840122580528259</score>
	//		</landmark>
	//		<landmark>
	//		  <class>오페라하우스</class>
	//		  <score>0.7840122580528259</score>
	//		</landmark>
	//	  </recognizelandmarkinfo>
	//	</assetinfo>
	//  </ex_update_jobresult>
	//</mamex_request>

	string stdstrRet;
	vector<int> vecFrameIndex;

	int nCount = m_queueFrameObjectEtcdentifyDataQueue.size();
	if (nCount >= m_nReportCount || m_bEndResultData)
	{
		int nProccessedCount = 0;
		string stdstrLandmarksInfo;

		deque<pair<int, int>>::iterator dequeIt = m_queueFrameObjectEtcdentifyDataQueue.begin();
		for (dequeIt; dequeIt != m_queueFrameObjectEtcdentifyDataQueue.end(); dequeIt++)
		{
			if (dequeIt->second != REPORTED_ANNOTATATION_DATATYPE_EVENT)
				break;

			string stdstrLandmark;

			vector<CObjectEtcIdentifyData> vecLandmarkIdData = m_mapLandmarkData[dequeIt->first];
			
			vector<CObjectEtcIdentifyData>::iterator vecIt = vecLandmarkIdData.begin();
			for (vecIt; vecIt != vecLandmarkIdData.end(); vecIt++)
			{
				stdstrLandmark = stdstrLandmark + Utility::string_format("<landmark><class>%s</class><score>%f</score></landmark>"
													, vecIt->m_stdstrClass.c_str(), vecIt->m_dbScore);
			}

			if (!stdstrLandmark.empty())
			{
				stdstrLandmarksInfo = stdstrLandmarksInfo + Utility::string_format("<recognizelandmarkinfo><assetid>%I64d</assetid>\
									<frameindex>%d</frameindex>%s</recognizelandmarkinfo>", m_llAssetID, dequeIt->first, stdstrLandmark.c_str());
			}

			if (!stdstrLandmarksInfo.empty())
				vecFrameIndex.push_back(dequeIt->first);

			dequeIt->second = REPORTED_ANNOTATATION_DATATYPE_LANDMARK;

			nProccessedCount++;
			if (nProccessedCount >= m_nReportCount)
				break;
		}

		vector<int>::iterator vecIt = vecFrameIndex.begin();
		for (vecIt; vecIt != vecFrameIndex.end(); vecIt++)
		{
			m_mapLandmarkData.erase(*vecIt);
		}

		if (!stdstrLandmarksInfo.empty())
			stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
								<assetinfo type = \"recognizelandmarkinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
								, m_pstdstrJobInfo->c_str(), stdstrLandmarksInfo.c_str());
	}

	return stdstrRet;
}

string CSKObjectEtcAnnotationResult::GetXmlReportPlaceData()
{
	//<mamex_request>
	//  <ex_update_jobresult>
	//	<workinfoxml>      
	//	</workinfoxml>
	//	<assetinfo type="recognizeplaceinfo">
	//	  <recognizeplaceinfo>
	//		<assetid>81184</assetid>        
	//		<frameindex>74578</frameindex>
	//		<place>
	//		  <class>노래방</class>
	//		  <score>0.7840122580528259</score>
	//		</place>
	//		<place>
	//		  <class>노래방</class>
	//		  <score>0.7840122580528259</score>
	//		</place>
	//	  </recognizeplaceinfo>
	//	  <recognizeplaceinfo>
	//		<assetid>81184</assetid>        
	//		<frameindex>74578</frameindex>
	//		<place>
	//		  <class>노래방</class>
	//		  <score>0.7840122580528259</score>
	//		</place>
	//		<place>
	//		  <class>노래방</class>
	//		  <score>0.7840122580528259</score>
	//		</place>
	//	  </recognizeplaceinfo>
	//	</assetinfo>
	//  </ex_update_jobresult>
	//</mamex_request>

	string stdstrRet;
	vector<int> vecFrameIndex;

	int nCount = m_queueFrameObjectEtcdentifyDataQueue.size();//(int)m_mapPlaceData.size();
	if (nCount >= m_nReportCount || m_bEndResultData)
	{
		int nProccessedCount = 0;
		string stdstrPlacesInfo;

		//map<int, vector<CObjectEtcIdentifyData>>::iterator mapIt = m_mapPlaceData.begin();
		//for (mapIt; mapIt != m_mapPlaceData.end(); mapIt++)
		deque<pair<int, int>>::iterator dequeIt = m_queueFrameObjectEtcdentifyDataQueue.begin();
		for (dequeIt; dequeIt != m_queueFrameObjectEtcdentifyDataQueue.end(); dequeIt++)
		{
			if (dequeIt->second != REPORTED_ANNOTATATION_DATATYPE_LANDMARK)
				break;

			string stdstrPlace;

			vector<CObjectEtcIdentifyData> vecPlaceIdData = m_mapPlaceData[dequeIt->first];
			
			vector<CObjectEtcIdentifyData>::iterator vecIt = vecPlaceIdData.begin();
			for (vecIt; vecIt != vecPlaceIdData.end(); vecIt++)
			{
				stdstrPlace = stdstrPlace + Utility::string_format("<place><class>%s</class><score>%f</score></place>"
													, vecIt->m_stdstrClass.c_str(), vecIt->m_dbScore);
			}

			if (!stdstrPlace.empty())
			{
				stdstrPlacesInfo = stdstrPlacesInfo + Utility::string_format("<recognizeplaceinfo><assetid>%I64d</assetid>\
									<frameindex>%d</frameindex>%s</recognizeplaceinfo>", m_llAssetID, dequeIt->first, stdstrPlace.c_str());
			}

			if (!stdstrPlacesInfo.empty())
				vecFrameIndex.push_back(dequeIt->first);

			dequeIt->second = REPORTED_ANNOTATATION_DATATYPE_PLACE;

			//vecFrameIndex.push_back(mapIt->first);

			//vector<CObjectEtcIdentifyData>::iterator vecIt = mapIt->second.begin();
			//for (vecIt; vecIt != mapIt->second.end(); vecIt++)
			//{
			//	stdstrPlace = stdstrPlace + Utility::string_format("<place><class>%s</class><score>%f</score></place>"
			//										, vecIt->m_stdstrClass.c_str(), vecIt->m_dbScore);
			//}

			//stdstrPlacesInfo = stdstrPlacesInfo + Utility::string_format("<recognizeplaceinfo><assetid>%I64d</assetid>\
			//					<frameindex>%d</frameindex>%s</recognizeplaceinfo>", mapIt->first, m_llAssetID, stdstrPlace.c_str());

			nProccessedCount++;
			if (nProccessedCount >= m_nReportCount)
				break;
		}

		vector<int>::iterator vecIt = vecFrameIndex.begin();
		for (vecIt; vecIt != vecFrameIndex.end(); vecIt++)
		{
			m_mapPlaceData.erase(*vecIt);
		}

		if (!stdstrPlacesInfo.empty())
			stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult><workinfoxml>%s</workinfoxml>\
								<assetinfo type = \"recognizeplaceinfo\">%s</assetinfo></ex_update_jobresult></mamex_request>"
								, m_pstdstrJobInfo->c_str(), stdstrPlacesInfo.c_str());
	}

	return stdstrRet;
}

string CSKObjectEtcAnnotationResult::GetXmlReportObjectEtcData()
{
	string stdstrRet;

	int nCount = (int)m_queueFrameObjectEtcdentifyDataQueue.size();
	if (nCount > 0)
	{
		if ((nCount >= m_nReportCount) || (nCount < m_nReportCount && m_bEndResultData))
		{
			pair<int, int> pairFrameIndexNReportedDataType(-1, -1);
			pairFrameIndexNReportedDataType	= m_queueFrameObjectEtcdentifyDataQueue.front();
			
			if (pairFrameIndexNReportedDataType.second == REPORTED_ANNOTATATION_DATATYPE_NONE)
			{
				stdstrRet = GetXmlReportObjectData();
			}

			if (pairFrameIndexNReportedDataType.second == REPORTED_ANNOTATATION_DATATYPE_OBJECT)
			{
				stdstrRet = GetXmlReportEventData();
			}

			if (pairFrameIndexNReportedDataType.second == REPORTED_ANNOTATATION_DATATYPE_EVENT)
			{
				stdstrRet = GetXmlReportLandmarkData();
			}

			if (pairFrameIndexNReportedDataType.second == REPORTED_ANNOTATATION_DATATYPE_LANDMARK)
			{
				stdstrRet = GetXmlReportPlaceData();

				int nRemoveCount = nCount;
				if (nCount > m_nReportCount)
					nRemoveCount = m_nReportCount;

				for (int i = 0; i < nRemoveCount; i++)
				{
					pairFrameIndexNReportedDataType = m_queueFrameObjectEtcdentifyDataQueue.front();

					if (pairFrameIndexNReportedDataType.second == REPORTED_ANNOTATATION_DATATYPE_PLACE)
						m_queueFrameObjectEtcdentifyDataQueue.pop_front();
					else
						break;
				}
			}
		}
	}
	else if (m_bEndResultData)
	{
		m_bReportEndData = true;
	}

	return stdstrRet;
}


unsigned __stdcall AddCharIdentifyDataProc(void *pData)
{
	unsigned nRet = 0;

	if (pData)
	{
		int nError = 0;
		string stdstrMsg;

		CKRestSKAnnotationService* pService = (CKRestSKAnnotationService*)pData;
		pService->AddCharIdentifyDataThreadProc();
	}

	return nRet;
}

CKRestSKAnnotationService::CKRestSKAnnotationService(void)
{
	m_nCurrentResultSceneIdx = -1;
	m_nDataCountPerReport = RESTSKSERVICE_IDENTIFY_RESULT_BATCH_COUNT;
	m_nGotCharIdentifyDataCount = 0;
	m_nError = 0;
	m_pJobInfo = NULL;

	m_pFuncStateCB = NULL;
	m_pDataStateCB = NULL;

	m_pCharIdentifyResultJsonData = NULL;

	m_pFaceAnnotationResult = new CSKFFaceAnnotationResult();
	m_pAnnotationResult = NULL;
	m_nReportCount = RESTSKSERVICE_IDENTIFY_RESULT_BATCH_COUNT;
}

CKRestSKAnnotationService::~CKRestSKAnnotationService(void)
{
	if (m_pJobInfo)
	{
		delete m_pJobInfo;
		m_pJobInfo = NULL;
	}

	if (m_pCharIdentifyResultJsonData)
	{
		json::value* pJsonData = (json::value*)m_pCharIdentifyResultJsonData;
		delete pJsonData;

		m_pCharIdentifyResultJsonData = NULL;
	}

	if (m_pFaceAnnotationResult)
	{
		delete m_pFaceAnnotationResult;
		m_pFaceAnnotationResult = NULL;
	}

	m_pFuncStateCB = NULL;
	m_pDataStateCB = NULL;

	if (m_pAnnotationResult)
	{
		delete m_pAnnotationResult;
		m_pAnnotationResult = NULL;
	}
}

void CKRestSKAnnotationService::SetServiceInfo(string stdstrServiceUrl)
{
	m_stdstrServiceUrl = stdstrServiceUrl;
}

void CKRestSKAnnotationService::SetJobStateCB(JOBSTATECB pFunc, void* pData)
{
	m_pFuncStateCB = pFunc;
	m_pDataStateCB = pData;
}

void CKRestSKAnnotationService::SetJobInfo(CSKAnnotationJobInfo* pJobInfo)
{
	if (m_pJobInfo)
	{
		delete m_pJobInfo;
		m_pJobInfo = NULL;
	}

	m_pJobInfo = pJobInfo;
	m_bAbort = false;
	m_bWorkingResult = false;

	if (m_pAnnotationResult)
	{
		delete m_pAnnotationResult;
		m_pAnnotationResult = NULL;
	}

	string stdstrAnnotationType = pJobInfo->GetAnnotationType();
	if (!stdstrAnnotationType.compare(RESTSKSERVICE_ANNOTATE_IDENTIFY_CHARACTER))
	{
		m_pAnnotationResult = new CSKFaceAnnotationResult;
	}

	if (!stdstrAnnotationType.compare(RESTSKSERVICE_ANNOTATE_IDENTIFY_OBJECT_ETC))
	{
		m_pAnnotationResult = new CSKObjectEtcAnnotationResult;
	}

	if (m_pAnnotationResult)
		m_pAnnotationResult->m_nDataCountPerReport = m_nReportCount;
}

int CKRestSKAnnotationService::AssignFaceFeatureGenJob()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	if (!m_pJobInfo)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JOB;
		m_stdstrErrMsg = "Job data is empty";
		
		return nRet;
	}

	string stdstrReqBody = m_pJobInfo->GetJsonJobInfo();

	CRestClient requestClient(m_stdstrServiceUrl);
	nRet = requestClient.Request(g_pairGenCharFeature.first, g_pairGenCharFeature.second, stdstrReqBody);
	if ((nRet/100) == 2)
	{
		// status ok
		nRet = RESTSKSERVICE_ERROR_SUCCESS;

		m_stdstrResponse = requestClient.GetResponseData();

		CheckResponseData(m_stdstrResponse, m_nError, m_stdstrErrMsg);
		
		if (m_nError != RESTSKSERVICE_ERROR_SUCCESS)
			nRet = m_nError;
	}
	else
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	return nRet;
}

int CKRestSKAnnotationService::AssignSceneCharacterIdentificationJob()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	if (!m_pJobInfo)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JOB;
		m_stdstrErrMsg = "Job data is empty";

		return nRet;
	}

	//string stdstrRetErrMsg;
	//eSKAnnotationJobState eRetJobState = eJobStateError;
	//int nRetProgress = 0;

	//nRet = GetStateAssignedJob(eRetJobState, nRetProgress, stdstrRetErrMsg);
	//if ((nRet != RESTSKSERVICE_ERROR_NOSUCHAJOB) && (eRetJobState > eJobStateError))
	//{
	//	RemoveAssignedJob();
	//}

	string stdstrReqBody = m_pJobInfo->GetJsonJobInfo();

	CRestClient requestClient(m_stdstrServiceUrl);
	nRet = requestClient.Request(g_pairScenCharIdentification.first, g_pairScenCharIdentification.second, stdstrReqBody);
	if ((nRet / 100) == 2)
	{
		// status ok
		nRet = RESTSKSERVICE_ERROR_SUCCESS;

		m_stdstrResponse = requestClient.GetResponseData();

		CheckResponseData(m_stdstrResponse, m_nError, m_stdstrErrMsg);

		if (m_nError != RESTSKSERVICE_ERROR_SUCCESS)
			nRet = m_nError;
	}
	else
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	return nRet;
}

int CKRestSKAnnotationService::AssignObjectEtcIdentificationJob()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	if (!m_pJobInfo)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JOB;
		m_stdstrErrMsg = "Job data is empty";

		return nRet;
	}

	string stdstrReqBody = m_pJobInfo->GetJsonJobInfo();

	CRestClient requestClient(m_stdstrServiceUrl);
	nRet = requestClient.Request(g_pairObjectEtcIdentification.first, g_pairObjectEtcIdentification.second, stdstrReqBody);
	if ((nRet / 100) == 2)
	{
		// status ok
		nRet = RESTSKSERVICE_ERROR_SUCCESS;

		m_stdstrResponse = requestClient.GetResponseData();

		CheckResponseData(m_stdstrResponse, m_nError, m_stdstrErrMsg);

		if (m_nError != RESTSKSERVICE_ERROR_SUCCESS)
			nRet = m_nError;
	}
	else
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	return nRet;
}

int CKRestSKAnnotationService::RemoveAssignedJob()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	m_bWorkingResult = false;

	if (!m_pJobInfo)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JOB;
		m_stdstrErrMsg = "Job data is empty";

		return nRet;
	}

	string stdstrJobID = Utility::string_format("%I64d", m_pJobInfo->m_nJobID);

	vector<string> vecVars;
	vecVars.push_back(stdstrJobID);

	CRestClient requestClient(m_stdstrServiceUrl);
	requestClient.SetResponseCB(g_pairRemoveJob.first, g_pairRemoveJob.second, NULL, NULL);
	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairRemoveJob, vecVars);

	nRet = requestClient.Request(g_pairRemoveJob.first, stdstrPath, "");
	if ((nRet / 100) == 2)
	{
		// status ok
		nRet = RESTSKSERVICE_ERROR_SUCCESS;

		m_stdstrResponse = requestClient.GetResponseData();

		CheckResponseData(m_stdstrResponse, m_nError, m_stdstrErrMsg);

		if (m_nError != RESTSKSERVICE_ERROR_SUCCESS)
			nRet = m_nError;
	}
	else
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	m_bAbort = true;

	if (m_pAnnotationResult)
		m_pAnnotationResult->AbortOperation();

	return nRet;
}

int CKRestSKAnnotationService::AbortAssignedJob()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	m_bWorkingResult = false;

	if (!m_pJobInfo)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JOB;
		m_stdstrErrMsg = "Job data is empty";

		return nRet;
	}

	string stdstrJobID = Utility::string_format("%I64d", m_pJobInfo->m_nJobID);

	vector<string> vecVars;
	vecVars.push_back(stdstrJobID);

	CRestClient requestClient(m_stdstrServiceUrl);
	requestClient.SetResponseCB(g_pairAbortJob.first, g_pairAbortJob.second, NULL, NULL);
	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairAbortJob, vecVars);

	nRet = requestClient.Request(g_pairAbortJob.first, stdstrPath, "");
	if ((nRet / 100) == 2)
	{
		// status ok
		nRet = RESTSKSERVICE_ERROR_SUCCESS;

		m_stdstrResponse = requestClient.GetResponseData();

		CheckResponseData(m_stdstrResponse, m_nError, m_stdstrErrMsg);

		if (m_nError != RESTSKSERVICE_ERROR_SUCCESS)
			nRet = m_nError;
	}
	else
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	m_bAbort = true;

	if (m_pAnnotationResult)
		m_pAnnotationResult->AbortOperation();

	return nRet;
}

int CKRestSKAnnotationService::GetStateAssignedJob(eSKAnnotationJobState& eRetJobState, int& nRetProgress, string& stdstrRetErrMsg)
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	if (!m_pJobInfo)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JOB;
		m_stdstrErrMsg = "Job data is empty";

		return nRet;
	}

	string stdstrJobID = Utility::string_format("%I64d", m_pJobInfo->m_nJobID);

	vector<string> vecVars;
	vecVars.push_back(stdstrJobID);

	CRestClient requestClient(m_stdstrServiceUrl);
	requestClient.SetResponseCB(g_pairGetJobState.first, g_pairGetJobState.second, NULL, NULL);
	string stdstrPath = requestClient.MakeRequestPath((pair<string, string>&)g_pairGetJobState, vecVars);

	nRet = requestClient.Request(g_pairGetJobState.first, stdstrPath, "");
	if ((nRet / 100) == 2)
	{
		// status ok
		nRet = RESTSKSERVICE_ERROR_SUCCESS;

		m_stdstrResponse = requestClient.GetResponseData();

		CheckResponseData(m_stdstrResponse, m_nError, m_stdstrErrMsg);

		GetJobStateInfo(eRetJobState, nRetProgress, stdstrRetErrMsg);

		if (m_nError != RESTSKSERVICE_ERROR_SUCCESS)
			nRet = m_nError;
	}
	else
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	return nRet;
}

void CKRestSKAnnotationService::CheckResponseData(string stdstrResponse, int& nRetError, string& stdstrRetErrMsg)
{
	utility::string_t strResponse = utility::conversions::utf8_to_utf16(stdstrResponse.c_str());
	
	try
	{
		utility::string_t strKey;

		json::value jsonResponse = json::value::parse(strResponse);
	
		strKey = L"result";
		if (!jsonResponse.has_field(strKey))
		{
			web::json::json_exception e(L"'result' is not exist.");
	
			throw e;
		}

		if (jsonResponse[strKey].as_string().compare(L"success"))
		{
			strKey = L"error";
			if (jsonResponse.has_field(strKey))
			{
				utility::string_t strError = jsonResponse[strKey].as_string();
				string stdstrError = utility::conversions::utf16_to_utf8(strError);
				nRetError = atoi((char*)stdstrError.c_str());
			}
			try
			{
				strKey = L"message";
				if (jsonResponse.has_field(strKey))
					stdstrRetErrMsg = utility::conversions::utf16_to_utf8(jsonResponse[strKey].as_string());
			}
			catch (web::json::json_exception const & e)
			{
				m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

				m_stdstrErrMsg.clear();
				m_stdstrErrMsg = Utility::string_format("%s - %s", e.what(), utility::conversions::utf16_to_utf8(strResponse).c_str());
			}

			return;
		}
	}
	catch (web::json::json_exception const & e)
	{
		m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;
	
		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}
}

void CKRestSKAnnotationService::GetJobStateInfo(eSKAnnotationJobState& eRetJobState, int& nRetProgress, string& stdstrRetErrMsg)
{
	utility::string_t strResponse = utility::conversions::utf8_to_utf16(m_stdstrResponse.c_str());

	try
	{
		utility::string_t strKey;

		json::value jsonResponse = json::value::parse(strResponse);

		strKey = L"state";
		if (!jsonResponse.has_field(strKey))
		{
			web::json::json_exception e(L"'state' is not exist.");

			throw e;
		}

		string stdstrState = utility::conversions::utf16_to_utf8(jsonResponse[strKey].as_string());

		if (!stdstrState.compare(RESTSKSERVICE_STATE_ERROR))
			eRetJobState = eJobStateError;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DOGALLERYFEATUREGEN))
			eRetJobState = eJobStateStartGalleryFeatureGen;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DONEGALLERYFEATUREGEN))
			eRetJobState = eJobStateEndGalleryFeatureGen;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DOFRAMEFEATUREGEN))
			eRetJobState = eJobStateStartFrameFeatureGen;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DONEFRAMEFEATUREGEN))
			eRetJobState = eJobStateEndFrameFeatureGen;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DOCHARACTERIDENTIFY))
			eRetJobState = eJobStateStartCharacterIdentify;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DONECHARACTERIDENTIFY))
			eRetJobState = eJobStateEndCharacterIdentify;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DOOBJECTETCIDENTIFY))
			eRetJobState = eJobStateStartObjectEtcIdentify;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DONEOBJECTETCIDENTIFY))
			eRetJobState = eJobStateEndObjectEtcIdentify;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DOVIDEOIDENTIFY))
			eRetJobState = eJobStateStartVideoIdentify;

		if (!stdstrState.compare(RESTSKSERVICE_STATE_DONEVIDEOIDENTIFY))
			eRetJobState = eJobStateEndVideoIdentify;

		strKey = L"progress";
		if (!jsonResponse.has_field(strKey))
		{
			web::json::json_exception e(L"'progress' is not exist.");

			throw e;
		}

		nRetProgress = jsonResponse[strKey].as_integer();

		//strKey = L"message";
		//if (jsonResponse.has_field(strKey))
		//{
		//	stdstrRetErrMsg = utility::conversions::utf16_to_utf8(jsonResponse[strKey].as_string());
		//}

		try
		{
			strKey = L"message";
			if (jsonResponse.has_field(strKey))
			{
				stdstrRetErrMsg = utility::conversions::utf16_to_utf8(jsonResponse[strKey].as_string());
				m_stdstrErrMsg = stdstrRetErrMsg;
			}
		}
		catch (web::json::json_exception const & e)
		{
			m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

			m_stdstrErrMsg.clear();
			m_stdstrErrMsg = Utility::string_format("%s - %s", e.what(), utility::conversions::utf16_to_utf8(strResponse).c_str());
		}
	}
	catch (web::json::json_exception const & e)
	{
		m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}
}

string CKRestSKAnnotationService::GetServiceMessage(int& nRetError)
{
	nRetError = m_nError;

	return m_stdstrErrMsg;
}

string CKRestSKAnnotationService::GetStrResponse()
{
	return m_stdstrResponse;
}

int CKRestSKAnnotationService::LoadIdentifyDataFile()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	if (m_pAnnotationResult)
	{
		m_pAnnotationResult->SetJobInfo((void*)m_pJobInfo);

		if (nRet = m_pAnnotationResult->LoadIdentifyDataFile())
		{
			m_stdstrErrMsg = m_pAnnotationResult->GetLastErrorMsg(m_nError);
		}
	}

	return nRet;
}

string CKRestSKAnnotationService::GetRefinedIdentifyData(eSKAnnotationResultProcState& eRetResultState)
{
	string stdstrRet;

	if (m_pAnnotationResult)
	{
		bool bAbnormalEnd;
		stdstrRet = m_pAnnotationResult->GetResultData(bAbnormalEnd);
		eRetResultState = eAnnotationResultProcStateStart;
		
		if (bAbnormalEnd)
		{
			eRetResultState = eAnnotationResultProcError;
			m_stdstrErrMsg = m_pAnnotationResult->GetLastErrorMsg(m_nError);

			stdstrRet = "";
			return stdstrRet;
		}

		if (m_pAnnotationResult->m_bReportEndData)
			eRetResultState = eAnnotationResultProcStateEnd;
	}

	return stdstrRet;
}

int CKRestSKAnnotationService::LoadSceneCharIdentifyDataFile()
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	try {
		m_nGotCharIdentifyDataCount = 0;
		m_nCurrentResultSceneIdx = 0;

		CGenerateSceneCharIdentificationJobInfo* pScenCharIdentifyJob = NULL;
		pScenCharIdentifyJob = (CGenerateSceneCharIdentificationJobInfo*)m_pJobInfo;
		if (pScenCharIdentifyJob)
		{
			if (m_pCharIdentifyResultJsonData)
			{
				delete (json::value*)m_pCharIdentifyResultJsonData;
				m_pCharIdentifyResultJsonData = NULL;
			}

			m_pCharIdentifyResultJsonData = (void*)(new json::value);
			json::value* pJsonData = (json::value*)m_pCharIdentifyResultJsonData;

			string stdstrJsonPath = pScenCharIdentifyJob->m_stdstrWorkingDirPath + "/" + pScenCharIdentifyJob->m_stdstrTitle + ".json";
			//stdstrJsonPath = "d:/NowYouSeeMe.json";
			//stdstrJsonPath = "Z:/test/video/tttt.json";
			
			CString strJsonPath;
			Utility::CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrJsonPath.c_str(), strJsonPath);

			//string_t        strImportFile = utility::conversions::utf8_to_utf16(stdstrJsonPath);     // extract filename
			//ifstream_t      fstream(strImportFile);                     // filestream of working file
			//stringstream_t    strStream;                                  // string stream for holding JSON read from file
			string			strImportFile((char*)strJsonPath.GetBuffer());// extract filename
			ifstream		fstream(strImportFile);                     // filestream of working file
			stringstream    strStream;                                  // string stream for holding JSON read from file

			strJsonPath.ReleaseBuffer();

			//json::value     jsonData;                                   // JSON read from input file

			if (fstream)
			{
				strStream << fstream.rdbuf();                           // stream results of reading from file stream into string stream
				fstream.close();                                        // close the filestream

				*pJsonData = json::value::parse(strStream);               // parse the resultant string stream.

				m_bWorkingResult = true;
				m_pFaceAnnotationResult->Clear();

				unsigned int uThreadID = 0;
				uintptr_t pThread = _beginthreadex(NULL, THREAD_STACK_SIZE, AddCharIdentifyDataProc, (void*)this, 0, &uThreadID);

				if (!pThread)
				{
					nRet = m_nError = RESTSKSERVICE_ERROR_RESULTPROCESS;

					string_t strMsg = L"Failed to create thread - character identification";
					utility::conversions::utf16_to_utf8(strMsg);

					m_stdstrErrMsg.clear();
					m_stdstrErrMsg.assign(utility::conversions::utf16_to_utf8(strMsg));
				}
				else
					Sleep(1500);
			}
			else
			{
				nRet = m_nError = RESTSKSERVICE_ERROR_FILE_IO;

				string_t strMsg = L"Failed to load file stream. - " + utility::conversions::utf8_to_utf16(stdstrJsonPath);
				//utility::conversions::utf16_to_utf8(strMsg);

				if (!Utility::IsFile(strJsonPath))
					strMsg = L"No exist file stream - " + utility::conversions::utf8_to_utf16(stdstrJsonPath);

				utility::conversions::utf16_to_utf8(strMsg);

				m_stdstrErrMsg.clear();
				m_stdstrErrMsg.assign(utility::conversions::utf16_to_utf8(strMsg));
			}
		}
	}
	catch (web::json::json_exception e)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}

	return nRet;
}

int CKRestSKAnnotationService::LoadSceneCharIdentifyDataFile(string stdstrResultJsonPath)
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	try {
		m_nGotCharIdentifyDataCount = 0;
		m_nCurrentResultSceneIdx = 0;

		CGenerateSceneCharIdentificationJobInfo* pScenCharIdentifyJob = NULL;
		pScenCharIdentifyJob = (CGenerateSceneCharIdentificationJobInfo*)m_pJobInfo;
		if (pScenCharIdentifyJob)
		{
			if (m_pCharIdentifyResultJsonData)
			{
				delete (json::value*)m_pCharIdentifyResultJsonData;
				m_pCharIdentifyResultJsonData = NULL;
			}

			m_pCharIdentifyResultJsonData = (void*)(new json::value);
			json::value* pJsonData = (json::value*)m_pCharIdentifyResultJsonData;

			CString strJsonPath;
			Utility::CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrResultJsonPath.c_str(), strJsonPath);

			//string_t        strImportFile = utility::conversions::utf8_to_utf16(stdstrResultJsonPath);     // extract filename
			//ifstream_t      fstream(strImportFile);                     // filestream of working file
			//stringstream_t    strStream;                                  // string stream for holding JSON read from file
			string			strImportFile((char*)strJsonPath.GetBuffer());// extract filename
			ifstream		fstream(strImportFile);                     // filestream of working file
			stringstream    strStream;                                  // string stream for holding JSON read from file

			strJsonPath.ReleaseBuffer();

			//json::value     jsonData;                                   // JSON read from input file

			if (fstream)
			{
				strStream << fstream.rdbuf();                           // stream results of reading from file stream into string stream
				fstream.close();                                        // close the filestream

				*pJsonData = json::value::parse(strStream);               // parse the resultant string stream.

				m_bWorkingResult = true;
				m_pFaceAnnotationResult->Clear();

				unsigned int uThreadID = 0;
				uintptr_t pThread = _beginthreadex(NULL, THREAD_STACK_SIZE, AddCharIdentifyDataProc, (void*)this, 0, &uThreadID);

				if (!pThread)
				{
					nRet = m_nError = RESTSKSERVICE_ERROR_RESULTPROCESS;

					string_t strMsg = L"Failed to create thread - character identification";
					utility::conversions::utf16_to_utf8(strMsg);

					m_stdstrErrMsg.clear();
					m_stdstrErrMsg.assign(utility::conversions::utf16_to_utf8(strMsg));
				}
				else
					Sleep(1500);
			}
			else
			{
				nRet = m_nError = RESTSKSERVICE_ERROR_FILE_IO;

				string_t strMsg = L"Failed to load file stream. - " + utility::conversions::utf8_to_utf16(stdstrResultJsonPath);
				//utility::conversions::utf16_to_utf8(strMsg);

				if (!Utility::IsFile(strJsonPath))
					strMsg = L"No exist file stream - " + utility::conversions::utf8_to_utf16(stdstrResultJsonPath);

				utility::conversions::utf16_to_utf8(strMsg);

				m_stdstrErrMsg.clear();
				m_stdstrErrMsg.assign(utility::conversions::utf16_to_utf8(strMsg));
			}
		}
	}
	catch (web::json::json_exception e)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}

	return nRet;
}

void CKRestSKAnnotationService::AddCharIdentifyDataThreadProc()
{
	m_bWorkingResult = true;

	////////////// face identification result json ////////////////////////////////////////
	/*
	{
		"jobid": "1111",
		"title": "NowYouSeeMe",
		"scenes" : [{
			"id": 1,
			"frames":[{
				"frameindex": 0,
				"framepath" : "/frames/NowYouSeeMe_/0.jpg",
				"characters" : [{
					"conf": 0.9722898937539345,
					"id" : "",
					"rect" : {
						"y": 22,
						"x" : 710,
						"w" : 255,
						"h" : 331
					},
					"name": "WoodyHarrelson"
				}]
			},
			{
				"frameindex": 10,
				"framepath" : "/frames/NowYouSeeMe_/10.jpg",
				"characters" : [{
					"conf": 0.9587138690283279,
					"id" : "",
					"rect" : {
						"y": 17,
						"x" : 714,
						"w" : 267,
						"h" : 344
					},
					"name": "WoodyHarrelson"
				}]
			}]
		}]
	}
	*/
	////////////// face identification result json ////////////////////////////////////////

	try 
	{
		if (m_pFaceAnnotationResult && m_pJobInfo)
			m_pFaceAnnotationResult->m_llAssetID = ((CGenerateSceneCharIdentificationJobInfo*)m_pJobInfo)->m_llAssetID;

		json::value* pJsonData = (json::value*)m_pCharIdentifyResultJsonData;

		if (pJsonData)
		{
			utility::string_t strKey;
			strKey = L"scenes";

			if (pJsonData->has_field(strKey))
			{
				json::array arrScenResult = pJsonData->at(strKey).as_array();
				int nSecenCount = (int)arrScenResult.size();

				__int64 llSecneID = 0;

				int i = m_nCurrentResultSceneIdx;
				for (i; i < nSecenCount; i++)
				{
					if (m_bAbort)
						break;

					strKey = L"id";
					if (arrScenResult[i].has_field(strKey))
					{
						llSecneID = arrScenResult[i].at(strKey).as_number().to_int64();
					}

					int nFrameIdex = 0;
					string stdstrFramePath = "";

					strKey = L"frames";
					if (arrScenResult[i].has_field(strKey))
					{
						vector<CCharacterIdentifyData> vecCharacterIdentifyData;

						json::array arrFrameResult = arrScenResult[i].at(strKey).as_array();

						int nFrameCount = (int)arrFrameResult.size();
						
						int j = 0;
						for (j; j < nFrameCount; j++)
						{
							if (m_bAbort)
								break;

							strKey = L"frameindex";
							if (arrFrameResult[j].has_field(strKey))
								nFrameIdex = arrFrameResult[j].at(strKey).as_integer();

							strKey = L"framepath";
							if (arrFrameResult[j].has_field(strKey))
								stdstrFramePath = utility::conversions::utf16_to_utf8(arrFrameResult[j].at(strKey).as_string());

							pair<int, string> pairFrameIndexNPath;
							pairFrameIndexNPath.first = nFrameIdex;
							pairFrameIndexNPath.second = stdstrFramePath;

							if (m_nError = GetFrameCharIdentifyResultData(&arrFrameResult[j], vecCharacterIdentifyData))
							{
								web::json::json_exception e(L"Failed to get frame character identification result data");
								throw e;
							}

							if ((int)vecCharacterIdentifyData.size() > 0)
							{
								if (m_pFaceAnnotationResult)
								{
									m_pFaceAnnotationResult->m_llSceneID = llSecneID;
									
									map<pair<int, string>, vector<CCharacterIdentifyData>> mapFrameCharIdentifyData;
									mapFrameCharIdentifyData[pairFrameIndexNPath] = vecCharacterIdentifyData;

									while (1)
									{
										if (m_bAbort)
											break;

										g_mutexCharIdentifyData.lock();

										if ((int)m_pFaceAnnotationResult->m_queueFrameCharIdentifyDataQueue.size() < m_nDataCountPerReport)
										{
											m_pFaceAnnotationResult->m_queueFrameCharIdentifyDataQueue.push(mapFrameCharIdentifyData);

											g_mutexCharIdentifyData.unlock();

											break;
										}

										g_mutexCharIdentifyData.unlock();

										Sleep(500);
									}
								}
							}
						}
					}
				}

				if (m_pFaceAnnotationResult)
				{
					m_pFaceAnnotationResult->m_bEndResultData = true;
				}
			}
		}
	}
	catch (web::json::json_exception e)
	{
		if (m_nError != RESTSKSERVICE_ERROR_INVALID_JSON)
			m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}

	m_bWorkingResult = false;
}

string CKRestSKAnnotationService::GetSceneCharIdentifyResultData(bool &bRetEndData)
{
	string stdstrRet;

	if (m_pFaceAnnotationResult)
	{
		stdstrRet = m_pFaceAnnotationResult->GetXmlResultData();
		bRetEndData = m_pFaceAnnotationResult->m_bReportEndData;
	}

	return stdstrRet;
}

//int CKRestSKAnnotationService::GetSceneCharIdentifyResultData(CSKAnnotationResult& retAnnotationResult, int nDataCountPerReport, bool bFromBeginning)
//{
//	////////////// face identification result json ////////////////////////////////////////
//	/*
//	{
//		"jobid": "1111",
//		"title": "NowYouSeeMe",
//		"scenes" : [{
//			"id": 1,
//			"frames":[{
//				"frameindex": 0,
//				"framepath" : "/frames/NowYouSeeMe_/0.jpg",
//				"characters" : [{
//					"conf": 0.9722898937539345,
//					"id" : "",
//					"rect" : {
//						"y": 22,
//						"x" : 710,
//						"w" : 255,
//						"h" : 331
//					},
//					"name": "WoodyHarrelson"
//				}]
//			},
//			{
//				"frameindex": 10,
//				"framepath" : "/frames/NowYouSeeMe_/10.jpg",
//				"characters" : [{
//					"conf": 0.9587138690283279,
//					"id" : "",
//					"rect" : {
//						"y": 17,
//						"x" : 714,
//						"w" : 267,
//						"h" : 344
//					},
//					"name": "WoodyHarrelson"
//				}]
//			}]
//		}]
//	}
//	*/
//	////////////// face identification result json ////////////////////////////////////////
//
//	int nRet = RESTSKSERVICE_ERROR_SUCCESS;
//
//	if (bFromBeginning)
//	{
//		m_nGotCharIdentifyDataCount = 0;
//		m_nCurrentResultSceneIdx = 0;
//	}
//
//	m_nDataCountPerReport = nDataCountPerReport;
//
//	retAnnotationResult.Clear();
//
//	try {
//		json::value* pJsonData = (json::value*)m_pCharIdentifyResultJsonData;
//
//		if (pJsonData)
//		{
//			utility::string_t strKey;
//			strKey = L"scenes";
//
//			if (pJsonData->has_field(strKey))
//			{
//				json::array arrScenResult = pJsonData->at(strKey).as_array();
//				int nSecenCount = (int)arrScenResult.size();
//
//				__int64 llSecneID = 0;
//
//				int i = m_nCurrentResultSceneIdx;
//				for (i; i < nSecenCount; i++)
//				{
//					strKey = L"id";
//					if (arrScenResult[i].has_field(strKey))
//					{
//						llSecneID = arrScenResult[i].at(strKey).as_number().to_int64();
//					}
//
//					int nFrameIdex = 0;
//					string stdstrFramePath = "";
//
//					strKey = L"frames";
//					if (arrScenResult[i].has_field(strKey))
//					{
//						vector<CCharacterIdentifyData> vecCharacterIdentifyData;
//
//						json::array arrFrameResult = arrScenResult[i].at(strKey).as_array();
//
//						int nFrameCount = (int)arrFrameResult.size();
//						
//						int j = m_nGotCharIdentifyDataCount;
//						for (j; j < nFrameCount; j++)
//						{
//							if (j == (m_nGotCharIdentifyDataCount + m_nDataCountPerReport))
//								break;
//
//							strKey = L"frameindex";
//							if (arrFrameResult[j].has_field(strKey))
//								nFrameIdex = arrFrameResult[j].at(strKey).as_integer();
//
//							strKey = L"framepath";
//							if (arrFrameResult[j].has_field(strKey))
//								stdstrFramePath = utility::conversions::utf16_to_utf8(arrFrameResult[j].at(strKey).as_string());
//
//							pair<int, string> pairFrameIndexNPath;
//							pairFrameIndexNPath.first = nFrameIdex;
//							pairFrameIndexNPath.second = stdstrFramePath;
//
//							if (nRet = GetFrameCharIdentifyResultData(&arrFrameResult[j], vecCharacterIdentifyData))
//								return nRet;
//
//							if ((int)vecCharacterIdentifyData.size() > 0)
//							{
//								retAnnotationResult.m_llSceneID = llSecneID;
//								retAnnotationResult.m_mapFrameCharIdentifyData[pairFrameIndexNPath] = vecCharacterIdentifyData;
//							}
//						}
//
//						m_nGotCharIdentifyDataCount = j;
//
//						if (j == nFrameCount)
//						{
//							m_nGotCharIdentifyDataCount = 0;
//							m_nCurrentResultSceneIdx++;
//						}
//					}
//
//					break;
//				}
//			}
//		}
//	}
//	catch (web::json::json_exception e)
//	{
//		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;
//
//		m_stdstrErrMsg.clear();
//		m_stdstrErrMsg.assign(e.what());
//	}
//
//	return nRet;
//}

int CKRestSKAnnotationService::GetFrameCharIdentifyResultData(void* pJsonFrameData, vector<CCharacterIdentifyData>& retFrameCharIdentifyData)
{
	int nRet = RESTSKSERVICE_ERROR_SUCCESS;

	retFrameCharIdentifyData.clear();

	try {
		json::value* pJsonData = (json::value*)pJsonFrameData;

		if (pJsonData)
		{
			utility::string_t strKey;

			strKey = L"characters";
			if (pJsonData->has_field(strKey))
			{
				CCharacterIdentifyData CharacterIdentifyData;

				json::array arrCharaterResult = pJsonData->at(strKey).as_array();

				int nCharResultCount = (int)arrCharaterResult.size();
				for (int i = 0; i < nCharResultCount; i++)
				{
					strKey = L"id";
					if (arrCharaterResult[i].has_field(strKey))
					{
						utility::string_t strCharID = arrCharaterResult[i].at(strKey).as_string();
						CharacterIdentifyData.m_llCharID = _ttoi64((TCHAR*)strCharID.c_str());
					}

					strKey = L"conf";
					if (arrCharaterResult[i].has_field(strKey))
						CharacterIdentifyData.m_dbConfidence = arrCharaterResult[i].at(strKey).as_double();

					strKey = L"rect";
					if (arrCharaterResult[i].has_field(strKey))
					{
						json::value jsonRectData = arrCharaterResult[i].at(strKey);

						strKey = L"x";
						if (jsonRectData.has_field(strKey))
							CharacterIdentifyData.m_nPositionX = jsonRectData.at(strKey).as_integer();

						strKey = L"y";
						if (jsonRectData.has_field(strKey))
							CharacterIdentifyData.m_nPositionY = jsonRectData.at(strKey).as_integer();

						strKey = L"w";
						if (jsonRectData.has_field(strKey))
							CharacterIdentifyData.m_nPositionWidth = jsonRectData.at(strKey).as_integer();

						strKey = L"h";
						if (jsonRectData.has_field(strKey))
							CharacterIdentifyData.m_nPositionHeight = jsonRectData.at(strKey).as_integer();
					}

					strKey = L"name";
					if (arrCharaterResult[i].has_field(strKey))
						CharacterIdentifyData.m_stdstrCharName = utility::conversions::utf16_to_utf8(arrCharaterResult[i].at(strKey).as_string());

					retFrameCharIdentifyData.push_back(CharacterIdentifyData);
				}
			}
		}
	}
	catch (web::json::json_exception e)
	{
		nRet = m_nError = RESTSKSERVICE_ERROR_INVALID_JSON;

		m_stdstrErrMsg.clear();
		m_stdstrErrMsg.assign(e.what());
	}

	return nRet;
}

string CKRestSKAnnotationService::GetXmlExtraValueData(__int64 llAssetID, string stdstrExtraValue, string stdstrJobInfo)
{
	/* Update complete generate frame feature
	<mamex_request>
	  <ex_update_jobresult>
		<workinfoxml>      
		</workinfoxml>
		<assetinfo type="recognizefaceextrainfo">
		  <recognizefaceextrainfo>
		    <assetid>81184</assetid>
		    <extravalue>{"example":"data"}<extravalue>
		  </recognizefaceextrainfo>
		</assetinfo>
	  </ex_update_jobresult>
	</mamex_request>
	*/

	string stdstrRet;
	stdstrRet = Utility::string_format("<mamex_request><ex_update_jobresult>\
									   <workinfoxml>%s</workinfoxml>\
									   <assetinfo type = \"recognizefaceextrainfo\">\
									   <recognizefaceextrainfo><assetid>%I64d</assetid>\
									   <extravalue>%s</extravalue></recognizefaceextrainfo>\
									   </assetinfo></ex_update_jobresult></mamex_request>"
									   , stdstrJobInfo.c_str(), llAssetID, stdstrExtraValue.c_str());

	return stdstrRet;
}

void CKRestSKAnnotationService::SetResultDataCountPerOneReport(int nCount)
{
	m_nReportCount = nCount;
}