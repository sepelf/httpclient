#pragma once

#include "http_header.h"

namespace httpclient {

class HttpResponse {
public:
  int status;
  HttpHeader header;
  std::string content;

  HttpResponse();
  HttpResponse(const int &status);
  HttpResponse(const int &status, const std::string &content);
  HttpResponse(const int &status, const HttpHeader &header, const std::string &content);
};

}
