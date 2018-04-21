#include <test-c.h>
#include <url-parse.h>
// verify we can include it twice without issue
#include <url-parse.h>

#define ASSERT_UNSET(member) \
  TEST_C_ASSERT_INT_EQUAL(0, t.member.size); \
  TEST_C_ASSERT_NULL(t.member.value, "t." #member ".value");

#define ASSERT(member, m_size, m_value) \
  TEST_C_ASSERT_INT_EQUAL(m_size, t.member.size); \
  TEST_C_ASSERT_STRING_STARTS_WITH(t.member.value, m_value);

TEST_C_START(it_can_handle_NULL)
  URL t = parse(NULL);
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_empty_string)
  URL t = parse("");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_login_separator)
  URL t = parse("@");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_port_separator)
  URL t = parse(":");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_implicit_file_scheme)
  URL t = parse("///");
  ASSERT(scheme, 4, "file");
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT(path, 1, "/");
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_host)
  URL t = parse("google.com");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT(host, 10, "google.com");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_host_and_port)
  URL t = parse("google.com:4001");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT(host, 10, "google.com");
  ASSERT(port, 4, "4001");
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_host_and_port_and_query)
  URL t = parse("google.com:4000?query");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT(host, 10, "google.com");
  ASSERT(port, 4, "4000");
  ASSERT_UNSET(path);
  ASSERT(query, 5, "query");
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_host_and_query)
  URL t = parse("google.com?query");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT(host, 10, "google.com");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT(query, 5, "query");
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_scheme)
  URL t = parse("http://");
  ASSERT(scheme, 4, "http");
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_scheme_and_host)
  URL t = parse("http://google.com");
  ASSERT(scheme, 4, "http");
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT(host, 10, "google.com");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_username_and_host)
  URL t = parse("bob@foo");
  ASSERT_UNSET(scheme);
  ASSERT(user, 3, "bob");
  ASSERT_UNSET(password);
  ASSERT(host, 3, "foo");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_username_and_password_and_host)
  URL t = parse("bob:secret@foo");
  ASSERT_UNSET(scheme);
  ASSERT(user, 3, "bob");
  ASSERT(password, 6, "secret");
  ASSERT(host, 3, "foo");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_scheme_username_and_password_and_host)
  URL t = parse("http://bob:secret@foo");
  ASSERT(scheme, 4, "http");
  ASSERT(user, 3, "bob");
  ASSERT(password, 6, "secret");
  ASSERT(host, 3, "foo");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_username_and_empty_password_and_host)
  URL t = parse("bob:@foo");
  ASSERT_UNSET(scheme);
  ASSERT(user, 3, "bob");
  ASSERT_UNSET(password);
  ASSERT(host, 3, "foo");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_path)
  URL t = parse("/foo");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT(path, 4, "/foo");
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_query)
  URL t = parse("?http://");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT(query, 7, "http://");
  ASSERT_UNSET(fragment);
TEST_C_END

TEST_C_START(it_can_handle_fragment)
  URL t = parse("#boo");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT(fragment, 3, "boo");
TEST_C_END

TEST_C_START(it_can_handle_scheme_in_fragment)
  URL t = parse("asdf#http://");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT(host, 4, "asdf");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT(fragment, 7, "http://");
TEST_C_END

TEST_C_START(it_can_handle_query__and_scheme_in_fragment)
  URL t = parse("a#?http://");
  ASSERT_UNSET(scheme);
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT(host, 1, "a");
  ASSERT_UNSET(port);
  ASSERT_UNSET(path);
  ASSERT_UNSET(query);
  ASSERT(fragment, 8, "?http://");
TEST_C_END

TEST_C_START(it_can_handle_a_full_url_without_scheme_and_empty_port)
  URL t = parse("//bob:foo@google.com:/foo/boo/coo?asdf=5#://asdf?::://");
  ASSERT_UNSET(scheme);
  ASSERT(user, 3, "bob");
  ASSERT(password, 3, "foo");
  ASSERT(host, 10, "google.com");
  ASSERT_UNSET(port);
  ASSERT(path, 12, "/foo/boo/coo");
  ASSERT(query, 6, "asdf=5");
  ASSERT(fragment, 13, "://asdf?::://");
TEST_C_END

TEST_C_START(it_can_handle_a_full_file_url_without_scheme)
  URL t = parse("///bob:foo@google.com:/foo/boo/coo?asdf=5#://asdf?::://");
  ASSERT(scheme, 4, "file");
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT(path, 32, "/bob:foo@google.com:/foo/boo/coo");
  ASSERT(query, 6, "asdf=5");
  ASSERT(fragment, 13, "://asdf?::://");
TEST_C_END

TEST_C_START(it_can_handle_a_full_file_url_with_scheme)
  URL t = parse("file:///bob:foo@google.com:/foo/boo/coo?asdf=5#://asdf?::://");
  ASSERT(scheme, 4, "file");
  ASSERT_UNSET(user);
  ASSERT_UNSET(password);
  ASSERT_UNSET(host);
  ASSERT_UNSET(port);
  ASSERT(path, 32, "/bob:foo@google.com:/foo/boo/coo");
  ASSERT(query, 6, "asdf=5");
  ASSERT(fragment, 13, "://asdf?::://");
TEST_C_END

TEST_C_START(it_can_handle_a_full_url)
  URL t = parse("http://bob:foo@google.com:8080/foo/boo/coo?asdf=5#://asdf?::://");
  ASSERT(scheme, 4, "http");
  ASSERT(user, 3, "bob");
  ASSERT(password, 3, "foo");
  ASSERT(host, 10, "google.com");
  ASSERT(port, 4, "8080");
  ASSERT(path, 12, "/foo/boo/coo");
  ASSERT(query, 6, "asdf=5");
  ASSERT(fragment, 13, "://asdf?::://");
TEST_C_END

TEST_C_START(it_can_handle_a_full_url_with_empty_port)
  URL t = parse("http://bob:foo@google.com:/foo/boo/coo?asdf=5#://asdf?::://");
  ASSERT(scheme, 4, "http");
  ASSERT(user, 3, "bob");
  ASSERT(password, 3, "foo");
  ASSERT(host, 10, "google.com");
  ASSERT_UNSET(port);
  ASSERT(path, 12, "/foo/boo/coo");
  ASSERT(query, 6, "asdf=5");
  ASSERT(fragment, 13, "://asdf?::://");
TEST_C_END

TEST_C_START(it_can_handle_a_full_url_with_empty_query)
  URL t = parse("http://bob:foo@google.com:8080/foo/boo/coo?#://asdf?::://");
  ASSERT(scheme, 4, "http");
  ASSERT(user, 3, "bob");
  ASSERT(password, 3, "foo");
  ASSERT(host, 10, "google.com");
  ASSERT(port, 4, "8080");
  ASSERT(path, 12, "/foo/boo/coo");
  ASSERT_UNSET(query);
  ASSERT(fragment, 13, "://asdf?::://");
TEST_C_END

TEST_C_START(it_can_handle_a_full_url_with_empty_query_and_empty_fragment)
  URL t = parse("http://bob:foo@google.com:8080/foo/boo/coo?#");
  ASSERT(scheme, 4, "http");
  ASSERT(user, 3, "bob");
  ASSERT(password, 3, "foo");
  ASSERT(host, 10, "google.com");
  ASSERT(port, 4, "8080");
  ASSERT(path, 12, "/foo/boo/coo");
  ASSERT_UNSET(query);
  ASSERT_UNSET(fragment);
TEST_C_END
