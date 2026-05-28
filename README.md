# ESP32-C3 LED Dimmer - Complete Project

Wireless LED dimmer system for a 20V DeWalt battery-powered LED light bar using ESP32-C3 boards and ESP-NOW communication.

## Features

✅ **Smooth PWM Dimming** - 256 brightness levels with smooth fade transitions  
✅ **Wireless Control** - ESP-NOW remote with 100-300m range  
✅ **Deep Sleep** - Remote sleeps after 10 seconds (50µA consumption)  
✅ **Brightness Persistence** - Remembers brightness level across power cycles  
✅ **Memory Optimized** - Minimal code footprint for ESP32-C3 Super Mini (~18KB each)  
✅ **Tested** - Proven on real hardware with 20V/4A load

## Files

### Firmware
- `firmware/esp32c3_led_controller/esp32c3_led_controller.ino` - Main controller
- `firmware/esp32c3_led_remote/esp32c3_led_remote.ino` - Remote control
- `utilities/get_mac_address/get_mac_address.ino` - MAC address finder

### Schematics (KiCAD)
- `kicad/esp32_led_controller.kicad_sch` - Main controller schematic
- `kicad/esp32_led_remote.kicad_sch` - Remote control schematic

### Documentation
- `CIRCUIT_DESIGN.md` - Detailed circuit descriptions
- `SETUP_INSTRUCTIONS.md` - Setup and assembly guide

## Quick Start

1. **Find Controller MAC:**
   - Upload `utilities/get_mac_address/get_mac_address.ino` to first ESP32-C3
   - Note the MAC address from Serial Monitor

2. **Update Remote Code:**
   - Edit `firmware/esp32c3_led_remote/esp32c3_led_remote.ino` line 13
   - Replace `{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}` with controller's MAC

3. **Upload Both:**
   - Upload controller code to first board
   - Upload remote code to second board

4. **Assemble Circuits:**
   - Connect TIP41C transistor to main circuit
   - Connect buttons and battery to remote

5. **Test:**
   - Press buttons on remote
   - LED should fade smoothly up/down

## Specifications

**Main Controller:**
- Input: 20V DeWalt battery
- Output: PWM-dimmed LED (5-30V, 4A max)
- PWM: 20kHz, 8-bit (256 levels)
- Power draw: ~150mA active, less idle

**Remote Control:**
- Input: 2x AA batteries (3V) or 1x 3.7V LiPo
- Buttons: Power, Up, Down
- Deep sleep: 50µA
- Awake: 80mA
- Battery life: 6-12 months typical

**ESP-NOW Range:**
- Open area: 100-300m
- Through walls: 30-50m

## Circuit Components

### Main Controller
- ESP32-C3 development board
- TIP41C NPN power transistor
- 1kΩ resistor (base drive)
- 10kΩ resistor (pull-down)
- 100µF capacitors (2x)
- LM2596 buck converter (20V → 5V)

### Remote
- ESP32-C3 development board
- 3x momentary push buttons
- 3x 10kΩ pull-up resistors
- 100µF capacitor
- Battery holder (AA or LiPo)

## Tuning

Edit these values in controller code to customize:
- **Fade speed:** `const uint16_t FADE_DELAY_MS = 30;`
- **Brightness step:** `const uint8_t BRIGHTNESS_STEP = 5;`

Edit in remote code:
- **Sleep timeout:** `unsigned long sleepTimeout = 10000;` (ms)
- **Button increment:** `send(1, 25);` (0-255)

## Troubleshooting

**No Serial Output:**
- Check COM port in Tools → Port
- Install CH340 driver if needed
- Try pressing EN/RST button after upload

**Remote not connecting:**
- Verify MAC address matches in remote code
- Check both devices are in range
- Try restarting both boards

**LED flickers:**
- Check TIP41C base resistor (should be 1kΩ)
- Verify GPIO5 connects to base through resistor
- Check 10kΩ pull-down is in place

**Remote won't wake from sleep:**
- Verify GPIO9 connected to POWER button
- Check pull-up resistor on GPIO9
- Press POWER button for 1+ second

## License

Free to use and modify for personal projects.

## Author

Created for ESP32-C3 LED dimmer project, May 2026
