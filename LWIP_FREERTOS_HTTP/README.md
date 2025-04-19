# LWWIP FREERTOS HTTP Server

We are not going to enable `LWIP` built-in `HTTPD` server, as it's designed only to serve static files

## Custom HTTP Server Implementation

`http_server.h`

```c
/* http_server.h */
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#ifndef HTTP_PORT
  #define HTTP_PORT 80
#endif

/**
 * @brief  Blocking HTTP‐server loop.
 *         Call from your FreeRTOS/CMSIS‑RTOS task entry.
 */
void start_http_server(void);

#endif /* HTTP_SERVER_H */
```

`http_server.c`

```c
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

```

## Add HTTP Task in .ioc

- Go to **Middleware > FREERTOS > Tasks and Queues tab**
- Click `+ Add`:
  - Name: httpTask
  - Function Name: HttpTask
  - Stack Size: 512 (→ internally means 512 words = 2048 bytes)
  - Priority: Normal (same as default task is fine)
  - Start: Enabled

Click OK / Save and let CubeIDE regenerate the code.

## Configure `freertos.c`

Add headers:

```c
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "http_server.h"
#include "lwip/netif.h"
/* USER CODE END Includes */
```

Add code insiede `HttpServerTask`:

```c
void HttpServerTask(void const * argument)
{
  /* USER CODE BEGIN HttpServerTask */

	/* wait a moment to ensure netif is up (DHCP/static done in StartDefaultTask) */
	osDelay(10000);

	/* start the lwIP-based HTTP server (spawns its own sys_thread) */
	start_http_server();

  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END HttpServerTask */
}
```



