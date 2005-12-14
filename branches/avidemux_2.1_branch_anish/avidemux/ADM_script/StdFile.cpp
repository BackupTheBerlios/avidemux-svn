// StdFile.cpp: implementation of the CStdFile class.
//
//////////////////////////////////////////////////////////////////////
/*
Copyright 2001-2005 Anish Mistry. All rights reserved.

Note:  This file is available under a BSD license.  Contact the author
at amistry@am-productions.biz
*/

#include "StdFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStdFile::CStdFile()
{// begin CStdFile constructor
	m_pPath = NULL;
	m_pName = NULL;
}// end CStdFile constructor

CStdFile::~CStdFile()
{// begin CStdFile destructor
	Close();
}// end CStdFile destructor

bool CStdFile::Open(const char *pFileName, ios::openmode nAccessFlags)
{// begin Open
	if(m_pPath != NULL || m_pName != NULL)
		throw("Error:  Trying to open another file on the same object without closing the previous file.");

	m_fsFile.open(pFileName,nAccessFlags);
	if(!m_fsFile)	// fail if file couldn't be opened
		return false;
	char *pTempName = new char[strlen(pFileName)];
	GetName(pFileName,pTempName);
	char pFilePath[_MAX_PATH+1] = {NULL};
	getcwd(pFilePath,_MAX_PATH);
	std::string sFilePath = pFilePath;
	if(sFilePath[sFilePath.size()-1] != DIRECTORY_DELIMITOR)
		sFilePath += DIRECTORY_DELIMITOR;
	sFilePath += pTempName;
	// allocate memory for path
	m_pPath = new char[sFilePath.size()+1];
	// copy to path buffer
	strncpy(m_pPath,sFilePath.c_str(),sFilePath.length()+1);
	// we don't need to allocate m_pName since pTempMame was
	// already allocated
	m_pName = pTempName;
	return true;
}// end Open

unsigned long int CStdFile::WriteLine(const char *pBuffer)
{// begin WriteLine
	unsigned long int nBytesWritten = Write(pBuffer,strlen(pBuffer));
	if(nBytesWritten)
	{// begin write endline
#ifdef WIN32
		unsigned long int nTemp = Write("\r\n",2);
#else
		unsigned long int nTemp = Write("\n",1);
#endif
		if(!nTemp)
		{
			return nBytesWritten;
		}
		return nBytesWritten+nTemp;
	}// end write endline
	return nBytesWritten;
}// end WriteLine

unsigned long int CStdFile::ReadLine(char *pBuffer)
{// begin ReadLine
	int i;
	for(i = 0;Read(&pBuffer[i], 1);i++)
	{// begin read file
		if(pBuffer[i] == '\r' || pBuffer[i] == '\n')
		{// begin end of line found
			pBuffer[i] = NULL;
			if(pBuffer[i] == '\r')	// need this for windows files
				Seek(1,ios::cur);
			return i;
		}// end end of line found		
	}// end read file
	return i;
}// end ReadLine

bool CStdFile::Close()
{// begin Close
	if(m_pPath)
		delete []m_pPath;
	if(m_pName)
		delete []m_pName;
	m_pPath = NULL;
	m_pName = NULL;
	m_fsFile.close();
	return true;
}// end Close

int CStdFile::Delete(const char *pFileName)
{// begin Delete
	return remove(pFileName);
}// end Delete

void CStdFile::GetName(const char *pFileNamePath, char *pBufferOut)
{// begin GetName
	int i;
	for(i = strlen(pFileNamePath)-1;pFileNamePath[i] != DIRECTORY_DELIMITOR && i > -1;i--);
	strncpy(pBufferOut,&pFileNamePath[i+1],(strlen(pFileNamePath))-i);
}// end GetName

const char * CStdFile::GetFileName()
{// begin GetFileName
	return m_pName;
}// end GetFileName

const char * CStdFile::GetFilePath()
{// begin GetFilePath
	return m_pPath;
}// end GetFilePath

unsigned long int CStdFile::Read(char *pBuffer, unsigned long int nBytesToRead)
{// begin Read
	m_fsFile.read(pBuffer,nBytesToRead);
	pBuffer[nBytesToRead] = '\0';
	if(m_fsFile.fail() || m_fsFile.eof())
		return 0;	// function failed
	return nBytesToRead;
}// end Read

unsigned long int CStdFile::Write(const char *pBuffer, unsigned long int nBytesToWrite)
{// begin Write
	m_fsFile.write(pBuffer,nBytesToWrite);
	if(m_fsFile.fail())
		return 0;	// function failed
	return nBytesToWrite;
}// end Write

unsigned long int CStdFile::GetLength()
{// begin GetLength
	// save the current position
	unsigned long int nCurrentPos = GetPos();
	Seek(0,ios::end);
	// the current pos it the length of the file
	unsigned long int nFileLength = GetPos();
	Seek(nCurrentPos,ios::beg);
	return nFileLength;
}// end GetLength

unsigned long int CStdFile::Seek(unsigned long int nPos,ios::seekdir fStartPos)
{// begin Seek
	if(m_fsFile.flags() & ios::in)
		m_fsFile.seekg(nPos,fStartPos);
	if(m_fsFile.flags() & ios::out)
		m_fsFile.seekp(nPos,fStartPos);
	return GetPos();
}// end Seek

int CStdFile::Rename(const char *pSource, const char *pDest)
{// begin Rename
	return rename(pSource,pDest);
}// end Rename

unsigned long int CStdFile::GetPos()
{// begin GetPos
	unsigned long int nPos = 0;
	if(m_fsFile.flags() & ios::in)
		nPos = m_fsFile.tellg();
	if(m_fsFile.flags() & ios::out)
		nPos = m_fsFile.tellp();
	return nPos;
}// end GetPos

char * CStdFile::ReadString(char *pBuffer, int nMaxLen,char cDelim)
{// begin ReadString
	char *pStart = pBuffer;
	while(Read(pBuffer,1) && *pBuffer != cDelim && pBuffer-pStart < nMaxLen)
	{
		pBuffer++;
	}
	// add the null terminator
	*pBuffer = '\0';
	return pStart;
}// end ReadString

int CStdFile::WriteString(const char *pBuffer)
{// begin WriteString
	int nLength = strlen(pBuffer);
	return Write(pBuffer,nLength);
}// end WriteString


char CStdFile::Peek()
{// begin Peek
	return (char)m_fsFile.peek();
}// end Peek
/*
bool CStdFile::Copy(const char *source,const char *dest, bool bOverwrite)
{// begin Copy
	// open the source file
	CStdFile sfSource;
	if(!sfSource.Open(source,ios::in|ios::open_mode::nocreate|ios::binary))
		return false;
	// create the source file
	CStdFile sfDest;
	unsigned long int nDestFlags = ios::out|ios::binary;
	if(!bOverwrite)
		nDestFlags |= ios::noreplace;
	if(!sfDest.Open(dest,nDestFlags))
		return false;
	// allocate copy buffer
	const int COPY_BLOCK = 1;
	char pBuffer[COPY_BLOCK+1] = {NULL};
	unsigned long int nBytesRead = 0;
	// begin the file copy
	while(sfSource.Read(pBuffer,COPY_BLOCK))
	{// begin copy bytes
		// write
		sfDest.Write(pBuffer,COPY_BLOCK);
	}// end copy bytes
	return true;
}// end Copy
*/
