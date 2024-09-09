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

// Given a line, create a map of column number -> value. For example, the first line of the 
// test input would be map[0] = 467, map[1] = 467, map[3] = 467, map[4] = 0, etc.
std::map<int, int> parse_number_line_into_map(std::string line) {
    std::vector<int> columns;
    std::string string_number;
    
    // initialize map with zero values
    std::map<int, int> number_line_map;
    for (int i = 0; i < line.size(); i++) {
        number_line_map[i] = 0;
    }
    
    // loop over line and check each character 
    for (int i = 0; i < line.size(); i++) {
        // if the char is a digit, add it to the string number and add
        // its column number to the list 
        if (std::isdigit(line[i])) {
            string_number.push_back(line[i]);
            columns.push_back(i);
          
            // make sure to get the edge case where a number ends a line
            if (i == line.size() - 1) {
                // loop through all column numbers we've gathered
                for (int j = 0; j < columns.size(); j++) {
                    number_line_map[columns[j]] = std::stoi(string_number);
                }
            }
        } else {
            // if the char is not a digit, make sure to add the number
            // to the map if necessary and o/w clear the string/bool
            if (!string_number.empty()) {
                // loop through all column numbers we've gathered
                for (int j = 0; j < columns.size(); j++) {
                    number_line_map[columns[j]] = std::stoi(string_number);
                }
            }
            string_number = "";
            columns.clear();
        }
    }        
    return number_line_map;
}

// Given a input, create a map of row number -> column number -> value. For example, the first line 
// of the test input would be map[0] = {map[0] = 467, map[1] = 467, map[3] = 467, map[4] = 0}, etc.
std::map<int, std::map<int, int> > parse_input_into_map(std::vector<std::string> lines) {
    std::map<int, std::map<int, int> > input_number_map;
    for (int i = 0; i < lines.size(); i++) {
        input_number_map[i] = parse_number_line_into_map(lines[i]);
    } 
    return input_number_map;
}

// Check around asterisk to see if there are two parts surrounding it. If two are found, 
// multiply them together, o/w return 0 
std::vector<int> create_num_list_around_asterisk(std::map<int, std::map<int, int> > number_map, int line_num, int char_num) {
    std::vector<int> number_list;
    // top row: only check top row if we aren't on the first line
    if (line_num > 0) {
        // if not on the first line, make sure we aren't at the first position
        if (char_num > 0) {
            number_list.push_back(number_map[line_num - 1][char_num - 1]);
        } 
        // if not on the first line, there will always be a char above 
        number_list.push_back(number_map[line_num - 1][char_num]);

        // if not on the first line, make sure we aren't at the last position
        if (char_num < number_map[line_num].size() - 1) {
            number_list.push_back(number_map[line_num - 1][char_num + 1]);
        } 
    }

    // middle row: this is the string of the char in question
    // make sure we aren't at the first position
    if (char_num > 0) {
        number_list.push_back(number_map[line_num][char_num - 1]);
    } 
    // make sure we aren't at the last position
    if (char_num < number_map[line_num].size() - 1) {
        number_list.push_back(number_map[line_num][char_num + 1]);
    } 

    // bottom row: only check bottom row if we aren't on the last line
    if (line_num < number_map.size() - 1) {
        // if not on the bottom line, make sure we aren't at the first position
        if (char_num > 0) {
            number_list.push_back(number_map[line_num + 1][char_num - 1]);
        } 
        // if not on the bottom line, there will always be a char below 
        number_list.push_back(number_map[line_num + 1][char_num]);

        // if not on the bottom line, make sure we aren't at the last position
        if (char_num < number_map[line_num].size() - 1) {
            number_list.push_back(number_map[line_num + 1][char_num + 1]);
        } 
    }
    return number_list;
}

// Given a number list surrounding an asterisk, remove duplicates (corresponding
// to numbers taking up multiple surrounding spots) and output product, o/w 0
int calculate_gear_product(std::vector<int> number_list) {
    std::vector<int> final_list; 
    int previous_value = 0; 
    for (int value : number_list) {
        // add non-zero and non-duplicate values to final list
        if (value != 0 && value != previous_value) {
            final_list.push_back(value);
            previous_value = value;
        }
    }
    // if there are two values in the final list, multiply them together
    if (final_list.size() == 2) {
        return final_list[0] * final_list[1];
    }
    return 0;
}

// Given the parsed input lines, determine the total sum of the gear products. 
int determine_total_gear_product_sum(std::vector<std::string> lines, std::map<int, std::map<int, int> > number_map) {
    // loop over input looking for asterisks
    int total_sum = 0;
    int height = lines.size();
    int width = lines[0].size();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (lines[i][j] == '*') {
                total_sum += calculate_gear_product(create_num_list_around_asterisk(number_map, i, j));
            }
        }
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

    // Calculate answer to part 2
    std::map<int, std::map<int, int> > number_map = parse_input_into_map(lines);
    int total_sum_part2 = determine_total_gear_product_sum(lines, number_map);
    std::cout << "The answer to part 2 is: " << total_sum_part2 << std::endl;
}