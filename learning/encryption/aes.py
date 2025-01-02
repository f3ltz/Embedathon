from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding
from cryptography.hazmat.backends import default_backend
import os

def aes_encrypt(plaintext, key):
    # Ensure the key is 32 bytes for AES-256
    key = key.ljust(32)[:32].encode('utf-8')

    # Generate a random 16-byte IV (Initialization Vector)
    iv = os.urandom(16)

    # Create the cipher object
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=default_backend())
    encryptor = cipher.encryptor()

    # Pad the plaintext to a multiple of the block size (16 bytes for AES)
    padder = padding.PKCS7(algorithms.AES.block_size).padder()
    padded_data = padder.update(plaintext.encode('utf-8')) + padder.finalize()

    # Encrypt the padded plaintext
    ciphertext = encryptor.update(padded_data) + encryptor.finalize()

    return iv + ciphertext  # Return IV and ciphertext

def aes_decrypt(ciphertext, key):
    # Ensure the key is 32 bytes for AES-256
    key = key.ljust(32)[:32].encode('utf-8')

    # Split IV and ciphertext
    iv = ciphertext[:16]
    actual_ciphertext = ciphertext[16:]

    # Create the cipher object
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=default_backend())
    decryptor = cipher.decryptor()

    # Decrypt the ciphertext
    padded_plaintext = decryptor.update(actual_ciphertext) + decryptor.finalize()

    # Remove padding
    unpadder = padding.PKCS7(algorithms.AES.block_size).unpadder()
    plaintext = unpadder.update(padded_plaintext) + unpadder.finalize()

    return plaintext.decode('utf-8')


key = "my_secure_key"
plaintext = "Hello, AES encryption!"

# Encrypt
ciphertext = aes_encrypt(plaintext, key)
print(f"Ciphertext (hex): {ciphertext.hex()}")

# Decrypt
decrypted_message = aes_decrypt(ciphertext, key)
print(f"Decrypted message: {decrypted_message}")
