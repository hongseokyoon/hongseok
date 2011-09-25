#include "log.h"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace hongseok {

log	g_log;

log::log()
: _level(normal), _global_level(normal), _output(console), _console(std::cout)
{
}

log::~log()
{
	_flush();

	if (_file.is_open())	_file.close();
}

#define	IO_LOCK	boost::mutex::scoped_lock	__io_lock(_mutex)

#define	PUT_LOG(T)	\
log& log::operator<<(const T& val)\
{\
    IO_LOCK;\
    if (_level >= _global_level)    _oss    << val;\
    return *this;\
}

PUT_LOG(hs_bool8)
PUT_LOG(hs_int8)
PUT_LOG(hs_int16)
PUT_LOG(hs_int32)
PUT_LOG(hs_int64)
PUT_LOG(hs_uint8)
PUT_LOG(hs_uint16)
PUT_LOG(hs_uint32)
PUT_LOG(hs_uint64)
PUT_LOG(hs_float32)
PUT_LOG(hs_float64)
//PUT_LOG(hs_float96)
PUT_LOG(std::string)

log& log::operator<<(const hs_char8* val)
{
	IO_LOCK;
	if (_level >= _global_level)	_oss	<< val;
	return *this;
}
/*
log& log::operator<<(const std::string& val)
{
	IO_LOCK;
	if (_level >= _global_level)	_oss	<< val;
	return *this;
}
*/
log& log::operator<<(const _end_marker& em)
{
    IO_LOCK;
	if (em == log::endl)
	{
		if (_level >= _global_level)	_oss	<< "\n";
	    _flush();
	}
    else    // log::flush
    {
        _flush();
    }

    return *this;
}

bool log::set_output(const output& o, 
			const hs_char8* filename, 
			const hs_char8* host, 
			const hs_char8* port)
{
	IO_LOCK;
	_output	|= o;
	
	if (o & file && filename)
	{
		if (_file.is_open())	_file.close();
		_file.open(filename, std::ios_base::ate | std::ios_base::out);
		if (_file.bad())	return false;
	}

	if (o & network)
	{
	}

	return true;
}

void log::unset_output(const output& o)
{
	IO_LOCK;
	_output	&= ~o;

	if (o & console)
	{
	}

	if (o & file)
	{
		if (_file.is_open())	_file.close();
	}

	if (o & network)
	{
	}
}

log& log::_flush()
{
    //IO_LOCK;
	if (_oss.str().empty())	return *this;

	boost::posix_time::ptime	curr(boost::posix_time::second_clock::local_time());

	if (_output & console)
	{
		_console	<< curr << "] ";
		_console	<< _oss.str() << std::flush;
	}
	if (_output & file && _file.is_open())
	{
		_file	<< curr << "] ";
		_file	<< _oss.str() << std::flush;
	}
	if (_output & network)
	{
	}

	_oss.str("");

    return *this;
}

};// namespace hongseok
