if (ARGV.length < 2)
	puts "Usage: ruby create_hongseok_project.rb <directory> <project_name>"	
	exit
end

DIRECTORY		= ARGV[0]
PROJECTNAME	= ARGV[1]

begin
	Dir.mkdir(DIRECTORY)
rescue
end

dst	= File.open("#{DIRECTORY}/build.rb", "w")
src	= File.open("template/build.rb.tmplt")
src.each_line do |line|
	line.gsub!(/\[\[PROJECTNAME\]\]/, PROJECTNAME)
	dst.puts line
end
src.close
dst.close

dst	= File.open("#{DIRECTORY}/main.cc", "w")
src	= File.open("template/main.cc.tmplt")
src.each_line do |line|
	dst.puts line
end
src.close
dst.close
