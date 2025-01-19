#include "extrender.h"
#include "itex-src/itex2MML.h"
#include <cstring>
#include <string>
#include <sstream>
#include <srchilite/sourcehighlight.h>
#include <srchilite/langmap.h>
char MATHML_DISPLAY = 0;
const char *CODE_LANG=NULL;
int CODE_LANG_SZ = 0;
std::stringstream hlbuf;
std::stringstream mathbuf;

void mathml_append(MD_HTML *r, const MD_CHAR *text, MD_SIZE size){
	mathbuf << std::string(text,size);
}
void render_mathml(MD_HTML *r){
	if(!mathbuf.str().size()){
		return;
	}
	std::string str;
	if(MATHML_DISPLAY)
		str = "$$" + mathbuf.str() + "$$";
	else
		str = "$" + mathbuf.str() + "$";
	char *mathml = itex2MML_parse(str.c_str(),str.size());
	if(!mathml){
		render_verbatim(r,str.c_str(),str.size());
		return;
	}
	render_verbatim(r,mathml,strlen(mathml));
	itex2MML_free_string(mathml);
	mathbuf = std::stringstream();
}
void highlight_append(MD_HTML *r, const MD_CHAR *text, MD_SIZE size){
	hlbuf<< std::string(text,size);
}
void highlight_code(MD_HTML *r){
	std::ostringstream oss_hlcode;
	srchilite::SourceHighlight hl("htmlcss.outlang");
	srchilite::LangMap lmap("lang.map");
	std::string lname(CODE_LANG,CODE_LANG_SZ);
	std::string mapped = lmap.getMappedFileName(lname);
	if(!mapped.size())
		mapped = "nohilite.lang";

	hl.highlight(hlbuf,oss_hlcode,mapped);
	hlbuf = std::stringstream();
	std::string hlcode = oss_hlcode.str();
	render_verbatim(r,hlcode.c_str(),hlcode.size()+1);
}

