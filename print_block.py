#!/usr/bin/env python

import struct
import sys
import os

if __name__ == '__main__':
    # filename = "A/0001.bin"

    filename = sys.argv[1]
    f = open(filename, "rb")

    read_len = 50
    print(filename)

    # Read in big-endian bytes
    while True:
        data = f.read(2*read_len)
        if(len(data) == 0):
            break
        successful_len = len(data)//2

        preamble = struct.unpack(f'>{successful_len}H', data)

        for word in preamble:
            # Reverse the entire word
            binary_word = "{:016b}".format(word) # [::-1]
            binary = (" ".join((binary_word[0:4],
                            binary_word[4:8],
                            binary_word[8:12],
                            binary_word[12:])))
            octal = "{:07o}".format(word)
            try:
                print(binary, " | ", octal)
            except BrokenPipeError:
                devnull = os.open(os.devnull, os.O_WRONLY)
                os.dup2(devnull, sys.stdout.fileno())
                sys.exit(0)


