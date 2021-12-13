#include "Util/StringProcessor.h"

/**
 * Empty Constructor
 */
StringProcessor::StringProcessor() {}

/**
 * Set the string to parse
 * @param input the input string to parse
 * @param input_len the length of the input string
 */
void StringProcessor::setInput(char* input, uint16_t input_len) {
    this->input = input;
    this->input_len = input_len;
    this->index = 0;
}

/**
 * Set the separator between arguments in the string and the end character of the string
 */
void StringProcessor::setSeparators(char separator, char endSeparator) {
    this->separator = separator;
    this->endSeparator = endSeparator;
}

/**
 * Read the next argument in the string
 * @param output the array into which the parsed argument will be stored
 * @param output_len the length of the output array
 * @return the length of the parsed argument or -1 if argument too long
 */
int16_t StringProcessor::next(char* output, uint16_t output_len) {
    uint16_t outIndex = 0;
    // Whilst we haven't reached the end of the input array
    while (this->index < this->input_len) {
        // Ensure we are in the boundaries of the output array length
        if (outIndex < output_len) {
            // Check if the character is a separator
            if (this->input[index] == this->separator) {
                // End Char
                output[outIndex] = '\0';
                // Skip the separator
                this->index += 1;
                // Return the length of the argument
                return outIndex + 1;
            } else if (this->input[index] == this->endSeparator) {
                // End Char
                output[outIndex] = '\0';
                // Move index to end of input string array
                this->index = this->input_len;
                // Return the length of the argument
                return outIndex + 1;
            } else {
                output[outIndex] = this->input[index];
                outIndex++;
            }
        } else {
            // Error if ran out of output array space
            return -1;
        }
        index++;
    }
    if (this->index == this->input_len) {
        if (outIndex < output_len) {
            output[outIndex] = '\0';
            outIndex++;
        } else {
            output[outIndex - 1] = '\0';
        }
    }
    return outIndex;
}

/**
 * Compare if 2 strings are equal
 * @param string1 the string to compare from
 * @param string1_len the length of the string to compare from
 * @param string2 the string to compare to
 * @return true if the strings are equal otherwise false
 */
bool StringProcessor::compare(char* string1, uint16_t string1_len, const char* string2) {
    // Check string sizes match
    if (string1_len == strlen(string2)) {
        // Iterate through each character in the strings
        for (uint16_t i = 0; i < string1_len; i++) {
            if (string1[i] != string2[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}