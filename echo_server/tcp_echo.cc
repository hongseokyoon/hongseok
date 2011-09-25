#include "tcp_echo.h"
#include "parser.h"
#include "util.h"

#include <iostream>
#include <sstream>

using namespace std;

tcp_echo::tcp_echo()
: tcp_connection(hongseok::parser::read_all)
{
SCOPE_LOGGER;
}

const std::string& tcp_echo::address() const
{
    return _address;
}

void tcp_echo::on_connect()
{
SCOPE_LOGGER;
    ostringstream   oss;
    oss << remote_endpoint().address().to_string() << ":" << remote_endpoint().port();
    _address    = oss.str();

	cout	<< address() << " connect" << endl;
}

void tcp_echo::on_disconnect()
{
SCOPE_LOGGER;
	cout	<< address() << " disconnect" << endl;
}

void tcp_echo::on_dispatch(const packet* p)
{
SCOPE_LOGGER;
    cout    << address() << " dispatch " << p->size() << "byte(s)" << endl;
	//cout	<< string((char*)p->data(), 0, p->size()) << endl;
	write(p);
}

void tcp_echo::on_error(const connection::error_division_t& division,
		const boost::system::error_code& code)
{
SCOPE_LOGGER;
    cout    << code.value() << endl;
	cout	<< address() << " error division: " << division << " msg: " << code.message() << endl;
}

void tcp_echo::init()
{
    cout    << "init" << endl;
}
