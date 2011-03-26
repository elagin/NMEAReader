// NMEAParser.h: interface for the NMEAParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMEAPARSER_H__26C0F55B_19A8_4E71_A1BA_A2EBA169FCEB__INCLUDED_)
#define AFX_NMEAPARSER_H__26C0F55B_19A8_4E71_A1BA_A2EBA169FCEB__INCLUDED_

#include "GPSInfo.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class NMEAParser  
{
public:
	NMEAParser();
	NMEAParser(LPCTSTR outputFileName);
	virtual ~NMEAParser();
	void Parse(const CHAR *buf, const UINT bufSize);
	GPSInfo& GetActualGPSInfo();

private:
	CFile m_outputFile;

	void ParseRecursive(const CHAR ch);
	void ParseNMEASentence(const CHAR *addressField, 
		                   const CHAR *buf, const UINT bufSize);
	void ProcessGPGGA(const CHAR *buf, const UINT bufSize);
	void ProcessGPGSA(const CHAR *buf, const UINT bufSize);
	void ProcessGPGSV(const CHAR *buf, const UINT bufSize);
	void ProcessGPRMB(const CHAR *buf, const UINT bufSize);
	void ProcessGPRMC(const CHAR *buf, const UINT bufSize);
	void ProcessGPZDA(const CHAR *buf, const UINT bufSize);

	BOOL m_logging;
	GPSInfo m_GPSInfo;
};

#endif // !defined(AFX_NMEAPARSER_H__26C0F55B_19A8_4E71_A1BA_A2EBA169FCEB__INCLUDED_)
