# GOM-JABBAR
[![CMake on a single platform](https://github.com/DustinRunkel/GOM-JABBAR/actions/workflows/ubuntu-latest.yml/badge.svg)](https://github.com/DustinRunkel/GOM-JABBAR/actions/workflows/ubuntu-latest.yml)

Generic builit-in test manager for embedded projects. Demo'd here on the RP2040. 

# Vision

Often when developing embedded products, companies have little choice of common frameworks to support built-in testing. Much of these frameworks must be purpose made, but this project, GOM-JABBAR, aims to commonize aspects of built-in testing, and deliver a framework as a starting point for companies that need to rapidly test integrity of embedded devices, before shipping them to the consumer. This project is the embedded demonstration for the GOM-JABBAR application image. The board selected for this project is an RP2040(W). This board offers a variety of features, you can read about them [here](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf). Also, this project demonstrates the usage of the test manager with one of the most popular embedded kernels, [FreeRTOS](https://www.freertos.org/Documentation/00-Overview). On the other side, is a GUI. The GUI receives a test enumeration message from the device. The GUI offers a low-code/no-code solution to building a test schedule. 

# Getting Started

1. Pull down the kernel sub-module with `git submodule update --init --recursive` and `git submodule update --remote --recursive`
2. configure the project with `cmake -S . -B build/`
3. build the project with `cmake --build build`

# Flashing your device

When you build the device project, a few binaries are generated. These binaries are provided for various boot modes of the device. We will focus on booting from flash, via a .uf2 file. This is the easiest way to get started, and requires us to not write custom bootloader code, as the nice people at raspberry pi have done this for us. 

1. Connect your device via micro USB to any one of the USB ports on your machine
2. Hold down the BOOTSEL button on the board
3. The device is in mass storage mode, and ready to be flashed
4. Drag and drop the .uf2 file onto the USB device
5. It will take a second to reprogram itself, then issue a power-on-reset
6. The device will boot into the entrypoint `main`

# Reading the com port of your device
Our device will be spewing data to the com port, so this is important. Apparently we are not outputing anything to COM on main yet, so to test this functionality you may need to use [this binary](https://datasheets.raspberrypi.com/soft/hello_world.uf2).

## Linux
1. Use dmesg to find which `/dev` the device was assigned
   ![image](https://github.com/user-attachments/assets/fdde8f19-6f3a-4389-9196-bb58c26b9e0a)
   
   As you can see, mine was assigned /dev/ttyACM0
3. run `screen /dev/ttyACM0 115200` (you may need to sudo this)
4. You should see the following output
   
   ![image](https://github.com/user-attachments/assets/40a63734-34cf-4f4f-8993-675639782bff)
5. crtl A + Z to exit

## Windows
1. Install PuTTy
2. Find which com port the device is on in system/hardware devices
3. Initalize a connection through PuTTy on the com port at baud 115200




# Resources
- [Raspberry Pi C++ SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)
- [FreeRTOS Kernel Documentation](https://www.freertos.org/Documentation/00-Overview)
- [Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
