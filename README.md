# Brainfuck interpreter
A brainfuck interpreter made with C++. It can read a bf program from a text file and outputs it to the terminal.

## Installation
Download `bf.exe` from the latest release.

## Usage
Open a terminal where you saved the .exe and run it.
```bash
./bf.exe [PATH] [--input-prompt]
```
The program should be a .bf file for clarity, but any normal text file will work.

## Arguments
`PATH` is the path where the brainfuck program is located. If you don't provide a path here you will be prompted to give one once you run the interpreter. If you do give a path here, it HAS to be the first argument.\
You can use `--input-prompt` to show a prompt for input when you use `,`. Not ideal.
