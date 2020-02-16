# Affine_Vigenere-Cyphers-Cryptoanalysis
## kit:
* QT - 5.12.6
* C compiler - MinGw 7.3.0 32-bit for C
* C++ compiler - MinGw 7.3.0 32-bit for C++

This is my student work from cryptography course. This is a QT app with UI, which realises simple Affine and Vigenere encryption, and decryption. However the main part is cryptoanalsis functions.
## App functions:
* Affine encryption
* Affine decryption
* Vigenere encryption
* Vigenere decryption
* Affine cryptoanalysis
* Vigenere cryptoanalysis

Afine cryptoanalysis is realysed with symbol frequency analysis, as frequensy doesn't change through encryption.
However, this method does not applicable to Vigenere cyphertext. For the Vigenere cypher we need to get key length, which is done with Kasiski examination, and then with Fridman frequency method we can get the key.
