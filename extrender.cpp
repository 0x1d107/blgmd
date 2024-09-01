#include "extrender.h"
void render_mathml(MD_HTML *r, const MD_CHAR *text, MD_SIZE size){
	render_verbatim(r,text,size);
}
void highlight_code(MD_HTML *r, const MD_CHAR *text, MD_SIZE size){
	render_verbatim(r,text,size);
	
}

