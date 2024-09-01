#pragma once

#include <md4c.h>

#define NEED_HTML_ESC_FLAG   0x1
#define NEED_URL_ESC_FLAG    0x2


/*****************************************
 ***  HTML rendering helper functions  ***
 *****************************************/

#define ISDIGIT(ch)     ('0' <= (ch) && (ch) <= '9')
#define ISLOWER(ch)     ('a' <= (ch) && (ch) <= 'z')
#define ISUPPER(ch)     ('A' <= (ch) && (ch) <= 'Z')
#define ISALNUM(ch)     (ISLOWER(ch) || ISUPPER(ch) || ISDIGIT(ch))


	
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MD_HTML_tag MD_HTML;
struct MD_HTML_tag {
    void (*process_output)(const MD_CHAR*, MD_SIZE, void*);
    void* userdata;
    unsigned flags;
    int image_nesting_level;
    char escape_map[256];
};
void render_verbatim(MD_HTML* r, const MD_CHAR* text, MD_SIZE size);

void highlight_code(MD_HTML* r, const MD_CHAR* data, MD_SIZE size);	
void render_mathml(MD_HTML* r, const MD_CHAR* data, MD_SIZE size);	


#ifdef __cplusplus
}
#endif

