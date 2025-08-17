# Framework-2025-Badge
Frame:Work NYC 2025 Interactive Pixel Badge Kitty
<img width="812" height="905" alt="Screenshot 2025-08-17 at 10 05 04 AM" src="https://github.com/user-attachments/assets/22a9af30-1bf0-447c-8f25-cd9704c998a3" />

<img width="864" height="947" alt="Screenshot 2025-08-17 at 10 05 17 AM" src="https://github.com/user-attachments/assets/08b9def8-2ac1-4ae6-a733-4a70c84a2668" />


# Getting started with the hardware.

The FrameWork badge was designed with the following features

Easy to use RP2040 platform for hacking
Low Resolution 15x7 Pixel Matrix Display
4MB of on board flash storage
3 Buttons and a dedicated power switch
USB Type C for power and uploading software
2x AA battery holder for power on the go
Infrared Sender and Receiver for interactive experiences
Optional expansion headers for hardware hacking


Getting started with the RP2040
The raspberry pi RP2040 chip is super easy to use. Because this design is based on the Raspberry Pi Pico, a wide variety of tutorials and example code exists for this platform. 
<img width="1122" height="738" alt="Screenshot 2025-08-17 at 10 06 19 AM" src="https://github.com/user-attachments/assets/a5e80cf0-fb2f-48fa-8716-27341e9cd766" />

More information about the RP2040 can be found here https://www.raspberrypi.com/documentation/microcontrollers/silicon.html#rp2040

This guide from Adafruit is super helpful for understand how to setup an Arduino style development environment
https://learn.adafruit.com/adafruit-feather-rp2040-pico/arduino-ide-setup

To program the Badge, simply put it in bootloader mode by plugging it into the computer you want to program it with, holding down the 'BOOT' button and then tapping the 'RESET' button. This will cause the RP2040 to start in bootloader mode and enumerate as a USB drive on the host computer. Compiled code can be copied to the device in this state and will run on reboot. Alternatively, various development environments can flash the chip while it's in this state. We recommend starting with Arduino, or if you are more comfortable with C/C++, Visual Studio Code.

# Getting Started With Framework Badge and Arduino

<<ARDUINO EXAMPLE>>

# Framework Badge Pinout

The RP2040 references its pins with an IO number. Your code will use these numbers to refer to the devices you wish to control. For example, if you want to blink the onboard status LED (labelled "Blink!") you would define and output pin on IO#11, as that LED is wired to IO#11.

Your badge has the following IO available:

## LED Matrix Display - IO#4

The LED Matrix is an array of WS2812B-2020 RGB LEDs arranged in a 15 column by 7 row grid. The LED's are addressed from Top Left to Bottom Right in columns going from top to bottom. The first Column is made of LEDs 1-7, the next 8-14, the next 15-21, and so on. The connection of the LEDs **DOES NOT** flow snake-wise, columns are always numbered from top to bottom. 

<img width="959" height="630" alt="Screenshot 2025-08-17 at 9 59 11 AM" src="https://github.com/user-attachments/assets/6e521ff8-1365-49bb-ae09-6281a28303b2" />

When using a large number of these LEDs power draw can get out of control. Each LED channel 'Red' 'Green' or 'Blue' can use up to 12mA of current, for a total of 36mA per LED. With 107 LED's you could theoretically draw up to 3,852mA of current, however the board can really only supply around 2500mA of current reliably. For this reason, 

**NEVER POWER THE LED MATRIX AT FULL BRIGHTNESS IN WHITE**

. Many LED software libraries, such as FastLED have the ability to estimate power usage at runtime to set proper maximum levels that vary dynamically with the number of pixels lit and what color they are. In our normal Badge Application, drawing text on the display, we statistically light up less than 1/4 of the pixels at any given moment, and have the brightness set to less than 25%. This gives us at least 8 hours of run time on batteries and keeps the power usage within reasonable limits, plus the LED's are distractingly too bright at full power anyway. **Failure to properly manage the LED matrix's power can result in hardware damage, quickly depleted batteries, and risk of burns from overheated components, so please be careful**

