#include <cerrno>
#include <cstring>
#include <md4c.h>
#include <md4c-html.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <unordered_map>
#include <boost/regex.hpp>
#include <boost/function.hpp>
#include <SQLiteCpp/SQLiteCpp.h>


std::istream *input_stream = nullptr;
std::ostream *output_stream = nullptr;


void print_usage(){
	std::cerr << "Usage: blgmd [-X metadata | -i input | -o output | -t template] ... "<<std::endl;
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
std::unordered_map<std::string,std::string> metadata;
SQLite::Database *metaDB = nullptr;
SQLite::Statement *meta_update_stmt=nullptr;
const char *SQL_CREATE_METADATA = "CREATE TABLE IF NOT EXISTS metadata(filename TEXT, key TEXT, value TEXT,UNIQUE (filename,key));";
const char *SQL_UPDATE_METADATA = "REPLACE INTO metadata(filename,key,value) VALUES (?,?,?)";

struct KVFormatter {
	std::string operator()(std::string key){
		return metadata[key];

	}
};

int main(int argc,char *argv[]){
	int c;
	char *metakey=NULL;
	char *tmpl_file=NULL;
	char *dbfile= NULL;
	bool parse_meta = true;
	const char *input_file = "-";
	const char *output_file = "-";
	while((c = getopt(argc,argv,"i:o:X:t:hd:N"))!=-1){
		switch(c){
			case 'i':
			input_stream = new std::ifstream(optarg);
			input_file = optarg;
			break;
			case 'o':
			output_stream = new std::ofstream(optarg);
			output_file = optarg;
			break;
			case 'X':
			metakey=optarg;
			break;
			case 't':
			tmpl_file=optarg;
			break;
			case 'd':
			dbfile = optarg;
			break;
			case 'N':
			parse_meta =false;
			break;
			case 'h':
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
	if(dbfile){
		metaDB = new SQLite::Database (dbfile,SQLite::OPEN_CREATE| SQLite::OPEN_READWRITE);
		metaDB->exec(SQL_CREATE_METADATA);
		meta_update_stmt = new SQLite::Statement(*metaDB,SQL_UPDATE_METADATA);
	}	

	while(parse_meta&&std::getline(*input_stream,line)){
		std::stringstream liness(line);
		std::string key,value;
		std::getline(liness,key,':');
		std::getline(liness,value);
		if(!line.size())
			break;
		metadata[key]=value;
		if(meta_update_stmt){
			meta_update_stmt->bind(1,input_file);
			meta_update_stmt->bind(2,key);
			meta_update_stmt->bind(3,value);
			meta_update_stmt->exec();
			meta_update_stmt->reset();
		}
	}
	if(meta_update_stmt){
		delete meta_update_stmt;
	}
	if(metakey){
		std::cout <<metadata[std::string(metakey)];
		return 0;
	}
	while(std::getline(*input_stream,line)){
		input_ss << line<<std::endl;
	}
	const std::string &input_string = input_ss.str();


	if(tmpl_file){
		std::ifstream tmpl(tmpl_file);
		std::string line;
		boost::regex variable_re("$$([a-zA-Z0-9]*)$$");
		std::ostream_iterator<char> osit(*output_stream);
		while(std::getline(tmpl,line)){
			if(line == "<!--html-->")
				md_html(input_string.c_str(),input_string.size(),process_output,NULL,parser_flags,render_flags);
			else{
				std::function<std::string(const boost::smatch &key)> cbk = [](const boost::smatch &key){
					return metadata[key.str()];
				}; 
				boost::regex_replace(osit,line.begin(),line.end(),variable_re,cbk);
				//*output_stream << line<<std::endl;
			}
		}


	}else{
		// Render markdown

		md_html(input_string.c_str(),input_string.size(),process_output,NULL,parser_flags,render_flags);
	}

	if(input_stream != &std::cin)
		delete input_stream;
	if(output_stream != &std::cout)
		delete output_stream;
	if(metaDB)
		delete metaDB;
	return 0;
}
