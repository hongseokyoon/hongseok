#ifndef __TIMER_H__
#define __TIMER_H__

#include "types.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace hongseok::types;

namespace hongseok {

class timer
{
public:
    typedef hs_void(*handler_t)();

    timer();
    ~timer();

    hs_void start(boost::asio::io_service& io_service,
                  const hs_uint32& interval_milliseconds,
                  handler_t h);

    hs_void stop();

private:
    hs_void _handler(const hs_uint32& interval_milliseconds, handler_t h);

private:
    hs_bool8                        _running;
    boost::asio::deadline_timer*    _timer;
};// class timer

};// namespace hongseok

#endif // __TIMER_H__
