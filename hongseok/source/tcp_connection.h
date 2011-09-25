#ifndef __TCP_CONNECTION_H__
#define	__TCP_CONNECTION_H__

#include "connection.h"
#include "parser.h"

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace hongseok {

class tcp_connection : 
	public connection, 
	public boost::enable_shared_from_this<tcp_connection>
{
public:
    tcp_connection(parser::parser_t p);
	virtual ~tcp_connection();

	typedef	connection::tcp	protocol;

	boost::asio::ip::tcp::socket*	socket();	// no throw
    boost::asio::ip::tcp::endpoint  remote_endpoint();

    hs_void init(boost::asio::io_service& io_service);

	virtual hs_void	start();	// no throw
	virtual hs_void	start(const hs_char8* host, const hs_char8* port);	// no throw
	virtual hs_void	stop();	// throw(boost::system::system_error);
	virtual hs_void	write(const packet* p);	// no throw
	virtual hs_void	write(const hs_char8* data, const hs_uint32& size);	// no throw

	virtual hs_void	on_connect()	= 0;
	virtual hs_void	on_disconnect()	= 0;
	virtual hs_void	on_dispatch(const packet* p)	= 0;
	// this function should throw no exceptions.
	virtual hs_void	on_error(const connection::error_division_t& division, 
						const boost::system::error_code& code)	= 0;

private:
	hs_void	_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator);	// no throw
    hs_void _stop();
	hs_void	_resolve(const hs_char8* host, const hs_char8* port);	// no throw
	hs_void	_read();	// no throw

	hs_void	_handle_resolve(const boost::system::error_code& err, 
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator);	// no throw
	hs_void	_handle_connect(const boost::system::error_code& err, 
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator);	// no throw

	hs_void	_handle_read(const boost::system::error_code& err, 
				std::size_t bytes_transferred);	// no throw
	hs_void	_handle_write(const boost::system::error_code& err, 
				std::size_t bytes_transferred);	// no throw

private:
	boost::asio::ip::tcp::resolver*		_resolver;
	boost::asio::ip::tcp::socket*		_socket;

	parser::parser_t					_parser;
};// class connection

};// namespace hongseok

#endif//__TCP_CONNECTION_H__
