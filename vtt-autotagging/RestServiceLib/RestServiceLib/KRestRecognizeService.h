// KRestRecognizeService.h : CKRestRecognizeService 헤더 파일입니다.
//
#pragma once

//#include <map>
#include <vector>

using namespace std;

#ifdef RESTRECOGNIZESERVICE_EXPORTS
#define RESTRECOGNIZESERVICE_API __declspec(dllexport)
#else
#define RESTRECOGNIZESERVICE_API __declspec(dllimport)
#endif

#define RESTRECOGNIZESERVICE_ERROR_SUCCESS		 (0)
#define RESTRECOGNIZESERVICE_ERROR_SIZE			 (300001)
#define RESTRECOGNIZESERVICE_ERROR_INVALID_JSON	 (300002)
#define RESTRECOGNIZESERVICE_ERROR_HTTP_STATUS	 (300003)				

class CRestClient;

class RESTRECOGNIZESERVICE_API CKRestRecognizeService
{
public:
	CKRestRecognizeService(void);
	virtual ~CKRestRecognizeService(void);

	void SetServiceInfo(string stdstrServiceUri, float fMinScore = 0.0);
	void SetMaxLabelResult(int nMax);

	int LabelDetectionOnImage(string stdstrImageFilePath);

	string GetServiceMessage(int& nRetError);
	string GetStrResponse();

private:
	int m_nError;
	int m_nMaxLabelResults;

	float m_fMinScore;

	string m_stdstrServiceUri;
	string m_stdstrErrMsg;

	string m_stdstrResponse;
};
