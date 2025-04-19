# LWIP BASIC

## Basic static IP LWIP Configuration

### 1. **Correct GPIOs for ETH**

The STM32F767ZI's Ethernet MAC is connected to specific GPIOs for RMII or MII. These GPIOs must be:
- Properly **configured in alternate function mode**
- Connected to the **PHY chip** (e.g., LAN8742)
- **Matched** with the ETH mode (RMII vs MII)

⚠️ If any GPIO is misconfigured, Ethernet communication won't work or might behave erratically.

---

### 2. **Enable RCC and Select Clock Source**

ETH requires a **reference clock**, and the system requires a proper **PLL configuration**:
- **Bypass mode**: selected when an external **PHY provides the RMII 50 MHz clock**
- **Crystal/Ceramic Resonator**: needed if you're generating ETH clock internally
- ETH doesn't use system clock directly — it uses the **HCLK**, **PCLK1**, **PCLK2**, and **PLL outputs**.

✔️ Your clocks must be:
- **216 MHz SYSCLK** via HSI or HSE and PLL
- APB1 ≤ 54 MHz for peripherals like ETH, TIM, etc.

---

### 3. **Enable TIM1 internal clock**

TIM1 isn't strictly required by LwIP itself, but may be used in:
- **Time base generation**
- **Debug or LED blinking**
- **Delays if HAL_Delay() is TIM-based**

Ensuring TIM clocks are active avoids side-effects from dependent services.

---

### 4. **Implement `sys_jiffies()` in `ethernetif.c`**

When using:
```c
LWIP_TIMERS = 1
NO_SYS = 1
```
You're using **raw LwIP stack without an RTOS**, and timers are driven manually.

`sys_jiffies()` must be implemented to provide a millisecond timestamp.
```c
u32_t sys_jiffies(void)
{
  return HAL_GetTick();
}
```
This is **mandatory** so LwIP can manage timeouts (e.g., ARP cache, TCP retransmit).

---

### 5. **Call `MX_LWIP_Process()` in `main.c` main loop**

This function is critical in **NO_SYS mode**:
- It processes Ethernet RX/TX
- Calls into LwIP stack to handle incoming packets
- Must be called as often as possible

In **RTOS mode**, LwIP has its own thread. But in **NO_SYS**, you're responsible for processing everything manually.

---

### Summary: Why all this is needed on F767ZI

Because:
- STM32F767ZI has **complex peripheral interdependencies** (ETH, GPIO, clocks)
- LwIP requires **manual setup when no OS is used**
- CubeMX does **not generate a fully working Ethernet config out of the box**
- Some PHYs (e.g., LAN8742) require **external 50 MHz clock**, and you must match this in hardware and RCC config
