🔐 STM32 Fingerprint + Keypad Smart Door Lock

MCU: Nucleo-F411RE
Features: R305 Fingerprint + 4×4 Keypad + Servo/Motor Lock + Flash Storage

📌 Overview

This project implements a complete smart door-lock system using an STM32F4 microcontroller.
It combines biometric security (R305 fingerprint sensor) with a 4×4 matrix keypad and a motor/servo-based locking mechanism. Users can unlock using either a fingerprint or a PIN code. PIN and settings are stored in internal Flash memory, so they persist across resets.

The entire firmware is written in Embedded C, structured with drivers for fingerprint, keypad, motor control, and non-volatile storage. The project is fully compatible with STM32CubeIDE + STM32CubeMX.

🚀 Features

✔ Fingerprint unlock (R305 optical sensor)

✔ PIN unlock using 4×4 keypad

✔ Register / verify fingerprints through UART

✔ Flash-based PIN storage (no external memory needed)

✔ Servo/Motor lock control (GPIO/PWM)

✔ Status LEDs (LOCKED, UNLOCKED, ERROR)

✔ Modular driver-based architecture

✔ Clean and production-ready code structure

✔ Working CubeMX configuration included

✔ Ready to compile in STM32CubeIDE

🧩 Hardware Components
Component	Model
MCU	STM32 Nucleo-F411RE
Fingerprint Sensor	R305 / ZFM-20 UART
Keypad	4×4 Matrix keypad
Lock Mechanism	SG90 Servo OR DC motor via L293D
Storage	Internal Flash sector (no external EEPROM)
Power	5V for R305 & servo, 3.3V for STM32


STM32F411RE                R305 Fingerprint
----------------------------------------------
PA9  (UART1_TX)   ----->   TX
PA10 (UART1_RX)   <-----   RX
5V                 ----->   VCC
GND                -----    GND

STM32F411RE                4x4 Keypad
----------------------------------------------
PB0..PB3   ---> Row0..Row3
PB4..PB7   ---> Col0..Col3

STM32F411RE                Servo / Motor
----------------------------------------------
PA8 (TIM1_CH1 PWM) ---> Servo Signal
5V -----------------> Servo VCC
GND ----------------> Servo GND


/Core
 ├── Src/main.c
 ├── Src/fingerprint.c      → UART R305 protocol
 ├── Src/keypad.c           → 4×4 keypad scanner
 ├── Src/motor.c            → Servo/Motor driver
 ├── Src/storage.c          → Flash-based PIN store
 ├── Inc/*.h
 └── ...
Drivers Included

🟢 fingerprint.c/h → R305 packet protocol (image → char → model → match)

🟢 keypad.c/h → Non-blocking keypad scanner

🟢 motor.c/h → Servo (PWM) or DC motor (GPIO/L293D)

🟢 storage.c/h → Flash SECTOR-7 write/read

Everything is fully integrated in main.c.

🛠 STM32CubeMX Configuration Summary

UART3 @ 57600 baud for fingerprint

TIM1_CH1 PWM for servo control

GPIO Input/Output for keypad & LEDs

Flash Sector 7 used for PIN storage

Detailed CubeMX pinout is also included in the repository under /Docs/.

▶️ How to Use

Power the R305 sensor (5V recommended)

Connect UART3 → PA9/PA10

Flash the firmware (STM32CubeIDE)

Default PIN = 1234

Press * → Enter Admin Mode

Add fingerprint → stores in ID slots

Unlock using fingerprint or PIN

📷 Demo & Screenshots

(Add pictures or GIFs here showing keypad, lock mechanism, STM32 board, etc.)

🧪 Future Improvements

OLED display for instructions

Web/App remote unlock

FreeRTOS-based scheduling

Logging access history in SD card

📄 License

MIT License – free to use and modify.

🤝 Contributing

Pull requests and feature suggestions are welcome!
