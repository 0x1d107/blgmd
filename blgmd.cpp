#include <md4c.h>
#include <md4c-html.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>

std::istream *input_stream = nullptr;
std::ostream *output_stream = nullptr;


void process_output(const MD_CHAR *out,MD_SIZE size, void *userdata){
	
	if(!output_stream||output_stream->bad()){
		std::cerr << "Bad output stream!"<<std::endl;
		exit(2);
	}
	output_stream->write(out,size);
}
const unsigned int parser_flags=MD_FLAG_LATEXMATHSPANS |MD_FLAG_TABLES;
const unsigned int render_flags=MD_HTML_FLAG_XHTML;
std::unordered_map<std::string,std::string> metadata_map;
int main(int argc,const char **argv){
	if(argc<2){
		input_stream = &std::cin;
	}else{
		input_stream = new std::ifstream(argv[1]);
	}
	if(argc>2)
		output_stream = new std::ofstream(argv[2]);
	else
		output_stream = &std::cout;
	if(!input_stream||input_stream->fail()){
		std::cerr << "Bad input stream!"<<std::endl;
		return 2;
	}
	if(!output_stream||output_stream->fail()){
		std::cerr << "Bad output stream!"<<std::endl;
		return 2;
	}

	std::ostringstream input_ss;
	std::string line;

	while(std::getline(*input_stream,line)){
	
		std::stringstream liness(line);
		std::string key,value;
		std::getline(liness,key,':');
		std::getline(liness,value);
		metadata_map[key] = value;

		
		if(!line.size())
			break;
		
	}
	while(std::getline(*input_stream,line)){
		input_ss << line<<std::endl;
	}
	
	const std::string &input_string = input_ss.str();

	md_html(input_string.c_str(),input_string.size(),process_output,NULL,parser_flags,render_flags);

	if(input_stream != &std::cin)
		delete input_stream;
	if(output_stream != &std::cout)
		delete output_stream;
	return 0;
}
