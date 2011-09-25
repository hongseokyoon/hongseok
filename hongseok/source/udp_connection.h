#ifndef	__UDP_CONNECTION_H__
#define	__UDP_CONNECTION_H__

#include "connection.h"

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace hongseok {

class udp_connection : 
	public connection, 
	public boost::enable_shared_from_this<udp_connection>
{
public:
    udp_connection();
	virtual	~udp_connection();

	typedef	connection::udp	protocol;

	boost::asio::ip::udp::socket*	socket();
	boost::asio::ip::udp::endpoint&	remote_endpoint();	

    hs_void init(boost::asio::io_service& io_service);

	hs_void	start();	// no throw
	hs_void	start(const hs_char8* host, const hs_char8* port);	// no throw
	hs_void	stop();		// no throw
	hs_void	write(const packet* p);	// no throw
	hs_void	write(const hs_char8* data, const hs_uint32& size);	// no throw
	hs_void	write(const packet* p, boost::asio::ip::udp::endpoint& dest);	// no throw
	hs_void	write(const hs_char8* data, const hs_uint32& size, 
					boost::asio::ip::udp::endpoint& dest);	// no throw
	hs_void	write(const packet* p, 
					const hs_char8* host, const hs_uint16& port);	// no throw
	hs_void	write(const hs_char8* data, const hs_uint32& size, 
					const hs_char8* host, const hs_uint16& port);	// no throw

	virtual hs_void	on_connect()	{}
	virtual hs_void	on_disconnect()	{}
	virtual hs_void	on_dispatch(const packet* p)	= 0;
	// this function should throw no exceptions.
	virtual hs_void	on_error(const connection::error_division_t& division,
						const boost::system::error_code& code)	= 0;

private:
	hs_void	_connect(boost::asio::ip::udp::resolver::iterator endpoint_iterator);	// no throw
    hs_void _stop();
	hs_void	_resolve(const hs_char8* host, const hs_char8* port);	// no throw
	hs_void	_read();	// no throw

	hs_void	_handle_resolve(const boost::system::error_code& err,
				boost::asio::ip::udp::resolver::iterator endpoint_iterator);	// no throw

	hs_void	_handle_connect(const boost::system::error_code& err,
				boost::asio::ip::udp::resolver::iterator endpoint_iterator);	// no throw

	hs_void	_handle_read(const boost::system::error_code& err,
				std::size_t bytes_transferred);	// no throw

	hs_void	_handle_write(const boost::system::error_code& err,
				std::size_t bytes_transferred);	// no throw

private:
	boost::asio::ip::udp::resolver*	_resolver;
	boost::asio::ip::udp::socket*	_socket;

	boost::asio::ip::udp::endpoint	_remote_endpoint;
};// class udp_connection

};// namespace hongseok

#endif//__UDP_CONNECTION_H__
