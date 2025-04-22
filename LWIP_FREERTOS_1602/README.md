# LWIP FreeRTOS 1602

 **Explanation:**
When using LWIP with an RTOS and the LWIP_TIMERS feature is enabled, the stack relies on periodic calls to `sys_check_timeouts()` to process time-based events. These events include managing DHCP leases, handling TCP retransmissions, and executing other protocol-specific timers. Without this function being called regularly, these timeouts will not be processed, leading to issues such as DHCP failing to acquire an IP address, TCP connections stalling, or other network-related problems.

In an RTOS environment, `sys_check_timeouts()` must be called in a loop with a small delay (e.g., 1ms)  to ensure that the timers are processed in a timely manner. This is why it is placed inside the `StartDefaultTask` function, which runs as a FreeRTOS task.

```c
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    sys_check_timeouts();  // This is critical for DHCP, TCP, and timers to work
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}
```