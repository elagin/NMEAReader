// NMEAReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "NMEAParser.h"
#include "CFile.h"
#include "Com.h"
#include <iostream>
#include <fstream>


//LPCTSTR = const char* or const wchar_t* depending on _UNICODE

/*
void thread_proc();

void thread_proc()
{
	cout << "thread_proc" << endl;
}

int fromFile()
{
	return 1;
}
*/
int main(int argc, char *argv[])
//int _tmain(int argc, _TCHAR* argv[])
{
    const char* inFileName;
	NMEAParser parse;
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
//	char name[256];

	switch ( argc )
    {
        case 0:
        case 1:
            cout << "Usage: filename" << endl;
            return 1;
        break;
        case 2:
            inFileName = argv[1];
        break;
    }
/*
	infile.open (inFileName, ifstream::in);
    if(!infile.good())
    {
        cout << "Error opening file: " << inFileName << endl;
        return 3;
    }
*/
	ofstream outfile ("out.txt",ofstream::out);
    string outFileName = "out.plt";
    CPLTFile plt(outFileName);
    plt.createHeader();
	{
		Com com;
		com.go();
		com.openPort();
		int i = 0;
		while( i < 10)
		{
			string res;
			res = com.getData();
//			cout << res << endl;
			if( res.length() > 0)
			{
				if(parse.ParseNMEASentence( res, res.c_str(), 512 ))
				{
					cout << parse.PrintStatus();
				}
				i++;
			}
		};
		com.stop();
		com.closePort();
	}
/*
	while (infile.good())
	{
		infile.getline( name, 256 );
		if(parse.ParseNMEASentence( name, name, 1024 ))
		{
            string line = parse.PrintByPlt();
            if(!line.empty())
            {
                plt.write(parse.PrintByPlt());
                cout << parse.PrintStatus();
            }
		}
	}
*/
//    boost::thread thread(&thread_proc);
//	thread.create_thread(&thread_proc);
//	thread.join();


//	string comRes = com.work();
//	cout << comRes;
//	outfile << comRes;

	infile.close();
	outfile.close();

	cout << "Press Enter";
	char enter[256];
	  cin.getline (enter,256);

	return 0;
}

