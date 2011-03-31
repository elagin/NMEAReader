// GPSInfo.h: interface for the GPSInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPSINFO_H__2040A232_786E_4C7B_8EA5_0A9438ED6C1F__INCLUDED_)
#define AFX_GPSINFO_H__2040A232_786E_4C7B_8EA5_0A9438ED6C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class gpsTime
{
public:
    void setTime(string aHour, string aMin, string aSec);
    void setHour(string aHour);
    void setMin(string aMin);
    void setSec(string aSec);
    string getTime();

private:
    string hour;
    string min;
    string sec;
};

struct GPSInfo  
{
public:
	string altitude;
	ULONG nSentences;
	UINT signalQuality;
	string satelitesInUse;
	string groundSpeed;
	string courseOverGround;
	int quality;
	string heightGeoid;
	string hdop;
	string magneticVariation;
	gpsTime time;
	gpsTime	date;

	GPSInfo();
	virtual ~GPSInfo();
        void setLatitude(double data);
        void setLongitude(double data);
        
        string getLatitude();
        string getLongitude();
        string getQuality();
                
private:
	string latitude;
	string longitude;
    
};

#endif // !defined(AFX_GPSINFO_H__2040A232_786E_4C7B_8EA5_0A9438ED6C1F__INCLUDED_)
