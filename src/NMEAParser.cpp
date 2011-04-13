// NMEAParser.cpp: implementation of the NMEAParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMEAParser.h"
#include "CFile.h"

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


NMEAParser::NMEAParser()
{
    _pPltFile = NULL;
}

NMEAParser::NMEAParser(string outputFileName): _pPltFile(NULL)
{
//  m_outputFile.Open(outputFileName/*, CFile::modeCreate | CFile::modeWrite */);
    outputFileName = "out2.plt";
/* 
	_pPltFile = new CPLTFile(outputFileName);
    if(_pPltFile!=NULL)
    {
        _pPltFile->createHeader();
    }
*/
    //    CPLTFile plt(outputFileName);
//    plt.createHeader();
}

NMEAParser::~NMEAParser()
{
    if(_pPltFile!=NULL)
    {
        delete _pPltFile;
    }
//	if(m_logging)
//		m_outputFile.Close();
}

int axtoi( const char *hexStg )
{
  int n = 0;         // position in string
  int m = 0;         // position in digit[] to shift
  int count;         // loop index
  int intValue = 0;  // integer value of hex string
  int digit[5];      // hold values to convert
  while (n < 4)
  {
     if (hexStg[n]=='\0')
        break;
     if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) //if 0 to 9
        digit[n] = hexStg[n] & 0x0f;            //convert to int
     else if (hexStg[n] >='a' && hexStg[n] <= 'f') //if a to f
        digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
     else if (hexStg[n] >='A' && hexStg[n] <= 'F') //if A to F
        digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
     else break;
    n++;
  }
  count = n;
  m = n - 1;
  n = 0;
  while(n < count)
  {
     intValue = intValue | (digit[n] << (m << 2));
     m--;   // adjust the position to set
     n++;   // next digit to process
  }
  return (intValue);
}

void NMEAParser::Parse(const char *buf, const UINT bufSize)
{
/*	m_outputFile.Write(buf, bufSize);
	for( UINT i = 0; i < bufSize; i++ )
	{
		ParseRecursive(buf[i]);
	}*/
}

