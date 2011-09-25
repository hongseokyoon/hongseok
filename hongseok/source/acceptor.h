#ifndef __ACCEPTOR_H__
#define	__ACCEPTOR_H__

#include "types.h"
#include "tcp_connection.h"
#include "udp_connection.h"
#include "util.h"

#include <iostream>

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using namespace hongseok::types;

namespace hongseok {

template <typename T, typename P = typename T::protocol>
class acceptor;

template <typename T>
class acceptor<T, connection::tcp> : 
		public boost::enable_shared_from_this<acceptor<T, connection::tcp> >
{
public:
	acceptor(boost::asio::io_service& io_service, const hs_uint16& port)
	: _io_service(io_service),
	_acceptor(io_service, 
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	{
		SCOPE_LOGGER;
		_acceptor.set_option(
			boost::asio::ip::tcp::acceptor::reuse_address(true));
	}

	void	accept()	// no throw
	{
		SCOPE_LOGGER;

        _acceptee.reset(new T());
        _acceptee->init(_io_service);

		_acceptor.async_accept(*(_acceptee->socket()),
			boost::bind(&acceptor<T>::_handler, 
				this->shared_from_this(), 
				boost::asio::placeholders::error));
	}

private:
	hs_void	_handler(const boost::system::error_code& e)	// no throw
	{
		SCOPE_LOGGER;
		if (e)
		{
			return;
		}

		_acceptee->start();

        _acceptee.reset(new T());
        _acceptee->init(_io_service);

		_acceptor.async_accept(*(_acceptee->socket()),
			boost::bind(&acceptor<T>::_handler, 
				this->shared_from_this(), 
				boost::asio::placeholders::error));
	}

private:
	boost::asio::ip::tcp::acceptor		_acceptor;
	boost::asio::io_service&			_io_service;
	boost::shared_ptr<tcp_connection>	_acceptee;
};// class acceptor

template <typename T>
class acceptor<T, connection::udp>
{
public:
	acceptor(boost::asio::io_service& io_service, const hs_uint16& port)
	{
		SCOPE_LOGGER;
        boost::shared_ptr<T>    a(new T());
        a->init(io_service);
		a->socket()->open(boost::asio::ip::udp::v4());
		a->socket()->bind(boost::asio::ip::udp::endpoint(
							boost::asio::ip::udp::v4(), port));
		a->start();
	}

	void	accept()
	{
		SCOPE_LOGGER;
	}
};// class udp_acceptor

};// namespace hongseok

#endif//__ACCEPTOR_H__
