// KRestSogangService.h : KRestSogangService 헤더 파일입니다.
//
#pragma once

//#include <map>
#include <vector>

using namespace std;

#ifdef RESTSOGANGSERVICE_EXPORTS
#define RESTSOGANGSERVICE_API __declspec(dllexport)
#else
#define RESTSOGANGSERVICE_API __declspec(dllimport)
#endif

#define RESTSOGANGSERVICE_ERROR_SUCCESS		 (0)
#define RESTSOGANGSERVICE_ERROR_SIZE			 (200001)
#define RESTSOGANGSERVICE_ERROR_INVALID_JSON	 (200002)
#define RESTSOGANGSERVICE_ERROR_HTTP_STATUS	 (200003)				

class CRestClient;

class RESTSOGANGSERVICE_API CKRestSogangService
{
public:
	CKRestSogangService(void);
	virtual ~CKRestSogangService(void);

	void SetServiceInfo(string stdstrServiceUri, float fMinScore = 0.0);
	void SetMaxLabelResult(int nMax);

	int LabelDetectionOnImage(string stdstrImageFilePath);
	int LabelDetectionOnImageWithModuleName(string stdstrImageFilePath, string stdstrModuleName);
	//vector<string> GetLabelDescriptions();
	//vector<pair<string, double>> GetLabelDescriptionsScore();

	string GetServiceMessage(int& nRetError);
	string GetStrResponse();

//protected:
//	char* ConvertImageToBase64CharBuff(string stdstrImageFilePath);

private:
	int m_nError;
	int m_nMaxLabelResults;

	float m_fMinScore;

	string m_stdstrServiceUri;
	string m_stdstrErrMsg;

	string m_stdstrResponse;
};
