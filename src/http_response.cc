#include "http_response.h"

namespace httpclient {

HttpResponse::HttpResponse() {
  status = 500;
}

HttpResponse::HttpResponse(const int &status) :
    status(status) {

}

HttpResponse::HttpResponse(const int &status, const std::string &content) :
    status(status), content(content) {

}

HttpResponse::HttpResponse(const int &status, const HttpHeader &header, const std::string &content) :
    status(status), header(header), content(content) {

}

}

