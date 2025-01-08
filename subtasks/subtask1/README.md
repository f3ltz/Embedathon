### Table of Contents

1. [Task 1](#task-1)
   - [Deciphered Paragraph](#deciphered-paragraph)
2. [Task 2](#task-2)
   - [Introduction](#introduction)
   - [Key Modifications](#key-modifications)
     - [Word Schedule Reduction](#1-word-schedule-reduction)
     - [Reduced Number of Rounds](#2-reduced-number-of-rounds)
     - [F and K Calculation](#3-f-and-k-calculation)
   - [How It Works](#how-it-works)
     - [Pre-processing (Padding)](#pre-processing-padding)
     - [Processing Chunks](#processing-chunks)
     - [Hash Computation](#hash-computation)
     - [Final Hash Output](#final-hash-output)
   - [Example Usage](#example-usage)
   - [References](#references)
   - [Code](#code)

---

### Task 1

#### Deciphered Paragraph

The **W**ave cras**H**ed **And** hi**T** th**E** sandcastle head-on. the sandcastle began to melt under the wa**V**es forc**E** and as the wave **R**eceded, half the sandcastle was gone. the next wave hit, not quite as strong, but still managed to cover the remains of the sandcastle and take more of it awa**Y**. the third wave, a big **O**ne, crashed over the sandcastle completely covering and eng**U**lfing it. when it receded, there w**A**s no t**R**ac**E** the sandcastle ever existed and hours of hard work disappeared forever.  
he couldn't remem**B**er exactly wh**E**re he h**A**d read it, but he was sure that he had. the fact that she didn't believe him was quite frustratin**G** as he began t**O** search the internet t**O** fin**D** the article. it wasn't as if it was s**O**methi**N**g that seemed impossibl**E**. yet she insisted on always seeing the source whenever he stated a fact.

> WHATEVERYOUAREBEAGOODONE → **WHAT EVER YOU ARE BE A GOOD ONE**

---

### Task 2

#### Introduction

The code provided implements a **modified version of the SHA-1 algorithm**. This modification alters the word schedule and reduces the number of processing rounds.

---

#### Key Modifications

##### 1. **Word Schedule Reduction**
- **Standard SHA-1**: Expands the initial 16 words into 80 words using a specific formula involving XOR operations and left rotations.  
- **Modified SHA-1**: Expands the words only up to 40 instead of 80, effectively halving the number of words processed.

##### 2. **Reduced Number of Rounds**
- **Standard SHA-1**: Performs 80 rounds of processing for each 512-bit chunk of the message.
- **Modified SHA-1**: Limits the processing to 40 rounds.

##### 3. **F and K Calculation**
- **Standard SHA-1**:
    ```python
    for i from 0 to 79:
        if 0 ≤ i ≤ 19:
            f = (b and c) or ((not b) and d)
            k = 0x5A827999
        elif 20 ≤ i ≤ 39:
            f = b xor c xor d
            k = 0x6ED9EBA1
        elif 40 ≤ i ≤ 59:
            f = (b and c) or (b and d) or (c and d) 
            k = 0x8F1BBCDC
        elif 60 ≤ i ≤ 79:
            f = b xor c xor d
            k = 0xCA62C1D6
    ```
- **Modified SHA-1**:
    ```python
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

#### How It Works

##### Pre-processing (Padding)
   - Converts the input message to bytes (if not already).
   - Appends a single `1` bit (`0x80` in hexadecimal) to the message.
   - Adds `0` bits until the message length is congruent to 448 modulo 512 bits.
   - Appends the original message length (in bits) as a 64-bit big-endian integer.

##### Processing Chunks
   - Divides the padded message into 512-bit (64-byte) chunks.
   - For each chunk:
     - Breaks it into sixteen 32-bit big-endian words.
     - Expands the words to forty using XOR operations and left rotations.

##### Hash Computation
   - Initializes five hash values (`h0` to `h4`) with specific constants.
   - Performs 40 rounds of processing:
     - Uses bitwise operations, left rotations, logical functions, and constants to mix input data.
     - Updates intermediate hash values.

##### Final Hash Output
   - Concatenates final hash values into a 40-character hexadecimal string, representing the 160-bit hash.

---

#### Example Usage

```python
if __name__ == "__main__":
    message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    hash_result = sha1_modified(message)
    print("SHA-1 Modified Hash:", hash_result)
```

**Output**:
```
SHA-1 Modified Hash: 4b23a4d233c5128fac41973d060a2388d5da41a8
```

---

#### References
1. [SHA-1 Explanation](https://youtu.be/YCf80-8xhGs?si=ibAXteqNRo3-DS8H)  
2. [SHA-1 Pseudocode](https://en.wikipedia.org/wiki/SHA-1#SHA-1_pseudocode)

---

#### Code

```python
"""
Candy Queen SHA-1 Modified Hash Function Submission

This module provides a modified version of the SHA-1 hash function with a word schedule of 40 words.
"""

import struct

def left_rotate(n, b):
    """
    Left rotate a 32-bit integer n by b bits.
    """
    return ((n << b) | (n >> (32 - b))) & 0xFFFFFFFF

def sha1_modified(message):
    """
    Compute the SHA-1 hash of a message with a modified word schedule of 40 words.
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
    print("SHA-1 Modified Hash:",

 hash_result)
```