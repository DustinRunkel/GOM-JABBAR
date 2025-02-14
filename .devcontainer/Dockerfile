FROM ubuntu:latest

RUN apt update &&\
apt install -y gcc-arm-none-eabi cmake python3 git build-essential

WORKDIR /GOM-JABBAR

ENV PICO_TOOLCHAIN_PATH /usr/bin/arm-none-eabi-gcc