source_file_str = ""
Dir["../*.cc"].each do |filename|
  source_file_str += (filename + " ")
end

test_file_str = ""
Dir["test_*.cc"].each do |filename|
  test_file_str += (filename + " ")
end

build_command	= "g++ -I.. -o test test.cc " + source_file_str + test_file_str + "-lgtest -lboost_thread -lpthread -lboost_system -lodbc -g"

puts build_command
system(build_command)
if File.exist?("test")
  system("./test")
  system("rm test")
else
  puts "Could not run test: failed to build."
end
