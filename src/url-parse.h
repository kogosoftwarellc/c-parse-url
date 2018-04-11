#ifndef URL_PARSE_HEADER_T
#define URL_PARSE_HEADER_T 5
typedef struct {
  char* value;
  int size;
} URL_part;

typedef struct {
  URL_part scheme;
  URL_part user;
  URL_part password;
  URL_part host;
  URL_part port;
  URL_part path;
  URL_part query;
  URL_part fragment;
} URL;
extern URL parse(char* url);
#endif
