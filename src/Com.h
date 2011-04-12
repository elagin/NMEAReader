#pragma once

#include <boost/thread/thread.hpp>
#include <Windows.h>
#include <vector>

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

private:
	void do_work();
	string readData();

	volatile bool _stoprequested;
	boost::shared_ptr<boost::thread> _thread;
	boost::mutex _mutex;

	string	_result;
	HANDLE	_portHandle;  // Handle COM port
	char	_buffer[500];
	int		_bStatus;
};
