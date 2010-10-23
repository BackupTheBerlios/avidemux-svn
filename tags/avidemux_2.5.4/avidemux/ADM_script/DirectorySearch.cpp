// DirectorySearch.cpp: implementation of the CDirectorySearch class.
//
//////////////////////////////////////////////////////////////////////
/*
Copyright 2001-2005 Anish Mistry. All rights reserved.

Note:  This file is available under a BSD license.  Contact the author
at amistry@am-productions.biz
*/
#include <string.h>
#include "config.h"
#include "DirectorySearch.h"

#ifdef __WIN32
extern int wideCharStringToUtf8(const wchar_t *wideCharString, int wideCharStringLength, char *utf8String);
extern int utf8StringToWideChar(const char *utf8String, int utf8StringLength, wchar_t *wideCharString);
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#if defined(__unix__) || defined(__APPLE__)
int CDirectorySearch::_wfindnext(unsigned long int hDir,_wfinddata_t *pfdData)
{// begin _wfindnext
	if(!hDir || hDir == 0xFFFFFFFF)
		return -1; // need to call opendir first
	struct stat stInfo;
	struct dirent *pEntry;
	pEntry = readdir((DIR *)hDir);
	if(!pEntry)
		return -1;
	std::string sFilePath = "";
#if defined( __linux__) || defined(__macosx__) || defined(__MINGW32__)
	strncpy(pfdData->name,pEntry->d_name,pEntry->d_reclen);
	// append NULL terminator
	pfdData->name[pEntry->d_reclen] = '\0';
#elif defined(__FreeBSD__) || defined(__OpenBSD__)
	strncpy(pfdData->name,pEntry->d_name,pEntry->d_namlen);
	// append NULL terminator
	pfdData->name[pEntry->d_namlen] = '\0';
#endif
	sFilePath += m_sDirectory;
	sFilePath += DIRECTORY_DELIMITOR;
	sFilePath += pfdData->name;
	stat(sFilePath.c_str(),&stInfo);
	pfdData->time_access = stInfo.st_atime;
	pfdData->time_create = stInfo.st_ctime;
	pfdData->time_write = stInfo.st_mtime;
	pfdData->size = stInfo.st_size;
	// assign the modes
	pfdData->attrib = 0;
	if(S_ISREG(stInfo.st_mode))
		pfdData->attrib = _A_NORMAL;
	if(S_ISDIR(stInfo.st_mode))
		pfdData->attrib |= _A_SUBDIR;
	if(S_ISCHR(stInfo.st_mode) ||
	S_ISBLK(stInfo.st_mode) ||
#if !defined(__WIN32)	
	S_ISLNK(stInfo.st_mode) ||
	S_ISSOCK(stInfo.st_mode) ||
#endif
	S_ISFIFO(stInfo.st_mode))
		pfdData->attrib = _A_NONFILE;
	return 0;
}// end _wfindnext

int CDirectorySearch::_wfindfirst(const char *path,_wfinddata_t *pfdData)
{// begin _wfindfirst
	unsigned long int hDir = (unsigned long int)opendir(path);
	if(!hDir)
		return -1;
	if(_wfindnext(hDir,pfdData) == -1)
		return -1;
	return hDir;
}// end _wfindfirst

int CDirectorySearch::_findclose(unsigned long int hDir)
{// begin _findclose
	if(hDir == 0xFFFFFFFF)
		return -1;
	return closedir((DIR *)hDir);
}// end _findclose
#endif

CDirectorySearch::CDirectorySearch()
{
	m_hSearch = 0;
	m_sDirectory.clear();
}

CDirectorySearch::~CDirectorySearch()
{
	Close();
}

bool CDirectorySearch::Init(std::string sDirectory)
{// begin Init
	// check for no search query
	if(sDirectory[sDirectory.length()-1] == DIRECTORY_DELIMITOR)
	{
		printf("End with directory delimitor\n");
		return false;
	}

#ifdef __WIN32
	const char *old = sDirectory.c_str();

	int len = utf8StringToWideChar(old, -1, NULL);
	wchar_t s[len + 2];

	utf8StringToWideChar(old, -1, s);
	wcscat(s, L"\\*");

	printf(">%ls<\n", s);
#else
	const char *s = sDirectory.c_str();

	printf(">%s<\n",s);
#endif

	m_hSearch = _wfindfirst(s,&m_fdData);

	if(m_hSearch == -1)
	{
		printf("Find first failed\n");
		return false;
	}

	m_sDirectory = sDirectory;

	return true;
}// end Init

