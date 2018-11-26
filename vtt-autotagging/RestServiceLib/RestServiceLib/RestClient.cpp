#include "stdafx.h"
#include "RestClient.h"

#include "Utility.h"

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/http_msg.h>
#include <cpprest/base_uri.h>
#include <cpprest/filestream.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <chrono>
#include <thread>
#include <array>

#define DEPTH_TYPE_OF_PATH_VAR						"VAR"
#define DEPTH_TYPE_OF_PATH_CONST					"CONST"

#define METHOD_GET									"GET"
#define METHOD_POST									"POST"
#define METHOD_PUT									"PUT"
#define METHOD_DELETE								"DELETE"

CRestClient::CRestClient()
{
	m_stdstrAuthKey.clear();
	m_stdstrAuthValue.clear();
}

CRestClient::CRestClient(string stdstrUri)
{
	CRestClient::CRestClient();

	SetServerUri(stdstrUri);
}

CRestClient::CRestClient(string stdstrUri, string stdstrAuthKey, string stdstrAuthValue)
{
	CRestClient::CRestClient(stdstrUri);

	m_stdstrAuthKey = stdstrAuthKey;
	m_stdstrAuthValue = stdstrAuthValue;
}

CRestClient::~CRestClient()
{
	m_mapFuncCB.clear();
	m_mapDataCB.clear();
	m_mapDepth.clear();
}

