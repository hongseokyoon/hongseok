if (ARGV.length < 1)
	puts "Usage: ruby make_tcp_connection.rb <class_name>"
	exit
end

classname	= ARGV[0]
classdef	= classname.upcase

header_file	= File.open("#{classname}.h", "w")
File.new("template/tcp_connection.h.tmplt").each_line do |line|
	line.gsub!(/\[\[CLASSNAME\]\]/, classname)
	line.gsub!(/\[\[CLASSDEF\]\]/, classdef)
	header_file.puts line
end
header_file.close

source_file	= File.open("#{classname}.cc", "w")
File.new("template/tcp_connection.cc.tmplt").each_line do |line|
	line.gsub!(/\[\[CLASSNAME\]\]/, classname)
	line.gsub!(/\[\[CLASSDEF\]\]/, classdef)
	source_file.puts line
end
source_file.close
