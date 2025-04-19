/* http_server.c */
#include "http_server.h"
#include "lwip/api.h"
#include <string.h>

static const char RESPONSE_HDR[] =
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/plain\r\n"
  "Content-Length: 12\r\n"
  "\r\n";
static const char RESPONSE_BODY[] = "Hello world!";

void start_http_server(void) {
    struct netconn *listener, *conn;
    struct netbuf  *buf;
    err_t err;

    /* create, bind and listen on TCP port 80 */
    listener = netconn_new(NETCONN_TCP);
    netconn_bind(listener, IP_ADDR_ANY, HTTP_PORT);
    netconn_listen(listener);

    for (;;) {
        /* accept incoming connection */
        if (netconn_accept(listener, &conn) == ERR_OK) {
            /* read & discard request */
            if (netconn_recv(conn, &buf) == ERR_OK) {
                netbuf_delete(buf);
                /* send static response */
                netconn_write(conn, RESPONSE_HDR, strlen(RESPONSE_HDR), NETCONN_COPY);
                netconn_write(conn, RESPONSE_BODY, strlen(RESPONSE_BODY), NETCONN_COPY);
            }
            netconn_close(conn);
            netconn_delete(conn);
        }
    }
}