int CRestClient::Request(string stdstrMethod, string stdstrPath, string stdstrBody)
{
	int nRet = 0;

	utility::string_t method = utility::conversions::utf8_to_utf16(stdstrMethod);
	utility::string_t query = utility::conversions::utf8_to_utf16(m_stdstrQuery);

	pair<string, string> pairMethodNPath;
	map<string, string> mapPathVarValue;

	GetPairMethodNPath(stdstrMethod, stdstrPath, pairMethodNPath, mapPathVarValue);

	RESPONSECB pFunCB = m_mapFuncCB[pairMethodNPath];
	void* pDataCB = m_mapDataCB[pairMethodNPath];

	if (stdstrMethod.empty())
	{
		m_nStatusCode = 405;

		m_stdstrReason.clear();
		m_stdstrReason.assign("Method Not Allowed");

		Clear();

		return m_nStatusCode;
	}

	int nStatusCode = 0;
	string stdstrReason;
	string stdstrResponseJsonData;

	int* pnStatusCode = new int(nStatusCode);
	string* pstdstrReason = new string(stdstrReason);
	string* pstdstrResponseJsonData = new string(stdstrResponseJsonData);
	//std::shared_ptr<int> pnStatusCode = std::make_shared<int>(nStatusCode);
	//std::shared_ptr<string> pstdstrReason = std::make_shared<string>(stdstrReason);
	//std::shared_ptr<string> pstdstrResponseJsonData = std::make_shared<string>(stdstrResponseJsonData);
	
	stdstrPath = Utility::ReplaceAll(stdstrPath, m_stdstrUriPath, "");
	http_client client(utility::conversions::utf8_to_utf16(m_stdstrUri));
	//uri base_uri = client.base_uri();
	//utility::string_t method = base_uri.path();
	uri_builder builder(utility::conversions::utf8_to_utf16(stdstrPath));

	if (!query.empty())
		builder.append_query(query);

	json::value jRequestData;

	if (!stdstrBody.empty())
	{
		try
		{
			jRequestData = json::value::parse(utility::conversions::utf8_to_utf16(stdstrBody));
		}
		catch (web::json::json_exception &e)
		{
			printf("json parse - %s\n", e.what());

			m_nStatusCode = 400;

			m_stdstrReason.clear();
			m_stdstrReason.assign(e.what());

			Clear();

			if (pnStatusCode)
			{
				delete pnStatusCode;
				pnStatusCode = NULL;
			}

			if (pstdstrReason)
			{
				delete pstdstrReason;
				pstdstrReason = NULL;
			}

			if (pstdstrResponseJsonData)
			{
				delete pstdstrResponseJsonData;
				pstdstrResponseJsonData = NULL;
			}

			return m_nStatusCode;
		}
	}

	//methods::GET;

	//json::value jValue = json::value::parse(utility::conversions::utf8_to_utf16(stdstrBody));
	//json::value obj;

	//try
	//{
	//	utility::string_t str = jValue.at(L"compservername").as_string();
	//}
	//catch (web::json::json_exception &e)
	//{
	//	printf("%s\n", e.what());
	//};
	////{"age":"19", "hasCar" : "false", "id" : "Luna", "name" : "Silver"}
	//obj[L"id"] = json::value::string(L"Luna");
	//obj[L"name"] = json::value::string(L"Silver");
	//obj[L"age"] = json::value::string(L"19");
	//obj[L"hasCar"] = json::value::string(L"false");
	//
	//utility::string_t str = obj.serialize();
	//json::value jValue2 = json::value::string(str);

	//jValue.at(L"age").as_string();

	pplx::task<void> requestTask;
	pplx::task<http_response> reponseTask;

	http_request req(method);

	if (!m_stdstrAuthKey.empty() && !m_stdstrAuthValue.empty())
	{
		//// Creating http_client
		//http_client_config config;
		//credentials cred(L"username", L"Password");
		//config.set_credentials(cred);
		//http_client client(U("https://serverip"), config);
		//// create header
		//http_request req(methods::GET);
		//// Add base64 result to header
		//req.headers().add(L"Authorization", L"Basic XYZtaW46Wr6yZW0xMAXY");
		//req.set_request_uri(L"/api/json/xyz");
		//pplx::task<http_response> responses = client.request(req);
		//pplx::task<web::json::value> jvalue = responses.get().extract_json();
		//wcout << jvalue.get().to_string();

		utility::string_t authKey = utility::conversions::utf8_to_utf16(m_stdstrAuthKey);
		utility::string_t authValue = utility::conversions::utf8_to_utf16(m_stdstrAuthValue);
		
		req.headers().add(authKey, authValue);
	}
	
	//if (!method.compare(U("GET")))
	//	reponseTask = client.request(method, builder.to_string());
	//else
	//	reponseTask = client.request(method, builder.to_string(), jRequestData);

	req.set_request_uri(builder.to_string());

	if (method.compare(U(METHOD_GET)))
		req.set_body(jRequestData);

	reponseTask = client.request(req);

	requestTask = reponseTask.then([&](http_response response)
	{
		utility::string_t reason = response.reason_phrase().c_str();

		*pnStatusCode = response.status_code();
		pstdstrReason->clear();
		pstdstrReason->assign(utility::conversions::utf16_to_utf8(reason));

		//printf("reponse body - %s\n", response.body.extract_string());

		//pplx::task<void> reponseJsonTask = response.extract_json()
		//.then([&](json::value jResponseValue)
		//{
		//	utility::string_t responsedata = jResponseValue.serialize();

		//	pstdstrResponseJsonData->clear();
		//	pstdstrResponseJsonData->assign(utility::conversions::utf16_to_utf8(responsedata));
		//});

		pplx::task<void> reponseTask = response.extract_string()
			.then([&](utility::string_t responsedata)
		{
			pstdstrResponseJsonData->clear();
			pstdstrResponseJsonData->assign(utility::conversions::utf16_to_utf8(responsedata));
		});

		try
		{
			reponseTask.wait();

			if (pFunCB && pDataCB)
			{
				pFunCB(*pnStatusCode, *pstdstrReason, *pstdstrResponseJsonData, pDataCB);
			}
		}
		catch (http_exception const &e)
		{
			*pnStatusCode = 400;

			pstdstrReason->clear();
			pstdstrReason->assign(e.what());
			pstdstrResponseJsonData->clear();

			cout << e.what() << endl;
		}
	});

	try
	{
		requestTask.wait();

		m_nStatusCode = *pnStatusCode;
		nRet = m_nStatusCode;

		m_stdstrReason.clear();
		m_stdstrReason = *pstdstrReason;
		m_stdstrResponseJsonData = *pstdstrResponseJsonData;
	}
	catch (http_exception const &e)
	{
		m_nStatusCode = 400;

		m_stdstrReason.clear();
		m_stdstrReason.assign(e.what());
		m_stdstrResponseJsonData.clear();

		cout << e.what() << endl;

		nRet = e.error_code().value();
	}

	if (pnStatusCode)
	{
		delete pnStatusCode;
		pnStatusCode = NULL;
	}

	if (pstdstrReason)
	{
		delete pstdstrReason;
		pstdstrReason = NULL;
	}

	if (pstdstrResponseJsonData)
	{
		delete pstdstrResponseJsonData;
		pstdstrResponseJsonData = NULL;
	}

	return nRet;
}

