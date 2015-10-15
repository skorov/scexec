#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char testcode[] = 
"\x48\x31\xc9\x48\x81\xe9\xfa\xff\xff\xff\x48\x8d\x05\xef\xff"
"\xff\xff\x48\xbb\xfa\x50\x20\xb7\x6c\xdf\x83\xfc\x48\x31\x58"
"\x27\x48\x2d\xf8\xff\xff\xff\xe2\xf4\x90\x6b\x78\x2e\x24\x64"
"\xac\x9e\x93\x3e\x0f\xc4\x04\xdf\xd0\xb4\x73\xb7\x48\x9a\x0f"
"\xdf\x83\xb4\x73\xb6\x72\x5f\x65\xdf\x83\xfc\x93\x36\x43\xd8"
"\x02\xb9\xea\x9b\xfa\x06\x77\xff\xe5\x39\x8c\xf9";


void printHelp(char *prog) {
    printf("scexec: Takes shellcode in the format\n\\x6a\\x3b\\x58\nand runs the hell out of it!\n\n");
    printf("Usage: %s { [options] | \"shellcode\" }\n"
           "NOTES:\n    - Use double quotes for your shellcode\n"
           "    - Remember, \\x00 is a bad char\n", prog);

    printf("\nOptions:\n"
           "    -i, --input    <filename>    The file containing shellcode\n"
           "    -h, --help                   Show this message\n"
    );
}

unsigned char* convert(unsigned char* in) {
    unsigned char* out = malloc(2048);

    int len = strlen(in);
    int i, j=0;
    char hex[3]; hex[2] = '\0';
    for (i = 0; i< len; i++) {
        if (in[i] != '\\' && in[i] != 'x') {
            hex[0] = in[i++];
            hex[1] = in[i++];
            out[j++] = strtol(hex, NULL, 16);
        }
    }
    return out;
}

int main(int argc, unsigned char **argv) {

    unsigned char *code = NULL;
    unsigned char *filename = NULL;
    FILE *fp;

    // The housekeeping
    if (argc < 2) {
        printHelp(argv[0]);
        return 0;
    }

    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printHelp(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--input") == 0 || strcmp(argv[i], "-i") == 0) {
            if (argc > i) {
                filename = argv[i+1];
            } else {
                printHelp(argv[0]);
                return 0;
            }
        }
    }

    if (argv[1][0] == '\\' && argv[1][1] == 'x') {
        code = convert(argv[1]);
    }

    // The prep

    if (code == NULL && filename != NULL) {
        fp = fopen(filename, "rt");
        unsigned char *buffer = malloc(2048);

        char c;
        i = 0;
        while ((c = fgetc(fp)) != EOF) {
            if (c != '\n' && c != '\r' && c != '"' && c != ';')
                buffer[i++] = c;
        }
        code = convert(buffer);
        free(buffer);
        fclose(fp);
    }

    if (code == NULL) {
        printf("[+] ERROR: No shellcode to work off.\n");
        free(code);
        return 0;
    }

    printf("Shellcode Length: %d\n", strlen(code));

    // The magic
    int (*ret)() = (int(*)())code;
    ret();

    free(code);
    return 0;
}
