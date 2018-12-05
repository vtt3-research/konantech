#include "stdafx.h"
#include "RestServer.h"

#include "Utility.h"

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/filestream.h>
#include <cpprest/http_headers.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace concurrency::streams;       // Asynchronous streams

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <chrono>
#include <thread>
#include <array>
#include <mutex>

#define DEPTH_TYPE_OF_PATH_VAR						"VAR"
#define DEPTH_TYPE_OF_PATH_CONST					"CONST"

#define METHOD_GET									"GET"
#define METHOD_POST									"POST"
#define METHOD_PUT									"PUT"
#define METHOD_DELETE								"DELETE"

//string CRestServer::m_stdstrUri;
//map<pair<string, string>, CRestServer::REQUESTCB> CRestServer::g_mapFuncCB;
//map<pair<string, string>, void*> CRestServer::g_mapDataCB;
//map<pair<string, string>, vector<pair<string, string>>> CRestServer::g_mapDepth;
map<CRestServer*, string> g_mapServer;
map<string, vector<string>> g_mapHostAlias;

mutex gMutexSyncRestServer;

//#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"
//
//map<utility::string_t, utility::string_t> dictionary;
//map<utility::string_t, array<void*, 2>> dictionary2;
//map<utility::string_t, map<utility::string_t, array<void*, 2>>> dictionary3;

CRestServer* GetRequestedServer(utility::string_t host);

void handle_get(http_request request);
void handle_post(http_request request);
void handle_put(http_request request);
void handle_del(http_request request);
void handle_request(http_request request, string stdstrMethod
	, function<void(json::value&, json::value&, status_code&, string&, map<string, string>&, CRestServer::REQUESTCB, void*)> action);


CRestServer::CRestServer()
{
	gMutexSyncRestServer.lock();
	g_mapServer[this] = "";
	gMutexSyncRestServer.unlock();

	m_bStarted = false;
	m_bStopped = true;
	m_pListener = NULL;
	m_nPort = 0;
}

CRestServer::CRestServer(string stdstrUri)
{
	gMutexSyncRestServer.lock();
	g_mapServer[this] = stdstrUri;
	gMutexSyncRestServer.unlock();

	m_stdstrUri = stdstrUri;
	m_bStarted = false;
	m_bStopped = true;
	
	utility::string_t serverUri = utility::conversions::utf8_to_utf16(m_stdstrUri);
	http::uri uriAddress(serverUri);

	m_nPort = uriAddress.port();
	
	if (m_nPort == 0)
		m_nPort = 80;
}

CRestServer::~CRestServer()
{
	if (!m_bStopped)
		Stop();

	if (m_pListener)
	{
		http_listener* pListener = (http_listener*)m_pListener;
		delete pListener;
		m_pListener = NULL;
	}

	//g_mapFuncCB.clear();
	//g_mapDataCB.clear();
	//g_mapDepth.clear();

	gMutexSyncRestServer.lock();
	g_mapServer.erase(this);
	gMutexSyncRestServer.unlock();
}

