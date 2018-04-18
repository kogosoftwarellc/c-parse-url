#include "url-parse.h"
#include "string.h"

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

  char* fragmentSeparator = strstr(url, "#");
  char* querySeparator = strstr(url, "?");
  char* pathSeparator = strstr(url, "/");
  char* portSeparator = strstr(url, ":");
  char* loginSeparator = strstr(url, "@");
  char* schemeSeparator = strstr(url, "://");

  if (strlen(url) >=3 && *url == '/' && *(url + 1) == '/' && *(url + 2) == '/') {
    pathSeparator = url + 2;
    schemeSeparator = NULL;
    portSeparator = NULL;
    loginSeparator = NULL;
  }

  if (fragmentSeparator) {
    int fragmentLength = strlen(fragmentSeparator);
    if (fragmentLength > 1) {
      parsed.fragment.value = fragmentSeparator + 1;
      parsed.fragment.size = fragmentLength - 1;
    }

    if (querySeparator > fragmentSeparator) {
      querySeparator = NULL;
    }

    if (pathSeparator > fragmentSeparator) {
      pathSeparator = NULL;
    }

    if (portSeparator > fragmentSeparator) {
      portSeparator = NULL;
    }

    if (loginSeparator > fragmentSeparator) {
      loginSeparator = NULL;
    }

    if (schemeSeparator > fragmentSeparator) {
      schemeSeparator = NULL;
    }
  }

  if (querySeparator) {
    if (fragmentSeparator) {
      if (fragmentSeparator - querySeparator > 1) {
        parsed.query.value = querySeparator + 1;
        parsed.query.size = fragmentSeparator - querySeparator - 1;
      }
    } else {
      parsed.query.value = querySeparator + 1;
      parsed.query.size = strlen(querySeparator) - 1;
    }

    if (pathSeparator > querySeparator) {
      pathSeparator = NULL;
    }

    if (portSeparator > querySeparator) {
      portSeparator = NULL;
    }

    if (schemeSeparator > querySeparator) {
      schemeSeparator = NULL;
    }

    if (loginSeparator > querySeparator) {
      loginSeparator = NULL;
    }
  }

  if (schemeSeparator) {
    if ((pathSeparator && schemeSeparator > pathSeparator)
        || (loginSeparator && schemeSeparator > loginSeparator)
        || (portSeparator && schemeSeparator > portSeparator)) {
      schemeSeparator = NULL;
    } else {
      parsed.scheme.value = url;
      parsed.scheme.size = schemeSeparator - url;
      char* afterSchemeSeparator = schemeSeparator + 3;
      if (afterSchemeSeparator) {
        pathSeparator = strstr(afterSchemeSeparator, "/");
      } else {
        pathSeparator = NULL;
        loginSeparator = NULL;
      }

      if (*afterSchemeSeparator == '/') {
        loginSeparator = NULL;
      }
    }
  }

  if (loginSeparator - url > 1) {
    char* login = url;
    if (schemeSeparator) {
      login = schemeSeparator + 3;
    } else if (*url == '/' && *(url + 1) == '/') {
      login = url + 2;
      pathSeparator = strstr(login, "/");
    }
    char* passwordSeparator = strstr(login, ":");
    parsed.user.value = login;
    if (passwordSeparator) {
      parsed.user.size = passwordSeparator - login;
      if (loginSeparator - passwordSeparator > 1) {
        parsed.password.value = passwordSeparator + 1;
        parsed.password.size = loginSeparator - passwordSeparator - 1;
      }
    } else {
      parsed.user.size = loginSeparator - login;
    }
  } else {
    loginSeparator = NULL;
  }

  if (pathSeparator) {
    if (loginSeparator > pathSeparator) {
      loginSeparator = NULL;
    }
    parsed.path.value = pathSeparator;
    if (querySeparator) {
      parsed.path.size = querySeparator - pathSeparator;
    } else if (fragmentSeparator) {
      parsed.path.size = fragmentSeparator - pathSeparator;
    } else {
      parsed.path.size = strlen(pathSeparator);
    }
  }

  char* host;
  if (loginSeparator) {
    host = loginSeparator + 1;
  } else if (schemeSeparator) {
    if (strlen(schemeSeparator) == 3 || *(schemeSeparator + 3) == '/') {
      host = NULL;
    } else {
      host = schemeSeparator + 3;
    }
  } else {
    switch(*url) {
      case '/':
      case '?':
      case '#':
      case '@':
      case ':':
        host = NULL;
        break;
      default:
        host = url;
    }
  }

  if (host) {
    char* end = strpbrk(host, ":/?#");
    parsed.host.value = host;
    if (end) {
      parsed.host.size = strlen(host) - strlen(end);
    } else {
      parsed.host.size = strlen(host);
    }
  }

  if (portSeparator - url > 1) {
    char* newPortSeparator;
    if (loginSeparator && portSeparator < loginSeparator) {
      newPortSeparator = strpbrk(loginSeparator, ":/?#");
    } else if (schemeSeparator && portSeparator <= schemeSeparator) {
      newPortSeparator = strpbrk(schemeSeparator + 3, ":/?#");
    } else {
      newPortSeparator = portSeparator;
    }

    if (newPortSeparator && *newPortSeparator == ':') {
      if (pathSeparator) {
        if (pathSeparator - newPortSeparator > 1) {
          parsed.port.value = newPortSeparator + 1;
          parsed.port.size = pathSeparator - newPortSeparator - 1;
        } else {
          portSeparator = NULL;
          newPortSeparator = NULL;
        }
      } else {
        char* nextSeparator = strpbrk(newPortSeparator + 1, ":/?#");
        parsed.port.value = newPortSeparator + 1;
        parsed.port.size = nextSeparator - newPortSeparator;
      }
    }
  }

  return parsed;
}
