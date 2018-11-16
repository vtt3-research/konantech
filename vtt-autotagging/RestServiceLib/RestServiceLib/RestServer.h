#pragma once

#include <map>
#include <vector>

using namespace std;

#ifdef RESTSERVER_EXPORTS
#define RESTSERVER_API __declspec(dllexport)
#else
#define RESTSERVER_API __declspec(dllimport)
#endif

class RESTSERVER_API CRestServer
{
public:
	typedef int(*REQUESTCB)(map<string, string>& mapPathVarValue, string stdstrQuery, string stdstrBodyJson, void* pData, string& stdstrResultJon);

public:
	CRestServer();
	CRestServer(string stdstrUri);

	virtual ~CRestServer();

	int Start();
	int Stop();

	bool IsRunning();

	void SetServiceUri(string stdstrUri);
	void SetListenerClosed();

	void SetRequestCB(string stdstrMethod, string stdstrInterfaceName, REQUESTCB pFuncCB, void* pDataCB);

	int GetServicePort();
	void* GetListener();

	//static void gGetPairMethodNPath(string stdstrMethod, string stdstrReceivedPath, pair<string, string>& pairMethodNPath, map<string, string>& retMapPathVarValue);
	//static void gGetPathDepth(string stdstrPath, vector<pair<string, string>>& retVecDepthPair);
	//static bool gComparePathDepth(string stdstrReceivedPath, vector<pair<string, string>>& vecDepthPair, map<string, string>& retMapPathVarValue);

	void GetPairMethodNPath(string stdstrMethod, string stdstrReceivedPath, pair<string, string>& pairMethodNPath, map<string, string>& retMapPathVarValue);
	void GetPathDepth(string stdstrPath, vector<pair<string, string>>& retVecDepthPair);
	bool ComparePathDepth(string stdstrReceivedPath, vector<pair<string, string>>& vecDepthPair, map<string, string>& retMapPathVarValue);

	REQUESTCB GetRequestCB(pair<string, string> pairMethodNPath, void** pretData = NULL);

protected:
	int ReceivePing();
	int ReceiveJob();
	int ReceiveJobCancel();

	void SetInterfaceVarNConstDepth(pair<string, string>& pairMethodNPath, string stdstrInterFace, int nDepth);

protected:
	string m_stdstrUri;

	int m_nPort;

	bool m_bStarted;
	bool m_bStopped;

	void* m_pListener;

	// <'method', 'path'> pair
	map<pair<string, string>, REQUESTCB> m_mapFuncCB;
	map<pair<string, string>, void*> m_mapDataCB;
	// <'method', 'path'> pair
	// <'type', 'depth name'> pair
	map<pair<string, string>, vector<pair<string, string>>> m_mapDepth;

//public:
//	// <'method', 'path'> pair
//	static map<pair<string, string>, REQUESTCB> g_mapFuncCB;
//	static map<pair<string, string>, void*> g_mapDataCB;
//	// <'method', 'path'> pair
//	// <'type', 'depth name'> pair
//	static map<pair<string, string>, vector<pair<string, string>>> g_mapDepth;
//	static map<CRestServer*, string> g_mapServer;
};

