# Vigenere Cipher Implementation

This program implements the Vigenere cipher, providing functions for encryption, decryption, and cryptanalysis. It uses the Index of Coincidence (IC) to analyze the ciphertext and guess the key length. The program supports reading and writing from files and allows users to specify input and output file names.

## Features

1. **Encryption**: Encrypts plaintext using the Vigenere cipher with a provided key.
2. **Decryption**: Decrypts ciphertext using the Vigenere cipher with a provided key.
3. **Cryptanalysis**: Attempts to determine the key length and recover the key from the ciphertext using frequency analysis.

## How It Works

- **Encryption**: The Vigenere cipher encrypts the plaintext by shifting each letter according to a repeating key sequence.
- **Decryption**: The Vigenere cipher decrypts the ciphertext by reversing the shifting process using the same key.
- **Cryptanalysis**: The program calculates the Index of Coincidence (IC) for different key lengths to guess the most likely key length. Once the key length is determined, the key is recovered using frequency analysis.

## Usage

### Compile the Code

To compile the program, use the following command:

gcc vigenere_cipher.c -o vigenere_cipher

### Run the Program

You will be prompted to select an operation:

1. **Encryption**
2. **Decryption**
3. **Cryptanalysis**

#### 1. Encryption

- Default input file: `plaintext.txt`
- Default output file: `ciphertext.txt`
- You can change the file names if needed.
- Enter the key to be used for encryption.

Example:

./vigenere_cipher
Choose operation:
1. Encryption
2. Decryption
3. Cryptanalysis
Enter your choice (1/2/3): 1
Default input file is plaintext.txt. Do you want to change it? (y/n): n
Enter the key: SECRET
Encryption completed. Output written to ciphertext.txt

#### 2. Decryption

- Default input file: `ciphertext.txt`
- Default output file: `decrypted.txt`
- You can change the file names if needed.
- Enter the key to be used for decryption.

Example:

./vigenere_cipher
Choose operation:
1. Encryption
2. Decryption
3. Cryptanalysis
Enter your choice (1/2/3): 2
Default input file is ciphertext.txt. Do you want to change it? (y/n): n
Enter the key: SECRET
Decryption completed. Output written to decrypted.txt

#### 3. Cryptanalysis

- Default input file: `ciphertext.txt`
- The program analyzes the ciphertext to guess the key length and recover the key.

Example:

./vigenere_cipher
Choose operation:
1. Encryption
2. Decryption
3. Cryptanalysis
Enter your choice (1/2/3): 3
Default input file is ciphertext.txt. Do you want to change it? (y/n): n
Possible Guess for Key length: 3 Avg IC: 0.068
Best matching key length: 3 with Avg IC: 0.068
The estimated key is: KEY


## Key Functions

- `Vig_Enc(char *plaintext, char *key)`: Encrypts plaintext using the provided key.
- `Vig_Dec(char *ciphertext, char *key)`: Decrypts ciphertext using the provided key.
- `VigAnalysis(char *ciphertext)`: Analyzes the ciphertext to estimate the key length.
- `recoverVigenereKey(char *ciphertext, int keyLength)`: Recovers the Vigenere key using frequency analysis.

## Frequency Analysis

The program uses frequency analysis based on the Index of Coincidence (IC) and Chi-squared tests to match English letter frequencies. This helps in determining the most probable key length and characters.

## Limitations

- The program assumes the input text contains only English alphabetic characters.
- Key recovery accuracy depends on the length and characteristics of the ciphertext.

## License

This project is open-source and free to use for educational purposes.

Feel free to adjust any details in the README file to better fit your needs or to provide more specific instructions based on your environment!