int CRestClient::RequestUploadFileWithMultipartFormdata(string stdstrMethod, string stdstrPath, string stdstrFilePath, string stdstrTextData)
{
	int nRet = 0;

	utility::string_t method = utility::conversions::utf8_to_utf16(stdstrMethod);
	utility::string_t query = utility::conversions::utf8_to_utf16(m_stdstrQuery);
	utility::string_t filepath = utility::conversions::utf8_to_utf16(stdstrFilePath);

	pair<string, string> pairMethodNPath;
	map<string, string> mapPathVarValue;

	GetPairMethodNPath(stdstrMethod, stdstrPath, pairMethodNPath, mapPathVarValue);

	RESPONSECB pFunCB = m_mapFuncCB[pairMethodNPath];
	void* pDataCB = m_mapDataCB[pairMethodNPath];

	if (stdstrMethod.empty())
	{
		m_nStatusCode = 405;

		m_stdstrReason.clear();
		m_stdstrReason.assign("Method Not Allowed");

		Clear();

		return m_nStatusCode;
	}

	int nStatusCode = 0;
	string stdstrBody;
	string stdstrReason;
	string stdstrResponseJsonData;

	int* pnStatusCode = new int(nStatusCode);
	string* pstdstrReason = new string(stdstrReason);
	string* pstdstrResponseJsonData = new string(stdstrResponseJsonData);
	//std::shared_ptr<int> pnStatusCode = std::make_shared<int>(nStatusCode);
	//std::shared_ptr<string> pstdstrReason = std::make_shared<string>(stdstrReason);
	//std::shared_ptr<string> pstdstrResponseJsonData = std::make_shared<string>(stdstrResponseJsonData);

	stdstrPath = Utility::ReplaceAll(stdstrPath, m_stdstrUriPath, "");
	http_client client(utility::conversions::utf8_to_utf16(m_stdstrUri));
	uri_builder builder(utility::conversions::utf8_to_utf16(stdstrPath));

	if (!query.empty())
		builder.append_query(query);

	concurrency::streams::basic_istream<unsigned char> fileStream = file_stream<unsigned char>::open_istream(filepath).get();

	concurrency::streams::stringstreambuf streamBuffer;
	fileStream.read_to_end(streamBuffer).get();
	std::string stdstrTextFile = move(streamBuffer.collection());
	fileStream.close();
	streamBuffer.close();

	int nIndex = -1;
	stdstrFilePath = Utility::ReplaceAll(stdstrFilePath, "\\\\", "/");
	stdstrFilePath = Utility::ReplaceAll(stdstrFilePath, "\\", "/");
	nIndex = stdstrFilePath.find_last_of("/");
	if (nIndex >= 0)
	{
		stdstrFilePath = stdstrFilePath.substr(nIndex + 1);
	}

	std::string stdstrBoundary = "----WebKitFormBoundary2nQa2i0bF1UMsZBa";
	stdstrBody += "--" + stdstrBoundary + "\r\n";
	//stdstrBody += "Content-Disposition: form-data; name=\"csrfmiddlewaretoken\"\r\n\r\n";
	//stdstrBody += "OhRvx5Q1k86lPPxVe3rrR4mZpPiffP8zkBmMZBXALmhKyW7y9xcy0wTelx778vgu\r\n";
	//stdstrBody += "--" + stdstrBoundary + "\r\n";
	stdstrBody += "Content-Disposition: form-data; name=\"image\"; filename=\"" + stdstrFilePath + "\"\r\n";
	//stdstrBody += "Content-Disposition: form-data; name=\"image\"; filename=\"\"\r\n";
	//stdstrBody += "Content-Type: image/jpeg\r\n\r\n";
	stdstrBody += "Content-Type: application/octet-stream\r\n\r\n";
	stdstrBody += stdstrTextFile + "\r\n";
	//stdstrBody += "\r\n";

	if (!stdstrTextData.empty())
	{
		stdstrBody += "--" + stdstrBoundary + "\r\n";
		stdstrBody += "Content-Disposition: form-data; name=\"callback_url\"\r\n\r\n";
		stdstrBody += stdstrTextData + "\r\n";
	}

	stdstrBody += "--" + stdstrBoundary + "--\r\n";

	pplx::task<void> requestTask;
	pplx::task<http_response> reponseTask;

	http_request req(method);

	if (!m_stdstrAuthKey.empty() && !m_stdstrAuthValue.empty())
	{
		utility::string_t authKey = utility::conversions::utf8_to_utf16(m_stdstrAuthKey);
		utility::string_t authValue = utility::conversions::utf8_to_utf16(m_stdstrAuthValue);

		req.headers().add(authKey, authValue);
	}

	req.set_request_uri(builder.to_string());

	if (method.compare(U(METHOD_GET)))
	{
		req.headers().set_content_length(stdstrBody.length());
		req.headers().set_content_type(L"multipart/form-data; boundary=----WebKitFormBoundary2nQa2i0bF1UMsZBa");
		req.set_body(stdstrBody);
	}

	reponseTask = client.request(req);

	requestTask = reponseTask.then([&](http_response response)
	{
		utility::string_t reason = response.reason_phrase().c_str();

		*pnStatusCode = response.status_code();
		pstdstrReason->clear();
		pstdstrReason->assign(utility::conversions::utf16_to_utf8(reason));

		pplx::task<void> reponseTask = response.extract_string()
			.then([&](utility::string_t responsedata)
		{
			pstdstrResponseJsonData->clear();
			pstdstrResponseJsonData->assign(utility::conversions::utf16_to_utf8(responsedata));
		});

		try
		{
			reponseTask.wait();

			if (pFunCB && pDataCB)
			{
				pFunCB(*pnStatusCode, *pstdstrReason, *pstdstrResponseJsonData, pDataCB);
			}
		}
		catch (http_exception const &e)
		{
			*pnStatusCode = 400;

			pstdstrReason->clear();
			pstdstrReason->assign(e.what());
			pstdstrResponseJsonData->clear();

			cout << e.what() << endl;
		}
	});

	try
	{
		requestTask.wait();

		m_nStatusCode = *pnStatusCode;
		nRet = m_nStatusCode;

		m_stdstrReason.clear();
		m_stdstrReason = *pstdstrReason;
		m_stdstrResponseJsonData = *pstdstrResponseJsonData;
	}
	catch (http_exception const &e)
	{
		m_nStatusCode = 400;

		m_stdstrReason.clear();
		m_stdstrReason.assign(e.what());
		m_stdstrResponseJsonData.clear();

		cout << e.what() << endl;

		nRet = e.error_code().value();
	}

	if (pnStatusCode)
	{
		delete pnStatusCode;
		pnStatusCode = NULL;
	}

	if (pstdstrReason)
	{
		delete pstdstrReason;
		pstdstrReason = NULL;
	}

	if (pstdstrResponseJsonData)
	{
		delete pstdstrResponseJsonData;
		pstdstrResponseJsonData = NULL;
	}

	return nRet;
}

