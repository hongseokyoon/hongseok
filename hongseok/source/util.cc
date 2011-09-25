#include "util.h"

#include <sstream>
#include <boost/thread.hpp>

namespace hongseok {
namespace util {

performance::time_duration::time_duration(hongseok::types::hs_int64& duration)
: _duration(duration)
{
	_begin	= boost::posix_time::microsec_clock::local_time();
}

performance::time_duration::~time_duration()
{
	_duration	= boost::posix_time::time_duration(boost::posix_time::microsec_clock::local_time() - _begin).total_microseconds();
}

const hs_char8* info::thread_id()
{
	std::ostringstream	oss;
	oss	<< boost::this_thread::get_id();
	return oss.str().c_str();
}
/*
errno_t memory::copy(hs_void* dest, size_t dest_size, const hs_void* src, size_t count)
{
	if (dest == 0 || src == 0)	return EINVAL;
	if (dest_size < count)		return ERANGE;

	memcpy(dest, src, count);

	return 0;
}

hs_void* memory::zero(hs_void* dest, size_t length)
{
	return memset(dest, 0, length);
}
*/

hs_uint32	    scope_logger::_idn	= 0;
boost::mutex    scope_logger::_mutex;

scope_logger::scope_logger(const std::string& message)
: _message(message)
{
    _mutex.lock();
	std::cout	<< _indent(_idn) << "[+] " << _message << std::endl;
    _mutex.unlock();
	_t	= boost::posix_time::microsec_clock::local_time();

    _mutex.lock();
    ++_idn;
    _mutex.unlock();
}

scope_logger::~scope_logger()
{
    _mutex.lock();
    --_idn;
    _mutex.unlock();

	hs_int64	dur	= boost::posix_time::time_duration(boost::posix_time::microsec_clock::local_time() - _t).total_microseconds();
    _mutex.lock();
	std::cout	<< _indent(_idn) << "[-] " << _message << " {" << dur << "}" << std::endl;
    _mutex.unlock();
}

std::string scope_logger::_indent(hs_uint32 idn) const
{
	return std::string(idn << 1, ' ');
}

};// namespace util
};// namespace hongseok
