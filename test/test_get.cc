#include "http_client.h"
#include <iostream>
#include <cassert>

int main() {
  httpclient::HttpClient client;

  {
    httpclient::HttpResponse resp = client.Get("http://localhost:8000/");

    assert(resp.status == 200);

    assert(!resp.header.empty());

    assert(!resp.content.empty());
  }

  {
    httpclient::HttpHeader header { { "XXXXX", "YYYYY" } };
    httpclient::HttpResponse resp = client.Get("http://localhost:8000/", header);

    assert(resp.status == 200);

    assert(!resp.header.empty());

    assert(!resp.content.empty());
  }

  {
    httpclient::HttpResponse resp = client.Post("http://localhost:8000/", "{\"name\":\"apple\"}");

    assert(resp.status == 200);

    assert(!resp.header.empty());

    assert(!resp.content.empty());
  }

  return 0;
}
