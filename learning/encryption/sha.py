import hashlib

# Real-world SHA-256 hashing with hashlib
def sha256_real_world(message):
    # Create a SHA-256 hash object
    sha256_hash = hashlib.sha256()

    # Update the hash object with the bytes of the input message
    sha256_hash.update(message.encode('utf-8'))

    # Get the hexadecimal digest of the hash
    return sha256_hash.hexdigest()

# Example usage
message = "hello world"
hash_result = sha256_real_world(message)
print(f"SHA-256 hash of '{message}': {hash_result}")
    