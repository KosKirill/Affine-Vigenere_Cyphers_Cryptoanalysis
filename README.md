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
F function is an SP-net with following parametres:
* All S-blocks are equal to (5,8,1,13,10,3,4,2,14,15,12,7,6,0,9,11).
* P block is (11i + 5) mod 16.


