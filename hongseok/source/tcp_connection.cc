#include "tcp_connection.h"

#include "log.h"
#include "util.h"

#include <boost/bind.hpp>

namespace hongseok {

tcp_connection::tcp_connection(parser::parser_t p)
: _resolver(0), _socket(0), _parser(p)
{
    SCOPE_LOGGER;
}

tcp_connection::~tcp_connection()
{
	SCOPE_LOGGER;

    if (_resolver)  delete _resolver;
    if (_socket)    delete _socket;
}

boost::asio::ip::tcp::socket* tcp_connection::socket()
{
	SCOPE_LOGGER;
	return _socket;
}

boost::asio::ip::tcp::endpoint tcp_connection::remote_endpoint()
{
    return _socket->remote_endpoint();
}

hs_void tcp_connection::init(boost::asio::io_service& io_service)
{
    _resolver   = new boost::asio::ip::tcp::resolver(io_service);
    _socket     = new boost::asio::ip::tcp::socket(io_service);
}

hs_void tcp_connection::start()
{
	SCOPE_LOGGER;
	try
	{
		on_connect();	// accepted
	}
	catch (boost::system::error_code ec)
	{
		on_error(connection::ed_connect, ec);
	}

	_read();
}

hs_void tcp_connection::start(const hs_char8* host, const hs_char8* port)
{
	SCOPE_LOGGER;
	_resolve(host, port);
}

hs_void tcp_connection::stop()
{
	SCOPE_LOGGER;
	try
	{
		_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send);
	}
	catch (boost::system::error_code ec)
	{
		on_error(connection::ed_stop, ec);
	}
}

hs_void tcp_connection::write(const packet* p)
{
	SCOPE_LOGGER;
	boost::asio::async_write(*_socket, 
		boost::asio::buffer(p->data(), p->size()),
		boost::bind(&tcp_connection::_handle_write, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

hs_void tcp_connection::write(const hs_char8* data, const hs_uint32& size)
{
	SCOPE_LOGGER;
	boost::asio::async_write(*_socket, 
		boost::asio::buffer(data, size),
		boost::bind(&tcp_connection::_handle_write, 
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

hs_void tcp_connection::_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	SCOPE_LOGGER;
	boost::asio::ip::tcp::endpoint	endpoint	= *endpoint_iterator;
	_socket->async_connect(endpoint,
		boost::bind(&tcp_connection::_handle_connect,
			shared_from_this(),
			boost::asio::placeholders::error,
			++endpoint_iterator));
}

hs_void tcp_connection::_stop()
{
    SCOPE_LOGGER;
	try
	{
		on_disconnect();
		_socket->close();
	}
	catch (boost::system::error_code ec)
	{
		on_error(connection::ed_stop, ec);
	}
}

hs_void tcp_connection::_resolve(const hs_char8* host, const hs_char8* port)
{
	SCOPE_LOGGER;
	boost::asio::ip::tcp::resolver::query	query(host, port);
	_resolver->async_resolve(query,
		boost::bind(&tcp_connection::_handle_resolve,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator));
}

hs_void tcp_connection::_read()
{
	SCOPE_LOGGER;
	_socket->async_read_some(boost::asio::buffer(_buffer.writable_data(),
		_buffer.writable_size()),
		boost::bind(&tcp_connection::_handle_read,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

hs_void tcp_connection::_handle_resolve(const boost::system::error_code& err, 
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
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

hs_void tcp_connection::_handle_connect(const boost::system::error_code& err, 
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	SCOPE_LOGGER;
	if (!err)
	{
		try
		{
			on_connect();	// connected
		}
		catch (boost::system::error_code ec)
		{
			on_error(connection::ed_connect, ec);
		}

		_read();
	}
	else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
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

hs_void tcp_connection::_handle_read(const boost::system::error_code& err, 
		std::size_t bytes_transferred)
{
	SCOPE_LOGGER;
	if (!err)
	{
		// async_read_some() function write data into _buffer 
		// as amount of bytes_transferred
		_buffer.write(bytes_transferred);	// move buffer position

		while (true)
		{
			// parse
			parser::range_t	r	= _parser(_buffer.readable_data(), 
										_buffer.readable_size());
	
			if (r.first > r.second || r.second > _buffer.readable_size())
			{
				// CRITICAL ERROR
				// error in _parser
				break;
			}

			_buffer.read(r.first);	// skip invalid data

			if (r.first == r.second)
			{
				// nothing to dispatch
				break;
			}

			// read buffer
			hs_packet	hsp(_buffer.readable_data(), r.second - r.first);
			_buffer.read(r.second - r.first);

			// process data
			try
			{
				on_dispatch(&hsp);
			}
			catch (boost::system::error_code ec)
			{
				on_error(connection::ed_read, ec);
			}
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

hs_void tcp_connection::_handle_write(const boost::system::error_code& err, 
		size_t bytes_transferred)
{
	SCOPE_LOGGER;
	if (!err)
	{
	}
	else
	{
		on_error(connection::ed_write, err);
		stop();
	}
}

};// namespace hongseok
