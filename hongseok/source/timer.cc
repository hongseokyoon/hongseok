#include "timer.h"

namespace hongseok {

timer::timer()
: _running(false), _timer(0)
{
}

timer::~timer()
{
    stop();
}

hs_void timer::start(boost::asio::io_service& io_service,
            const hs_uint32& interval_milliseconds,
            handler_t h)
{
    if (_running)   return;
        
    _timer  = new boost::asio::deadline_timer(io_service, 
                    boost::posix_time::milliseconds(interval_milliseconds));

    _running    = true;
    _timer->async_wait(boost::bind(&timer::_handler, this, interval_milliseconds, h));
}

hs_void timer::stop()
{
    if (!_running)  return;

    try
    {
        _running    = false;
        _timer->cancel();
    }
    catch (...)
    {
    }
}

hs_void timer::_handler(const hs_uint32& interval_milliseconds, handler_t h)
{
    if (!_running)
    {
        delete  _timer;
        _timer  = 0;

        return;
    }

    _timer->expires_from_now(boost::posix_time::milliseconds(interval_milliseconds));
    h();
    _timer->async_wait(boost::bind(&timer::_handler, this, interval_milliseconds, h));
}

};// namespace hongseok
