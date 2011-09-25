#include "udp_connection.h"

#include "log.h"
#include "util.h"

#include <boost/bind.hpp>

namespace hongseok {

udp_connection::udp_connection()
: _resolver(0), _socket(0)
{
	SCOPE_LOGGER;
}

udp_connection::~udp_connection()
{
	SCOPE_LOGGER;
	L_NOR   << "udp_connection::~udp_connection" << L_ENDL;

    if (_resolver)  delete _resolver;
    if (_socket)    delete _socket;
}

boost::asio::ip::udp::socket* udp_connection::socket()
{
	SCOPE_LOGGER;
	return _socket;
}

boost::asio::ip::udp::endpoint& udp_connection::remote_endpoint()
{
	SCOPE_LOGGER;
	return _remote_endpoint;
}

hs_void udp_connection::init(boost::asio::io_service& io_service)
{
    _resolver   = new boost::asio::ip::udp::resolver(io_service);
    _socket     = new boost::asio::ip::udp::socket(io_service);
}

hs_void udp_connection::start()
{
	SCOPE_LOGGER;
	_read();
}

hs_void udp_connection::start(const hs_char8* host, const hs_char8* port)
{
	SCOPE_LOGGER;
	_resolve(host, port);
}

hs_void udp_connection::stop()
{
	SCOPE_LOGGER;
	try
	{
		_socket->close();
	}
	catch (boost::system::error_code ec)
	{
		on_error(connection::ed_stop, ec);
	}
}

hs_void udp_connection::write(const packet* p)
{
	SCOPE_LOGGER;
	write(static_cast<const hs_char8*>(p->data()), p->size());
}

hs_void udp_connection::write(const hs_char8* data, const hs_uint32& size)
{
	SCOPE_LOGGER;
    std::cout   << "async_send" << std::endl;//////
	_socket->async_send(
		boost::asio::buffer(data, size),
		boost::bind(&udp_connection::_handle_write, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

hs_void udp_connection::write(const packet* p, boost::asio::ip::udp::endpoint& dest)
{
	SCOPE_LOGGER;
	write(static_cast<const hs_char8*>(p->data()), p->size(), dest);
}

hs_void	udp_connection::write(const hs_char8* data, const hs_uint32& size, 
				boost::asio::ip::udp::endpoint& dest)
{
	SCOPE_LOGGER;
    std::cout   << "async_send_to" << std::endl;//////
	_socket->async_send_to(
		boost::asio::buffer(data, size),
		dest,
		boost::bind(&udp_connection::_handle_write, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

hs_void udp_connection::write(const packet* p,
		const hs_char8* host, const hs_uint16& port)
{
	SCOPE_LOGGER;
	write(static_cast<const hs_char8*>(p->data()), p->size(), host, port);
}

hs_void	udp_connection::write(const hs_char8* data, const hs_uint32& size, 
			const hs_char8* host, const hs_uint16& port)
{
	SCOPE_LOGGER;
    std::cout   << "async_send_to 2" << std::endl;//////
	_socket->async_send_to(
		boost::asio::buffer(data, size),
		boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(host), port),
		boost::bind(&udp_connection::_handle_write, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

hs_void udp_connection::_connect(boost::asio::ip::udp::resolver::iterator endpoint_iterator)
{
	SCOPE_LOGGER;
	boost::asio::ip::udp::endpoint	endpoint	= *endpoint_iterator;
	
	_socket->async_connect(endpoint,	
		boost::bind(&udp_connection::_handle_connect,
			shared_from_this(),
			boost::asio::placeholders::error,
			++endpoint_iterator));
}

hs_void udp_connection::_stop()
{
    SCOPE_LOGGER;
    
    try
    {
        on_disconnect();
    }
    catch (boost::system::error_code ec)
    {
        on_error(connection::ed_stop, ec);
    }
}

hs_void udp_connection::_resolve(const hs_char8* host, const hs_char8* port)
{
	SCOPE_LOGGER;
	boost::asio::ip::udp::resolver::query	query(host, port);

	_resolver->async_resolve(query,
		boost::bind(&udp_connection::_handle_resolve,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator));
}

hs_void udp_connection::_read()
{
	SCOPE_LOGGER;
	_socket->async_receive_from(
		boost::asio::buffer(_buffer.writable_data(), _buffer.writable_size()), 
		_remote_endpoint,
		boost::bind(&udp_connection::_handle_read,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

hs_void udp_connection::_handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::udp::resolver::iterator endpoint_iterator)
{
	SCOPE_LOGGER;
	if (!err)
	{
		_connect(endpoint_iterator);
	}
	else
	{
		on_error(connection::ed_resolve, err);
	}
}

hs_void udp_connection::_handle_connect(const boost::system::error_code& err,
		boost::asio::ip::udp::resolver::iterator endpoint_iterator)
{
	SCOPE_LOGGER;
	if (!err)
	{
		try
		{
			on_connect();
		}
		catch (boost::system::error_code ec)
		{
			on_error(connection::ed_connect, ec);
		}

		_read();
	}
	else if (endpoint_iterator != boost::asio::ip::udp::resolver::iterator())
	{
		try
		{
			_socket->close();
		}
		catch (boost::system::error_code ec)
		{
			on_error(connection::ed_connect, ec);
		}

		_connect(endpoint_iterator);
	}
	else
	{
		on_error(connection::ed_connect, err);
	}
}

hs_void udp_connection::_handle_read(const boost::system::error_code& err, 
		std::size_t bytes_transferred)
{
	SCOPE_LOGGER;
	if (!err)
	{
		_buffer.write(bytes_transferred);

		hs_packet	hsp(_buffer.readable_data(), _buffer.readable_size());

		_buffer.read(bytes_transferred);

		try
		{
			on_dispatch(&hsp);
		}
		catch (boost::system::error_code ec)
		{
			on_error(connection::ed_read, ec);
		}

		_buffer.normalize();
		_read();
	}
	else
	{
        on_error(connection::ed_read, err);
        _stop();
	}
}

hs_void udp_connection::_handle_write(const boost::system::error_code& err, 
		std::size_t bytes_transferred)
{
	SCOPE_LOGGER;
	if (!err)
	{
	}
	else
	{
		on_error(connection::ed_write, err);
	}
}

};// namespace hongseok
