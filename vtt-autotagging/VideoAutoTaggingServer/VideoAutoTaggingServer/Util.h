#pragma once

class CFileItemInfo 
{
public:
	CFileItemInfo() : 
	  strFileName("")
		  , strMidPath("")
		  , strRootPath("")
		  , nFileSize(0)
		  , strType("")
		  , strTime("")
		  , strGroup("")
		  , strAccess("")
		  , strOwnder("")
		  , bIsDirectory(FALSE)
	  {};
	  virtual~CFileItemInfo() {};

	  CFileItemInfo& operator=(const CFileItemInfo operand)
	  {
		  strFileName = operand.strFileName;
		  strMidPath	=  operand.strMidPath;
		  strRootPath	=  operand.strRootPath;
		  nFileSize = operand.nFileSize;
		  strType = operand.strType;
		  ftLastWriteTime = operand.ftLastWriteTime;	
		  strTime = operand.strTime;
		  strGroup = operand.strGroup;
		  strAccess = operand.strAccess;
		  strOwnder = operand.strOwnder;
		  bIsDirectory = operand.bIsDirectory;

		  return *this;
	  };

	  CString GetFilePathNameWithoutRootDir ()
	  {
		  CString strPathName = strMidPath +"\\"+ strFileName;
		  strPathName.Replace(_T('/'), _T('\\'));
		  strPathName.Replace(_T("//"), _T("/"));

		  return strPathName;
	  };

	  CString GetFullFilePath()
	  {
		  CString strPathName = strRootPath + "\\" + strMidPath + "\\" + strFileName;
		  strPathName.Replace(_T('/'), _T('\\'));
		  strPathName.Replace(_T("//"), _T("/"));

		  return strPathName;
	  };

	  CString					strFileName;
	  CString					strMidPath;
	  CString					strRootPath;
	  unsigned __int64		nFileSize; 
	  CString					strType; 
	  FILETIME				ftLastWriteTime; 

	  CString					strTime;	
	  CString					strGroup;
	  CString					strAccess;
	  CString					strOwnder;	

	  BOOL					bIsDirectory;
};

class CUtil
{
public:
	CUtil(void);
	virtual ~CUtil(void);
	static CString GetCurTime();
	
	static BOOL IsPathExist (LPCTSTR lpszPath);
	static BOOL IsFile (LPCTSTR lpszPathName);
	static BOOL IsDirectory (LPCTSTR lpszPathName);

	static BOOL MakePath (LPCTSTR lpszPathName, BOOL bFile=TRUE);
	static CString FixPrefix (LPCTSTR lpszPrefix);
	static CString FixMidPathName (LPCTSTR lpszPathName);

	static __int64 GetFileSize (LPCTSTR lpszFilePathName);

	static CString GetVolumeLetter (LPCTSTR lpszFilePathName);
	static CString GetFilePath (LPCTSTR lpszFilePathName);
	static CString GetFileName (LPCTSTR lpszFilePathName);
	static CString GetFileTitle (LPCTSTR lpszFilePathName);
	static CString GetFileExtention (LPCTSTR lpszFilePathName);

	static BOOL DeleteFile(LPCTSTR lpszFilePathName);
	static BOOL DeleteFolder(LPCTSTR lpszFilePathName, BOOL IncludeRoot=TRUE);

	static BOOL List (LPCTSTR lpszRootDir, LPCTSTR lpszSubDir, CList<CFileItemInfo ,CFileItemInfo &> *pFileList, __int64 *plnTotalSize, BOOL bIncludeSubDir=TRUE);
	static BOOL List (LPCTSTR lpszRootDir, LPCTSTR lpszSubDir, CList<CString ,CString &> * pExceptFileList, CList<CFileItemInfo ,CFileItemInfo &> *pFileList, __int64 *plnTotalSize, BOOL bIncludeSubDir=TRUE);

	static BOOL IsHostIP(LPCTSTR strHostName);
	static CString m_getHostIP(LPCTSTR lpszHostName, BOOL & bResult);

	static CString GeneralizationPath(LPCTSTR lpszPath);

	static CString BuildRootPath(LPCTSTR lpszRootPath);
	static CString BuildFullPath(LPCTSTR lpszRootPath, LPCTSTR lpszFilePath);
};
