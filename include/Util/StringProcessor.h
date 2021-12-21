#ifndef StringProcessor_h
#define StringProcessor_h

#include <Arduino.h>

/**
 * This class is used to parse a string into multiple arguments
 */

class StringProcessor
{
public:
    // Constructor
    StringProcessor();

    // Set the string to parse
    void setInput(char *input, uint16_t input_len);
    // Set the separator character between arguments and the ending character of the string
    void setSeparators(char separator, char endSeparator);
    // Read the next argument in the string
    int16_t next(char *output, uint16_t output_len);
    // Compare strings
    static bool compare(char *string1, uint16_t string1_len, const char *string2);

private:
    // The Input String to process
    char *input;
    // The length of the input string
    int input_len;
    // The separator character
    char separator;
    // The end of string separator character
    char endSeparator;
    // The current index in the input string
    int index;
};

#endif