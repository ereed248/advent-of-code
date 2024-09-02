#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>

// Create digit word list 1-9.
std::vector<std::string> initialize_digit_words() {
    std::vector<std::string> digit_words;
    digit_words.push_back("one");
    digit_words.push_back("two");
    digit_words.push_back("three");
    digit_words.push_back("four");
    digit_words.push_back("five");
    digit_words.push_back("six");
    digit_words.push_back("seven");
    digit_words.push_back("eight");
    digit_words.push_back("nine");

    return digit_words;
}

// Create digit map mapping words to digit strings. 
std::map<std::string, std::string> initilize_digit_map() {
    std::map<std::string, std::string> digit_words;
    digit_words["one"] = "1"; 
    digit_words["two"] = "2";
    digit_words["three"] = "3";
    digit_words["four"] = "4";
    digit_words["five"] = "5";
    digit_words["six"] = "6";
    digit_words["seven"] = "7";
    digit_words["eight"] = "8";
    digit_words["nine"] = "9";

    return digit_words;
}

// Recursive algo to check if pattern is at the
// start of text. 
bool check_match(const char *text, const char *pattern) {
    // if text has ended but pattern has not, return false
    if (*text == '\0' && *pattern != '\0') {
        return false;
    }
    // if we reach the end of the pattern, we can assume
    // that all other chars before matched
    if (*pattern == '\0') {
        return true;
    }
    // if chars match, we need to continue to iterate
    // along the pattern and text
    if (*text == *pattern) {
        return check_match(text + 1, pattern + 1); 
    }
    // otherwise, there is no pattern in text -- this case
    // happens when text runs out before pattern or the 
    // characters don't match
    return false;
} 

// Part 2: Creates a two-digit number out of a string.
// First digit is the first number (either digit or digit word) found in string, 
// and the second is the last number (either digit or digit word) found in string.
int find_coordinate_value(std::string coordinate) {
    std::vector<std::string> digit_words = initialize_digit_words();
    std::map<std::string, std::string> digit_map = initilize_digit_map();

    // iterate over coordinate string using pointers to find first 
    // value (either digit or word)
    std::string coordinate_values;
    const char* coordinate_char = coordinate.c_str();
    while (*coordinate_char != '\0') {
        if (std::isdigit(*coordinate_char)) {
            coordinate_values.push_back(*coordinate_char);
        } else {
            // otherwise, check if a digit word if present
            // at this position in the coordinate
            for (int j = 0; j < digit_words.size(); j++) {
                if (check_match(coordinate_char, digit_words[j].c_str())) {
                    // need to map digit words to digit string and add
                    std::string digit = digit_map[digit_words[j]];
                    coordinate_values = coordinate_values + digit;
                }
            }
        }
        coordinate_char++;
    }
    // once coordinate value string is created, create final integer 
    // value from first and last characters 
    char first_digit = coordinate_values[0] - '0';
    char last_digit = coordinate_values[coordinate_values.size() - 1] - '0';
    return (first_digit * 10) + last_digit; 
}

// Part 1: Creates a two-digit number out of a string.
// First digit is the first number found in string, 
// and the second is the last number found in string. 
int find_coordinate_digit(std::string coordinate) {
    // Find the first digit 
    int first_digit = 0; 
    for (int i = 0; i < coordinate.size(); i++) {
        if (std::isdigit(coordinate[i])) {
            first_digit = coordinate[i] - '0';
            break;
        }
    }

    // Find the second digit
    int second_digit = 0; 
    for (int j = coordinate.size(); j > 0; j--) {
        if (std::isdigit(coordinate[j - 1])) {
            second_digit = coordinate[j - 1] - '0';
            break;
        }
    }
    return (first_digit * 10) + second_digit; 
}

int main() {
    // Open the file
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1; 
    }
    // Read each line
    std::string line; 
    std::vector<std::string> lines; 
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    // Calculate answer to part 1
    int total_sum_part1;    
    for (int i = 0; i < lines.size(); i++) { 
        total_sum_part1 = total_sum_part1 + find_coordinate_digit(lines[i]);
    }
    std::cout << "The answer to part 1 is: " << total_sum_part1 << std::endl;

    // Calculate answer to part 2 
    int total_sum_part2;
    for (int i = 0; i < lines.size(); i++) { 
        total_sum_part2 = total_sum_part2 + find_coordinate_value(lines[i]);
    }
    std::cout << "The answer to part 2 is: " << total_sum_part2 << std::endl;

    file.close();
    return 0;
}