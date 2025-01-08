# Table of Contents

1. [Introduction](#introduction)  
2. [Key Modifications](#key-modifications)  
   - [Word Schedule Reduction](#word-schedule-reduction)  
   - [Reduced Number of Rounds](#reduced-number-of-rounds)  
   - [F and K Calculation](#f-and-k-calculation)  
3. [How It Works](#how-it-works)  
   - [Pre-processing (Padding)](#pre-processing-padding)  
   - [Processing Chunks](#processing-chunks)  
   - [Hash Computation](#hash-computation)  
   - [Final Hash Output](#final-hash-output)  
4. [Example Usage](#example-usage)  
5. [References](#references)  
6. [Code](#code)  

---

## [Introduction](#introduction)
The code provided implements a **modified version of the SHA-1 algorithm**. This modification alters the word schedule and reduces the number of processing rounds.

---

## [Key Modifications](#key-modifications)

### [Word Schedule Reduction](#word-schedule-reduction)
- **Standard SHA-1**: Expands the initial 16 words into 80 words using a specific formula involving XOR operations and left rotations.  
- **Modified SHA-1**: Expands the words only up to 40 instead of 80, effectively halving the number of words processed.  


## Introduction

The code provided implements a **modified version of the SHA-1 algorithm**. This modification alters the word schedule and reducing the number of processing rounds.

## Key Modifications

1. **Word Schedule Reduction**:
   - **Standard SHA-1**: Expands the initial 16 words into 80 words using a specific formula involving XOR operations and left rotations.
   - **Modified SHA-1**: Expands the words only up to 40 instead of 80, effectively halving the number of words processed. 

2. **Reduced Number of Rounds**:
   - **Standard SHA-1**: Performs 80 rounds of processing for each 512-bit chunk of the message.
   - **Modified SHA-1**: Limits the processing to 40 rounds.
3. **F and K caluculation**
    - **Standard SHA-1**:
``` python 
pseudo code
for i from 0 to 79
        if 0 ≤ i ≤ 19 then
            f = (b and c) or ((not b) and d)
            k = 0x5A827999
        else if 20 ≤ i ≤ 39
            f = b xor c xor d
            k = 0x6ED9EBA1
        else if 40 ≤ i ≤ 59
            f = (b and c) or (b and d) or (c and d) 
            k = 0x8F1BBCDC
        else if 60 ≤ i ≤ 79
            f = b xor c xor d
            k = 0xCA62C1D6

```
   - **Modified SHA-1**:
   ``` python
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

           
   ```

---

## How It Works

1. **Pre-processing (Padding)**:
   - The input message is first converted to bytes (if it's not already).
   - A single `1` bit (`0x80` in hexadecimal) is appended to the message.
   - Additional `0` bits are added until the message length is congruent to 448 modulo 512 bits. This ensures the message is properly padded for processing.
   - Finally, the original message length (in bits) is appended as a 64-bit big-endian integer.

2. **Processing Chunks**:
   - The padded message is divided into 512-bit (64-byte) chunks.
   - For each chunk:
     - It's broken down into sixteen 32-bit big-endian words.
     - These words are then expanded to forty words using XOR operations and left rotations.

3. **Hash Computation**:
   - Five initial hash values (`h0` to `h4`) are set to specific constants.
   - For each of the 40 rounds:
     - A series of bitwise operations and left rotations are performed to mix the input data.
     - Different logical functions and constants are used depending on the current round.
     - The intermediate hash values are updated accordingly.

4. **Final Hash Output**:
   - After processing all chunks, the final hash values are concatenated and formatted as a 40-character hexadecimal string, representing the 160-bit hash.

---

## Example Usage

```python
if __name__ == "__main__":
    message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    hash_result = sha1_modified(message)
    print("SHA-1 Modified Hash:", hash_result)
```


```
SHA-1 Modified Hash: 4b23a4d233c5128fac41973d060a2388d5da41a8
```
---

## References
[Sha-1 explaination](https://youtu.be/YCf80-8xhGs?si=ibAXteqNRo3-DS8H)
[SHA-1 pseudo-code](https://en.wikipedia.org/wiki/SHA-1#SHA-1_pseudocode)

---

## Code
``` python
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
```