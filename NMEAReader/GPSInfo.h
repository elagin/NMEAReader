// GPSInfo.h: interface for the GPSInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPSINFO_H__2040A232_786E_4C7B_8EA5_0A9438ED6C1F__INCLUDED_)
#define AFX_GPSINFO_H__2040A232_786E_4C7B_8EA5_0A9438ED6C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ULONG unsigned long 
#define UINT unsigned int 
#define DOUBLE double 

struct GPSInfo  
{
public:
	DOUBLE m_latitude;
	DOUBLE m_longitude;
    DOUBLE m_altitude;
	ULONG m_nSentences;
	UINT m_signalQuality;
	UINT m_satelitesInUse;

	GPSInfo();
	virtual ~GPSInfo();
};

#endif // !defined(AFX_GPSINFO_H__2040A232_786E_4C7B_8EA5_0A9438ED6C1F__INCLUDED_)
