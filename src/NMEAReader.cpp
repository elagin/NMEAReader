// NMEAReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NMEAParser.h"
//#include "windows.h"
#include <iostream>
#include <fstream>



//LPCTSTR = const char* or const wchar_t* depending on _UNICODE
int main(int argc, char *argv[])
//int _tmain(int argc, _TCHAR* argv[])
{
	NMEAParser parse("Log 2011-03-26 181859.txt");
/*
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen( "d:\\Projects\\GPS\\Log 2011-03-26 181859.nmea", "r" );
	if(pFile == NULL)
	{
		fputs ("File not found", stderr); exit (3);
	}
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL)
	{
		fputs ("Memory error",stderr); exit (2);
	}
	memset( buffer, 0, (sizeof(char)*lSize));
	result = fread (buffer,1,lSize,pFile);
*/
//	if (result != lSize)
	{
//		fputs ("Reading error",stderr); exit (3);
	}

	ifstream infile;
	string line;

	char name[256];

//	infile.open ("d:\\Projects\\GPS\\NMEAReader\\Debug\\test.txt", ifstream::in);
	infile.open ("gps_log_clean.txt", ifstream::in);

	while (infile.good())
	{
		infile.getline( name, 256 );
//		cout << ":" << name << endl;
		char buf2[1024];
//		const UINT bufSize;

		if(parse.ParseNMEASentence( name, name, 1024 ))
		{
			cout << "res: " << name << endl;
			parse.PrintGpsInfo();
		}
	}

	infile.close();

//	parse.Parse( buffer, result );

//	fclose (pFile);
//	free (buffer);

	return 0;
}

