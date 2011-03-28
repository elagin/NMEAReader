// NMEAParser.h: interface for the NMEAParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMEAPARSER_H__26C0F55B_19A8_4E71_A1BA_A2EBA169FCEB__INCLUDED_)
#define AFX_NMEAPARSER_H__26C0F55B_19A8_4E71_A1BA_A2EBA169FCEB__INCLUDED_

#include "stdafx.h"
#include "GPSInfo.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFile
{
public:
	void Open(LPCSTR outputFileName);
	void Close();
	void Write(const CHAR *buf, const UINT bufSize);

	FILE * pFile;
	long lSize;
	size_t result;

};

class NMEAParser  
{
public:
	NMEAParser();
	NMEAParser(LPCSTR outputFileName);
	virtual ~NMEAParser();
	void Parse(const CHAR *buf, const UINT bufSize);
	GPSInfo& GetActualGPSInfo();
//	void ParseNMEASentence(const CHAR *addressField, const CHAR *buf, const UINT bufSize);
	bool ParseNMEASentence(const string addressField, const CHAR *buf, const UINT bufSize);
	void PrintGpsInfo();

private:
	CFile m_outputFile;

	void ParseRecursive(const CHAR ch);

	bool ProcessGPGGA(const CHAR *buf, const UINT bufSize);
	bool ProcessGPGSA(const CHAR *buf, const UINT bufSize);
	void ProcessGPGSV(const CHAR *buf, const UINT bufSize);
	void ProcessGPRMB(const CHAR *buf, const UINT bufSize);
	bool ProcessGPRMC(const CHAR *buf, const UINT bufSize);
	void ProcessGPZDA(const CHAR *buf, const UINT bufSize);


	BOOL m_logging;
	GPSInfo m_GPSInfo;
};

#endif // !defined(AFX_NMEAPARSER_H__26C0F55B_19A8_4E71_A1BA_A2EBA169FCEB__INCLUDED_)