int CRestClient::RequestUploadFileWithMultipartFormdata(string stdstrMethod, string stdstrPath, string stdstrFilePath, string stdstrParamName, string stdstrParamData)
{
	int nRet = 0;

	utility::string_t method = utility::conversions::utf8_to_utf16(stdstrMethod);
	utility::string_t query = utility::conversions::utf8_to_utf16(m_stdstrQuery);
	utility::string_t filepath = utility::conversions::utf8_to_utf16(stdstrFilePath);

	pair<string, string> pairMethodNPath;
	map<string, string> mapPathVarValue;

	GetPairMethodNPath(stdstrMethod, stdstrPath, pairMethodNPath, mapPathVarValue);

	RESPONSECB pFunCB = m_mapFuncCB[pairMethodNPath];
	void* pDataCB = m_mapDataCB[pairMethodNPath];

	if (stdstrMethod.empty())
	{
		m_nStatusCode = 405;

		m_stdstrReason.clear();
		m_stdstrReason.assign("Method Not Allowed");

		Clear();

		return m_nStatusCode;
	}

	int nStatusCode = 0;
	string stdstrBody;
	string stdstrReason;
	string stdstrResponseJsonData;

	int* pnStatusCode = new int(nStatusCode);
	string* pstdstrReason = new string(stdstrReason);
	string* pstdstrResponseJsonData = new string(stdstrResponseJsonData);
	//std::shared_ptr<int> pnStatusCode = std::make_shared<int>(nStatusCode);
	//std::shared_ptr<string> pstdstrReason = std::make_shared<string>(stdstrReason);
	//std::shared_ptr<string> pstdstrResponseJsonData = std::make_shared<string>(stdstrResponseJsonData);

	stdstrPath = Utility::ReplaceAll(stdstrPath, m_stdstrUriPath, "");
	http_client client(utility::conversions::utf8_to_utf16(m_stdstrUri));
	uri_builder builder(utility::conversions::utf8_to_utf16(stdstrPath));

	if (!query.empty())
		builder.append_query(query);

	concurrency::streams::basic_istream<unsigned char> fileStream = file_stream<unsigned char>::open_istream(filepath).get();

	concurrency::streams::stringstreambuf streamBuffer;
	fileStream.read_to_end(streamBuffer).get();
	std::string stdstrTextFile = move(streamBuffer.collection());
	fileStream.close();
	streamBuffer.close();

	int nIndex = -1;
	stdstrFilePath = Utility::ReplaceAll(stdstrFilePath, "\\\\", "/");
	stdstrFilePath = Utility::ReplaceAll(stdstrFilePath, "\\", "/");
	nIndex = stdstrFilePath.find_last_of("/");
	if (nIndex >= 0)
	{
		stdstrFilePath = stdstrFilePath.substr(nIndex + 1);
	}

	std::string stdstrBoundary = "----WebKitFormBoundary2nQa2i0bF1UMsZBa";
	stdstrBody += "--" + stdstrBoundary + "\r\n";
	stdstrBody += "Content-Disposition: form-data; name=\"image\"; filename=\"" + stdstrFilePath + "\"\r\n";
	stdstrBody += "Content-Type: application/octet-stream\r\n\r\n";
	stdstrBody += stdstrTextFile + "\r\n";

	if (!stdstrParamName.empty() && !stdstrParamData.empty())
	{
		stdstrBody += "--" + stdstrBoundary + "\r\n";
		stdstrBody += "Content-Disposition: form-data; name=\"" + stdstrParamName + "\"\r\n\r\n";
		stdstrBody += stdstrParamData + "\r\n";
	}

	stdstrBody += "--" + stdstrBoundary + "--\r\n";

	pplx::task<void> requestTask;
	pplx::task<http_response> reponseTask;

	http_request req(method);

	if (!m_stdstrAuthKey.empty() && !m_stdstrAuthValue.empty())
	{
		utility::string_t authKey = utility::conversions::utf8_to_utf16(m_stdstrAuthKey);
		utility::string_t authValue = utility::conversions::utf8_to_utf16(m_stdstrAuthValue);

		req.headers().add(authKey, authValue);
	}

	req.set_request_uri(builder.to_string());

	if (method.compare(U(METHOD_GET)))
	{
		req.headers().set_content_length(stdstrBody.length());
		req.headers().set_content_type(L"multipart/form-data; boundary=----WebKitFormBoundary2nQa2i0bF1UMsZBa");
		req.set_body(stdstrBody);
	}

	reponseTask = client.request(req);

	requestTask = reponseTask.then([&](http_response response)
	{
		utility::string_t reason = response.reason_phrase().c_str();

		*pnStatusCode = response.status_code();
		pstdstrReason->clear();
		pstdstrReason->assign(utility::conversions::utf16_to_utf8(reason));

		pplx::task<void> reponseTask = response.extract_string()
			.then([&](utility::string_t responsedata)
		{
			pstdstrResponseJsonData->clear();
			pstdstrResponseJsonData->assign(utility::conversions::utf16_to_utf8(responsedata));
		});

		try
		{
			reponseTask.wait();

			if (pFunCB && pDataCB)
			{
				pFunCB(*pnStatusCode, *pstdstrReason, *pstdstrResponseJsonData, pDataCB);
			}
		}
		catch (http_exception const &e)
		{
			*pnStatusCode = 400;

			pstdstrReason->clear();
			pstdstrReason->assign(e.what());
			pstdstrResponseJsonData->clear();

			cout << e.what() << endl;
		}
	});

	try
	{
		requestTask.wait();

		m_nStatusCode = *pnStatusCode;
		nRet = m_nStatusCode;

		m_stdstrReason.clear();
		m_stdstrReason = *pstdstrReason;
		m_stdstrResponseJsonData = *pstdstrResponseJsonData;
	}
	catch (http_exception const &e)
	{
		m_nStatusCode = 400;

		m_stdstrReason.clear();
		m_stdstrReason.assign(e.what());
		m_stdstrResponseJsonData.clear();

		cout << e.what() << endl;

		nRet = e.error_code().value();
	}

	if (pnStatusCode)
	{
		delete pnStatusCode;
		pnStatusCode = NULL;
	}

	if (pstdstrReason)
	{
		delete pstdstrReason;
		pstdstrReason = NULL;
	}

	if (pstdstrResponseJsonData)
	{
		delete pstdstrResponseJsonData;
		pstdstrResponseJsonData = NULL;
	}

	return nRet;
}
//#include <fcntl.h>  
//#include <sys/types.h>  
//#include <sys/stat.h>  
//#include <io.h>  
//#include <stdio.h>
//#include <ostream>
int CRestClient::RequestDownloadFile(string stdstrMethod, string stdstrPath, string stdstrSaveFilePath, string stdstrBodyText)
{
	int nRet = 0;

	utility::string_t method = utility::conversions::utf8_to_utf16(stdstrMethod);
	utility::string_t query = utility::conversions::utf8_to_utf16(m_stdstrQuery);
	utility::string_t* pfilepath = new utility::string_t(utility::conversions::utf8_to_utf16(stdstrSaveFilePath));

	pair<string, string> pairMethodNPath;
	map<string, string> mapPathVarValue;

	GetPairMethodNPath(stdstrMethod, stdstrPath, pairMethodNPath, mapPathVarValue);

	RESPONSECB pFunCB = m_mapFuncCB[pairMethodNPath];
	void* pDataCB = m_mapDataCB[pairMethodNPath];

	if (stdstrMethod.empty())
	{
		m_nStatusCode = 405;

		m_stdstrReason.clear();
		m_stdstrReason.assign("Method Not Allowed");

		Clear();

		if (pfilepath)
			delete pfilepath;

		return m_nStatusCode;
	}

	int nStatusCode = 0;
	string stdstrBody;
	string stdstrReason;
	string stdstrResponseJsonData;

	int* pnStatusCode = new int(nStatusCode);
	string* pstdstrReason = new string(stdstrReason);
	string* pstdstrResponseJsonData = new string(stdstrResponseJsonData);

	long long* pllFileSize = new long long(0);

	stdstrPath = Utility::ReplaceAll(stdstrPath, m_stdstrUriPath, "");
	http_client client(utility::conversions::utf8_to_utf16(m_stdstrUri));
	uri_builder builder(utility::conversions::utf8_to_utf16(stdstrPath));

	if (!query.empty())
		builder.append_query(query);

	json::value jRequestData;

	if (!stdstrBody.empty())
	{
		try
		{
			jRequestData = json::value::parse(utility::conversions::utf8_to_utf16(stdstrBody));
		}
		catch (web::json::json_exception &e)
		{
			printf("json parse - %s\n", e.what());

			m_nStatusCode = 400;

			m_stdstrReason.clear();
			m_stdstrReason.assign(e.what());

			Clear();

			if (pfilepath)
			{
				delete pfilepath;
				pfilepath = NULL;
			}

			if (pllFileSize)
			{
				delete pllFileSize;
				pllFileSize = NULL;
			}

			if (pnStatusCode)
			{
				delete pnStatusCode;
				pnStatusCode = NULL;
			}

			if (pstdstrReason)
			{
				delete pstdstrReason;
				pstdstrReason = NULL;
			}

			if (pstdstrResponseJsonData)
			{
				delete pstdstrResponseJsonData;
				pstdstrResponseJsonData = NULL;
			}

			return m_nStatusCode;
		}
	}

	pplx::task<void> requestTask;
	pplx::task<http_response> reponseTask;

	http_request req(method);

	if (!m_stdstrAuthKey.empty() && !m_stdstrAuthValue.empty())
	{
		utility::string_t authKey = utility::conversions::utf8_to_utf16(m_stdstrAuthKey);
		utility::string_t authValue = utility::conversions::utf8_to_utf16(m_stdstrAuthValue);

		req.headers().add(authKey, authValue);
	}

	req.set_request_uri(builder.to_string());

	if (method.compare(U(METHOD_GET)))
		req.set_body(jRequestData);

	reponseTask = client.request(req);

	requestTask = reponseTask.then([&](http_response response)
	{
		utility::string_t reason = response.reason_phrase().c_str();

		*pllFileSize = (long long)response.headers().content_length();
		utility::string_t content_type = response.headers().content_type();

		*pnStatusCode = response.status_code();
		pstdstrReason->clear();
		pstdstrReason->assign(utility::conversions::utf16_to_utf8(reason));

		try
		{
			concurrency::streams::basic_istream<unsigned char> bodyStream = response.body();
			concurrency::streams::stringstreambuf streamBuffer;

			pplx::task<void> readTask;
			readTask = bodyStream.read_to_end(streamBuffer).then([&](size_t bytesRead)
			{
			});

			readTask.wait();
			*pstdstrResponseJsonData = streamBuffer.collection();

			bodyStream.close();
			streamBuffer.close();
		}
		catch (const exception& e)
		{
			*pnStatusCode = 400;

			pstdstrReason->clear();
			pstdstrReason->assign(e.what());
			pstdstrResponseJsonData->clear();

			cout << e.what() << endl;
		}

		try
		{
			reponseTask.wait();

			if (pFunCB && pDataCB)
			{
				pFunCB(*pnStatusCode, *pstdstrReason, *pstdstrResponseJsonData, pDataCB);
			}
		}
		catch (http_exception const &e)
		{
			*pnStatusCode = 400;

			pstdstrReason->clear();
			pstdstrReason->assign(e.what());
			pstdstrResponseJsonData->clear();

			cout << e.what() << endl;
		}
	});

	try
	{
		requestTask.wait();

		m_nStatusCode = *pnStatusCode;
		nRet = m_nStatusCode;

		m_stdstrReason.clear();
		m_stdstrReason = *pstdstrReason;
		//m_stdstrResponseJsonData = *pstdstrResponseJsonData;

		string* pstdstrData = new string;
		*pstdstrData = Utility::ReplaceAll(*pstdstrResponseJsonData, "\r\n", "\n");

		ofstream wFile(stdstrSaveFilePath.c_str());
		wFile << *pstdstrData;
		wFile.close();

		delete pstdstrData;
	}
	catch (http_exception const &e)
	{
		m_nStatusCode = 400;

		m_stdstrReason.clear();
		m_stdstrReason.assign(e.what());
		m_stdstrResponseJsonData.clear();

		cout << e.what() << endl;

		nRet = e.error_code().value();
	}

	if (pfilepath)
	{
		delete pfilepath;
		pfilepath = NULL;
	}

	if (pllFileSize)
	{
		delete pllFileSize;
		pllFileSize = NULL;
	}

	if (pnStatusCode)
	{
		delete pnStatusCode;
		pnStatusCode = NULL;
	}

	if (pstdstrReason)
	{
		delete pstdstrReason;
		pstdstrReason = NULL;
	}

	if (pstdstrResponseJsonData)
	{
		delete pstdstrResponseJsonData;
		pstdstrResponseJsonData = NULL;
	}

	return nRet;
}


