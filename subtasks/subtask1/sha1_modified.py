"""
Candy Queen SHA-1 Modified Hash Function submission


This module provides a modified version of the SHA-1 hash function with a word schedule of 40 words.
"""

import struct

def left_rotate(n, b):
    """
    Left rotate a 32-bit integer n by b bits.

    Args:
        n (int): The integer to rotate.
        b (int): The number of bits to rotate by.

    Returns:
        int: The result of left rotating n by b bits.
    """
    return ((n << b) | (n >> (32 - b))) & 0xFFFFFFFF

def sha1_modified(message):
    """
    Compute the SHA-1 hash of a message with a modified word schedule of 40 words.

    This function processes the input message, applies padding, and computes the hash using a 
    40-round version of the SHA-1 algorithm.

    Args:
        message (str or bytes): The input message to hash.

    Returns:
        str: The resulting SHA-1 hash as a hexadecimal string.
    """
    if isinstance(message, str):
        message = message.encode('utf-8')
    
    original_byte_len = len(message)
    original_bit_len = original_byte_len * 8

    message += b'\x80'

    while (len(message) * 8) % 512 != 448:
        message += b'\x00'

    message += struct.pack('>Q', original_bit_len)

    chunks = [message[i:i+64] for i in range(0, len(message), 64)]

    h0 = 0x67452301
    h1 = 0xEFCDAB89
    h2 = 0x98BADCFE
    h3 = 0x10325476
    h4 = 0xC3D2E1F0

    for chunk in chunks:
        W = list(struct.unpack('>16I', chunk))

        for i in range(16, 40):
            val = W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]
            W.append(left_rotate(val, 1))

        a = h0
        b = h1
        c = h2
        d = h3
        e = h4

        for i in range(40):
            if 0 <= i <= 19:
                f = (b & c) | (b & d) | (c & d)
                k = 0x8F1BBCDC
            elif 20 <= i <= 39:
                f = b ^ c ^ d
                k = 0xCA62C1D6
            else:
                f = 0
                k = 0

            temp = (left_rotate(a, 5) + f + e + k + W[i]) & 0xFFFFFFFF
            e = d
            d = c
            c = left_rotate(b, 30)
            b = a
            a = temp

        h0 = (h0 + a) & 0xFFFFFFFF
        h1 = (h1 + b) & 0xFFFFFFFF
        h2 = (h2 + c) & 0xFFFFFFFF
        h3 = (h3 + d) & 0xFFFFFFFF
        h4 = (h4 + e) & 0xFFFFFFFF

    return '{:08x}{:08x}{:08x}{:08x}{:08x}'.format(h0, h1, h2, h3, h4)


if __name__ == "__main__":
    message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    hash_result = sha1_modified(message)
    print("SHA-1 Modified Hash:", hash_result)