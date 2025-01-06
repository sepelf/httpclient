#include <memory>

#include "http_client.h"
#include "http_response.h"
#include <curl/curl.h>

namespace httpclient {

HttpClient::HttpClient() {

}

HttpClient::~HttpClient() {

}

struct ReadCtx {
  const std::string *content;
  size_t send_size;

  ReadCtx() :
      send_size(0), content(nullptr) {
  }

  ReadCtx(const std::string *content) :
      send_size(0), content(content) {
  }
};
static size_t httpclient_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
  ReadCtx *ctx = reinterpret_cast<ReadCtx*>(userdata);
  size_t len = std::min(size * nitems, ctx->content->size() - ctx->send_size);
  if (len) {
    std::memcpy(buffer, ctx->content->data() + ctx->send_size, len);
    ctx->send_size += len;
  }

  return len;
}

static size_t httpclient_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  std::string *content = reinterpret_cast<std::string*>(userdata);
  content->append(ptr, size * nmemb);

  return size * nmemb;
}

static HttpResponse request(const std::string &url, const HttpHeader *header = nullptr, const std::string *data =
                                nullptr) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    return HttpResponse();
  }
  std::shared_ptr<int> curl_holder(nullptr, [&](int*) {
    curl_easy_cleanup(curl);
  });

  CURLcode res;

  res = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  if (res != CURLcode::CURLE_OK) {
    return HttpResponse(400);
  }

  // http header
  curl_slist *hdr_list = nullptr;
  std::shared_ptr<int> hdr_holder(nullptr, [&](int*) {
    if (hdr_list) {
      curl_slist_free_all(hdr_list);
    }
  });
  if (header) {
    for (auto &p : *header) {
      std::string kv = p.first + ": " + p.second;
      curl_slist_append(hdr_list, kv.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdr_list);
  }

  // post data
  ReadCtx readCtx(data);
  if (data) {
    curl_easy_setopt(curl, CURLOPT_POST, 1);

    if (!data->empty()) {
      curl_easy_setopt(curl, CURLOPT_READFUNCTION, httpclient_read_callback);
      curl_easy_setopt(curl, CURLOPT_READDATA, &readCtx);
    }
  }

  // response data
  std::string content;
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, httpclient_write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);

  // do request
  res = curl_easy_perform(curl);
  if (res != CURLcode::CURLE_OK) {
    return HttpResponse(400);
  }

  // response header
  HttpHeader http_header;
  curl_header *hdr = nullptr;
  while ((hdr = curl_easy_nextheader(curl, CURLH_HEADER, -1, hdr))) {
    http_header[std::string(hdr->name)] = std::string(hdr->value);
  }

  // response status
  long status = 0;
  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
  if (res != CURLcode::CURLE_OK) {
    return HttpResponse(400);
  }

  return HttpResponse(status, std::move(http_header), content);
}

HttpResponse HttpClient::Get(const std::string &url) {
  return request(url);
}

HttpResponse HttpClient::Get(const std::string &url, const HttpHeader &header) {
  return request(url, &header);
}

HttpResponse HttpClient::Post(const std::string &url) {
  std::string data;
  return request(url, nullptr, &data);
}

HttpResponse HttpClient::Post(const std::string &url, const HttpHeader &header) {
  std::string data;
  return request(url, &header, &data);
}

HttpResponse HttpClient::Post(const std::string &url, const std::string &data) {
  return request(url, nullptr, &data);
}

HttpResponse HttpClient::Post(const std::string &url, const HttpHeader &header, const std::string &data) {
  return request(url, &header, &data);
}

}
