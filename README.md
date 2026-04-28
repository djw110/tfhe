# Torus-Based FHE in C
## Overview
Efficient fully homomorphic encryption is an important area of modern cryptographic research. General, encrypted computing could revolutionize how we treat data privacy. However, computing on encrypted data is slow, especially if we want to be able to manipulate it like traditional arithmetic or binary logic. Many mockups implementing homomorphic encryption schemes do so in python, for ease of understanding and rapid prototyping. However, the high-level nature of python is obviously poor for performance. I intend to demonstrate a fully homomorphic encryption scheme in C, with memory managed manually to minimize computation complexity.

Several examples exist of attempts at homomorphic encryption, such as Pallier's and ElGamal's partially homomorphic schemes. We will be using a new approach based off of [this paper](https://tches.iacr.org/index.php/TCHES/article/view/9836).

We will be able to:
- Represent bits using cryptographically secure ciphertexts
- Handle homomorphic XOR and NOT gates on these encrypted bits
- Represent strings of 128 packed bits using cryptographically secure ciphertexts
- Handle homomorphic XOR and NOT gates on these strings, allowing for 128 bit-operations with the cost of a single gate

## Learning with Errors: Over the Torus
### What is the Torus?
The Torus is the set of all real numbers modulo 1 $(0,1]$. Thus, all operations that exceed 1 'wrap around' back to the fractional portion, similar to the shape of a Torus. We use the Torus because we can incorporate small noise into our 'bits', and use rounding to determine whether the bit is either 0 or 1 without requiring an exact match. Since bits cannot exceed 1, it makes perfect sense to perform all operations modulo 1.

### Implementing the Torus in C
Modulo operations are expensive. Additionally, floating point real numbers are extremely difficult to work with computationally, and expensive. Fortunately, C handles unsigned integers with an implicit modulus: if a 32-bit integer exceeds $2^{32}-1$, it wraps around to 0, no modulus required. We will therefore "scale up" the Torus, representing elements on the Torus as unsigned 32-bit integers. Thus, an unsigned integer $2^{31}$ would represent 0.5 over the Torus.

### LWE Ciphertexts
To encrypt a bit, we first need to raise it to the Torus space. Our strategy is to add a mask to the bit, which is created by taking the dot product of 2 vectors, and then add a small amount of random noise. The mask is a value over the Torus, and the noise is a very small value over the Torus. To decrypt, we subtract the mask, and then estimate whether the result is closer to "zero" or "one" to account for the small noise. Of course, this means the noise can't be larger than the encoding of our bit!

Thus, when encoding our bit into the torus space, we will assign it the value of 0 if the bit is 0, and 1/4 of the Torus space if the bit is 1, or roughly $2^{30}$. This gives us plenty of room in **either direction** to account for noise. So, if the bit is between 1/8 and 3/8 of the Torus space, it is almost certainly a 1. Otherwise, its a 0.

To encrypt a bit, we need a secret key *s*. This is a string of 128 bits. When encrypting a bit with a secret key, we generate a public vector *a* of 128 random Torus elements. We take the dot product of the vector *s* and *a*, and add it to our Torus encoded bit. Finally, we generate a small amount of random noise, a Torus element, *e*, to yield $b=s*a + m + e$.

To decrypt a bit, we simply use our secret key and compute the dot product $s*a$ and subtract that value from $b$. We then estimate if the resulting Torus element is closer to the bounds of a 1 or a 0.

### RLWE Ciphertexts
Encrypting a string of packed bits uses a similar problem, however it treats *m, s* and *a* as *polynomials.* We treat each vector as a polynomial, with each element at index *i* in the vector as a coefficient with power $x^i$.

To encrypt a string of bits $m$, we generate a random polynomial with Torus-element coefficients *a* and compute $b= a*s + m + e$. Computing $a*s$ involves ring polynomial multiplication. To maximize efficiency, a Fast Fourier Transform should be used, but for simplicity we are using a schoolbook multiplication. Then, we simply add the coefficients of the product with each coefficient of *m*, and to each resulting coefficient we add a small amount of random noise.

To decrypt, we leverage the fact that *s* is binary. We are able to, similarly to the LWE encryption, recompute the product and subtract it from the message polynomial. Then, for each coefficient in the resulting "phase" polynomial, we estimate whether that coefficient lies closer to the 0 or 1 bound.

### Homomorphic addition (XOR)
Homomorphic addition of ciphertexts *c1, c2* is simple. To compute the XOR gate, we simply add each element of *a1, a2* in the respective ciphertexts, and the result is the new *a* stored in *c*. Additionally, we add all of the elements (or element) stored in *b1, b2* and store that in the new *b.* The noise is very small, and only grows linearly. Thus, it will take thousands of operations to affect decryption.

### Negation (NOT)
Negation of a ciphertext *c* is as simple as taking the opposite values $-a$ and $-b$ encoded into the torus. Thanks to implicit modulus, this is super easy to do in C.

## Gentry Sahai Waters
After incorporating RLWE and LWE ciphertexts with XOR and NOT gates, the next logical step is to include AND gates. That involves homomorphic multiplication. But, multiplication involves an explosion of noise. Gentry Sahai Waters proposed a workaround in [their paper](https://eprint.iacr.org/2013/340.pdf).

### The GSW Ciphertext
A GSW is a unique ciphertext in that it is simply a matrix where each row is an RLWE ciphertext, so each element is a coefficient. To encrypt a string of bits *m* into a GSW ciphertext, we again treat *m* as a polynomial. However, we then "shard it", by multiplying it by something called the "Gadget Matrix". This scales the polynomial up and down based on the column of the gadget matrix. What's left is a number of polynomials, each scaled by a column of the gadget matrix. For each of these shared polynomials $m^i$, we encrypt 2 polynomials of all zeroes using RLWE encryption, then we add $m^i$ to one of the zero cipher's *a* polynomial, and the other zero cipher's *b*. Thus, for every sharded polynomial, there will be 2 ciphers in the GSW matrix.

### The External Product (AND ?)
The External Product is an operation that multiplies 2 ciphertexts, one RLWE ciphertext and one GSW ciphertext, and outputs an RLWE ciphertext. In other words, it takes 2 encrypted polynomials, and outputs an encrypted polynomial, without major noise explosion. For an in-depth explanation on how this works, consult the linked article. Essentially, we will *decompose* both the *a* and *b* components of the input RLWE ciphertext into 2 matrices. We then multiply the matching  rows of the GSW matrix and decomposed matrices, by performing polynomial multiplication. We add the resulting polynomial from each pair of rows to the corresponding a or b value in the output ciphertext.

On it's own, this is still not perfect. The external product is used a tool in a larger operation: programmable bootstrapping. This allows for noise to be completely reset while performing *any* arbitrary operation.

## Bootstrapping, continuation
As you may notice, the external product operation is incomplete. The arithmetic and logic is correct, but it is not yet functional for binary decryption. C is exceptional for performance, but it is also quite difficult to debug complex operations like EP with such little room for failure. We plan to completely implement external product for homomorphic multiplication of strings of packed bits. Afterward, we intend to implement programmable bootstrapping, allowing us to develop a full demonstration of complete encrypted computation.

## Running the Scheme
The executable compiles the framework, and runs several unit tests. Currently it tests the encryption and decryption of random bits. It tests the homomorphic XOR gates of randomly generated bits. It then tests the correct encryption and decryption of randomly generated polynomials, and performing XOR and NOT gates on the encrypted polynomials.