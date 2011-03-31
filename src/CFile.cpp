/* 
 * File:   CFile.cpp
 * Author: elagin
 * 
 * Created on 30 Март 2011 г., 13:12
 */
#include "stdafx.h"
#include "CFile.h"

CFile::CFile(const string & fileName)
{
   _fileName = fileName;
}

CFile::~CFile()
{
    fclose(pFile);
}

void CFile::open()
{
    pFile = fopen( _fileName.c_str(), "w" );
}

void CFile::write(const string & data)
{
    string end = "\r\n";
    if(pFile != NULL)
    {
        fwrite (data.c_str(), 1 , data.length(), pFile );
        fwrite (end.c_str(), 1 , end.length(), pFile );
    }
}

CPLTFile::CPLTFile(const string & fileName):CFile(fileName)
{
    open();
}

void CPLTFile::createHeader()
{
    string header = "OziExplorer Track Point File Version 2.1\r\nWGS 84\r\nAltitude is in Feet\r\nReserved 3\r\n0,8,8800273,28.03.2010,0,0,2,8421376\r\n1793\r\n";
    write(header);
}