void NMEAParser::ParseRecursive(const char ch)
{	
  enum NMEAParserState { SearchForSOS = 1,
                         RetrieveAddressField,
                         ReceiveSentenceData,
                         GetFirstChecksumCharacter,
                         GetSecondChecksumCharacter,
                         WaitForST,
                         ValidSentence };

  static const UINT ADDRESS_FIELD_MAX_LENGTH = 10;
  static const UINT NMEA_SEQUENCE_MAX_LENGTH = 81;

  static NMEAParserState m_State = SearchForSOS;
  static UINT m_CalcChecksum;
  static char m_Checksum[3];
  static char m_NMEASequence[NMEA_SEQUENCE_MAX_LENGTH];
  static UINT m_NMEASequenceIndex;
  static char m_AddressField[ADDRESS_FIELD_MAX_LENGTH];
  static UINT m_AddressFieldIndex;

  switch( m_State )
  {
    case SearchForSOS:
    {
	  if( ch == '$' )
	  {
	    m_AddressFieldIndex = 0;
	    m_NMEASequenceIndex = 0;
	    m_CalcChecksum = 0;
	    m_State = RetrieveAddressField;
      }
	  break;
	}

    case RetrieveAddressField:
    {
      if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 )
        m_State = SearchForSOS;
      else
      {
        m_NMEASequence[m_NMEASequenceIndex++] = ch;
        m_CalcChecksum ^= ch;
        if( ch == ',' )
 	    {
		  m_AddressField[m_AddressFieldIndex] = '\0';
		  m_State = ReceiveSentenceData;
	    }
	    else if( m_AddressFieldIndex == ADDRESS_FIELD_MAX_LENGTH - 1 ||
	             !isalpha(ch) || islower(ch) )
	      m_State = SearchForSOS;
	    else
		  m_AddressField[m_AddressFieldIndex++] = ch;
	  }
	  break;
	}

    case ReceiveSentenceData:
    {
      if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 )
        m_State = SearchForSOS;
      else
      {
        m_NMEASequence[m_NMEASequenceIndex++] = ch;
  	    if( ch == '*' )
 	      m_State = GetFirstChecksumCharacter;
 	    else if( ch == 10 )
 	      m_State = WaitForST;
 	    else if( ch == 13 )
        {
		  m_NMEASequence[m_NMEASequenceIndex++] = ch;
          m_NMEASequence[m_NMEASequenceIndex] = '\0';
          ParseNMEASentence( m_AddressField, m_NMEASequence, m_NMEASequenceIndex );
  	      m_State = SearchForSOS;
        }
		else
 	      m_CalcChecksum ^= ch;
	  }
	  break;
    }

    case GetFirstChecksumCharacter:
    {
      if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 ||
          ( !isdigit(ch) && ( ch < 'A' || ch > 'F' ) ) )
        m_State = SearchForSOS;
      else
      {
        m_NMEASequence[m_NMEASequenceIndex++] = ch;
		m_Checksum[0] = ch;
		m_State = GetSecondChecksumCharacter;
	  }
	  break;
	}

    case GetSecondChecksumCharacter:
    {
      if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 ||
          ( !isdigit(ch) && ( ch < 'A' || ch > 'F' ) ) )
        m_State = SearchForSOS;
      else
      {
        m_NMEASequence[m_NMEASequenceIndex++] = ch;
		m_Checksum[1] = ch;
		m_Checksum[2] = '\0';
        UINT iChecksum = axtoi( m_Checksum );
        if( iChecksum == m_CalcChecksum )
          m_State = WaitForST;
        else
          m_State = SearchForSOS;
	  }
	  break;
	}

    case WaitForST:
    {
      if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 ||
          (ch != 10 && ch != 13) )
        m_State = SearchForSOS;
      else if(ch == 13)
      {
        m_NMEASequence[m_NMEASequenceIndex++] = ch;
        m_NMEASequence[m_NMEASequenceIndex] = '\0';
        ParseNMEASentence( m_AddressField, m_NMEASequence, m_NMEASequenceIndex );
  	    m_State = SearchForSOS;
      }
	  break;
	}
  }

}

//void NMEAParser::ParseNMEASentence(const char *addressField, const char *buf, const UINT bufSize)
bool NMEAParser::ParseNMEASentence(const string addressField, const char *buf, const UINT bufSize)
{
    bool res = false;
	if(addressField.find( "GPGGA" ) != -1 )
	{
		res = ProcessGPGGA(++buf, bufSize);
	}
/*	else if(addressField.find( "GPGSA" ) != -1 )
	{
		res = ProcessGPGSA(++buf, bufSize);
	}
	else if( strcmp(addressField, "$GPGSV") == NULL )
	{
		ProcessGPGSV(buf, bufSize);
	}
	else if( strcmp(addressField, "$GPRMB") == NULL )
	{
		ProcessGPRMB(buf, bufSize);
	}
 */
        else if(addressField.find( "GPRMC" ) != -1 )
	{
		res = ProcessGPRMC(++buf, bufSize);
	}
/*
	else if( strcmp(addressField, "$GPZDA") == NULL )
	{
		ProcessGPZDA(buf, bufSize);
	}*/
    PrintByPlt();
    return res;
}

GPSInfo& NMEAParser::GetActualGPSInfo()
{
	return m_GPSInfo;
}


