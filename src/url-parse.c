#include "url-parse.h"
#include "string.h"

static const short HAS_FRAGMENT = 1;
static const short HAS_QUERY = 2;
static const short HAS_PATH = 4;
static const short HAS_PORT = 8;
static const short HAS_HOST = 16;
static const short HAS_LOGIN = 32;
static const short HAS_SCHEME = 64;
static char* URL_PARSE_FILE_SCHEME = "file";

#define _URL_PARSE_HANDLE_HOST() \
  if (index) { \
    parsed.host.size = index_length; \
    parsed.host.value = index; \
    index = NULL; \
    index_length = 0; \
  }

#define _URL_PARSE_HANDLE_PORT() \
  if (index_after_separator) { \
    parsed.port.size = index_after_separator_length; \
    parsed.port.value = index_after_separator; \
    index_after_separator = NULL; \
    index_after_separator_length = 0; \
  }

#define _URL_PARSE_INCREMENT_QUERY() \
  if (state & HAS_QUERY) { \
    ++parsed.query.size; \
    break; \
  }

#define _URL_PARSE_INCREMENT_PATH() \
  if (state & HAS_PATH) { \
    ++parsed.path.size; \
    break; \
  }

static URL init() {
  return (URL) {
    .scheme = {
      .value = NULL,
      .size = 0,
    },
    .user = {
      .value = NULL,
      .size = 0,
    },
    .password = {
      .value = NULL,
      .size = 0,
    },
    .host = {
      .value = NULL,
      .size = 0,
    },
    .port = {
      .value = NULL,
      .size = 0,
    },
    .path = {
      .value = NULL,
      .size = 0,
    },
    .query = {
      .value = NULL,
      .size = 0,
    },
    .fragment = {
      .value = NULL,
      .size = 0,
    },
  };
}

URL parse(char* url) {
  URL parsed = init();

  if (url == NULL || !*url) {
    return parsed;
  }

  short state = 0;
  char* index = NULL;
  char* index_after_separator = NULL;
  int index_length = 0;
  int index_after_separator_length = 0;
  int len = strlen(url);

  if (len >= 2) {
    if (*(url) == '/' && *(url + 1) == '/') {
      state |= HAS_SCHEME;
      len -= 2;
      url += 2;

      if (len >= 1 && *(url) == '/') {
        parsed.scheme.value = URL_PARSE_FILE_SCHEME;
        parsed.scheme.size = strlen(URL_PARSE_FILE_SCHEME);
      }
    }
  }

  for (int i = 0; i < len; i++) {
    switch(*(url + i)) {
      case '#':
        if (len - i > 1) {
          parsed.fragment.size = len - i - 1;
          parsed.fragment.value = url + i + 1;
        }
        i = len; // stop parsing
        break;

      case '?':
        _URL_PARSE_INCREMENT_QUERY()
        _URL_PARSE_HANDLE_HOST()
        _URL_PARSE_HANDLE_PORT()

        if (len - i > 1 && *(url + i + 1) != '#') {
          state |= HAS_QUERY;
          parsed.query.value = url + i + 1;
        }
        break;

      case '/':
        _URL_PARSE_INCREMENT_QUERY()
        _URL_PARSE_INCREMENT_PATH()
        parsed.path.size = 1;
        parsed.path.value = url + i;
        state |= HAS_PATH;
        break;

      case '@':
        _URL_PARSE_INCREMENT_QUERY()
        _URL_PARSE_INCREMENT_PATH()

        if (index) {
          parsed.user.size = index_length;
          parsed.user.value = index;
          index_length = 0;
          index = NULL;
        }

        if (index_after_separator) {
          parsed.password.size = index_after_separator_length;
          parsed.password.value = index_after_separator;
          index_after_separator_length = 0;
          index_after_separator = NULL;
        }
        break;

      case ':':
        _URL_PARSE_INCREMENT_QUERY()
        _URL_PARSE_INCREMENT_PATH()

        if (
            !(state & HAS_SCHEME) &&
            len - i > 2 &&
            *(url + i + 1) == '/' &&
            *(url + i + 2) == '/'
        ) {
          state |= HAS_SCHEME;
          parsed.scheme.size = i;
          parsed.scheme.value = index;
          i += 2;
          index = NULL;
          index_length = 0;
        } else if(
            !index_after_separator &&
            len - i > 1
        ) {
          char* after_separator = url + i + 1;
          switch(*(after_separator)) {
            case '@':
            case '/':
            case '?':
            case '#':
              break;
            default:
              index_after_separator = after_separator;
              index_after_separator_length = 1;
              ++i;
          }
        }
        break;

      default:
        _URL_PARSE_INCREMENT_QUERY()
        _URL_PARSE_INCREMENT_PATH()

        if (!index && !index_after_separator) {
          index = url + i;
        }

        if (index_after_separator) {
          ++index_after_separator_length;
        } else if (index) {
          ++index_length;
        }
    }
  }

  _URL_PARSE_HANDLE_HOST()
  _URL_PARSE_HANDLE_PORT()

  return parsed;
}
