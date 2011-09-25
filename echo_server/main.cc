#include "log.h"

#include "server.h"

#include "tcp_echo.h"
#include "udp_echo.h"
#include "util.h"

using namespace hongseok;

void print()
{
    std::cout   << "hongseok" << std::endl;
}

class test_c
{
public:
    static void print()
    {
        std::cout   << "test_c::print()" << std::endl;
    }
};// class test_c

int main(int argc, char** argv)
{
	L_NOR   << "server_started." << L_ENDL;

	try
	{
		server	s(4);
		//s.accept<tcp_echo>(30001, &tcp_echo::init);
		//s.connect<tcp_echo>("localhost", "30001", &tcp_echo::init);
//        s.connect<tcp_echo>("localhost", "30001", util::invoker(&tcp_echo::init));
//        s.accept<udp_echo>(30002);
		//s.connect<udp_echo>("localhost", "30002");

        s.set_timer(1000, test_c::print);
        s.set_timer(3000, print);

		s.run();
	}
	catch (std::exception& e)
	{
		std::cerr	<< "exception: " << e.what() << "\n";
	}

	return 0;
}
