#include <md4c.h>
#include <md4c-html.h>
#include <iostream>
#include <string>
#include <fstream>

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
int main(int argc,const char **argv){
	if(argc<3){
		std::cerr << "Usage: blgmd <input> <output>" << std::endl;
		return 1;
	}
	input_stream = new std::ifstream(argv[1]);
	output_stream = new std::ofstream(argv[2]);
	if(!input_stream||input_stream->bad()){
		std::cerr << "Bad input stream!"<<std::endl;
		return 2;
	}
	if(!output_stream||output_stream->bad()){
		std::cerr << "Bad output stream!"<<std::endl;
		return 2;
	}
	
	std::string input_string;
	input_stream->seekg(0,std::ios::end);
	size_t filesize = input_stream->tellg();
	if(!filesize){
		std::cerr<<"Bad input file size!"<<std::endl;
		return 2;
	}
	input_string.resize(filesize);
	input_stream->seekg(0,std::ios::beg);
	input_stream->read(&input_string[0],input_string.size());	

	md_html(input_string.c_str(),input_string.size(),process_output,NULL,parser_flags,render_flags);

	
	delete input_stream;
	delete output_stream;
	return 0;
}
