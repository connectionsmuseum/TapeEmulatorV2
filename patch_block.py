#!/usr/bin/env python

import struct
import sys
import os

# Patches are word location, original word, then new word
# Position is 1-indexed from the first real word, preamble not counted.

# Track 1 block 0001
patches = [(269, 0o103024, 0o153520),
           (270, 0o031000, 0o010410)
           ]

def find_patch(word_n, patches):
    return list(filter(lambda x: x[0] == word_n, patches))

if __name__ == '__main__':

    filename = sys.argv[1]
    f = open(filename, "rb")

    f_out = open(filename.replace(".bin", "_patched.bin"), "wb")

    word_n = 0
    while True:
        input_word = f.read(2)
        if(len(input_word) == 0):
            break
        word_n += 1

        input_value, = struct.unpack('>H', input_word)

        patch = find_patch(word_n, patches)
        if(len(patch) == 0):
            f_out.write(struct.pack('>H', input_value))
            continue
        else:

            patch_n, patch_original, patch_new = patch[0]
            if(patch_original != input_value):
                print(f"Value {input_value:06o} at patch position {patch_n:d} does not match value {patch_original:06o}")
                sys.exit(1)
            else:
                print(f"Patch value {input_value:06o} to {patch_new:06o}")
                f_out.write(struct.pack('>H', patch_new))

    f_out.close()





