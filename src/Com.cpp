#include "stdafx.h"
#include "Com.h"

using boost::lexical_cast;
using boost::bad_lexical_cast;

Com::Com(): _stoprequested(false),
			_portHandle(NULL)
{
}

Com::~Com()
{
	if(_portHandle != NULL)
	{
		CloseHandle(_portHandle);
	}
	cout << "~Com()" << endl;
}

bool Com::openPort()
{
	bool res;
	DWORD        bytes_written = 0;    // Number of bytes written to the port
	DCB          comSettings;          // Contains various port settings
	COMMTIMEOUTS CommTimeouts;
//	strcpy(&OUTBUFFER[0], "The quick brown fox jumped over the lazy dog. \n\r\0");
	// Open COM port
	if ((_portHandle = 
//		CreateFile(L"\\\\.\\COM4",                // open com5:
		CreateFile(L"\\\\.\\COM10",                // open com5:
		GENERIC_READ | GENERIC_WRITE, // for reading and writing
		0,                            // exclusive access
		NULL,                         // no security attributes
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		// error processing code goes here
	}
	// Set timeouts in milliseconds
	CommTimeouts.ReadIntervalTimeout         = 0; 
	CommTimeouts.ReadTotalTimeoutMultiplier  = 0; 
	CommTimeouts.ReadTotalTimeoutConstant    = 100;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant   = 100;
	_bStatus = SetCommTimeouts(_portHandle,&CommTimeouts);
	if (_bStatus != 0)
	{
		// error processing code goes here
	}
	// Set Port parameters. Make a call to GetCommState() first in order to fill
	// the comSettings structure with all the necessary values. Then change the ones you want and call SetCommState().
	GetCommState(_portHandle, &comSettings);
	comSettings.BaudRate = 9600;
	comSettings.StopBits = ONESTOPBIT;
	comSettings.ByteSize = 8;
	comSettings.Parity   = NOPARITY;
	comSettings.fParity  = FALSE;
	_bStatus = SetCommState(_portHandle, &comSettings);

	int i = 0;

	if (_bStatus == 0)
	{
		// error processing code goes here
	}
//	while( i < 10 )
	{
/*
		_bStatus = ReadFile(_portHandle,   // Handle
			&_buffer,            // Incoming data
			500,                  // Number of bytes to read
			&bytes_read,          // Number of bytes read
			NULL);
		if (_bStatus != 0)
		{
			// error processing code goes here
		}
*/
//		cout << "bytes_read: " << bytes_read << endl;
//		cout << "data: " << _buffer << endl;
//		cout << "================================" << endl;
		// code to do something with the data goes here
//		i++;
//		res += _buffer;
	}
	res = true;
	return res;
}


void Com::go()
{
	assert(!_thread);
	_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Com::do_work, this)));
}

std::string Com::getData()
{
	boost::mutex::scoped_lock l(_mutex); //Note 2
	return _result;
}

std::string Com::readData()
{
	std::string res;
	DWORD bytes_read = 0;    // Number of bytes read from port

	_bStatus = ReadFile(_portHandle, &_buffer, 500, &bytes_read, NULL);
	if (_bStatus != 0)
	{
		// error processing code goes here
	}
	res.assign(_buffer, bytes_read);
	return res;
}

void Com::do_work()
{
	while (!_stoprequested)
	{
		std::string value = readData();
		boost::mutex::scoped_lock l(_mutex);
		::Sleep(1000);
		_result.assign(value);
	}

	cout << "end" << endl;
}

void Com::stop()
{
	assert(_thread);
	_stoprequested = true;
	_thread->join();
}

void Com::closePort()
{
	CloseHandle(_portHandle);
	_portHandle = NULL;
}