test_files	= ""

Dir.glob("source/*.cc").each {|filename| test_files += (filename + " ")}

build_command	= "g++ -c -I. -I../boost/include -L../boost/lib " + test_files + "-lboost_system -lpthread -lboost_thread -lodbc -static"

puts build_command
system(build_command)

test_file	= ""
Dir.glob("*.o").each {|filename| test_files += (filename + " ")}
build_command	= "ar rscv libhongseok.a " + test_files

puts build_command
system(build_command)
system("rm *.o")
system("cp ./source/*.h .")

# add some comment for test
