# Installation
Download the source and compile using
Always compile with the following

$ gcc scexec.c -o scexec -fno-stack-protector -z execstack

OR

$ i586-mingw32msvc-gcc scexec.c -o scexec.exe -fno-stack-protector

Remember to disable ASLR:
$ echo 0 > /proc/sys/kernel/randomize_va_space


# Usage
scexec: Takes shellcode in the format
\x6a\x3b\x58
and runs the hell out of it!

Usage: ./scexec { [options] | "shellcode" }
NOTES:
    - Use double quotes for your shellcode
    - Remember, \x00 is a bad char

Options:
    -i, --input    <filename>    The file containing shellcode
    -h, --help                   Show this message
