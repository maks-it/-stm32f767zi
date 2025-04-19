# LWIP FREERTOS STATIC IP

**Prerequisites:**
- First complete tutorial in `LWIP_BASIC`

## 1. **Enable FreeRTOS**
- Go to **Middleware > FREERTOS**
- Enable it with:
  - Interface: `CMSIS v1`
- Into **Advanced setting**:
- Enalbe `USE_NEWLIB_REENTRANT`

## 2. **Change SYS Timebase Source**
- Go to **System Core > SYS**
- Timebase Source set to: `TIM6` (should not be already used)