int CRestClient::RequestDownloadFileWithMultipartFormdata(string stdstrMethod, string stdstrPath, string stdstrSaveFilePath, string stdstrTextData)
{
	int nRet = 0;

	utility::string_t method = utility::conversions::utf8_to_utf16(stdstrMethod);
	utility::string_t query = utility::conversions::utf8_to_utf16(m_stdstrQuery);
	utility::string_t* pfilepath = new utility::string_t(utility::conversions::utf8_to_utf16(stdstrSaveFilePath));

	pair<string, string> pairMethodNPath;
	map<string, string> mapPathVarValue;

	GetPairMethodNPath(stdstrMethod, stdstrPath, pairMethodNPath, mapPathVarValue);

	RESPONSECB pFunCB = m_mapFuncCB[pairMethodNPath];
	void* pDataCB = m_mapDataCB[pairMethodNPath];

	if (stdstrMethod.empty())
	{
		m_nStatusCode = 405;

		m_stdstrReason.clear();
		m_stdstrReason.assign("Method Not Allowed");

		Clear();

		if (pfilepath)
			delete pfilepath;

		return m_nStatusCode;
	}

	int nStatusCode = 0;
	string stdstrBody;
	string stdstrReason;
	string stdstrResponseJsonData;

	int* pnStatusCode = new int(nStatusCode);
	string* pstdstrReason = new string(stdstrReason);
	string* pstdstrResponseJsonData = new string(stdstrResponseJsonData);

	long long* pllFileSize = new long long(0);

	stdstrPath = Utility::ReplaceAll(stdstrPath, m_stdstrUriPath, "");
	http_client client(utility::conversions::utf8_to_utf16(m_stdstrUri));
	uri_builder builder(utility::conversions::utf8_to_utf16(stdstrPath));

	if (!query.empty())
		builder.append_query(query);

	json::value jRequestData;

	if (!stdstrBody.empty())
	{
		try
		{
			jRequestData = json::value::parse(utility::conversions::utf8_to_utf16(stdstrBody));
		}
		catch (web::json::json_exception &e)
		{
			printf("json parse - %s\n", e.what());

			m_nStatusCode = 400;

			m_stdstrReason.clear();
			m_stdstrReason.assign(e.what());

			Clear();

			if (pfilepath)
			{
				delete pfilepath;
				pfilepath = NULL;
			}

			if (pllFileSize)
			{
				delete pllFileSize;
				pllFileSize = NULL;
			}

			if (pnStatusCode)
			{
				delete pnStatusCode;
				pnStatusCode = NULL;
			}

			if (pstdstrReason)
			{
				delete pstdstrReason;
				pstdstrReason = NULL;
			}

			if (pstdstrResponseJsonData)
			{
				delete pstdstrResponseJsonData;
				pstdstrResponseJsonData = NULL;
			}

			return m_nStatusCode;
		}
	}

	//concurrency::streams::basic_istream<unsigned char> fileStream = file_stream<unsigned char>::open_istream(filepath).get();

	//concurrency::streams::stringstreambuf streamBuffer;
	//fileStream.read_to_end(streamBuffer).get();
	//std::string stdstrTextFile = move(streamBuffer.collection());
	//fileStream.close();
	//streamBuffer.close();

	//int nIndex = -1;
	//stdstrFilePath = Utility::ReplaceAll(stdstrFilePath, "\\\\", "/");
	//stdstrFilePath = Utility::ReplaceAll(stdstrFilePath, "\\", "/");
	//nIndex = stdstrFilePath.find_last_of("/");
	//if (nIndex >= 0)
	//{
	//	stdstrFilePath = stdstrFilePath.substr(nIndex + 1);
	//}

	//std::string stdstrBoundary = "----WebKitFormBoundary2nQa2i0bF1UMsZBa";
	//stdstrBody += "--" + stdstrBoundary + "\r\n";
	////stdstrBody += "Content-Disposition: form-data; name=\"csrfmiddlewaretoken\"\r\n\r\n";
	////stdstrBody += "OhRvx5Q1k86lPPxVe3rrR4mZpPiffP8zkBmMZBXALmhKyW7y9xcy0wTelx778vgu\r\n";
	////stdstrBody += "--" + stdstrBoundary + "\r\n";
	//stdstrBody += "Content-Disposition: form-data; name=\"image\"; filename=\"" + stdstrFilePath + "\"\r\n";
	////stdstrBody += "Content-Disposition: form-data; name=\"image\"; filename=\"\"\r\n";
	////stdstrBody += "Content-Type: image/jpeg\r\n\r\n";
	//stdstrBody += "Content-Type: application/octet-stream\r\n\r\n";
	//stdstrBody += stdstrTextFile + "\r\n";
	////stdstrBody += "\r\n";

	//if (!stdstrTextData.empty())
	//{
	//	stdstrBody += "--" + stdstrBoundary + "\r\n";
	//	stdstrBody += "Content-Disposition: form-data; name=\"callback_url\"\r\n\r\n";
	//	stdstrBody += stdstrTextData + "\r\n";
	//}

	//stdstrBody += "--" + stdstrBoundary + "--\r\n";

	pplx::task<void> requestTask;
	pplx::task<http_response> reponseTask;

	http_request req(method);

	if (!m_stdstrAuthKey.empty() && !m_stdstrAuthValue.empty())
	{
		utility::string_t authKey = utility::conversions::utf8_to_utf16(m_stdstrAuthKey);
		utility::string_t authValue = utility::conversions::utf8_to_utf16(m_stdstrAuthValue);

		req.headers().add(authKey, authValue);
	}

	req.set_request_uri(builder.to_string());

	if (method.compare(U(METHOD_GET)))
		req.set_body(jRequestData);

	reponseTask = client.request(req);

	requestTask = reponseTask.then([&](http_response response)
	{
		utility::string_t reason = response.reason_phrase().c_str();

		*pllFileSize = (long long)response.headers().content_length();

		*pnStatusCode = response.status_code();
		pstdstrReason->clear();
		pstdstrReason->assign(utility::conversions::utf16_to_utf8(reason));

		pplx::task<void> reponseTask = response.extract_string()
			.then([&](utility::string_t responsedata)
		{
			pstdstrResponseJsonData->clear();
			pstdstrResponseJsonData->assign(utility::conversions::utf16_to_utf8(responsedata));
		});

		try
		{
			reponseTask.wait();

			if (pFunCB && pDataCB)
			{
				pFunCB(*pnStatusCode, *pstdstrReason, *pstdstrResponseJsonData, pDataCB);
			}
		}
		catch (http_exception const &e)
		{
			*pnStatusCode = 400;

			pstdstrReason->clear();
			pstdstrReason->assign(e.what());
			pstdstrResponseJsonData->clear();

			cout << e.what() << endl;
		}
	});

	try
	{
		requestTask.wait();

		m_nStatusCode = *pnStatusCode;
		nRet = m_nStatusCode;

		m_stdstrReason.clear();
		m_stdstrReason = *pstdstrReason;
		m_stdstrResponseJsonData = *pstdstrResponseJsonData;
	}
	catch (http_exception const &e)
	{
		m_nStatusCode = 400;

		m_stdstrReason.clear();
		m_stdstrReason.assign(e.what());
		m_stdstrResponseJsonData.clear();

		cout << e.what() << endl;

		nRet = e.error_code().value();
	}

	if (pfilepath)
	{
		delete pfilepath;
		pfilepath = NULL;
	}

	if (pllFileSize)
	{
		delete pllFileSize;
		pllFileSize = NULL;
	}

	if (pnStatusCode)
	{
		delete pnStatusCode;
		pnStatusCode = NULL;
	}

	if (pstdstrReason)
	{
		delete pstdstrReason;
		pstdstrReason = NULL;
	}

	if (pstdstrResponseJsonData)
	{
		delete pstdstrResponseJsonData;
		pstdstrResponseJsonData = NULL;
	}

	return nRet;
}

