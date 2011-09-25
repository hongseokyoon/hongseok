test_files	= ""

Dir.glob("*.cc").each {|filename| test_files += (filename + " ")}

Dir.glob("../hongseok/source/*.cc").each {|filename| test_files += (filename + " ")}

#build_command	= "g++ -g -I. -I../boost/include -I../hongseok -L../boost/lib -L../hongseok -o session_server " + test_files + "-lboost_system -lpthread -lboost_thread -lodbc -lhongseok -static"

#build_command	= "g++ -g -I. -I../boost/include -I../hongseok/source -L../boost/lib -L../hongseok -o echo_server " + test_files + "-lboost_system -lpthread -lboost_thread -lodbc"

build_command = "g++ -Wall -g -I../hongseok/source -o echo_server " + test_files + "-lboost_system -lpthread -lboost_thread -lodbc"

puts build_command
system(build_command)
