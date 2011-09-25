require 'rexml/document'

# return hash(name, attrs)
def collect_node(e)
	nodes	= {} 

	e.elements.each do |child|
		child_nodes	= collect_node(child)
		child_nodes.each_pair do |name, attrs|
			nodes[name] = [] unless nodes.has_key? name
			nodes[name]	= nodes[name] + attrs
		end
	end

	nodes[e.name] = [] unless nodes.has_key? e.name
	e.attributes.each_key {|attr_name| nodes[e.name] << attr_name}

	nodes.each_value {|attr_names| attr_names.uniq!}

	return nodes
end

if (ARGV.length < 2)
	puts "Usage: ruby make_xml_reader.rb <xml filename> <export class name>"
	exit
end

filename	= ARGV[0]
classname	= ARGV[1]

doc	= REXML::Document.new(File.new(filename))

# collect node names and attrs
nodes	= collect_node(doc.root)

source	= File.new("#{classname}.h", "w")

# include
source.puts IO.read("template/include.tmplt")

# node class
File.new("template/node.tmplt").each_line do |line|
	if (line.match(/([\s]*)\[\[ENUMLIST\]\]/))
		ts	= $1
		nodes.each_key do |node_name|
			source.puts("#{ts}#{node_name},")
		end
		line	= ""
	end

	source.puts line
end
source.puts

# node-inherited classes
nodes.each_pair do |node_name, node_attrs|
	source.puts("struct #{node_name} : public node")
	source.puts("{")
	source.puts("\tenum { mytype = node::#{node_name} };")
	source.puts
	source.print("\t#{node_name}(")
	attr_count	= 0
	node_attrs.each do |attr_name| 
		source.print("const char* _#{attr_name}")
		source.print(", ") if attr_count + 1 < node_attrs.count
		attr_count	+= 1
	end
	source.puts(")")
	source.puts("\t: node(mytype)")
	source.puts("\t{")
	node_attrs.each {|attr_name| source.puts("\t\tif (_#{attr_name})\t#{attr_name}\t= _#{attr_name};")}
	source.puts("\t}")
	source.puts
	node_attrs.each {|attr_name| source.puts("\tstring\t#{attr_name};")}
	source.puts("};// #{node_name} : public node")
	source.puts
end

# reader class
File.new("template/reader.tmplt").each_line do |line|
	if (line.match(/\[\[CLASSNAME\]\]/))
		line.gsub!(/\[\[CLASSNAME\]\]/, classname)
	elsif (line.match(/([\s]*)\[\[PRINTCASE\]\]/))
		ts	=	$1
		nodes.each_pair do |node_name, node_attrs|
			source.puts("#{ts}case node::#{node_name}:")
			source.puts("#{ts}\t{")
			source.puts("#{ts}\t\tcout\t<< \"#{node_name} \";")
			source.puts("#{ts}\t\tconst #{node_name}*\tdn\t= reinterpret_cast<const #{node_name}*>(n);")
			node_attrs.each do |attr_name|
				source.puts("#{ts}\t\tcout\t<< \"#{attr_name}=\\\"\" << dn->#{attr_name} << \"\\\" \";")
			end
			source.puts("#{ts}\t\tcout\t<< endl;")
			source.puts("#{ts}\t}")
			source.puts("#{ts}\tbreak;")
		end
		line	= ""
	elsif (line.match(/([\s]*)\[\[MAKEIF\]\]/))
		ts	= $1
		nodes.each_pair do |node_name, node_attrs|
			source.puts("#{ts}if (string(\"#{node_name}\") == e->Value())")
			source.puts("#{ts}{")
			source.print("#{ts}\tn\t= new #{node_name}(")
			attr_count	= 0
			node_attrs.each do |attr_name|
				source.print("e->Attribute(\"#{attr_name}\")")
				source.print(", ") if attr_count + 1 < node_attrs.count
				attr_count	+= 1
			end
			source.puts(");")
			source.puts("#{ts}}")
		end
		line	= ""
	end

	source.puts line
end

source.close
