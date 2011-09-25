#include "server.h"

#include <boost/bind.hpp>

namespace hongseok {

server::server(const hs_uint32& thread_pool_size)
: _thread_pool_size(thread_pool_size)
{
	SCOPE_LOGGER;
}

server::~server()
{
	SCOPE_LOGGER;

    for (std::vector<timer*>::iterator itr = _timers.begin(); itr != _timers.end(); ++itr)
    {
        delete  (*itr);
        (*itr)  = 0;
    }
    _timers.clear();
}

hs_void server::run()
{
	SCOPE_LOGGER;
	for (hs_uint32 i = 0; i < _thread_pool_size; ++i)
		_thread_group.create_thread(
			boost::bind(&boost::asio::io_service::run, 
						&_io_service));

	_thread_group.join_all();
}

hs_void server::stop()
{
	SCOPE_LOGGER;
	_io_service.stop();
}

};// namespace hongseok
