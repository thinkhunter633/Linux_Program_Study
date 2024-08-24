#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "http_parser.h"

typedef struct http_message_tag {

    char request_url[512];
    enum http_method method;
    int  keepalive_flag;
    unsigned short http_major;
    unsigned short http_minor;
}http_message;

http_message g_xHttpMessage = {0};

int32_t http_register_uri_GET_index(const char* uri)
{
    /*@todo, procedure index.html*/
    printf("http_register_uri_GET_index: %s\n", uri);
    return 0;
}
typedef struct http_uri_table_tag {
    enum http_method method;
    const char* uri;
    int32_t (*handler)(const char* uri);
}http_uri_table;

http_uri_table g_xHttpUriTable[] = {
    {HTTP_GET,"/index.html",http_register_uri_GET_index},
};

#define HTTP_URI_TABLE_SIZE (sizeof(g_xHttpUriTable)/sizeof(g_xHttpUriTable[0]))


static int32_t prv_http_uri_find_handler(enum http_method method, const char* uri) {
    for (int i = 0; i < HTTP_URI_TABLE_SIZE; i++) {
        if (g_xHttpUriTable[i].method == method && strcmp(g_xHttpUriTable[i].uri, uri) == 0) {
            return i;
        }
    }
    return -1;
}


static int on_info(http_parser* p) {
  return 0;
}


static int on_data(http_parser* p, const char *at, size_t length) {
  return 0;
}

static int request_url_cb(http_parser* p, const char* buf, size_t len) {
    strncat(g_xHttpMessage.request_url, buf, len);
    printf("request_url_cb: %d, %s\n", (int)len, g_xHttpMessage.request_url);
    return 0;
}


static int headers_complete_cb(http_parser* p) {
    int32_t index = 0;
    g_xHttpMessage.method = p->method;
    g_xHttpMessage.http_major = p->http_major;
    g_xHttpMessage.http_minor = p->http_minor;
    g_xHttpMessage.keepalive_flag = http_should_keep_alive(p);

    if ((index = prv_http_uri_find_handler(p->method, g_xHttpMessage.request_url)) >= 0) {
        printf("uri find handler\n");
        g_xHttpUriTable[index].handler(g_xHttpMessage.request_url);
    }
    return 0;
}

static int message_complete_cb(http_parser* p) {
    
    printf("message_complete_cb, keep alive,%d \r\n", g_xHttpMessage.keepalive_flag);
    return 0;
}

static http_parser_settings settings = {
    .on_message_begin = on_info,
    .on_headers_complete = on_info,
    .on_message_complete = message_complete_cb,
    .on_headers_complete = headers_complete_cb,
    .on_header_field = on_data,
    .on_header_value = on_data,
    .on_url = request_url_cb,
    .on_status = on_data,
    .on_body = on_data
};


int32_t frame_procedure(char* buf, int32_t buf_len) {
    // TODO: implement frame procedure
    struct http_parser parser;

    size_t parsed;
    http_parser_init(&parser, HTTP_REQUEST);

    parsed = http_parser_execute(&parser, &settings, buf, buf_len);

    return parsed;
}