#ifndef	__CONNECTION_H__
#define	__CONNECTION_H__

#include "types.h"
#include "buffer.h"
#include "packet.h"

#include <boost/asio.hpp>

namespace hongseok {

class connection
{
public:
	enum error_division_t
	{
		ed_connect,
		ed_disconnect,
		ed_read,
		ed_resolve,
		ed_stop,
		ed_write,
	};// enum error_divition_t

	// protocol
	class not_specified{};
	class tcp{};
	class udp{};

	typedef	not_specified	protocol;

    virtual hs_void init(boost::asio::io_service& io_service)   = 0;

	virtual hs_void	start()	= 0;
	virtual hs_void	start(const hs_char8* host, const hs_char8* port)	= 0;
	virtual hs_void	stop()	= 0;
	virtual hs_void	write(const packet* p)	= 0;
	virtual hs_void	write(const hs_char8* data, const hs_uint32& size)	= 0;

	virtual hs_void	on_connect()	= 0;
	virtual hs_void	on_disconnect()	= 0;
	virtual hs_void	on_dispatch(const packet* p)	= 0;
	virtual hs_void	on_error(const error_division_t& division,
						const boost::system::error_code& code)	= 0;

protected:
	buffer	_buffer;
};// class connection

};// namespace hongseok

#endif//__CONNECTION_H__
