
#include "stdafx.h"
#include "KRestRecognizeService.h"
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



const pair<string, string> g_ObjectDetection("POST", "/object_detection/");

CKRestRecognizeService::CKRestRecognizeService(void)
{
	m_nError = 0;
	m_nMaxLabelResults = 0;

	m_fMinScore = 0.0;
}

CKRestRecognizeService::~CKRestRecognizeService(void)
{

}

void CKRestRecognizeService::SetServiceInfo(string stdstrServiceUri, float fMinScore)
{
	m_stdstrServiceUri = stdstrServiceUri;
	m_fMinScore = fMinScore;
}

void CKRestRecognizeService::SetMaxLabelResult(int nMax)
{
	m_nMaxLabelResults = nMax;
}

int CKRestRecognizeService::LabelDetectionOnImage(string stdstrImageFilePath)
{
	int nRet = 0, nMaxResults = 0;

	nMaxResults = m_nMaxLabelResults;
	if (nMaxResults == 0)
		nMaxResults = DEFAULT_MAX_LABLE_RESULTS;

	string stdstrReqBody;
	printf("request - %s(%s)\n", g_ObjectDetection.second.c_str(), stdstrReqBody.c_str());
	string stdstrQuery;

	m_nError = 0;
	m_stdstrErrMsg = "";
	m_stdstrResponse = "";

	CRestClient requestClient(m_stdstrServiceUri);

	nRet = requestClient.RequestUploadFileWithMultipartFormdata(g_ObjectDetection.first, g_ObjectDetection.second, stdstrImageFilePath, stdstrReqBody);
	if ((nRet/100) == 2)
	{
		// status ok
		m_stdstrResponse = requestClient.GetResponseData();

		nRet = RESTRECOGNIZESERVICE_ERROR_SUCCESS;
	}
	else
	{
		nRet = m_nError = RESTRECOGNIZESERVICE_ERROR_HTTP_STATUS;
		m_stdstrErrMsg = requestClient.GetReason();
	}

	return nRet;
}

string CKRestRecognizeService::GetServiceMessage(int& nRetError)
{
	nRetError = m_nError;

	return m_stdstrErrMsg;
}

string CKRestRecognizeService::GetStrResponse()
{
	return m_stdstrResponse;
}