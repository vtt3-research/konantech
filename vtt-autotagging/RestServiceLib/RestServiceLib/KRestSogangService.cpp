
#include "stdafx.h"
#include "KRestSogangService.h"
#include "RestClient.h"
#include "Utility.h"

#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
//#include <mutex>

#include <cpprest/json.h>

using namespace utility;
using namespace web;

#pragma warning (disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_MAX_LABLE_RESULTS					(5)



const pair<string, string> g_AnalyzerAll("POST", "/analyzer/");

CKRestSogangService::CKRestSogangService(void)
{
	m_nError = 0;
	m_nMaxLabelResults = 0;

	m_fMinScore = 0.0;
}

CKRestSogangService::~CKRestSogangService(void)
{

}

void CKRestSogangService::SetServiceInfo(string stdstrServiceUri, float fMinScore)
{
	m_stdstrServiceUri = stdstrServiceUri;
	m_fMinScore = fMinScore;
}

void CKRestSogangService::SetMaxLabelResult(int nMax)
{
	m_nMaxLabelResults = nMax;
}

int CKRestSogangService::LabelDetectionOnImage(string stdstrImageFilePath)
{
	int nRet = 0, nMaxResults = 0;

	nMaxResults = m_nMaxLabelResults;
	if (nMaxResults == 0)
		nMaxResults = DEFAULT_MAX_LABLE_RESULTS;

	//char* pContent = NULL;
	//pContent = ConvertImageToBase64CharBuff(stdstrImageFilePath);

	//if (!pContent)
	//	return m_nError;

	string stdstrReqBody;
	///*
	//stdstrReqBody = Utility::string_format(
	//	"{\
	//		\"requests\":[\
	//			{\
	//			   \"image\":{\
	//				   \"content\":\"%s\"\
	//			   },\
	//			   \"features\":[\
	//				   {\
	//					 \"type\":\"LABEL_DETECTION\",\
	//					 \"maxResults\": %d\
	//				   }\
	//			   ]\
	//			}\
	//		]\
	//	}", pContent, nMaxResults);
	//*/
	//if (pContent)
	//{
	//	delete[]pContent;
	//	pContent = NULL;
	//}

	//string stdstrQuery = Utility::string_format("key=%s", m_stdstrKey.c_str());
	printf("request - %s(%s)\n", g_AnalyzerAll.second.c_str(), stdstrReqBody.c_str());
	string stdstrQuery;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	CRestClient requestClient(m_stdstrServiceUri);

	//nRet = requestClient.Request(g_AnalyzerAll.first, g_AnalyzerAll.second, stdstrQuery, stdstrReqBody);
	//nRet = requestClient.Request(g_AnalyzerAll.first, g_AnalyzerAll.second, stdstrReqBody);
	nRet = requestClient.RequestUploadFileWithMultipartFormdata(g_AnalyzerAll.first, g_AnalyzerAll.second, stdstrImageFilePath, stdstrReqBody);
	if ((nRet/100) == 2)
	{
		// status ok
		m_stdstrResponse = requestClient.GetResponseData();

		nRet = RESTSOGANGSERVICE_ERROR_SUCCESS;
	}
	else
	{
		nRet = m_nError = RESTSOGANGSERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	return nRet;
}

int CKRestSogangService::LabelDetectionOnImageWithModuleName(string stdstrImageFilePath, string stdstrModuleName)
{
	int nRet = 0, nMaxResults = 0;

	nMaxResults = m_nMaxLabelResults;
	if (nMaxResults == 0)
		nMaxResults = DEFAULT_MAX_LABLE_RESULTS;

	string stdstrReqBody;
	
	printf("request - %s(%s)\n", g_AnalyzerAll.second.c_str(), stdstrReqBody.c_str());
	string stdstrQuery;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	CRestClient requestClient(m_stdstrServiceUri);

	nRet = requestClient.RequestUploadFileWithMultipartFormdata(g_AnalyzerAll.first, g_AnalyzerAll.second, stdstrImageFilePath, "modules", stdstrModuleName);
	if ((nRet / 100) == 2)
	{
		// status ok
		m_stdstrResponse = requestClient.GetResponseData();

		nRet = RESTSOGANGSERVICE_ERROR_SUCCESS;
	}
	else
	{
		nRet = m_nError = RESTSOGANGSERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	return nRet;
}

//#include <cpprest/http_client.h>
//#include <cpprest/http_msg.h>
//#include <cpprest/base_uri.h>

//using namespace web::http;                  // Common HTTP functionality
//using namespace web::http::client;          // HTTP client features
//using namespace concurrency::streams;       // Asynchronous streams

//vector<string> CKRestSogangService::GetLabelDescriptions()
//{
//	vector<string> vecRetLableDescription;
//
//	//{"responses":[{"labelAnnotations":[{"description":"professional","mid":"/m/01qkbx","score":0.66083440000000004},{"description":"technology","mid":"/m/07c1v","score":0.65353130000000004},{"description":"official","mid":"/m/035y33","score":0.63041157000000003},{"description":"job","mid":"/m/04115t2","score":0.62610334000000001},{"description":"product","mid":"/m/01jwgf","score":0.58517739999999996}]}]}
//	//string stdstrResponse = "{\"responses\":[{\"labelAnnotations\":[{\"description\":\"professional\",\"mid\":\"/m/01qkbx\",\"score\":0.66083440000000004},{\"description\":\"technology\",\"mid\":\"/m/07c1v\",\"score\":0.65353130000000004},{\"description\":\"official\",\"mid\":\"/m/035y33\",\"score\":0.63041157000000003},{\"description\":\"job\",\"mid\":\"/m/04115t2\",\"score\":0.62610334000000001},{\"description\":\"product\",\"mid\":\"/m/01jwgf\",\"score\":0.58517739999999996}]}]}";
//	//stdstrResponse = Utility::string_format("\
//		//	{\
//		//		\"responses\":[\
//		//			{\
//		//				\"labelAnnotations\":[\
//		//					{\
//		//						\"score\":0.97231364,\
//		//						\"mid\":\"/m/05xh2\",\
//		//						\"description\":\"pyramid\"\
//		//					}\
//		//				]\
//		//			}\
//		//		]\
//		//	}");
//
//	//m_stdstrResponse = stdstrResponse;
//
//	if (!m_stdstrResponse.empty())
//	{
//		utility::string_t strKey;
//		utility::string_t strResponse = utility::conversions::utf8_to_utf16(m_stdstrResponse.c_str());
//
//		try
//		{
//			//json::value jsonData = json::value::parse(stdstrData);
//			//m_nCheckInID = jsonData[L"compserverid"].as_integer();
//
//			json::value jsonResponse = json::value::parse(strResponse);
//
//			strKey = L"responses";
//			if (!jsonResponse.has_field(strKey))
//			{
//				web::json::json_exception e(L"'responses' is not exist.");
//
//				throw e;
//			}
//
//			json::array jsonArrAnnotations = jsonResponse.at(strKey).as_array();
//			if (jsonArrAnnotations.size() == 0)
//			{
//				web::json::json_exception e(L"'responses' is empty.");
//
//				throw e;
//			}
//
//			strKey = L"labelAnnotations";
//			for (int i = 0; i < (int)jsonArrAnnotations.size(); i++)
//			{
//				if (jsonArrAnnotations[i].has_field(strKey))
//				{
//					json::array jsonArrLabelAnnotations = jsonArrAnnotations[i].at(strKey).as_array();
//					if (jsonArrAnnotations.size() == 0)
//					{
//						web::json::json_exception e(L"'labelAnnotations' is empty.");
//
//						throw e;
//					}
//
//					double dbScore = 0.0;
//					utility::string_t strDescription;
//					utility::string_t strScore;
//					for (int j = 0; j < (int)jsonArrLabelAnnotations.size(); j++)
//					{
//						strKey = L"description";
//						if (jsonArrLabelAnnotations[j].has_field(strKey))
//						{
//							strDescription = jsonArrLabelAnnotations[j].at(strKey).as_string();
//						}
//
//						strKey = L"score";
//						if (jsonArrLabelAnnotations[j].has_field(strKey))
//						{
//							dbScore = jsonArrLabelAnnotations[j].at(strKey).as_double();
//						}
//
//						if (dbScore >= (double)m_fMinScore)
//						{
//							string stdstrDescription = utility::conversions::utf16_to_utf8(strDescription);
//							vecRetLableDescription.push_back(stdstrDescription);
//						}
//					}
//				}
//			}
//		}
//		catch (web::json::json_exception const & e)
//		{
//			m_nError = RESTSOGANGSERVICE_ERROR_INVALID_JSON;
//
//			m_stdstrErrMsg.clear();
//			m_stdstrErrMsg.assign(e.what());
//		}
//	}
//
//	return vecRetLableDescription;
//}
//
//vector<pair<string, double>> CKRestSogangService::GetLabelDescriptionsScore()
//{
//	vector<pair<string, double>> vecRetLableDescriptionScore;
//
//	if (!m_stdstrResponse.empty())
//	{
//		utility::string_t strKey;
//		utility::string_t strResponse = utility::conversions::utf8_to_utf16(m_stdstrResponse.c_str());
//
//		try
//		{
//			json::value jsonResponse = json::value::parse(strResponse);
//
//			strKey = L"responses";
//			if (!jsonResponse.has_field(strKey))
//			{
//				web::json::json_exception e(L"'responses' is not exist.");
//
//				throw e;
//			}
//
//			json::array jsonArrAnnotations = jsonResponse.at(strKey).as_array();
//			if (jsonArrAnnotations.size() == 0)
//			{
//				web::json::json_exception e(L"'responses' is empty.");
//
//				throw e;
//			}
//
//			strKey = L"labelAnnotations";
//			for (int i = 0; i < (int)jsonArrAnnotations.size(); i++)
//			{
//				if (jsonArrAnnotations[i].has_field(strKey))
//				{
//					json::array jsonArrLabelAnnotations = jsonArrAnnotations[i].at(strKey).as_array();
//					if (jsonArrAnnotations.size() == 0)
//					{
//						web::json::json_exception e(L"'labelAnnotations' is empty.");
//
//						throw e;
//					}
//
//					double dbScore = 0.0;
//					utility::string_t strDescription;
//					utility::string_t strScore;
//					for (int j = 0; j < (int)jsonArrLabelAnnotations.size(); j++)
//					{
//						strKey = L"description";
//						if (jsonArrLabelAnnotations[j].has_field(strKey))
//						{
//							strDescription = jsonArrLabelAnnotations[j].at(strKey).as_string();
//						}
//
//						strKey = L"score";
//						if (jsonArrLabelAnnotations[j].has_field(strKey))
//						{
//							dbScore = jsonArrLabelAnnotations[j].at(strKey).as_double();
//						}
//
//						if (dbScore >= (double)m_fMinScore)
//						{
//							string stdstrDescription = utility::conversions::utf16_to_utf8(strDescription);
//							pair<string, double> pairDescriptionScore(stdstrDescription, dbScore);
//							vecRetLableDescriptionScore.push_back(pairDescriptionScore);
//						}
//					}
//				}
//			}
//		}
//		catch (web::json::json_exception const & e)
//		{
//			m_nError = RESTSOGANGSERVICE_ERROR_INVALID_JSON;
//
//			m_stdstrErrMsg.clear();
//			m_stdstrErrMsg.assign(e.what());
//		}
//	}
//
//	return vecRetLableDescriptionScore;
//}

string CKRestSogangService::GetServiceMessage(int& nRetError)
{
	nRetError = m_nError;

	return m_stdstrErrMsg;
}

string CKRestSogangService::GetStrResponse()
{
	return m_stdstrResponse;
}

//char* CKRestSogangService::ConvertImageToBase64CharBuff(string stdstrImageFilePath)
//{
//	char* pRetBuff = NULL;
//	char* pImageBuff = NULL;
//
//	int hReadFile = -1;
//
//	bool bError = false;
//
//	CString strFilePath;
//	Utility::CConvertUTF8Operation::ConvertUTF8ToString((char*)stdstrImageFilePath.c_str(), strFilePath);
//
//	struct _stati64 stat;
//
//	do
//	{
//		int nRet = _tstati64(strFilePath, &stat);
//		if (nRet == -1)
//		{
//			m_nError = ::GetLastError();
//			m_stdstrErrMsg = Utility::string_format("_tstati64 (%s)", stdstrImageFilePath.c_str());
//
//			bError = true;
//
//			break;
//		}
//
//		if (stat.st_size == 0)
//		{
//			m_nError = RESTSOGANGSERVICE_ERROR_SIZE;
//			m_stdstrErrMsg = Utility::string_format("_tstati64 (%s)", stdstrImageFilePath.c_str());
//
//			bError = true;
//
//			break;
//		}
//
//		pImageBuff = new char[(int)stat.st_size];
//
//		hReadFile = _topen(strFilePath, _O_BINARY, _S_IREAD);
//		if (hReadFile == -1)
//		{
//			m_nError = ::GetLastError();
//			m_stdstrErrMsg = Utility::string_format("_open (%s)", stdstrImageFilePath.c_str());
//
//			bError = true;
//
//			break;
//		}
//
//		int nRead = _read(hReadFile, pImageBuff, (int)stat.st_size);
//		if (nRead <= 0)
//		{
//			m_nError = ::GetLastError();
//			m_stdstrErrMsg = Utility::string_format("_read (%s)", stdstrImageFilePath.c_str());
//
//			bError = true;
//
//			break;
//		}
//	} while (0);
//
//	if (hReadFile > 0)
//	{
//		_close(hReadFile);
//		hReadFile = -1;
//	}
//
//	if (!bError)
//	{
//		int nResultLength = ((int)stat.st_size + 2 - (((int)stat.st_size + 2) % 3)) / 3 * 4;
//		pRetBuff = new char[nResultLength + 10];
//		memset(pRetBuff, 0, nResultLength);
//		Utility::encode_base64((unsigned char*)pRetBuff, (unsigned char*)pImageBuff, (int)stat.st_size);
//	}
//	else
//		printf("%s\n", m_stdstrErrMsg.c_str());
//
//	if (pImageBuff)
//	{
//		delete[]pImageBuff;
//		pImageBuff = NULL;
//	}
//
//	return pRetBuff;
//}
