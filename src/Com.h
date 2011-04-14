#pragma once

#include <boost/thread/thread.hpp>
#include <Windows.h>
#include <vector>

#include "GPSInfo.h"

class Com
{
public:
	Com();
	~Com();
	
	bool openPort();
	void closePort();
	void go();
	void stop();
	string getData();
	string getLine();

private:
	void do_work();
	string readData();

	volatile bool _stoprequested;
	boost::shared_ptr<boost::thread> _thread;
	boost::mutex _mutex;

	string	_result;
	string	_line;
	HANDLE	_portHandle;  // Handle COM port
	string	_buffer;
	size_t	_bufferPos;
	int		_bStatus;
	int		_startPos;
	int		_endPos;
};
