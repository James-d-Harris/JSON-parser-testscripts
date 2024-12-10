#include <cstdio>  // For FILE, fopen, fclose, perror, fread
#include <cstdlib> // For malloc, free
#include "json.h"

int main(int argc, char **argv) {
    // Check that input is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file) {
        perror("Failed to open input file");
        return 1;
    }

    // Determine the size of the input
    fseek(input_file, 0, SEEK_END);
    long input_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    if (input_size <= 0) {
        fclose(input_file);
        return 1;
    }

    // Allocate memory and read the input
    char *input_data = (char *)malloc(input_size + 1);
    if (!input_data) {
        perror("Failed to allocate memory for input");
        fclose(input_file);
        return 1;
    }

    fread(input_data, 1, input_size, input_file);
    fclose(input_file);
    input_data[input_size] = '\0'; // Null-terminate the input

    // Parse the JSON input
    json_value *parsed = json_parse(input_data, input_size);

    // Validate the parser behavior and cleanup
    if (parsed != nullptr) {
        json_value_free(parsed); // Free the parsed JSON if valid
    }

    free(input_data);
    return 0;
}
