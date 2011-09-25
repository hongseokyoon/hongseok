#ifndef __SERVER_H__
#define __SERVER_H__

#include "acceptor.h"
#include "connection.h"
#include "types.h"
#include "util.h"
#include "timer.h"

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

using namespace hongseok::types;

namespace hongseok {

class server
{
public:
	server(const hs_uint32& thread_pool_size);
	~server();

	hs_void	run();	// no throw
	hs_void	stop();	// no throw

	template <typename T>
	hs_void	accept(const hs_uint16& port)	// throw boost::system::system_error
	{
		SCOPE_LOGGER;
		boost::shared_ptr<acceptor<T> >	a(new acceptor<T>(_io_service, port));
		a->accept();
	}

    template <typename T, typename F>
	hs_void	accept(const hs_uint16& port, F f)
	{
		SCOPE_LOGGER;
		boost::shared_ptr<acceptor<T> >	a(new acceptor<T>(_io_service, port));
        f(reinterpret_cast<T*>(a.get()));
		a->accept();
	}

    // end-user should not invoke connection::start() function directly.
    //   this is only for acceptor class.
/*
    template <typename T>
    hs_void connect()
    {
        SCOPE_LOGGER;
        boost::shared_ptr<connection>   c(new T());
        if (dynamic_cast<udp_connection*>(c.get()) == 0)
        {
            throw std::exception("udp_connection instance cannot invoke connect() function.");
        }
        c->init(_io_service);
        c->start();
    }
*/
	template <typename T>
	hs_void	connect(const hs_char8* host, const hs_char8* port)	// no throw
	{
		SCOPE_LOGGER;
        boost::shared_ptr<connection>   c(new T());
        c->init(_io_service);
		c->start(host, port);
	}

    template <typename T, typename F>
    hs_void connect(const hs_char8* host, const hs_char8* port, F f)
    {
        SCOPE_LOGGER;
        boost::shared_ptr<connection>   c(new T());
        c->init(_io_service);
        f(reinterpret_cast<T*>(c.get()));
        c->start(host, port);
    }

    hs_void set_timer(const hs_uint32& interval_milliseconds, timer::handler_t h)
    {
        timer*  t   = new timer();
        t->start(_io_service, interval_milliseconds, h);
        _timers.push_back(t);
    }

private:
	boost::asio::io_service	_io_service;
	hs_uint32				_thread_pool_size;
	boost::thread_group		_thread_group;
    std::vector<timer*>     _timers;
};// class server

};// namespace hongseok

#endif//__SERVER_H__
