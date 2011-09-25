test_files	= ""

Dir.glob("*.cc").each {|filename| test_files += (filename + " ")}

Dir.glob("../hongseok/source/*.cc").each {|filename| test_files += (filename + " ")}

#build_command	= "g++ -g -I. -I../odbc/include -I../hongseok -L../odbc/lib -L../hongseok -o session_server " + test_files + "-lboost_system -lpthread -lboost_thread -lodbc -lhongseok -static"

build_command	= "g++ -g -I. -I../hongseok/source -I../../odbc/include -L../hongseok -L../../odbc/lib -o echo_server " + test_files + "-lboost_system -lpthread -lboost_thread -lodbc"

puts build_command
system(build_command)
