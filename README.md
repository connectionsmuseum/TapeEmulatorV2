
3ESS TapeEmulator v2.

Some useful gdb commands:
```
docker run --rm  -it -v `pwd`:/build ctslater/docker-gcc-arm-none-eabi  bash
arm-none-eabi-gdb gcc/AtmelStart.elf
target extended-remote docker.for.mac.localhost:2331
load
monitor reset
```