/*
GPGGA Sentence format

$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M, ,*47
   |   |	  |			 |			 | |  |	  |		  |      | | 
   |   |	  |			 |			 | |  |	  |		  |		 | checksum data
   |   |	  |			 |			 | |  |	  |		  |		 |
   |   |	  |			 |			 | |  |	  |		  |		 empty field
   |   |	  |			 |			 | |  |	  |		  |
   |   |	  |			 |			 | |  |	  |		  46.9,M Height of geoid (m) above WGS84 ellipsoid
   |   |	  |			 |			 | |  |	  |
   |   |	  |			 |			 | |  |	  545.4,M Altitude (m) above mean sea level
   |   |	  |			 |			 | |  |
   |   |	  |			 |			 | |  0.9 Horizontal dilution of position (HDOP)
   |   |	  |			 |			 | |
   |   |	  |			 |			 | 08 Number of satellites being tracked
   |   |	  |			 |			 |
   |   |	  |			 |			 1 Fix quality:	0 = invalid
   |   |	  |			 |							1 = GPS fix (SPS)
   |   |	  |			 |							2 = DGPS fix
   |   |	  |			 |							3 = PPS fix
   |   |	  |			 |							4 = Real Time Kinematic
   |   |	  |			 |							5 = Float RTK
   |   |	  |			 |							6 = estimated (dead reckoning) (2.3 feature)
   |   |	  |			 |							7 = Manual input mode
   |   |	  |			 |							8 = Simulation mode
   |   |	  |			 |
   |   |	  |			 01131.000,E Longitude 11 deg 31.000' E
   |   |	  |
   |   |	  4807.038,N Latitude 48 deg 07.038' N	
   |   |
   |   123519 Fix taken at 12:35:19 UTC
   |
   GGA Global Positioning System Fix Data

*/
bool NMEAParser::ProcessGPGGA(const char *buf, const UINT bufSize)
{
	// To disable handling this sentence uncomment the next line
	// return false;

	//	char auxBuf[10];
	string aux;
	const char *p1 = buf;
	const char *p2 = NULL;

	// GGA
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if(bufSize < 6)
		return false;
	//	strncpy(auxBuf, buf, 5);
	aux.assign(buf, 5);
	if(aux.compare("GPGGA"))
	{
		return false;
	}
	p1 += 6;
	int size = 0;
	getTime(p1, buf, bufSize, m_GPSInfo.time, size);
	p1 += size;

	// Latitude
	int sizeLat = 0;
	double latitude = 0;
	getLat(p1, buf, bufSize, latitude, sizeLat, true);
	m_GPSInfo.setLatitude(latitude);

	p1 += sizeLat;

	sizeLat = 0;
	double longitude = 0;
	getLat(p1, buf, bufSize, longitude, sizeLat, false);
	p1 += sizeLat;

        m_GPSInfo.setLongitude(longitude);

	// GPS quality
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
        
        string quality;
        quality.assign(p1, p2 - p1);
        m_GPSInfo.quality = lexical_cast<int>(quality);
//      	m_GPSInfo.quality.assign(p1, p2 - p1);
	p1 = p2 + 1;

	// Satellites in use
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	m_GPSInfo.satelitesInUse.assign(p1, p2 - p1);
	p1 = p2 + 1;

	// HDOP
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	m_GPSInfo.hdop.assign(p1, p2 - p1);
	p1 = p2 + 1;

	// Altitude
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	m_GPSInfo.altitude.assign(p1, p2 - p1);
	p1 = p2 + 1;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	if(p2 - p1 != 1)
		return false;
	if(*p1 != 'M')
		return false;
	p1 = p2 + 1;

	// Height of geoid
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	m_GPSInfo.heightGeoid.assign(p1, p2 - p1);
	p1 = p2 + 1;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	//	if(p2 - p1 != 1)
	//		return false;
	//	if(*p1 != 'M')
	//		return false;
	p1 = p2 + 1;

	// Empty field
/*
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	p1 = p2 + 1;

	// Last Empty field

	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) != NULL)
		return false;
	if((p2 = strchr(p1, '*')) == NULL)
		return false;
*/
	return true;
}

void NMEAParser::ProcessGPGSA(const char *buf, const UINT bufSize)
{

}

void NMEAParser::ProcessGPGSV(const char *buf, const UINT bufSize)
{

}

