// GPSInfo.cpp: implementation of the GPSInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GPSInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using boost::lexical_cast;
using boost::bad_lexical_cast;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GPSInfo::GPSInfo(){}
GPSInfo::~GPSInfo(){};
void GPSInfo::setLatitude(double data)
{
    latitude=lexical_cast<string>(data);
    latitude.resize(10);
}

void GPSInfo::setLongitude(double data)
{
    longitude=lexical_cast<string>(data);
    longitude.resize(10);
}

string GPSInfo::getLatitude(){return latitude;}
string GPSInfo::getLongitude(){return longitude;}
string GPSInfo::getQuality()
{
    if(quality == 0)
    {
        return "Invalid";
    }
    else
    {
        return "GPS fix";
    }
}


void gpsTime::setTime(string aHour, string aMin, string aSec)
{
    hour = aHour;
    min = aMin;
    sec = aSec;
}

string gpsTime::getTime()
{
    string res;
    res.append(hour);
    res.append(":");
    res.append(min);
    res.append(":");
    res.append(sec);
    return res;
}

void gpsTime::setHour(string aHour){hour=aHour;}
void gpsTime::setMin(string aMin){min=aMin;}
void gpsTime::setSec(string aSec){sec=aSec;}
