/* 
 * File:   CFile.h
 * Author: elagin
 *
 * Created on 30 Март 2011 г., 13:12
 */

#ifndef CFILE_H
#define	CFILE_H

class CFile {
public:
    CFile(const string & fileName);
    CFile(const CFile& orig);
    void write(const string & data);
    void open();
    virtual ~CFile();
    
private:
    FILE* pFile;
    string _fileName;
};

class CPLTFile : public CFile
{
public:
    CPLTFile(const string & fileName);
    void createHeader();
};
/*	void Open(string outputFileName);
	void Close();
	void Write(const char *buf, const UINT bufSize);

	FILE * pFile;
	long lSize;
	size_t result;*/
#endif	/* CFILE_H */

