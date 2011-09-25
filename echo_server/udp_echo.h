#ifndef	__UDP_ECHO__
#define	__UDP_ECHO__

#include "udp_connection.h"
#include <string>

using namespace hongseok;

class udp_echo : public udp_connection
{
public:
	udp_echo();

    std::string address();

	void	on_connect();
	void	on_disconnect();
	void	on_dispatch(const packet* p);
	void	on_error(const connection::error_division_t& division,
				const boost::system::error_code& code);
};// class udp_echo : public udp_connection

#endif//__UDP_ECHO__
