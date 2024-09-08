#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>

// Given a position of a char in the engine manifest, check surrounding chars to
// determine if there is an adjacent symbol. 
bool check_adjacent_symbol(std::vector<std::string> lines, int line_num, int char_num) {
    // top row: only check top row if we aren't on the first line
    if (line_num > 0) {
        // if not on the first line, make sure we aren't at the first position
        if (char_num > 0) {
            if (ispunct(lines[line_num - 1][char_num - 1]) && lines[line_num - 1][char_num - 1] != '.') {
                return true;
            }
        } 
        // if not on the first line, there will always be a char above 
        if (ispunct(lines[line_num - 1][char_num]) && lines[line_num - 1][char_num] != '.') {
            return true;
        }

        // if not on the first line, make sure we aren't at the last position
        if (char_num < lines[line_num].size() - 1) {
            if (ispunct(lines[line_num - 1][char_num + 1]) && lines[line_num - 1][char_num + 1] != '.') {
                return true;
            }
        } 
    }

    // middle row: this is the string of the char in question
    // make sure we aren't at the first position
    if (char_num > 0) {
        if (ispunct(lines[line_num][char_num - 1]) && lines[line_num][char_num - 1] != '.') {
            return true;
        }
    } 
    // make sure we aren't at the last position
    if (char_num < lines[line_num].size() - 1) {
        if (ispunct(lines[line_num][char_num + 1]) && lines[line_num][char_num + 1] != '.') {
            return true;
        }
    } 

    // bottom row: only check bottom row if we aren't on the last line
    if (line_num < lines.size() - 1) {
        // if not on the bottom line, make sure we aren't at the first position
        if (char_num > 0) {
            if (ispunct(lines[line_num + 1][char_num - 1]) && lines[line_num + 1][char_num - 1] != '.') {
                return true;
            }
        } 
        // if not on the bottom line, there will always be a char below 
        if (ispunct(lines[line_num + 1][char_num]) && lines[line_num + 1][char_num] != '.') {
            return true;
        }

        // if not on the bottom line, make sure we aren't at the last position
        if (char_num < lines[line_num].size() - 1) {
            if (ispunct(lines[line_num + 1][char_num + 1]) && lines[line_num + 1][char_num + 1] != '.') {
                return true;
            }
        } 
    }
    return false;
}

// Create a boolean matrix mirroring the size of the parsed lines where each element
// is a boolean corresponding to whether or not that char has an adjacent symbol. 
std::vector<std::vector<bool> > create_bool_matrix(std::vector<std::string> lines) {
    int height = lines.size();
    int width = lines[0].size();
    std::vector<std::vector<bool> > bool_matrix(height, std::vector<bool>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool has_adjacent_symbol = check_adjacent_symbol(lines, i, j);
            bool_matrix[i][j] = has_adjacent_symbol;
        }
    }
    return bool_matrix;
}

// Given a line, create a vector containing the string numbers 
// that has adjacent symbols. 
std::vector<std::string> parse_number_line(std::string line, std::vector<bool> bool_line) {
    std::vector<std::string> string_numbers;
    std::string string_number;
    bool to_add = false; 

    // loop over line and check each character 
    for (int i = 0; i < line.size(); i++) {
        // if the char is a digit, add it to the string number and check
        // it's value in the bool matrix 
        if (std::isdigit(line[i])) {
            string_number.push_back(line[i]);
            if (bool_line[i]) {
                to_add = true; 
            }

            // Make sure to get the edge case where a number ends a line
            if (i == line.size() - 1 && to_add) {
                string_numbers.push_back(string_number);
            }
        } else {
            // if the char is not a digit, make sure to add the number
            // to the list if necessary but o/w clear the string/bool
            if (!string_number.empty() && to_add) {
                string_numbers.push_back(string_number);
            }
            string_number = "";
            to_add = false;
        }
    }        
    return string_numbers;
}

// Add a list of string numbers together 
int add_string_numbers(std::vector<std::string> parsed_numbers) {
    // each string corresponds to a number -- for each string
    // convert to an integer and add together
    int total_sum = 0; 
    for (int i = 0; i < parsed_numbers.size(); i++) {
        total_sum += std::stoi(parsed_numbers[i]);
    }
    return total_sum; 
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
    int total_sum_part1 = 0;
    std::vector<std::string> parsed_numbers;
    std::vector<std::vector<bool> > bool_matrix = create_bool_matrix(lines);
    for (int i = 0; i < lines.size(); i++) {
        parsed_numbers = parse_number_line(lines[i], bool_matrix[i]);
        total_sum_part1 += add_string_numbers(parsed_numbers);
    }
    std::cout << "The answer to part 1 is: " << total_sum_part1 << std::endl;

    // Calculate answer to part 1
    // int total_sum_part2; 
    // std::cout << "The answer to part 2 is: " << total_sum_part2 << std::endl;

    

}