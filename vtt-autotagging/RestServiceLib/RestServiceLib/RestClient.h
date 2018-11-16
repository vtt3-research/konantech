#pragma once

#include <map>
#include <vector>

using namespace std;

#ifdef RESTCLIENT_EXPORTS
#define RESTCLIENT_API __declspec(dllexport)
#else
#define RESTCLIENT_API __declspec(dllimport)
#endif

class RESTCLIENT_API CRestClient
{
public:
	typedef int(*RESPONSECB)(int nStatusCode, string stdstrReason, string stdstrBodyJson, void* pData);

public:
	CRestClient();
	CRestClient(string stdstrUri);
	CRestClient(string stdstrUri, string stdstrAuthKey, string stdstrAuthValue);

	virtual ~CRestClient();

	int Request(string stdstrMethod, string stdstrPath, string stdstrBody);
	int Request(string stdstrMethod, string stdstrPath, string stdstrQuery, string stdstrBody);
	int RequestUploadFileWithMultipartFormdata(string stdstrMethod, string stdstrPath, string stdstrFilePath, string stdstrBodyText);
	int RequestUploadFileWithMultipartFormdata(string stdstrMethod, string stdstrPath, string stdstrFilePath, string stdstrParamName, string stdstrParamData);

	int RequestDownloadFile(string stdstrMethod, string stdstrPath, string stdstrSaveFilePath, string stdstrBodyText);
	int RequestDownloadFileWithMultipartFormdata(string stdstrMethod, string stdstrPath, string stdstrSaveFilePath, string stdstrBodyText);

	string MakeRequestPath(pair<string, string>& pairMethodNPath, vector<string>& vecVars);

	void SetResponseCB(string stdstrMethod, string stdstrPath, RESPONSECB pFuncCB, void* pDataCB);
	void SetServerUri(string stdstrUri);

	string GetResponseData() { return m_stdstrResponseJsonData; };
	string GetReason() { return m_stdstrReason; };

protected:
	void Clear();

	void SetInterfaceVarNConstDepth(pair<string, string>& pairMethodNPath, string stdstrPath, int nDepth);
	void GetPairMethodNPath(string stdstrMethod, string stdstrReceivedPath, pair<string, string>& pairMethodNPath
		, map<string, string>& retMapPathVarValue);
	void GetPathDepth(string stdstrPath, vector<pair<string, string>>& retVecDepthPair);
	bool ComparePathDepth(string stdstrReceivedPath, vector<pair<string, string>>& vecDepthPair
		, map<string, string>& retMapPathVarValue);

	void ParseDownloadFileDataWithResponseData(void* pData, string stdstrSaveFilePath);

protected:
	string m_stdstrUri;
	string m_stdstrUriPath;
	string m_stdstrReason;
	string m_stdstrResponseJsonData;

	string m_stdstrMethod;
	string m_stdstrQuery;
	string m_stdstrRequestJsonData;

	string m_stdstrAuthKey;
	string m_stdstrAuthValue;

	int m_nStatusCode;

	// <'method', 'path'> pair
	map<pair<string, string>, RESPONSECB> m_mapFuncCB;
	map<pair<string, string>, void*> m_mapDataCB;
	// <'method', 'path'> pair
	// <'type', 'depth name'> pair
	map<pair<string, string>, vector<pair<string, string>>> m_mapDepth;
};

