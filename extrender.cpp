#include "extrender.h"
#include "itex-src/itex2MML.h"
#include <cstring>
#include <string>
char MATHML_DISPLAY = 0;
void render_mathml(MD_HTML *r, const MD_CHAR *text, MD_SIZE size){
	if(!text||!size){
		return;
	}
	std::string str;
	if(MATHML_DISPLAY)
		str = "$$" + std::string(text,size) + "$$";
	else
		str = "$" + std::string(text,size) + "$";
	char *mathml = itex2MML_parse(str.c_str(),str.size());
	if(!mathml){
		render_verbatim(r,str.c_str(),str.size());
		return;
	}
	render_verbatim(r,mathml,strlen(mathml));
	itex2MML_free_string(mathml);
}
void highlight_code(MD_HTML *r, const MD_CHAR *text, MD_SIZE size){
	render_verbatim(r,text,size);
	
}

