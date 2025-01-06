#pragma once

#include "http_response.h"
#include "http_header.h"

namespace httpclient {

class HttpClient {
public:
  HttpClient();
  ~HttpClient();

  HttpResponse Get(const std::string &url);
  HttpResponse Get(const std::string &url, const HttpHeader &header);

  HttpResponse Post(const std::string &url);
  HttpResponse Post(const std::string &url, const HttpHeader &header);
  HttpResponse Post(const std::string &url, const std::string &data);
  HttpResponse Post(const std::string &url, const HttpHeader &header, const std::string &data);
};

}

