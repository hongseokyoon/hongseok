#ifndef	__TCP_ECHO__
#define	__TCP_ECHO__

#include "tcp_connection.h"
#include <string>

using namespace hongseok;

class tcp_echo : public tcp_connection
{
public:
    tcp_echo();

    const std::string&  address() const;

	void	on_connect();
	void	on_disconnect();
	void	on_dispatch(const packet* p);
	void	on_error(const connection::error_division_t& division,
				const boost::system::error_code& code);

    void    init();

private:
    std::string _address;
};// class tcp_echo : public udp_connection

#endif//__TCP_ECHO__
