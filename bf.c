#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct loop {
    int pos;
    struct loop* prev;
} loop_t;

char* getScript(char* target) {
    FILE* scriptFile = fopen(target, "r");
    if (!scriptFile) {
        perror("Couldn't open that file");
        exit(EXIT_FAILURE);
    }

    fseek(scriptFile, 0L, SEEK_END);
    long scriptSize = ftell(scriptFile);    //Number of characters including comments
    rewind(scriptFile);
    char* script = calloc(scriptSize + 1, sizeof(char));    //+1 to make sure there's space for 0

    int c;
    scriptSize = 0;
    while ((c = fgetc(scriptFile)) != EOF) {
        if (c == '-' || c == '+' || c == '<' || c == '>' || c == ',' || c == '.' || c == '[' || c == ']') {
            script[scriptSize] = c;
            scriptSize++;
        }
    }
    script = realloc(script, scriptSize + 1);    //Ends with 0

    fclose(scriptFile);
    return script;
}

void runScript(char* script, int prompt, int lf) {
    char* mem = calloc(30000, sizeof(char));
    unsigned int iPos = 0;
    unsigned int mPos = 0;
    loop_t* currLoop = NULL;
    while (script[iPos]) {
        switch (script[iPos]) {
            case '-':
                mem[mPos]--;
                break;
            case '+':
                mem[mPos]++;
                break;
            case '<':
                mPos = mPos > 0 ? mPos-1 : 29999;
                break;
            case '>':
                mPos = mPos < 29999 ? mPos+1 : 0;
                break;
            case '.':
                putchar(mem[mPos]);
                break;
            case ',':
                if (prompt) printf("Input a character: ");
                int c = getchar();
                if (c != 10) {
                    mem[mPos] = c;
                    while ((c = getchar()) != '\n' && c != EOF);     //Clear buffer
                } else mem[mPos] = lf ? 10 : 0;
                break;
            case ']':
                if (currLoop == NULL) {
                    printf("\nThere's a \"]\" that's missing its \"[\".\n");
                    exit(EXIT_FAILURE);
                } else if (mem[mPos]) {
                    iPos = currLoop->pos;
                } else {
                    loop_t* a = currLoop;
                    currLoop = currLoop->prev;
                    free(a);
                }
                break;
            case '[':
                if (mem[mPos]) {
                    loop_t* newLoop = malloc(sizeof(loop_t));
                    newLoop->prev = currLoop;
                    newLoop->pos = iPos;
                    currLoop = newLoop;
                } else {
                    int open = 0;
                    iPos++;
                    while (script[iPos] != ']' || open != 0) {
                        if (script[iPos] == '[') open++;
                        if (script[iPos] == ']') open--;
                        iPos++;
                    }
                    iPos++;
                }
                break;
        }
        iPos++;
    }
    free(mem);
}

int main(int argc, char* argv[]) {
    //I dont check that argv[1] is valid because getScript already does.
    char* script = getScript(argv[1]);
    int prompt = 0;
    int lf = 0;
    for (int i = 2; i < argc; i++) {
        char* arg = argv[i];
        if (!strcmp(arg, "--prompt") || !strcmp(arg, "-p")) prompt = 1;
        if (!strcmp(arg, "-lf")) lf = 1;
    }
    runScript(script, prompt, lf);
    free(script);
    return 0;
}