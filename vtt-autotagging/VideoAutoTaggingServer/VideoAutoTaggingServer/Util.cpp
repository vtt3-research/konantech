#include "StdAfx.h"
#include "Util.h"

#include "KException.h"

#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <direct.h>
#include <tchar.h>

#define __SOURCE__		_T("Util.cpp")


CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}


CString CUtil::GetCurTime()
{
	CString strTime;
	CTime cTime = CTime::GetCurrentTime();
	strTime.Format( _T("%d-%02d-%02d %02d:%02d:%02d "), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	return strTime;
}

BOOL CUtil::IsPathExist (LPCTSTR lpszPath)
{
	struct _tstati64 stati64;
	int nRet = _tstati64 (lpszPath, &stati64);
	if (nRet == 0)
		return TRUE;

	return FALSE;
}

BOOL CUtil::IsFile (LPCTSTR lpszPathName)
{
	struct _tstati64 stati64;
	int nRet = _tstati64 (lpszPathName, &stati64);
	if (nRet == 0)
	{
		if (stati64.st_mode & _S_IFREG)
		{
			if(!(stati64.st_mode & _S_IFDIR))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CUtil::IsDirectory (LPCTSTR lpszPathName)
{
	struct _tstati64 stati64;
	int nRet = _tstati64 (lpszPathName, &stati64);
	if (nRet == 0)
	{
		if (stati64.st_mode & _S_IFDIR)
			return TRUE;
	}

	return FALSE;
}

BOOL CUtil::MakePath (LPCTSTR lpszPathName, BOOL bFile)
{
	int nIndex;
	CString strPathName, strCurPath, strRestPathName;

	strPathName.Format(_T("%s"), lpszPathName);
	strPathName.Replace(_T('/'), _T('\\'));

	strPathName.Replace(_T("//"), _T("/"));

	strCurPath = _T("");
	strRestPathName = strPathName;

	nIndex = 0;
	do
	{
		nIndex = strRestPathName.Find(_T('\\'));
		if (nIndex < 0) 
		{
			if (bFile == FALSE)
				_tmkdir(strPathName);

			break;
		}

		strCurPath += strRestPathName.Mid(0, nIndex);
		strRestPathName = strRestPathName.Mid(nIndex+1);

		_tmkdir(strCurPath);

		strCurPath += _T('\\');
	} while (1);

	return TRUE;
}

CString CUtil::FixPrefix (LPCTSTR lpszPrefix)
{
	CString strPrefix(lpszPrefix);

	strPrefix.Trim(_T(' '));
	if (strPrefix.IsEmpty () == TRUE)
		return strPrefix;

	strPrefix.Replace(_T("::"), _T(":"));

	strPrefix.Replace(_T('/'), _T('\\'));
	strPrefix.Replace(_T("\\\\"), _T("\\"));

	if (strPrefix.GetAt(strPrefix.GetLength() - 1) == _T('\\'))
		strPrefix = strPrefix.Mid(0, strPrefix.GetLength() - 1);

	return strPrefix;

}

CString CUtil::FixMidPathName (LPCTSTR lpszPathName)
{
	CString strPathName(lpszPathName);

	strPathName.Trim(_T(' '));
	if (strPathName.IsEmpty () == TRUE)
		return strPathName;

	strPathName.Replace(_T('/'), _T('\\'));
	strPathName.Replace(_T("\\\\"), _T("\\"));

	if (strPathName.GetAt(0) != _T('\\') && strPathName.GetAt(0) != _T('.')) strPathName = _T("\\") + strPathName;

	if (strPathName.GetAt(strPathName.GetLength() - 1) == _T('\\'))
		strPathName = strPathName.Mid(0, strPathName.GetLength() - 1);

	return strPathName;
}

__int64 CUtil::GetFileSize (LPCTSTR lpszFilePathName)
{
	struct _tstati64 stati64;
	int nRet = _tstati64 (lpszFilePathName, &stati64);
	if (nRet == 0)
		return stati64.st_size;

	return -1;
}

CString CUtil::GetVolumeLetter (LPCTSTR lpszFilePathName)
{
	int nIndex;
	CString strVolumeLetter(lpszFilePathName);

	nIndex = strVolumeLetter.ReverseFind (_T(':'));
	if (nIndex >= 0)
		strVolumeLetter = strVolumeLetter.Mid(0, nIndex);
	else 
		strVolumeLetter = _T("");

	if (strVolumeLetter.GetLength () != 1)
		strVolumeLetter = _T("");		

	return strVolumeLetter;
}

CString CUtil::GetFilePath (LPCTSTR lpszFilePathName)
{
	int nIndex;
	CString strFilePath;

	strFilePath.Format(_T("%s"), lpszFilePathName);
	strFilePath.Replace(_T('/'), _T('\\'));

	nIndex = strFilePath.ReverseFind (_T('\\'));
	if (nIndex >= 0)
		strFilePath = strFilePath.Mid(0, nIndex);
	else 
		strFilePath = _T("");

	return strFilePath;
}

CString CUtil::GetFileName (LPCTSTR lpszFilePathName)
{
	int nIndex;
	CString strFileName;

	strFileName.Format(_T("%s"), lpszFilePathName);
	strFileName.Replace(_T('/'), _T('\\'));

	nIndex = strFileName.ReverseFind (_T('\\'));
	if (nIndex >= 0)
		strFileName = strFileName.Mid(nIndex + 1);
	else 
		strFileName = _T("");

	return strFileName;
}

CString CUtil::GetFileTitle (LPCTSTR lpszFilePathName)
{
	int nIndex;
	CString strFileTitle;

	strFileTitle.Format(_T("%s"), lpszFilePathName);
	strFileTitle.Replace(_T('/'), _T('\\'));

	nIndex = strFileTitle.ReverseFind (_T('\\'));
	if (nIndex >= 0)
		strFileTitle = strFileTitle.Mid(nIndex + 1);

	nIndex = strFileTitle.ReverseFind(_T('.'));
	if (nIndex >= 0)
		strFileTitle = strFileTitle.Mid(0, nIndex);

	return strFileTitle;
}

CString CUtil::GetFileExtention (LPCTSTR lpszFilePathName)
{
	CString strFileName, strExtention;
	strFileName.Format(_T("%s"), lpszFilePathName);
	int nIndex = strFileName.Find(_T('.'));
	if (nIndex >= 0)
		strExtention = strFileName.Mid(nIndex + 1);
	else 
		strExtention = _T("");

	return strExtention;
}

BOOL CUtil::DeleteFile(LPCTSTR lpszFilePathName)
{
	CString strMsg;

	DECL_EX_HANDLING_VAR();
	try
	{
		int nRet = _unlink (lpszFilePathName);
		if (nRet == -1)
		{
			strMsg.Format(_T("_unlink(%s)"), lpszFilePathName);
			THROW_EX(strMsg, GetLastError(), 0);
		}
	}
	catch (CKException *pEx)
	{
		EXMSGOUT();

		delete pEx;

		return FALSE;
	}

	return TRUE;
}

BOOL CUtil::DeleteFolder(LPCTSTR lpszFolerPathName, BOOL IncludeRoot)
{
	BOOL bRet;
	int nRet=0;
	CString strMsg;
	CString strFolderPathName, strPathName;

	intptr_t hFile=-1L;
	struct _tfinddatai64_t	finderi64;
	DECL_EX_HANDLING_VAR();
	try
	{
		strFolderPathName.Format(_T("%s"), lpszFolerPathName);
		strFolderPathName.Replace(_T('/'), _T('\\'));
		strFolderPathName.Replace(_T("\\\\"), _T("\\"));
		if (strFolderPathName.GetAt(strFolderPathName.GetLength ()-1) == _T('\\'))
			strFolderPathName.Delete(strFolderPathName.GetLength ()-1);

		bRet = IsDirectory (strFolderPathName);
		if (bRet == FALSE)
		{
			strMsg.Format(_T("Invalid folder path (%s)"), strFolderPathName);
			THROW_EX(strMsg, -1, 0);
		}

		CString strQuery;
		strQuery.Format(_T("%s\\*.*"), strFolderPathName);
		hFile = _tfindfirsti64 (strQuery, &finderi64);
		if (hFile == -1L)
		{
			strMsg.Format(_T("_tfindfirsti64(%s)"), strQuery);
			THROW_CRUNTIME_EX(strMsg, 0)	
		}

		do
		{
			if (_tcscmp (finderi64.name, _T(".")) == 0 
				|| _tcscmp (finderi64.name, _T("..")) == 0)
			{
				continue;					
			}
			else 
			{
				strPathName.Format(_T("%s\\%s"), strFolderPathName, finderi64.name);

				if (finderi64.attrib & _A_SUBDIR) // if folder
				{
					DeleteFolder(strPathName);
				}
				else
				{
					nRet = _tunlink (strPathName);
					if (nRet == -1)
					{
						strMsg.Format(_T("_tunlink(%s)"), strPathName);
						THROW_CRUNTIME_EX(strMsg, 0)	
					}
				}
			}

		} while (_tfindnexti64(hFile, &finderi64) == 0);

		if (hFile != -1) 
		{
			_findclose(hFile);
			hFile = -1;
		}

		if (IncludeRoot)
		{
			nRet = _trmdir(strFolderPathName);
			if (nRet == -1)
			{
				strMsg.Format(_T("_trmdir(%s)"), strFolderPathName);
				THROW_CRUNTIME_EX(strMsg, 0)	
			}
		}
	}
	catch (CKException *pEx)
	{
		EXMSGOUT();

		if (hFile != -1) 
		{
			_findclose(hFile);
			hFile = -1;
		}

		delete pEx;

		return FALSE;
	}

	return TRUE;
}

BOOL CUtil::List (LPCTSTR lpszRootDir, LPCTSTR lpszSubDir, CList<CString ,CString &> * pExceptFileList, CList<CFileItemInfo ,CFileItemInfo &> *pFileList, __int64 *plnTotalSize, BOOL bIncludeSubDir)
{
	int nRet=0;
	CString strMsg;
	CString strRootDir, strSubDir, strFolderPath;
	CString strSubDirPathName, strFilePathName;

	intptr_t hFile=-1L;
	struct _tfinddatai64_t	finderi64;
	CString month [] = { _T("Jan"),_T("Feb"), _T("Mar") ,_T("Apr"), _T("May"), _T("Jun"), 
		_T("Jul"), _T("Aug"), _T("Sep"), _T("Oct"), _T("Nov"), _T("Dec") };

	DECL_EX_HANDLING_VAR();
	try
	{
		strRootDir.Format(_T("%s"), lpszRootDir);
		strRootDir.Replace(_T('/'), _T('\\'));
		strRootDir.Replace(_T("\\\\"), _T("\\"));
		if (strRootDir.GetAt(strRootDir.GetLength ()-1) == _T('\\'))
			strRootDir.Delete(strRootDir.GetLength ()-1);

		strSubDir.Format(_T("%s"), lpszSubDir);
		if (strSubDir.IsEmpty() == false)
		{
			strSubDir.Replace(_T('/'), _T('\\'));
			strSubDir.Replace(_T("\\\\"), _T("\\"));
			if (strSubDir.GetAt(0) != _T('\\'))
				strSubDir = _T("\\") + strSubDir;
			if (strSubDir.GetAt(strSubDir.GetLength () - 1) == _T('\\'))
				strSubDir.Delete(strSubDir.GetLength () - 1);
		}

		strFolderPath.Format(_T("%s%s"), strRootDir, strSubDir);

		nRet = _taccess(strFolderPath, 0);
		if (nRet == -1)
		{
			strMsg.Format(_T("_taccess (%s)"), strFolderPath);
			THROW_CRUNTIME_EX(strMsg, 0);	
		}

		CString strQuery;
		strQuery.Format(_T("%s\\*.*"), strFolderPath);
		hFile = _tfindfirsti64 (strQuery, &finderi64);
		if (hFile != -1)
		{
			do
			{
				strFilePathName.Format(_T("%s\\%s"), strFolderPath, finderi64.name);

				bool bFindExceptFile = FALSE;

				for(int i=0;i<pExceptFileList->GetCount();i++)
				{
					CString strExceptFile = pExceptFileList->GetAt(pExceptFileList->FindIndex(i));
					strExceptFile.Replace(_T("/"), _T("\\"));
					strExceptFile.Replace(_T("\\\\"), _T("\\"));

					if(strFilePathName.CompareNoCase(strExceptFile.GetString())==0)
					{
						bFindExceptFile = TRUE;
						break;
					}
				}

				if(bFindExceptFile == TRUE) continue;

				if (_tcscmp (finderi64.name, _T(".")) == 0 
					|| _tcscmp (finderi64.name, _T("..")) == 0)
				{
					continue;					
				}
				else 
				{
					if (pFileList)
					{
						CFileItemInfo fileInfo;
						fileInfo.bIsDirectory = ((finderi64.attrib & _A_SUBDIR) ? TRUE : FALSE);
						fileInfo.nFileSize = finderi64.size;
						fileInfo.strRootPath.Format(_T("%s"), strRootDir);
						fileInfo.strMidPath.Format(_T("%s"), strSubDir);
						fileInfo.strFileName.Format(_T("%s"), finderi64.name);

						CTime time(finderi64.time_write);
						fileInfo.strTime.Format(_T("%s %0.2d %d:%0.2d"), 
							(LPSTR)(LPCTSTR) month[time.GetMonth()-1], 
							time.GetDay (), 
							time.GetHour (), 
							time.GetMinute ());

						fileInfo.strAccess = _T("");
						fileInfo.strGroup = _T("");
						fileInfo.strOwnder= _T(""); 

						pFileList->AddTail (fileInfo);
					}

					(*plnTotalSize) += finderi64.size;

					// if folder
					if (finderi64.attrib & _A_SUBDIR)
					{
						strSubDirPathName.Format(_T("%s\\%s"), lpszSubDir, finderi64.name);
						if (bIncludeSubDir == TRUE)
						{
							// Add sub directory
							List (strRootDir, strSubDirPathName, pExceptFileList, pFileList, plnTotalSize, bIncludeSubDir);
						}
						else 
						{
							List (strRootDir, strSubDirPathName, pExceptFileList, NULL, plnTotalSize, bIncludeSubDir);
						}
					}
				}

			} while (_tfindnexti64(hFile, &finderi64) == 0);
		}

		if (hFile != -1) 
		{
			_findclose(hFile);
			hFile = -1;
		}
	}
	catch (CKException *pEx)
	{
		EXMSGOUT();

		if (hFile != -1) 
		{
			_findclose(hFile);
			hFile = -1;
		}

		delete pEx;

		return FALSE;
	}

	return TRUE;
}

BOOL CUtil::List (LPCTSTR lpszRootDir, LPCTSTR lpszSubDir, CList<CFileItemInfo, CFileItemInfo &> *pFileList, __int64 *plnTotalSize, BOOL bIncludeSubDir)
{
	int nRet=0;
	CString strMsg;
	CString strRootDir, strSubDir, strFolderPath;
	CString strSubDirPathName, strFilePathName;

	intptr_t hFile=-1L;
	struct _tfinddatai64_t	finderi64;
	CString month [] = { _T("Jan"),_T("Feb"), _T("Mar") ,_T("Apr"), _T("May"), _T("Jun"), 
		_T("Jul"), _T("Aug"), _T("Sep"), _T("Oct"), _T("Nov"), _T("Dec") };

	DECL_EX_HANDLING_VAR();
	try
	{
		strRootDir.Format(_T("%s"), lpszRootDir);
		strRootDir.Replace(_T('/'), _T('\\'));
		strRootDir.Replace(_T("\\\\"), _T("\\"));
		if (strRootDir.GetAt(strRootDir.GetLength ()-1) == _T('\\'))
			strRootDir.Delete(strRootDir.GetLength ()-1);

		strSubDir.Format(_T("%s"), lpszSubDir);
		if (strSubDir.IsEmpty() == false)
		{
			strSubDir.Replace(_T('/'), _T('\\'));
			strSubDir.Replace(_T("\\\\"), _T("\\"));
			if (strSubDir.GetAt(0) != _T('\\'))
				strSubDir = _T("\\") + strSubDir;
			if (strSubDir.GetAt(strSubDir.GetLength () - 1) == _T('\\'))
				strSubDir.Delete(strSubDir.GetLength () - 1);
		}

		strFolderPath.Format(_T("%s%s"), strRootDir, strSubDir);

		nRet = _taccess(strFolderPath, 0);
		if (nRet == -1)
		{
			strMsg.Format(_T("_taccess (%s)"), strFolderPath);
			THROW_CRUNTIME_EX(strMsg, 0);	
		}

		CString strQuery;
		strQuery.Format(_T("%s\\*.*"), strFolderPath);
		hFile = _tfindfirsti64 (strQuery, &finderi64);
		if (hFile != -1)
		{
			do
			{
				strFilePathName.Format(_T("%s\\%s"), strFolderPath, finderi64.name);

				if (_tcscmp (finderi64.name, _T(".")) == 0 
					|| _tcscmp (finderi64.name, _T("..")) == 0)
				{
					continue;					
				}
				else 
				{
					if (pFileList)
					{
						CFileItemInfo fileInfo;
						fileInfo.bIsDirectory = ((finderi64.attrib & _A_SUBDIR) ? TRUE : FALSE);
						fileInfo.nFileSize = finderi64.size;
						fileInfo.strRootPath.Format(_T("%s"), strRootDir);
						fileInfo.strMidPath.Format(_T("%s"), strSubDir);
						fileInfo.strFileName.Format(_T("%s"), finderi64.name);

						CTime time(finderi64.time_write);
						fileInfo.strTime.Format(_T("%s %0.2d %d:%0.2d"), 
							(LPSTR)(LPCTSTR) month[time.GetMonth()-1], 
							time.GetDay (), 
							time.GetHour (), 
							time.GetMinute ());

						fileInfo.strAccess = _T("");
						fileInfo.strGroup = _T("");
						fileInfo.strOwnder= _T(""); 

						pFileList->AddTail (fileInfo);
					}

					(*plnTotalSize) += finderi64.size;

					// if folder
					if (finderi64.attrib & _A_SUBDIR)
					{
						strSubDirPathName.Format(_T("%s\\%s"), lpszSubDir, finderi64.name);
						if (bIncludeSubDir == TRUE)
						{
							// Add sub directory
							List (strRootDir, strSubDirPathName, pFileList, plnTotalSize, bIncludeSubDir);
						}
						else 
						{
							List (strRootDir, strSubDirPathName, NULL, plnTotalSize, bIncludeSubDir);
						}
					}
				}

			} while (_tfindnexti64(hFile, &finderi64) == 0);
		}

		if (hFile != -1) 
		{
			_findclose(hFile);
			hFile = -1;
		}
	}
	catch (CKException *pEx)
	{
		EXMSGOUT();

		if (hFile != -1) 
		{
			_findclose(hFile);
			hFile = -1;
		}

		delete pEx;

		return FALSE;
	}

	return TRUE;
}

BOOL CUtil::IsHostIP(LPCTSTR lpszHostName)
{
	BOOL bResult = TRUE;

	CString strHostName;
	strHostName.Format(_T("%s"), lpszHostName);

	for(int i=0; i< strHostName.GetLength(); i++)
	{
		if( strHostName.GetAt(i) != '.' && isdigit(strHostName.GetAt(i)) == 0 ) 
			bResult = FALSE;
	}

	return bResult;
}

CString CUtil::m_getHostIP(LPCTSTR lpszHostName, BOOL & bResult)
{
	WSADATA wsaData;
	int nResult = 0;
	DWORD dwError = 0;
	struct hostent *remoteHost;
	struct in_addr addr;

	CString strHostName;
	CString strErrMsg;

	bResult = TRUE;
	strHostName.Format(_T("%s"), lpszHostName);

	CT2CA pszConvertedAnsiString(strHostName);
	string strIpAddr(pszConvertedAnsiString);

	// Initialize Winsock
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (nResult != 0) 
	{
		bResult = FALSE;
		return strErrMsg;
	}

	remoteHost = gethostbyname(strIpAddr.c_str());

	if (remoteHost == NULL) 
	{
		dwError = WSAGetLastError();
		if (dwError != 0) 
		{
			if (dwError == WSAHOST_NOT_FOUND) 
			{
				strErrMsg.Format(_T("Host not found"));
				bResult = FALSE;
				return strErrMsg;
			} else if (dwError == WSANO_DATA) 
			{
				strErrMsg.Format(_T("No data record found"));
				bResult = FALSE;
				return strErrMsg;
			} else 
			{
				strErrMsg.Format(_T("Function failed with error : %ld"), dwError);
				bResult = FALSE;
				return strErrMsg;
			}
		}
	}
	else
	{
		addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
		strIpAddr = string(inet_ntoa(addr));
	} 	

	strHostName.Format(_T("%s"),strIpAddr.c_str());

	return strHostName;
}

CString CUtil::GeneralizationPath(LPCTSTR lpszPath)
{
	CString strPath;
	strPath.Format(_T("%s"), lpszPath);

	while(1)
	{
		if(strPath.Replace("/","\\")<=0)
			break;
	}

	while(1)
	{
		if(strPath.Replace("\\\\","\\")<=0)
			break;
	}

	return strPath;
}

CString CUtil::BuildRootPath(LPCTSTR lpszRootPath)
{
	CString strRootPath;
	strRootPath.Format(_T("%s"), lpszRootPath);

	if(IsHostIP(strRootPath) != TRUE)
	{
		int npoint1 = strRootPath.Find(_T(":"));

		if(npoint1 < 0)  strRootPath += _T(":");

		if (strRootPath.Right (1) != _T ("\\"))
			strRootPath += _T ("\\");
	}

	return strRootPath;
}

CString CUtil::BuildFullPath(LPCTSTR lpszRootPath, LPCTSTR lpszFilePath)
{
	CString strRoot;
	CString strFilePath;
	CString strFullPath;

	strRoot.Format(_T("%s"),BuildRootPath(lpszRootPath));
	strFilePath.Format(_T("%s"),lpszFilePath);

	strFullPath = strRoot + strFilePath;

	return GeneralizationPath(strFullPath.GetString());
}