![IMG_6655](https://github.com/user-attachments/assets/ce696a2b-158f-4d97-b547-57a86cdc6598)


More information about he WS2812B 2020 LEDs used in this project can be found at the data sheet here: 
https://www.lcsc.com/datasheet/C965555.pdf

## Infrared Transmitter - IO#3

The Infrared TX is simply an Infrared LED that you can blink on and off really fast to send data, like a universal remote. (In fact, if you wanted to you could probably program your badge to emulate remote control signals and wreak havoc in a Buffalo Wild Wings, but please use this feature responsibly) This LED emits light in the 940nm range, typical for IR applications. In our laser tag application we're using a 38khz modulation scheme, which is common for remote control applications.


## Buttons - IO#5, 6, and 7

IO Pins can also be configured as inputs to read the state of buttons. The buttons on this badge are normally open and are considered pressed when they are pulled to ground. The buttons are IO#5-7 from Left to Right
<img width="593" height="197" alt="Screenshot 2025-08-17 at 9 57 51 AM" src="https://github.com/user-attachments/assets/12a1ac62-03f2-42ad-a901-8571498a9e8a" />



## Infrared Receiver - IO#27

The Infrared Receiver used is specifically designed for Remote Control applications and is designed specifically for 38khz modulated signals. This means that there is logic in the receiver itself designed to filter out noise at other frequencies, such as fluorescent lighting. 

<img width="671" height="530" alt="Screenshot 2025-08-17 at 9 58 30 AM" src="https://github.com/user-attachments/assets/3fe5fc9d-c625-401b-a709-63712a68a93c" />


More information about the IRM-H638T/TR2 can be found at the data sheet here:
https://www.everlighteurope.com/custom/files/datasheets/DMO-0000008.pdf

## Battery Voltage Monitor - IO#26

The badge has the ability to monitor the voltage of the AA batteries through a voltage divider. The voltage divider scales the voltage by .357 allowing for you to measure the battery voltage in a range of 0-9v (However it should never exceed 3v. 

## Red LED - IO#11

The badge features a simple LED you can blink on and off for testing

<img width="537" height="383" alt="Screenshot 2025-08-17 at 10 02 42 AM" src="https://github.com/user-attachments/assets/34016ebc-02f5-4bed-89e5-9397f1ee9053" />

## USB Port

The onboard USB port can be configured with various libraries, such as Tiny USB, for all sorts of neat applications, from serial interfaces to keyboard emulators, to audio devices. We've used this chip's USB functionality in the past to emulate a USB sound card to provide timecode to windows computers.
<img width="636" height="433" alt="Screenshot 2025-08-17 at 10 03 17 AM" src="https://github.com/user-attachments/assets/aca6f73c-70b2-44f1-b831-0daf690bc1a6" />



# Expansion Port Pinouts

## Expansion "Whisker" Port

Because our badge is kitty cat shaped, we named our expansion ports 'Whiskers' Many dev boards create their own custom standards for expansion boards, so why cant us.

These ports are usually non soldered on production badges, however simple 1x5 header sockets can be soldered in place to allow for modules to stack onto the bottom side of the board. 

The following IO is available:
+5V - For powering LEDs
+3.3V - For powering other devices
GND - Very important
IO#23, 24, and 25 - For connecting to various external devices
IO#28 (ADC#2) - A special analog input pin that can read voltages, useful for sensors

The following image shows how these pins are laid out. The schematic and PCB files included in this repository can show how to build a board that connects to this expansion port.
 <img width="1334" height="767" alt="Screenshot 2025-08-17 at 9 42 59 AM" src="https://github.com/user-attachments/assets/013c7f2c-18e0-4163-838f-4f8988aef018" />


## DVI "Whisker" Port

We chose to add DVI output to our board using an existing open source board called the DVI Sock. This board connects a specific set of IO pins to an HDMI plug in order to allow the RP2040 to drive a display with a 640x480 DVI signal. 
<img width="757" height="935" alt="Screenshot 2025-08-17 at 9 56 45 AM" src="https://github.com/user-attachments/assets/be0a78f2-fa07-4865-805a-9eac2667bf02" />

The DVI Sock can be found here:
https://www.adafruit.com/product/5957

More information about the DVI Sock and its implementation can be found at the original creator's GitHub: 
https://github.com/Wren6991/Pico-DVI-Sock
We take no credit for how this feature works, we are simply passing the needed IO through our board from the RP2040 to the pins for the expansion board to make it easier to experiment with this functionality. 

The DVI expansion uses IO#12-IO#19

## SAO Port

In the badge hacking community, a standard developed for connecting Simple Add Ons to existing hardware badges, using a 4-6 pin socket on the badge with matching pins on the add on. This allows for hackers to dress their badges up with flair that pulls power from the main badge. We are able to re-use some of the whisker header pins in a slightly different shape to allow for a basic 4 pin SAO port, when a header is soldered to the top side of the badge. 
<img width="796" height="935" alt="Screenshot 2025-08-17 at 9 55 27 AM" src="https://github.com/user-attachments/assets/c90a2857-cfbf-4ac1-8e1c-24587efb2883" />



More information about the SAO standard can be found on the creator's page here:
https://hackaday.io/project/52950-shitty-add-ons/log/159806-introducing-the-shitty-add-on-v169bis-standard
We only implement the v0.1 version of this standard with 4 pins, however a full 6 pin SAO will fit mechanically without conflict