bool CDirectorySearch::Close()
{// begin Close
	bool bRtn = false;
	// clean up directory string
	m_sDirectory.clear();
	if(m_hSearch != 0)
	{
		bRtn = (bool)_findclose(m_hSearch);
		m_hSearch = 0;
	}
	return bRtn;
}// end Close

std::string CDirectorySearch::GetFileName()
{
	std::string sBuffer;

#if defined(__WIN32)
	int len = wideCharStringToUtf8(m_fdData.name, -1, NULL);
	char fileName[len];

	wideCharStringToUtf8(m_fdData.name, -1, fileName);

	sBuffer = fileName;
#else
	sBuffer = m_fdData.name;
#endif

	return sBuffer;
}

std::string CDirectorySearch::GetFilePath()
{// begin GetFilePath
	std::string sBuffer = m_sDirectory;

	if(sBuffer[sBuffer.length()-1] != DIRECTORY_DELIMITOR && m_fdData.name[0] != DIRECTORY_DELIMITOR)
		sBuffer += DIRECTORY_DELIMITOR;

#if defined(__WIN32)
	int len = wideCharStringToUtf8(m_fdData.name, -1, NULL);
	char filePath[len];

	wideCharStringToUtf8(m_fdData.name, -1, filePath);

	sBuffer += filePath;
#else
	sBuffer += m_fdData.name;
#endif

	return sBuffer;
}// end GetFilePath

std::string CDirectorySearch::GetFileDirectory(std::string sFilePath) const
{// begin GetFileDirectory
	std::string sCurrentDirectory = "";
	for(int i = sFilePath.length()-1;i >= 0;i--)
		if(sFilePath[i] == DIRECTORY_DELIMITOR)
		{// begin copy directory name into buffer
#ifdef __WIN32
			if(sFilePath[0] == DIRECTORY_DELIMITOR)
			{
				sFilePath.erase(0,1);
				i--;
			}
#endif
			sCurrentDirectory = sFilePath.substr(0,i+1);
			break;
		}// end copy directory name into buffer
	return sCurrentDirectory;
}// end GetFileDirectory

std::string CDirectorySearch::GetFileDirectory()
{// begin GetFileDirectory
	return m_sDirectory;
}// end GetFileDirectory

bool CDirectorySearch::IsSingleDot()
{// begin IsSingleDot
#if defined(__WIN32)
	return (IsDirectory() && wcscmp(L".", m_fdData.name) == 0);
#else
	return (IsDirectory() && strcmp(".",m_fdData.name) == 0);
#endif
}// end IsSingleDot

bool CDirectorySearch::IsDoubleDot()
{// begin IsDoubleDot
#if defined(__WIN32)
	return (IsDirectory() && wcscmp(L"..", m_fdData.name) == 0);
#else
	return (IsDirectory() && strcmp("..",m_fdData.name) == 0);
#endif
}// end IsDoubleDot

bool CDirectorySearch::IsExtension(const char *pExtension)
{// begin IsExtension
	int nExtLen = strlen(pExtension);
	char *pTempBuffer = new char[nExtLen+1];
	std::string fileExt = GetExtension();

	strcpy(pTempBuffer, fileExt.c_str());

	for(int i = 0,nCurrExtLen = 0;i < nExtLen;i++,nCurrExtLen++)
	{
		if(pTempBuffer[i+1] == '\0')
		{
			nCurrExtLen++;
			i++;
			pTempBuffer[i] = ';';
		}
		if(pTempBuffer[i] == ';' && nCurrExtLen)
		{
			pTempBuffer[i] = '\0';
			if(pTempBuffer[i-nCurrExtLen] == '*' || pTempBuffer[i-nCurrExtLen] == '?' 
				|| strcmp(&pTempBuffer[i-nCurrExtLen], fileExt.c_str()) == 0)
			{
				// free the buffer
				delete []pTempBuffer;
				return true;
			}
			nCurrExtLen = -1;
		}
	}
	// free the buffer
	delete []pTempBuffer;
	return false;
}// end IsExtension

std::string CDirectorySearch::GetExtension()
{// begin GetExtension
#if defined(__WIN32)
	int len = wideCharStringToUtf8(m_fdData.name, -1, NULL);
	char filePath[len];

	wideCharStringToUtf8(m_fdData.name, -1, filePath);
#else
	char *filePath = m_fdData.name;
#endif

	std::string sBuffer;

	for (int i = strlen(filePath) - 1; i >= 0; i--)
	{
		if (filePath[i] == '.')
			sBuffer = &filePath[i + 1];
	}

	return sBuffer;
}// end GetExtension