int CRestServer::Start()
{
	int nRet = 0;

	utility::string_t serverUri = utility::conversions::utf8_to_utf16(m_stdstrUri);

	http::uri uriAddress(serverUri);

	m_pListener = new http_listener(uriAddress);

	((http_listener*)m_pListener)->support(methods::GET, handle_get);
	((http_listener*)m_pListener)->support(methods::POST, handle_post);
	((http_listener*)m_pListener)->support(methods::PUT, handle_put);
	((http_listener*)m_pListener)->support(methods::DEL, handle_del);

	try
	{
		//listener
		//	.open()
		//	.then([&listener](){TRACE(L"\nStarting to listen\n"); })
		//	.wait();
		((http_listener*)m_pListener)->open()
			.then([]()
		{
			printf("\nStarting to listen\n"); 
		})
			.wait();

		m_bStarted = true;
		m_bStopped = false;

		pplx::task<void> closeTask([this]()
		{
			while (1)
			{
				if (!this->IsRunning())
				//if (!*pbStarted)
				{
					//pplx::task<void> listnerCloseTask = listener.close();
					http_listener* pListener = (http_listener*)this->GetListener();
					if (pListener)
					{
						pplx::task<void> listnerCloseTask = pListener->close();
						listnerCloseTask.wait();
					}

					//*pbStopped = true;
					this->SetListenerClosed();

					break;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		});
	}
	catch (exception const & e)
	{
		wcout << e.what() << endl;
	}

	return nRet;
}

int CRestServer::Stop()
{
	int nRet = 0;

	if (m_bStarted)
	{
		m_bStarted = false;
	}

	while (1)
	{
		if (m_bStopped)
		{
			printf("\nClose listening\n");

			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return nRet;
}

bool CRestServer::IsRunning()
{
	bool bRet = false;

	if (m_bStarted)
		bRet = true;

	return bRet;
}

void CRestServer::SetServiceUri(string stdstrUri)
{
	gMutexSyncRestServer.lock();
	g_mapServer[this] = stdstrUri;
	gMutexSyncRestServer.unlock();

	m_stdstrUri = stdstrUri;

	utility::string_t serverUri = utility::conversions::utf8_to_utf16(m_stdstrUri);
	http::uri uriAddress(serverUri);

	m_nPort = uriAddress.port();

	if (m_nPort == 0)
		m_nPort = 80;
}

void* CRestServer::GetListener()
{
	return m_pListener;
}

int CRestServer::GetServicePort()
{
	return m_nPort;
}

void CRestServer::SetListenerClosed()
{
	m_bStopped = true;
}

void CRestServer::SetRequestCB(string stdstrMethod, string stdstrInterfaceName, REQUESTCB pFuncCB, void* pDataCB)
{
	string stdstrPath = stdstrInterfaceName;

	pair<string, string> pairMethodNPath(stdstrMethod, stdstrPath);

	//CRestServer::g_mapFuncCB[pairMethodNPath] = pFuncCB;
	//CRestServer::g_mapDataCB[pairMethodNPath] = pDataCB;

	m_mapFuncCB[pairMethodNPath] = pFuncCB;
	m_mapDataCB[pairMethodNPath] = pDataCB;

	SetInterfaceVarNConstDepth(pairMethodNPath, stdstrInterfaceName, 0);
}

void CRestServer::SetInterfaceVarNConstDepth(pair<string, string>& pairMethodNPath, string stdstrInterFace, int nDepth)
{
	int nVariableStartPos = stdstrInterFace.find("/{");
	int nVariableEndPos = stdstrInterFace.find("}");

	int nConstStartPos = stdstrInterFace.find("/");
	int nConstEndPos = 0;

	string stdstrRemainInterface;
	//vector<pair<string, string>> vecDepth = g_mapDepth[pairMethodNPath];
	vector<pair<string, string>> vecDepth = m_mapDepth[pairMethodNPath];

	if (nVariableStartPos == 0)
	{
		string stdstrVarName = stdstrInterFace.substr(0, nVariableEndPos + 1);
		pair<string, string> pairVar(DEPTH_TYPE_OF_PATH_VAR, stdstrVarName);
		vecDepth.push_back(pairVar);

		stdstrRemainInterface = stdstrInterFace.substr(nVariableEndPos + 1);
	}
	else if (nConstStartPos == 0)
	{
		stdstrRemainInterface = stdstrInterFace.substr(1);
		nConstEndPos = stdstrRemainInterface.find("/");

		string stdstrConstName;

		if (nConstEndPos >= 0)
		{
			stdstrConstName = stdstrInterFace.substr(0, nConstEndPos + 1);
			stdstrRemainInterface = stdstrInterFace.substr(nConstEndPos + 1);
		}
		else
			stdstrConstName = stdstrInterFace;

		pair<string, string> pairConst(DEPTH_TYPE_OF_PATH_CONST, stdstrConstName);
		vecDepth.push_back(pairConst);
	}
	else
		return;

	//g_mapDepth[pairMethodNPath] = vecDepth;
	m_mapDepth[pairMethodNPath] = vecDepth;

	SetInterfaceVarNConstDepth(pairMethodNPath, stdstrRemainInterface, nDepth + 1);

	return;
}

//void CRestServer::GetPairMethodNPath(string stdstrMethod, string stdstrReceivedPath, pair<string, string>& pairMethodNPath
//	, map<string, string>& retMapPathVarValue)
//{
//	if (g_mapDepth.size() > 0)
//	{
//		vector<pair<string, string>> vecPair;
//
//		map<pair<string, string>, vector<pair<string, string>>>::iterator it = g_mapDepth.begin();
//		for (it; it != g_mapDepth.end(); it++)
//		{
//			vecPair.clear();
//
//			if (!it->first.first.compare(stdstrMethod))
//			{
//				vecPair = it->second;
//				if (!ComparePathDepth(stdstrReceivedPath, vecPair, retMapPathVarValue))
//				{
//					pairMethodNPath = it->first;
//					break;
//				}
//			}
//		}
//	}
//
//	return;
//}
//
//void CRestServer::GetPathDepth(string stdstrPath, vector<pair<string, string>>& retVecDepthPair)
//{
//	int nVariableStartPos = stdstrPath.find("/{");
//	int nVariableEndPos = stdstrPath.find("}");
//
//	int nConstStartPos = stdstrPath.find("/");
//	int nConstEndPos = 0;
//
//	string stdstrRemainPath;
//
//	if (nVariableStartPos == 0)
//	{
//		string stdstrVarName = stdstrPath.substr(0, nVariableEndPos + 1);
//		pair<string, string> pairVar(DEPTH_TYPE_OF_PATH_VAR, stdstrVarName);
//		retVecDepthPair.push_back(pairVar);
//
//		stdstrRemainPath = stdstrPath.substr(nVariableEndPos + 1);
//	}
//	else if (nConstStartPos == 0)
//	{
//		stdstrRemainPath = stdstrPath.substr(1);
//		nConstEndPos = stdstrRemainPath.find("/");
//
//		string stdstrConstName;
//
//		if (nConstEndPos >= 0)
//		{
//			stdstrConstName = stdstrPath.substr(0, nConstEndPos + 1);
//			stdstrRemainPath = stdstrPath.substr(nConstEndPos + 1);
//		}
//		else
//			stdstrConstName = stdstrPath;
//
//		pair<string, string> pairConst(DEPTH_TYPE_OF_PATH_CONST, stdstrConstName);
//		retVecDepthPair.push_back(pairConst);
//	}
//	else
//		return;
//
//	CRestServer::GetPathDepth(stdstrRemainPath, retVecDepthPair);
//
//	return;
//}

//bool CRestServer::ComparePathDepth(string stdstrReceivedPath, vector<pair<string, string>>& vecDepthPair
//	, map<string, string>& retMapPathVarValue)
//{
//	bool bRet = true;
//
//	if (vecDepthPair.size() > 0)
//	{
//		bRet = false;
//
//		int nIndex = 0, nConstCount = 0, nFindCount = 0;
//		pair<string, string> pairDepth;
//		pair<string, string> pairReceivedDepth;
//
//		vector<pair<string, string>> vecrReceivedDepthPair;
//		CRestServer::GetPathDepth(stdstrReceivedPath, vecrReceivedDepthPair);
//
//		if (vecrReceivedDepthPair.size() != vecDepthPair.size())
//			bRet = true;
//		else
//		{
//			vector<pair<string, string>>::iterator it = vecDepthPair.begin();
//			vector<pair<string, string>>::iterator it2 = vecrReceivedDepthPair.begin();
//
//			for (it; it != vecDepthPair.end(); it++)
//			{
//				nIndex = 0;
//				pairDepth = *it;
//				pairReceivedDepth = *it2;
//
//				if (!pairDepth.first.compare(DEPTH_TYPE_OF_PATH_CONST))
//				{
//					if(pairReceivedDepth.second.compare(pairDepth.second))
//					{
//						bRet = true;
//
//						break;
//					}
//				}
//
//				if (!pairDepth.first.compare(DEPTH_TYPE_OF_PATH_VAR))
//				{
//					retMapPathVarValue[pairDepth.second] = pairReceivedDepth.second;
//				}
//
//				it2++;
//			}
//		}
//	}
//
//	return bRet;
//}

void CRestServer::GetPairMethodNPath(string stdstrMethod, string stdstrReceivedPath, pair<string, string>& pairMethodNPath
	, map<string, string>& retMapPathVarValue)
{
	if (m_mapDepth.size() > 0)
	{
		vector<pair<string, string>> vecPair;

		map<pair<string, string>, vector<pair<string, string>>>::iterator it = m_mapDepth.begin();
		for (it; it != m_mapDepth.end(); it++)
		{
			vecPair.clear();

			if (!it->first.first.compare(stdstrMethod))
			{
				vecPair = it->second;
				if (!ComparePathDepth(stdstrReceivedPath, vecPair, retMapPathVarValue))
				{
					pairMethodNPath = it->first;
					break;
				}
			}
		}
	}

	return;
}

void CRestServer::GetPathDepth(string stdstrPath, vector<pair<string, string>>& retVecDepthPair)
{
	int nVariableStartPos = stdstrPath.find("/{");
	int nVariableEndPos = stdstrPath.find("}");

	int nConstStartPos = stdstrPath.find("/");
	int nConstEndPos = 0;

	string stdstrRemainPath;

	if (nVariableStartPos == 0)
	{
		string stdstrVarName = stdstrPath.substr(0, nVariableEndPos + 1);
		pair<string, string> pairVar(DEPTH_TYPE_OF_PATH_VAR, stdstrVarName);
		retVecDepthPair.push_back(pairVar);

		stdstrRemainPath = stdstrPath.substr(nVariableEndPos + 1);
	}
	else if (nConstStartPos == 0)
	{
		stdstrRemainPath = stdstrPath.substr(1);
		nConstEndPos = stdstrRemainPath.find("/");

		string stdstrConstName;

		if (nConstEndPos >= 0)
		{
			stdstrConstName = stdstrPath.substr(0, nConstEndPos + 1);
			stdstrRemainPath = stdstrPath.substr(nConstEndPos + 1);
		}
		else
			stdstrConstName = stdstrPath;

		pair<string, string> pairConst(DEPTH_TYPE_OF_PATH_CONST, stdstrConstName);
		retVecDepthPair.push_back(pairConst);
	}
	else
		return;

	GetPathDepth(stdstrRemainPath, retVecDepthPair);

	return;
}

bool CRestServer::ComparePathDepth(string stdstrReceivedPath, vector<pair<string, string>>& vecDepthPair
	, map<string, string>& retMapPathVarValue)
{
	bool bRet = true;

	if (vecDepthPair.size() > 0)
	{
		bRet = false;

		int nIndex = 0, nConstCount = 0, nFindCount = 0;
		pair<string, string> pairDepth;
		pair<string, string> pairReceivedDepth;

		vector<pair<string, string>> vecrReceivedDepthPair;
		GetPathDepth(stdstrReceivedPath, vecrReceivedDepthPair);

		if (vecrReceivedDepthPair.size() != vecDepthPair.size())
			bRet = true;
		else
		{
			vector<pair<string, string>>::iterator it = vecDepthPair.begin();
			vector<pair<string, string>>::iterator it2 = vecrReceivedDepthPair.begin();

			for (it; it != vecDepthPair.end(); it++)
			{
				nIndex = 0;
				pairDepth = *it;
				pairReceivedDepth = *it2;

				if (!pairDepth.first.compare(DEPTH_TYPE_OF_PATH_CONST))
				{
					if (pairReceivedDepth.second.compare(pairDepth.second))
					{
						bRet = true;

						break;
					}
				}

				if (!pairDepth.first.compare(DEPTH_TYPE_OF_PATH_VAR))
				{
					retMapPathVarValue[pairDepth.second] = pairReceivedDepth.second;
				}

				it2++;
			}
		}
	}

	return bRet;
}

CRestServer::REQUESTCB CRestServer::GetRequestCB(pair<string, string> pairMethodNPath, void** retData)
{
	REQUESTCB pFunc = NULL;

	pFunc = m_mapFuncCB[pairMethodNPath];
	*retData = m_mapDataCB[pairMethodNPath];

	return pFunc;
}

CRestServer* GetRequestedServer(utility::string_t host)
{
	CRestServer* pRetServer = NULL;

	gMutexSyncRestServer.lock();

	if (!g_mapServer.empty())
	{
		map<CRestServer*, string>::iterator it = g_mapServer.begin();
		for (it; it != g_mapServer.end(); it++)
		{
			utility::string_t value = utility::conversions::utf8_to_utf16(it->second);
			int nIndex = (int)value.find(host);
			if (nIndex >= 0)
				pRetServer = it->first;
		}
	}

	gMutexSyncRestServer.unlock();

	return pRetServer;
}

void handle_get(http_request request)
{
	//TRACE(L"\nhandle GET\n");

	/*json::value answer;

	for (auto const & p : dictionary)
	{
	answer.push_back(make_pair(json::value(p.first), json::value(p.second)));
	}

	request.reply(status_codes::OK, json::value::as_object(answer));*/

	//request.request_uri;

	status_code statuscode = status_codes::OK;

	utility::string_t path = request.request_uri().path();
	utility::string_t query = request.request_uri().query();

	utility::string_t nameHost = L"host";
	http_headers headers = request.headers();
	utility::string_t host = headers[nameHost];

	string stdstrPath = utility::conversions::utf16_to_utf8(path);
	string stdstrQuery = utility::conversions::utf16_to_utf8(query);

	if (!query.empty() && !path.empty())
		printf("receive - method: %s, path: %s, query: %s\n", METHOD_GET, stdstrPath.c_str(), stdstrQuery.c_str());
	else
		printf("receive - method: %s, path: %s\n", METHOD_GET, stdstrPath.c_str());

	CRestServer* pServer = GetRequestedServer(host);
	if (!pServer)
		printf("Not found host server: %s\n", utility::conversions::utf16_to_utf8(host));

	//CRestServer::REQUESTCB pFunc = CRestServer::m_mapFuncCB[stdstrPath];
	//void* pData = CRestServer::m_mapDataCB[stdstrPath];

	//pair<string, string> pairMethodNPath(METHOD_GET, stdstrPath);
	pair<string, string> pairMethodNPath;
	map<string, string> mapPathVarValue;

	//CRestServer::GetPairMethodNPath(METHOD_GET, stdstrPath, pairMethodNPath, mapPathVarValue);

	//CRestServer::REQUESTCB pFunc = CRestServer::g_mapFuncCB[pairMethodNPath];
	//void* pData = CRestServer::g_mapDataCB[pairMethodNPath];

	pServer->GetPairMethodNPath(METHOD_GET, stdstrPath, pairMethodNPath, mapPathVarValue);

	void* pData = NULL;
	CRestServer::REQUESTCB pFunc = pServer->GetRequestCB(pairMethodNPath, &pData);

	if (!pFunc && !pData)
	{
		if (!query.empty() && !path.empty())
			printf("reply: %d %s - path: %s, query: %s\n", 501, "Not Implemented", stdstrPath.c_str(), stdstrQuery.c_str());
		else
			printf("reply: %d %s - path: %s \n", 501, "Not Implemented", stdstrPath.c_str());

		request.reply(status_codes::NotImplemented).wait();

		return;
	}

	string stdstrResult;
	int nResult = pFunc(mapPathVarValue, stdstrQuery, "", pData, stdstrResult);
	statuscode = nResult;

	utility::string_t result = utility::conversions::utf8_to_utf16(stdstrResult);

	json::value body_data;
	body_data = json::value::parse(result);

	request.reply(statuscode, body_data).wait();

	return;

	//request.extract_json()
	//	.then([&request](pplx::task<json::value> task) {

	//	auto & jvalue = task.get();
	//	utility::string_t jjjjjson = jvalue.serialize();

	//	json::value obj;
	//	for (auto const & p : dictionary)
	//	{
	//		obj[p.first] = json::value::string(p.second);
	//	}

	//	obj[L"id"] = json::value::string(L"Luna");
	//	obj[L"name"] = json::value::string(L"Silver");
	//	obj[L"age"] = json::value::string(L"19");
	//	obj[L"hasCar"] = json::value::string(L"false");

	//	// this is just for debugging
	//	utility::stringstream_t stream;
	//	obj.serialize(stream);
	//	std::wcout << stream.str();

	//	request.reply(status_codes::OK, obj);
	//})
	//	.wait();
	//request.reply(status_codes::OK, json::value(L"SUCCESS2"));
}

void handle_request(http_request request, string stdstrMethod
	, function<void(json::value&, json::value&, status_code&, string&, map<string, string>&, CRestServer::REQUESTCB, void*)> action)
{
	utility::string_t path = request.request_uri().path();
	utility::string_t query = request.request_uri().query();

	utility::string_t nameHost = L"host";
	http_headers headers = request.headers();
	utility::string_t host = headers[nameHost];

	string stdstrPath = utility::conversions::utf16_to_utf8(path);
	string stdstrQuery = utility::conversions::utf16_to_utf8(query);

	if (!query.empty() && !path.empty())
		printf("receive - method: %s, path: %s, query: %s\n", stdstrMethod.c_str(), stdstrPath.c_str(), stdstrQuery.c_str());
	else
		printf("receive - method: %s, path: %s\n", stdstrMethod.c_str(), stdstrPath.c_str());

	CRestServer* pServer = GetRequestedServer(host);
	if (!pServer)
	{
		string stdstrHost = utility::conversions::utf16_to_utf8(host);
		printf("Not found host server: %s\n", stdstrHost.c_str());

		request.reply(status_codes::NotFound).wait();

		return;
	}

	//CRestServer::REQUESTCB pFunc = CRestServer::m_mapFuncCB[stdstrPath];
	//void* pData = CRestServer::m_mapDataCB[stdstrPath];

	pair<string, string> pairMethodNPath;
	map<string, string> mapPathVarValue;

	//CRestServer::GetPairMethodNPath(stdstrMethod, stdstrPath, pairMethodNPath, mapPathVarValue);
	pServer->GetPairMethodNPath(stdstrMethod, stdstrPath, pairMethodNPath, mapPathVarValue);

	//CRestServer::REQUESTCB pFunc = CRestServer::g_mapFuncCB[pairMethodNPath];
	//void* pData = CRestServer::g_mapDataCB[pairMethodNPath];
	void* pData = NULL;
	CRestServer::REQUESTCB pFunc = pServer->GetRequestCB(pairMethodNPath, &pData);

	if (!pFunc && !pData)
	{
		if (!query.empty() && !path.empty())
			printf("reply: %d %s - path: %s, query: %s\n", 501, "Not Implemented", stdstrPath.c_str(), stdstrQuery.c_str());
		else
			printf("reply: %d %s - path: %s \n", 501, "Not Implemented", stdstrPath.c_str());

		request.reply(status_codes::NotImplemented).wait();

		return;
	}

	json::value answer;
	status_code statuscode = status_codes::OK;

	try
	{
		request
			.extract_json()
			.then([&answer, &action, &statuscode, &stdstrQuery, &mapPathVarValue, pFunc, pData](pplx::task<json::value> task)
		{
			try
			{
				auto & jvalue = task.get();
				action(jvalue, answer, statuscode, stdstrQuery, mapPathVarValue, pFunc, pData);
			}
			catch (http_exception const & e)
			{
				wcout << e.what() << endl;
			}
		})
			.wait();
	}
	catch (http_exception const & e)
	{
		wcout << e.what() << endl;

		statuscode = status_codes::BadRequest;
		answer = json::value::null();
	}

	request.reply(statuscode, json::value(answer)).wait();
}

void handle_post(http_request request)
{
	json::value answer = json::value::null();
	handle_request(
	request,
	METHOD_POST,
	[](json::value& jvalue, json::value& answer, status_code& statuscode, string& stdstrQuery, map<string, string>& mapPathVarValue
	, CRestServer::REQUESTCB pFunc, void* pData)
	{
		//utility::string_t channel = jvalue[L"channel"].as_string();
		utility::string_t data = jvalue.serialize();

		//string stdstrChannel = utility::conversions::utf16_to_utf8(channel);
		string stdstrData = utility::conversions::utf16_to_utf8(data);

		string stdstrResult;
		int nResult = pFunc(mapPathVarValue, stdstrQuery, stdstrData, pData, stdstrResult);
		statuscode = nResult;

		utility::string_t result = utility::conversions::utf8_to_utf16(stdstrResult);

		if (!stdstrResult.empty())
		{
			try
			{
				answer = json::value::parse(result);
			}
			catch (web::json::json_exception const & e)
			{
				statuscode = status_codes::InternalError;

				string strMsg = Utility::string_format(
					"{\
					\"error\":%d,\
					\"message\":\"response json exception - %s\",\
					\"response\":\"%s\"\
					}", (int)statuscode, e.what(), stdstrResult.c_str());

				wcout << strMsg.c_str() << endl;

				result = utility::conversions::utf8_to_utf16(strMsg.c_str());
				answer = json::value::parse(result);
			}
		}
	});
}

void handle_put(http_request request)
{
	handle_request(
	request,
	METHOD_PUT,
	[](json::value & jvalue, json::value & answer, status_code& statuscode, string & stdstrQuery, map<string, string>& mapPathVarValue
	, CRestServer::REQUESTCB pFunc, void* pData)
	{
		//utility::string_t channel = jvalue[L"channel"].as_string();
		utility::string_t data = jvalue.serialize();

		//string stdstrChannel = utility::conversions::utf16_to_utf8(channel);
		string stdstrData = utility::conversions::utf16_to_utf8(data);

		string stdstrResult;
		int nResult = pFunc(mapPathVarValue, stdstrQuery, stdstrData, pData, stdstrResult);
		statuscode = nResult;

		utility::string_t result = utility::conversions::utf8_to_utf16(stdstrResult);

		if (!stdstrResult.empty())
		{
			try
			{
				answer = json::value::parse(result);
			}
			catch (web::json::json_exception const & e)
			{
				statuscode = status_codes::InternalError;

				string strMsg = Utility::string_format(
					"{\
					\"error\":%d,\
					\"message\":\"response json exception - %s\",\
					\"response\":\"%s\"\
					}", (int)statuscode, e.what(), stdstrResult.c_str());

				wcout << strMsg.c_str() << endl;

				result = utility::conversions::utf8_to_utf16(strMsg.c_str());
				answer = json::value::parse(result);
			}
		}
	});
}

void handle_del(http_request request)
{
	handle_request(
	request,
	METHOD_DELETE,
	[](json::value & jvalue, json::value & answer, status_code& statuscode, string & stdstrQuery, map<string, string>& mapPathVarValue
	, CRestServer::REQUESTCB pFunc, void* pData)
	{
		//utility::string_t channel = jvalue[L"channel"].as_string();
		utility::string_t data = jvalue.serialize();

		//string stdstrChannel = utility::conversions::utf16_to_utf8(channel);
		string stdstrData = utility::conversions::utf16_to_utf8(data);

		string stdstrResult;
		int nResult = pFunc(mapPathVarValue, stdstrQuery, stdstrData, pData, stdstrResult);
		statuscode = nResult;

		utility::string_t result = utility::conversions::utf8_to_utf16(stdstrResult);

		if (!stdstrResult.empty())
		{
			try
			{
				answer = json::value::parse(result);
			}
			catch (web::json::json_exception const & e)
			{
				statuscode = status_codes::InternalError;

				string strMsg = Utility::string_format(
					"{\
					\"error\":%d,\
					\"message\":\"response json exception - %s\",\
					\"response\":\"%s\"\
					}", (int)statuscode, e.what(), stdstrResult.c_str());

				wcout << strMsg.c_str() << endl;

				result = utility::conversions::utf8_to_utf16(strMsg.c_str());
				answer = json::value::parse(result);
			}
		}
	});
}