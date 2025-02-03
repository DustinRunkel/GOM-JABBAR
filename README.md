# GOM-JABBAR
Generic builit-in test manager for embedded projects. Demo'd here on the RP2040. 

# Vision

Often when developing embedded products, companies have little choice of common frameworks to support built-in testing. Much of these frameworks must be purpose made, but this project, GOM-JABBAR, aims to commonize aspects of built-in testing, and deliver a framework as a starting point for companies that need to rapidly test integrity of embedded devices, before shipping them to the consumer. This project is the embedded demonstration for the GOM-JABBAR application image. The board selected for this project is an RP2040(W). This board offers a variety of features, you can read about them [here](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf). Also, this project demonstrates the usage of the test manager with one of the most popular embedded kernels, [FreeRTOS](https://www.freertos.org/Documentation/00-Overview). On the other side, is a GUI. The GUI receives a test enumeration message from the device. The GUI offers a low-code/no-code solution to building a test schedule. 

# Getting Started

1. [Set up your build environment](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) (make sure to install the vscode extension)
2. Pull down the kernel sub-module with `git submodule update --init --recursive` and `git submodule update --remote --recursive`
3. configure the project with `cmake --build build`
4. build the project with `cd build` and `make`

# Resources
[Raspberry Pi C++ SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)
[FreeRTOS Kernel Documentation](https://www.freertos.org/Documentation/00-Overview)
[Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
