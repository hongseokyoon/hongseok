#include "udp_echo.h"

#include <iostream>
#include <sstream>

using namespace std;

udp_echo::udp_echo()
: udp_connection()
{
}

std::string udp_echo::address()
{
    ostringstream   oss;
    oss << remote_endpoint().address().to_string() << ":" << remote_endpoint().port();
    return oss.str();
}

void udp_echo::on_connect()
{
	cout	<< address() << " connect" << endl;
}

void udp_echo::on_disconnect()
{
	cout	<< address() << " disconnect" << endl;
}

void udp_echo::on_dispatch(const packet* p)
{
	//cout	<< string((char*)p->data(), 0, p->size()) << endl;	
    cout    << address() << " dispatch " << p->size() << " byte(s)" << endl;
	write(p, remote_endpoint());

    stop();
    start();
}

void udp_echo::on_error(const connection::error_division_t& division,
		const boost::system::error_code& code)
{
	cout	<< "error division: " << division << " msg: " << code.message() << " " << code.value() << endl;
}