void NMEAParser::ProcessGPRMB(const char *buf, const UINT bufSize)
{

}

/*
Format

  $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
     |	 |		| |			 |			 |	   |	 |		|	   |
     |	 |		| |			 |			 |	   |	 |		|	   *6A Checksum data
     |	 |		| |			 |			 |	   |	 |		|
     |	 |		| |			 |			 |	   |	 |		003.1,W Magnetic Variation
     |	 |		| |			 |			 |	   |	 |
     |	 |		| |			 |			 |	   |	 230394 Date - 23rd of March 1994
     |	 |		| |			 |			 |	   |
     |	 |		| |			 |			 |	   084.4 Track angle in degrees
     |	 |		| |			 |			 |
     |	 |		| |			 |			 022.4 Speed over the ground in knots
     |	 |		| |			 |
     |	 |		| |			 01131.000,E Longitude 11 deg 31.000' E
     |	 |		| |
     |	 |		| 4807.038,N Latitude 48 deg 07.038' N
     |	 |		|
     |	 |		A Status A=active or V=Void
     |	 |
     |	 123519 Fix taken at 12:35:19 UTC
     |
     RMC Recommended Minimum sentence C

*/
bool NMEAParser::ProcessGPRMC( const char *buf, const UINT bufSize )
{
//	cout << "ProcessGPRMC" << endl;
//	char auxBuf[10];
	const char *p1 = buf;
	const char *p2 = NULL;
	string aux;

	// GGA
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if(bufSize < 6)
		return false;
	aux.assign(buf, 5);
	if(aux.compare("GPRMC"))
		return false;
	p1 += 6;

	// Time
	int size = 0;
	getTime(p1, buf, bufSize, m_GPSInfo.time, size );
	p1 =  p1 + size;

	// Status 
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	if(p2 == p1)
		return false;
//	if(*p1 != 'A')
//		return false;
	p1 = p2 + 1;

    // Latitude
	int sizeLat = 0;
	double latitude = 0;
	getLat(p1, buf, bufSize, latitude, sizeLat, true);

	p1 += sizeLat;

    // Longitude
	double longitude = 0;
	getLat(p1, buf, bufSize, longitude, sizeLat, false);
	m_GPSInfo.setLongitude(longitude);
	p1 =  p1 + sizeLat;

	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	m_GPSInfo.groundSpeed.assign(p1, p2 - p1);
//	strncpy(auxBuf, p1, p2 - p1);
//	auxBuf[p2 - p1] = '\0';
	p1 = p2 + 1;
//	 = atof(aux.c_str());

	// Course over ground (degrees) 
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
        string cource;
//        cource.assign(p1, p2 - p1);
	m_GPSInfo.setCource(cource.assign(p1, p2 - p1));
//	strncpy(auxBuf, p1, p2 - p1);
//	auxBuf[p2 - p1] = '\0';
	p1 = p2 + 1;

	// Date
	getTime(p1, buf, bufSize, m_GPSInfo.date, size);
/*
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	UINT day, month, year;
	strncpy(auxBuf, p1, 2);
	auxBuf[2] = '\0';
	day = atoi(auxBuf);
	p1 += 2;
	strncpy(auxBuf, p1, 2);
	auxBuf[2] = '\0';
	month = atoi(auxBuf);
	p1 += 2;
	strncpy(auxBuf, p1, 2);
	auxBuf[2] = '\0';
	year = 2000 + atoi(auxBuf);
	p1 = p2 + 1;
*/
	p1 = p1 + size;
	// Magnetic variation
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	m_GPSInfo.magneticVariation.assign(p1, p2 - p1);
//	strncpy(auxBuf, p1, p2 - p1);
//	auxBuf[p2 - p1] = '\0';
	p1 = p2 + 1;
//	double magneticVariation = atof(auxBuf);
	if((p2 = strchr(p1, '*')) == NULL)
		return false;
	if(p2 - p1 > 1)
		return false;
	if(*p1 == 'W')
	{
		latitude = -latitude;
		m_GPSInfo.setLatitude(latitude);
	}
//		lat = -lat;
	else if(*p1 != 'E' && *p1 != '*')
		return false;
	return true;
}

