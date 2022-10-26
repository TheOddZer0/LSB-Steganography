# LSB-Steganography

This project is a simple implementation of *Least Significant Bit* (LSB) Steganography in C. For more explanation about the algorithm itself, see [DESIGN.md](https://github.com/TheOddZer0/LSB-Steganography/blob/main/DESIGN.md). For just the implementation of the algorithm without the file handling and command line stuff, see [algo.c](https://github.com/TheOddZer0/LSB-Steganography/blob/main/algo.c).

## Building

The algorithm ([algo.c](https://github.com/TheOddZer0/LSB-Steganography/blob/main/algo.c)) is freestanding so any mainstream compiler can compile it.
The driver code ([main.c](https://github.com/TheOddZer0/LSB-Steganography/blob/main/main.c)) uses some C standard library calls but nothing OS-specific, so it's fairly cross-platform.

## Building manually

If you don't have CMake installed, or don't want to use it. Just compile `algo.c` and `main.c` together and then link them, it will be fine.

## Building using CMake

CMake was preferred during development. all you need to do is to run:

```sh
cd build
cmake ../
```

And the build files will be available, on Microsoft Windows you probably got some VS stuff. So open the solution file and build it. On Linux or MacOS you possibly get Make files or Ninja files. You can let CMake do the invocation: `cmake --build . --target lsb-stego`.

## Contacting the author

You can always send an email to me. I'm available at `TheOddZer0@protonmail.com`.

## License

This project is licensed under the MIT License, see the [LICENSE](https://github.com/TheOddZer0/LSB-Steganography/blob/main/LICENSE) for more information.
