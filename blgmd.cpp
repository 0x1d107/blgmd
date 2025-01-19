#include <md4c.h>
#include <md4c-html.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unistd.h>

#include "mini-mustach.h"

std::istream *input_stream = nullptr;
std::ostream *output_stream = nullptr;


void print_usage(){
std::cerr << "Usage: blgmd [-X metadata] [-i <input>] [-o output]"<<std::endl;
exit(10);
}
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
int main(int argc,char *argv[]){
	int c;
	char *metakey=NULL;
	while((c = getopt(argc,argv,"i:o:X:"))!=-1){
		switch(c){
			case 'i':
			input_stream = new std::ifstream(optarg);
			break;
			case 'o':
			output_stream = new std::ofstream(optarg);
			break;
			case 'X':
			metakey=optarg;
			break;
			default:
			print_usage();
			


		}


	}



	if(!input_stream)
		input_stream = &std::cin;
	if(!output_stream)
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
	if(metakey){
		std::cout <<metadata_map[std::string(metakey)];
		return 0;
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
