#include <stdio.h>

#define MAX_NESTED_LOOPS 10

void interpretBetween(char **ptr, int loopLevel, char code[], int start, int end) 
{
    char c;
    int loopStarts[MAX_NESTED_LOOPS] = {0};

    for (int i = start; i <= end; i++) {
        c = code[i];
        // Interpret each possible instruction 
        if      (c == '+') ++**ptr;
        else if (c == '-') --**ptr;
        else if (c == '>') ++*ptr;
        else if (c == '<') --*ptr;
        else if (c == '.') putchar(**ptr);
        else if (c == ',') **ptr = getchar();
        else if (c == '[') {
            // In nested loops, there are multiple positions to keep track of.
            // These positions are treated like a stack 
            // where the innermost loop is at the head.
            loopStarts[loopLevel] = i+1;
            loopLevel++;
        }
        else if (c == ']') {
            while (**ptr) {
                interpretBetween(ptr, loopLevel, code, loopStarts[loopLevel-1], i-1);
            }
            loopLevel--;
        }
    }
}

int main(int argc, char const *argv[])
{
    // Initialise memory
    char memory[30000] = {0};
    char* ptr = memory;

    // Create array to contain instructions
    FILE* file = fopen(argv[1], "r");
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    char code[size];
    rewind(file);

    // Fill array with instructions
    char c;
    for (int i = 0; i < size; i++) {
        c = fgetc(file);
        code[i] = c;
    }
    fclose(file);
    
    // Interpret all code
    interpretBetween(&ptr, 0, code, 0, size-1);

    return 0;
}

