#include "extrender.h"
#include "itex-src/itex2MML.h"
#include <cstring>
void render_mathml(MD_HTML *r, const MD_CHAR *text, MD_SIZE size){
	if(!text){
		return;
	}
	char *mathml = itex2MML_parse(text,size);
	if(!mathml){
		render_verbatim(r,text,size);
		return;
	}
	render_verbatim(r,mathml,strlen(mathml));
	itex2MML_free_string(mathml);
}
void highlight_code(MD_HTML *r, const MD_CHAR *text, MD_SIZE size){
	render_verbatim(r,text,size);
	
}

