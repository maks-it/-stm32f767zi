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
