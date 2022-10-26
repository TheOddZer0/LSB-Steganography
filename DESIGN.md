# Design notes on this project

## What is LSB

LSB (Least Significant Bit) in simple language means the last bit in a byte. Two's complement representation of integral value `254` is `11111110`, which means the 8th bit (the one with the value of `0`) is the least significant bit.

## What is LSB steganography

LSB steganography is a steganography method which was originally designed to be used with images. In this method, two input sources are required, one the message to encode and second the holder. The message will be separated into bits, and each bit will replace the LSB of each byte in the holder.

Considering this the holder file:

```text
11111110 11111110 11111110 11111110 11111110 11111110 11111110 11111110
```

And `S` the message (0x53 in ASCII, Two's complement: `01010011`). The result would be:

```text
11111110 11111111 11111110 11111111 11111110 11111110 11111111 11111111
```

Representing the operation would be:

```text
11111110 11111111 11111110 11111111 11111110 11111110 11111111 11111111
       |        |        |        |        |        |        |        |
       |        |        |        ---|  |---        |        |        |
       |        |        ----------| |  | |----------        |        |
       |        -----------------| | |  | | |-----------------        |
       ------------------------0 1 0 1  0 0 1 1------------------------
```

## Why LSB

Because it does the least impact! In Two's complement, difference of `11111110` (254) and `11111111` (255) is just 1. But difference of `11111110` (254) and `01111110` (126) is 128. As the original design was supposed to be used on images, stealthiness of the steganography was important, the difference of #000000 (Assuming RGB, one byte for each) and #010101 is hardly noticeable (if noticeable at all) to human eyes.

## Problems of LSB steganography

Three of the biggest problems of LSB steganography are:

### It requires a lot of space

As each bit will be used for a byte in the holder, the holder has to be at least 8 times bigger than the message. The Linux x86_64 Meterpreter stager is around 250 bytes. Which means a holder file of at least 2000 bytes is required!

**Solution**: A compression algorithm can be used to decrease the amount of bytes required for big files. Please see the end of the file for comprehension of different compression algorithms.

### Lossy compressions will likely mess with the message

Lossy compressions result into loss of bytes of the image, for each byte lost, a bit is going to be lost. This means not only it can make the whole original message unrecoverable, it may the message completely unrecoverable as there is no guarantee which bytes are going to be removed.

**Solution**: There are many effective loseless compression algorithms which can solve this issue.

### The privacy can be easily broken

With knowing the original image, one can find the difference between the original image and the one with a message encoded, knowing the index of the said byte, the attacker can find the length of the encoded message. Knowing the length, programs can easily decode the message.

**Solution 1**: One can use an encryption algorithm to hide the original message.

**Solution 2**: The result can be hardened (I will explain this in just a few lines), which makes guessing the original length using the original image impossible.

Please note that these two solutions are not mutually exclusive and used together can be very effective.

## What do you mean by `hardening`

Here are the way we can harden the result using modifications/additions to the original method:

**First**, The original design starts from the beginning of the message, linear access. One can randomly choose in the bits of the original message and then produce a map, allowing the decoder to find the order bytes were changed.

**Second**, We can add an offset parameter, so instead of the start from the beginning of the holder file, we can start from the specified byte.

**Third**, we can write a random bit as the LSB of each byte in the rest of the image which would be originally untouched, making recovering the original length of the message impossible. As we are deciding if we should write a `1` or not, we can get random integers from a CSPRNG (`/dev/urandom` on Linux machines) so guessing which bytes were used in this process almost impossible, if the fetched integer was odd, we write a `1` else we write a `0`.

To keep the algorithm untouched, we decided to use none.

## Comprehension of different compression algorithms

Test files were created using `/dev/urandom` on my Linux machine (`dd bs=<COUNT> count=1 if=/dev/urandom`, coreutils v8.32-4).

| File size (in bytes) | LZMA | XZ   | GZIP | BZIP2 |
|----------------------|------|------|------|-------|
| 250                  | 286  | 312  | 283  | 407   |
| 500                  | 539  | 560  | 533  | 665   |
| 1000                 | 1044 | 1060 | 1033 | 1299  |
| 2000                 | 2054 | 2060 | 2033 | 2410  |
| 4000                 | 4079 | 4060 | 4033 | 4495  |
| 8000                 | 8119 | 8060 | 8033 | 8501  |

Note: All compressions were done using default settings.

By these tests. None turn out to be effective or even usable for decreasing the size for our specific scenario, which calls for using compression algorithms optimized for small files.