int CRestClient::Request(string stdstrMethod, string stdstrPath, string stdstrQuery, string stdstrBody)
{
	int nRet = 0;

	m_stdstrQuery = stdstrQuery;
	nRet = Request(stdstrMethod, stdstrPath, stdstrBody);

	return nRet;
}

string CRestClient::MakeRequestPath(pair<string, string>& pairMethodNPath, vector<string>& vecVars)
{
	string stdstrRetPath;

	vector<pair<string, string>> vecPathDepth = m_mapDepth[pairMethodNPath];
	vector<pair<string, string>>::iterator it = vecPathDepth.begin();
	vector<string>::iterator it2 = vecVars.begin();

	for (it; it != vecPathDepth.end(); it++)
	{
		pair<string, string> pairDepth = *it;
		if (!pairDepth.first.compare(DEPTH_TYPE_OF_PATH_VAR))
		{
			stdstrRetPath = stdstrRetPath + "/" + *it2;

			it2++;
		}
		else
		{
			stdstrRetPath = stdstrRetPath + pairDepth.second;
		}
	}

	return stdstrRetPath;
}

void CRestClient::SetResponseCB(string stdstrMethod, string stdstrPath, RESPONSECB pFuncCB, void* pDataCB)
{
	pair<string, string> pairMethodNPath(stdstrMethod, stdstrPath);

	m_mapFuncCB[pairMethodNPath] = pFuncCB;
	m_mapDataCB[pairMethodNPath] = pDataCB;

	SetInterfaceVarNConstDepth(pairMethodNPath, stdstrPath, 0);
}

