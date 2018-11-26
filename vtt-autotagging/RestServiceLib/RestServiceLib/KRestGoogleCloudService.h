// KRestGoogleCloudService.h : KRestGoogleCloudService 헤더 파일입니다.
//
#pragma once

//#include <map>
#include <vector>

using namespace std;

#ifdef RESTGOOGLECLOUDSERVICE_EXPORTS
#define RESTGOOGLECLOUDSERVICE_API __declspec(dllexport)
#else
#define RESTGOOGLECLOUDSERVICE_API __declspec(dllimport)
#endif

#define RESTGOOGLECLOUDSERVICE_ERROR_SUCCESS		 (0)
#define RESTGOOGLECLOUDSERVICE_ERROR_SIZE			 (200001)
#define RESTGOOGLECLOUDSERVICE_ERROR_INVALID_JSON	 (200002)
#define RESTGOOGLECLOUDSERVICE_ERROR_HTTP_STATUS	 (200003)				

class CRestClient;

class RESTGOOGLECLOUDSERVICE_API CKRestGoogleCloudService
{
public:
	CKRestGoogleCloudService(void);
	virtual ~CKRestGoogleCloudService(void);

	void SetServiceInfo(string stdstrServiceUri, string stdstrKey, float fMinScore = 0.0);
	void SetMaxLabelResult(int nMax);

	int LabelDetectionOnImage(string stdstrImageFilePath);
	vector<string> GetLabelDescriptions();
	vector<pair<string, double>> GetLabelDescriptionsScore();

	string GetServiceMessage(int& nRetError);
	string GetResponseData() { return m_stdstrResponse; };

protected:
	char* ConvertImageToBase64CharBuff(string stdstrImageFilePath);

private:
	int m_nError;
	int m_nMaxLabelResults;

	float m_fMinScore;

	string m_stdstrServiceUri;
	string m_stdstrKey;
	string m_stdstrErrMsg;

	string m_stdstrResponse;
};
