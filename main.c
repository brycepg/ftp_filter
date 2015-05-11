#include <stdlib.h> // for free() and macros
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
    char* line = NULL; // individual line from hexdump
    size_t len = 0; // length of readline


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

    while(getline(&line, &len, fp_in) != -1) {
        if(strstr(line, "RETR") == NULL || !is_filtered) {
            fprintf(fp_out, "%s", line);
        } 
    }

    fclose(fp_in);
    fclose(fp_out);
    if(line) {
        free(line);
    }

    return(EXIT_SUCCESS);
}