void CRestClient::SetInterfaceVarNConstDepth(pair<string, string>& pairMethodNPath, string stdstrPath, int nDepth)
{
	int nVariableStartPos = stdstrPath.find("/{");
	int nVariableEndPos = stdstrPath.find("}");

	int nConstStartPos = stdstrPath.find("/");
	int nConstEndPos = 0;

	string stdstrRemainPath;
	vector<pair<string, string>> vecDepth = m_mapDepth[pairMethodNPath];

	if (nVariableStartPos == 0)
	{
		string stdstrVarName = stdstrPath.substr(0, nVariableEndPos + 1);
		pair<string, string> pairVar(DEPTH_TYPE_OF_PATH_VAR, stdstrVarName);
		vecDepth.push_back(pairVar);

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
		vecDepth.push_back(pairConst);
	}
	else
		return;

	m_mapDepth[pairMethodNPath] = vecDepth;

	SetInterfaceVarNConstDepth(pairMethodNPath, stdstrRemainPath, nDepth + 1);

	return;
}

void CRestClient::GetPairMethodNPath(string stdstrMethod, string stdstrReceivedPath, pair<string, string>& pairMethodNPath
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

void CRestClient::GetPathDepth(string stdstrPath, vector<pair<string, string>>& retVecDepthPair)
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

bool CRestClient::ComparePathDepth(string stdstrReceivedPath, vector<pair<string, string>>& vecDepthPair
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

void CRestClient::Clear()
{
	m_stdstrMethod.clear();
	m_stdstrQuery.clear();
	m_stdstrResponseJsonData.clear();
}

void CRestClient::SetServerUri(string stdstrUri)
{
	m_stdstrUri = stdstrUri;

	uri base_uri(utility::conversions::utf8_to_utf16(stdstrUri));
	utility::string_t base_path = base_uri.path();
	m_stdstrUriPath = utility::conversions::utf16_to_utf8(base_path);
	if (m_stdstrUriPath.compare("/") == 0)
		m_stdstrUriPath = "";
}