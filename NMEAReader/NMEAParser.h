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
	void Open(string outputFileName);
	void Close();
	void Write(const char *buf, const UINT bufSize);

	FILE * pFile;
	long lSize;
	size_t result;

};

class NMEAParser  
{
public:
	NMEAParser();
	NMEAParser(string outputFileName);
	virtual ~NMEAParser();
	void Parse(const char *buf, const UINT bufSize);
	GPSInfo& GetActualGPSInfo();
//	void ParseNMEASentence(const char *addressField, const char *buf, const UINT bufSize);
	bool ParseNMEASentence(const string addressField, const char *buf, const UINT bufSize);
	void PrintGpsInfo();

private:
	CFile m_outputFile;

	void ParseRecursive(const char ch);

	bool ProcessGPGGA(const char *buf, const UINT bufSize);
	bool ProcessGPGSA(const char *buf, const UINT bufSize);
	void ProcessGPGSV(const char *buf, const UINT bufSize);
	void ProcessGPRMB(const char *buf, const UINT bufSize);
	bool ProcessGPRMC(const char *buf, const UINT bufSize);
	void ProcessGPZDA(const char *buf, const UINT bufSize);


	bool m_logging;
	GPSInfo m_GPSInfo;
};

#endif // !defined(AFX_NMEAPARSER_H__26C0F55B_19A8_4E71_A1BA_A2EBA169FCEB__INCLUDED_)
