* [中文说明](./README_CN.md)

### RTL8721Dx PWM Timer Example — Safe ISR Callbacks & mbed GPIO (FreeRTOS)

🔹 This example demonstrates how to use the **general timer (gtimer)** on the **RTL8721Dx SoC** together with the **mbed GPIO API** to control 3 LED channels blinking.

- 📎 Development board purchase links:  
  - [🛒 Taobao](https://item.taobao.com/item.htm?id=904981157046)  
  - [📦 Amazon](https://www.amazon.com/-/zh/dp/B0FB33DT2C/)  
- 📄 [Chip & module information](https://aiot.realmcu.com/cn/module/index.html)  
- 📚 [PWM / Timer documentation](https://aiot.realmcu.com/zh/latest/rtos/peripherals/timer/index.html#pwm-timer)  
- 🎬 [Ameba SoC PWM tutorial video](https://aiot.realmcu.com/zh/center/videos/detail/29?theme_id=3)

---

### Features

- **TIMER1** (periodical mode)  
  - Uses `gtimer_start_periodical` with period `PWM_TIME_US`.  
  - When the interrupt occurs, the ISR only sets `time1_expired` to `1`.  
  - In the main loop, when the flag is detected:
    - Toggle the GPIO corresponding to `LED1_PIN`.  
    - Print the `"timer1 timeout..."` log.  

- **TIMER2** (one-shot mode)  
  - Uses `gtimer_start_one_shout` with timeout `LED2_TIME_US`.  
  - The ISR sets `time2_expired` to `1`.  
  - In the main loop, when the flag is detected:
    - Toggle the GPIO corresponding to `LED2_PIN`.  
    - Print the `"timer2 timeout..."` log.  
    - Call `gtimer_start_one_shout` again to restart TIMER2, so that LED2 blinks periodically.  

- **TIMER3** (one-shot mode)  
  - Same as TIMER2, but for `LED3_PIN`, with timeout `LED3_TIME_US`.  

---

### Hardware Setup

1️⃣ **Required components**

- 3 LEDs (optional)  
  ⚠️ Most EVBs already have one or more on‑board LEDs. You can still connect 3 external LEDs to clearly demonstrate the 3 timer channels.

2️⃣ **Wiring (if using external LEDs)**

- Connect each LED to the corresponding board pin and GND  
  (actual pins are defined in `pwm_timer_mbed_RGB.h`, for example):

  ```c
  #define LED1_PIN _PA_14
  #define LED2_PIN _PA_15
  #define LED3_PIN _PA_16
  ```

- Recommended wiring:  
  - `LED1_PIN` → LED1 anode  
  - `LED2_PIN` → LED2 anode  
  - `LED3_PIN` → LED3 anode  
  - Each LED cathode is connected to `GND` through a current‑limiting resistor  

> If you only want to experience the software logic, you can use the on‑board LEDs directly without external components.

---

### Quick Start

1️⃣ **Initialize SDK environment**

- Configure `env.sh` (or `env.bat`) path and run:

  ```bash
  source {sdk}/env.sh
  ```

- Replace `{sdk}` with the absolute path to the root directory of the  
  [ameba-rtos SDK](https://github.com/Ameba-AIoT/ameba-rtos).

   ⚡ **Note**: This example requires SDK version **≥ v1.2**.
---

2️⃣ **Build**

```bash
source env.sh
ameba.py build
```

---

3️⃣ **Flash to FLASH**

```bash
ameba.py flash --p COMx \
  --image km4_boot_all.bin 0x08000000 0x8014000 \
  --image km0_km4_app.bin 0x08014000 0x8200000
```

⚠️ Note: you can also use the prebuilt bin files provided in the parent directory:

```bash
ameba.py flash --p COMx \
  --image ../km4_boot_all.bin 0x08000000 0x8014000 \
  --image ../km0_km4_app.bin 0x08014000 0x8200000
```

---

4️⃣ **Open serial monitor**

```bash
ameba.py monitor --port COMx --b 1500000
```

---

5️⃣ **Press the EVB RST button and observe the log output**

After reset, the example starts running. You can observe:

- Serial logs indicating timer timeout events.  
- LEDs on the board (or external LEDs) blinking according to their timer configuration.

---

### Log Example

```bash
Log:
   [MONITOR] 2026-04-13 17:55:23.980 ROM:[V1.1]
   [MONITOR] 2026-04-13 17:55:23.980 FLASH RATE:1, Pinmux:1
   [MONITOR] 2026-04-13 17:55:23.980 IMG1(OTA1) VALID, ret: 0
   [MONITOR] 2026-04-13 17:55:23.980 IMG1 ENTRY[f800779:0]
   [MONITOR] 2026-04-13 17:55:23.980 [BOOT-I] KM4 BOOT REASON 0: Initial Power on
   [MONITOR] 2026-04-13 17:55:23.980 [BOOT-I] KM4 CPU CLK: 240000000 Hz
   [MONITOR] 2026-04-13 17:55:23.980 [BOOT-I] KM0 CPU CLK: 96000000 Hz
   [MONITOR] 2026-04-13 17:55:23.980 [BOOT-I] PSRAM Ctrl CLK: 240000000 Hz
   [MONITOR] 2026-04-13 17:55:23.980 [BOOT-I] IMG1 ENTER MSP:[30009FDC]
   [MONITOR] 2026-04-13 17:55:23.980 [BOOT-I] Build Time: Apr 13 2026 16:29:22
   [MONITOR] 2026-04-13 17:55:23.980 [BOOT-I] IMG1 SECURE STATE: 1
   [MONITOR] 2026-04-13 17:55:23.980 [FLASH-I] FLASH CLK: 80000000 Hz
   [MONITOR] 2026-04-13 17:55:23.981 [FLASH-I] Flash ID: 85-20-16 (Capacity: 32M-bit)
   [MONITOR] 2026-04-13 17:55:23.981 [FLASH-I] Flash Read 4IO
   [MONITOR] 2026-04-13 17:55:23.981 [FLASH-I] FLASH HandShake[0x2 OK]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] KM0 XIP IMG[0c000000:8600]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] KM0 SRAM[20068000:860]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] KM0 PSRAM[0c008e60:20]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] KM0 ENTRY[20004d00:60]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] KM4 XIP IMG[0e000000:18aa0]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] KM4 SRAM[2000b000:520]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] KM4 PSRAM[0e018fc0:20]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] KM4 ENTRY[20004d80:40]
   [MONITOR] 2026-04-13 17:55:23.990 [BOOT-I] IMG2 BOOT from OTA 1, Version: 1.1
   [MONITOR] 2026-04-13 17:55:23.991 [BOOT-I] Image2Entry @ 0xe0079d9 ...
   [MONITOR] 2026-04-13 17:55:23.991 [APP-I] KM4 APP STA[LOCKS-IRT
   [MONITOR] 2026-04-13 17:55:23.991 [AP] KM0 inP-I] VTOit_retarR: 30007get_lock000, VTOR_NS:300s
   [MONITOR] 2026-04-13 17:55:23.991 07000
   [MONITOR] 2026-04-13 17:55:23.991 [APP-I] VTOR: 30007000, VTOR_NS:30007000
   [MONITOR] 2026-04-13 17:55:23.991 [APP-I] IMG2 SECURE ST[MAIN-I]ATE: 1
   [MONITOR] 2026-04-13 17:55:23.991  IWDG refresh on!
   [MONITOR] 2026-04-13 17:55:23.991 [MAIN-I] KM0 OS START
   [MONITOR] 2026-04-13 17:55:23.991 [CLK-I] [CAL4M]: delta:1 target:320 PPM: 3125 PPM_Limit:30000
   [MONITOR] 2026-04-13 17:55:24.000 [CLK-I] [CAL131K]: delta:11 target:2441 PPM: 4506 PPM_Limit:30000
   [MONITOR] 2026-04-13 17:55:24.000 [LOCKS-I] KM4 init_retarget_locks
   [MONITOR] 2026-04-13 17:55:24.000 [APP-I] BOR arises when supply voltage decreases under 2.57V and recovers above 2.7V.
   [MONITOR] 2026-04-13 17:55:24.000 [MAIN-I] KM4 MAIN
   [MONITOR] 2026-04-13 17:55:24.000 [VER-I] AMEBA-RTOS SDK VERSION: 1.3.0
   [MONITOR] 2026-04-13 17:55:24.010 [MAIN-I] File System Init Success
   [MONITOR] 2026-04-13 17:55:24.010 [MAIN-I] KM4 START SCHEDULER
   [MONITOR] 2026-04-13 17:55:24.010 [PWM_TIMER_DEMO-I] pwm_timer_mbed_RGB demo start!
   [MONITOR] 2026-04-13 17:55:24.262 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:24.514 [PWM_TIMER_DEMO-I] timer3 timeout: led_16, period:500 ms
   [MONITOR] 2026-04-13 17:55:24.514 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:24.766 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:25.018 [PWM_TIMER_DEMO-I] timer3 timeout: led_16, period:500 ms
   [MONITOR] 2026-04-13 17:55:25.018 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:25.269 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:25.511 [PWM_TIMER_DEMO-I] timer3 timeout: led_16, period:500 ms
   [MONITOR] 2026-04-13 17:55:25.521 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:25.773 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:26.014 [PWM_TIMER_DEMO-I] timer1 timeout: led_14, period:2000 ms
   [MONITOR] 2026-04-13 17:55:26.014 [PWM_TIMER_DEMO-I] timer3 timeout: led_16, period:500 ms
   [MONITOR] 2026-04-13 17:55:26.014 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:26.266 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:26.518 [PWM_TIMER_DEMO-I] timer3 timeout: led_16, period:500 ms
   [MONITOR] 2026-04-13 17:55:26.518 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:26.770 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:27.021 [PWM_TIMER_DEMO-I] timer3 timeout: led_16, period:500 ms
   [MONITOR] 2026-04-13 17:55:27.021 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:27.273 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:27.515 [PWM_TIMER_DEMO-I] timer3 timeout: led_16, period:500 ms
   [MONITOR] 2026-04-13 17:55:27.525 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:27.777 [PWM_TIMER_DEMO-I] timer2 timeout: led_15, period:250 ms
   [MONITOR] 2026-04-13 17:55:28.009 [PWM_TIMER_DEMO-I] timer1 timeout: led_14, period:2000 ms
```