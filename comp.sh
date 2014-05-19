#!/bin/bash

TFTPY=../tftpy

echo "[MAS]" $1 -o $TFTPY/root/mpu.bin
./binary/mpu_assembler/mpu_assembler.elf -o $TFTPY/root/mpu.bin $1