void NMEAParser::ProcessGPZDA(const char *buf, const UINT bufSize){}


bool NMEAParser::getTime(const char *p1, const char *buf, const UINT bufSize, gpsTime & time, int & size)
{
    bool res = false;
    string aux;
	const char *p2 = NULL;
	const char* pStart = p1;

    if((UINT)(p1 - buf) >= bufSize)
        res = false;
    if((p2 = strchr(p1, ',')) == NULL)
        res = false;

    aux.assign(p1, 2);
    time.setHour(aux);

    p1 += 2;
    aux.assign(p1, 2);
    time.setMin(aux);

    p1 += 2;
    aux.assign(p1, 2);
    time.setSec(aux);

    if((p2 = strchr(p1, ',')) == NULL)
        res = false;

    size = p2-pStart+1;
    res = true;

    return res;
}

bool NMEAParser::getLat(const char *p1, const char *buf, const UINT bufSize, double & latNew, int & size, const bool isLat)
{
        bool res = false;
        string aux;
    
	const char *p2 = NULL;
	const char* pStart = p1;
	if((UINT)(p1 - buf) >= bufSize)
		return false;
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	aux.assign(p1, p2 - p1);
	p1 = p2 + 1;
	int pos = aux.find('.');
	if(pos == -1)
		return false;
	latNew = atof(aux.substr(pos - 2, aux.length() - 2).c_str()) / 60;
	latNew += atof(aux.substr(pos - 4, 2).c_str());
	if((p2 = strchr(p1, ',')) == NULL)
		return false;
	if(p2 - p1 != 1)
		return false;
	if(isLat)
	{
		if(*p1 == 'S')
			latNew = -latNew;
		else if(*p1 != 'N')
			return false;
		size = 12;
	}
	else
	{
		if(*p1 == 'W')
			latNew = -latNew;
		else if(*p1 != 'E')
			return false;
		size = 13;
	}
	return true;
}

bool NMEAParser::PrintByPlt()
{
    string out;
    if(_pPltFile!=NULL)
    {
        if(!m_GPSInfo.getLatitude().empty()
            && !m_GPSInfo.getLongitude().empty())
        {
            out.append(m_GPSInfo.getLatitude() + ",");
            out.append(m_GPSInfo.getLongitude() + ",");
            out.append("0,");
            out.append(m_GPSInfo.altitude + ",");
            out.append(",");
            out.append("28-may-10,");
            out.append(m_GPSInfo.time.getTime());
            _pPltFile->write(out);
        }

    }
    return true;
}

void NMEAParser::PrintGpsInfo()
{
    cout << "Lat: " << m_GPSInfo.getLatitude()
            << "\tLon: " << m_GPSInfo.getLongitude()
            << "\tSats In Use: " << m_GPSInfo.satelitesInUse
            << "\tSpeed: " << m_GPSInfo.groundSpeed
            << "\tTime: " << m_GPSInfo.time.getTime()
            << "\tAltitude: " << m_GPSInfo.altitude
            << endl;
}

string NMEAParser::PrintStatus()
{
    string out;

    if(m_GPSInfo.quality != 0)
    {
 	out = "Lat: " + m_GPSInfo.getLatitude()
            + " | Lon: " + m_GPSInfo.getLongitude()
            + " | Speed: " + m_GPSInfo.groundSpeed
            + " | Course: " + m_GPSInfo.getCourse()
            + " | Altitude: " + m_GPSInfo.altitude;
    }
    else
    {
        out = "No data";
    }
    return out;
}

bool NMEAParser::getQuality()
{
    string out;
    return m_GPSInfo.getQuality(out);
}