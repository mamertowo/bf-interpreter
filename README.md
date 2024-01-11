# Brainfuck interpreter
A brainfuck interpreter made in C (previously C++). Read a bf script from a text file and output it to the terminal.

## Installation
Download the appropriate binary file from the latest release.

## Usage
```bash
./bf PATH [-p, --prompt] [-lf]
```
The program should be a .bf file for clarity, but any normal text file will work.

## Arguments
`PATH` is the path where the brainfuck script is located. \
You can use `-p` or `--prompt` to show a prompt for input when you use `,`. Not ideal. \
`-lf` makes it so that line feeds (`\n`) get read as 10 instead of 0.
