#include <stdlib.h> // macros
#include <stdio.h> // for fopen()
#include <string.h> // for strstr()
#include <stdbool.h> // c99 boolean

// Input is ./a.out [<-f|-nof> [<input_file> <output_file> ]]

int main(int argc, char** argv) {
    const char* ARG_FORMAT = "Input is ./a.out [<-f|-nof> [<input_file> <output_file> ]]";
    // Argument checking
    char* file_in_name = NULL;
    char* file_out_name = NULL;

    FILE* fp_in = NULL;
    FILE* fp_out = NULL;

    bool is_filtered = true; // argument check
    const int BUFSIZE = 256; // length of readline
    char line[BUFSIZE]; // individual line from hexdump


    if(argc == 1 || argc == 2) {
        // Default files
        file_in_name = "hexdump.txt";
        file_out_name = "filtered_output.txt";
        is_filtered = true;
    } 
    if(argc == 2 || argc == 4) {
        if(strcmp(argv[1], "-f") == 0) {
            is_filtered = true;
        } else if(strcmp(argv[1], "-nof") == 0) {
            is_filtered = false;
        } else {
            printf("Incorrect filter specifier\n");
            printf("%s\n", ARG_FORMAT);
            return(EXIT_FAILURE);
        }
    }
    if(argc == 4) {
        file_in_name = argv[2];
        file_out_name = argv[3];
    }
    fp_in = fopen(file_in_name, "r");
    fp_out = fopen(file_out_name, "w");
    

    if (fp_in == NULL || fp_out == NULL) {
        printf("Cannot read or write file\n");
        return(EXIT_FAILURE);
    }

    if(is_filtered) {
        printf("Filtered output\n");
    } else {
        printf("No filtered output\n");
    }
    printf("Reading from %s. Writing to %s\n", file_in_name, file_out_name);

    int beg_strip_length = 0; // Cut from beginning
    const int READ_LENGTH = 48; // What to actually read
    int len; // length of fgets line
    while(fgets(line, BUFSIZE, fp_in) != NULL) {
        len = strlen(line);
        beg_strip_length = 10; // address line without indent

        // For some reason there is an 4 char indent on some lines
        // Strip
        if(line[0] == ' ') {
            beg_strip_length += 4;
        }
        if(len == 1) { // Empty lines
            continue;
        }
        if(strstr(line, "RETR") == NULL || !is_filtered) {
            for(int i = beg_strip_length; i < beg_strip_length + READ_LENGTH; 
                i++) {
                fputc(line[i], fp_out);
                printf("%c", line[i]);
            }
            fputc('\n', fp_out);
            printf("\n");
        } 
    }

    fclose(fp_in);
    fclose(fp_out);

    return(EXIT_SUCCESS);
}
