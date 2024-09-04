#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Split a string based on a deliminator and output a list of strings  
std::vector<std::string> split_with_deliminator(std::string data, char deliminator) {
    // Create string stream object 
    std::stringstream ss(data);
    std::vector<std::string> data_parsed;

    // Use stream to split on deliminator to split
    std::string split_data;
    while (std::getline(ss, split_data, deliminator)) {
        // Only add data that is not empty
        if (!split_data.empty()) {
            data_parsed.push_back(split_data);
        }
    }
    return data_parsed;
}

// Given a list of cube quantities, create an int vector of red, green, blue quantities
std::vector<int> create_color_data(std::vector<std::string> data) {
    // Loop over data all color data 
    std::vector<int> final_color_data(3); 
    for (int i = 0; i < data.size(); i++) {
        std::vector<std::string> split_data = split_with_deliminator(data[i], ' ');
        int value = std::stoi(split_data[0]);

        // Depending on the color, place the value in the final vector
        if (split_data[1] == "red") {
            final_color_data[0] = value;
        } else if (split_data[1] == "green") {
            final_color_data[1] = value;
        } else if (split_data[1] == "blue") {
            final_color_data[2] = value;
        }
    }
    return final_color_data;
}

// Take in a list of color data int vectors and output a final int vector
// representing the max number of "red", "green", and "blue" cubes drawn
// for that given game. 
std::vector<int> finalize_color_data(std::vector<std::vector<int> > color_data) {
    int max_red = 0, max_green = 0, max_blue = 0;
    std::vector<int> final_data(3);
    for (int i = 0; i < color_data.size(); i++) {
        if (color_data[i][0] > max_red) {
            max_red = color_data[i][0];
        }
        if (color_data[i][1] > max_green) {
            max_green = color_data[i][1]; 
        }
        if (color_data[i][2] > max_blue) {
            max_blue = color_data[i][2]; 
        }
    }
    final_data[0] = max_red;
    final_data[1] = max_green;
    final_data[2] = max_blue;
    return final_data;
}

// Parse an input line containing game data. Return an int vector containing
// the max red, green, and blue cubes drawn for that game ([red, green, blue]).
std::vector<int> parse_game_info(std::string game_line) {
    // First find the position of ":" and create a string
    // only consisting of the game data
    size_t game_pos = game_line.find(':');
    std::string game_data = game_line.substr(game_pos + 1);

    // Split on ';' to separate out draws of cubes
    std::vector<std::string> cube_draws = split_with_deliminator(game_data, ';');

    // For each draw of cubes, split on ',' to separate and create color data. 
    std::vector<std::string> color_info;
    std::vector<std::vector<int> > final_color_data;
    for (int i = 0; i < cube_draws.size(); i++) {
        // std::cout << "New draw!" << std::endl;
        color_info = split_with_deliminator(cube_draws[i], ',');
        final_color_data.push_back(create_color_data(color_info));
    }

    // Given the color data for each draw in a game, finalize the data for the game by 
    // taking the max of each color to create a final int vector. 
    std::vector<int> final_draw_data = finalize_color_data(final_color_data);
    return final_draw_data;
}

// Given all lines of the input, parse out each game's data. Put each game
// data into a vector (where index + 1 will correspond to game number)
std::vector<std::vector<int> > parse_input(std::vector<std::string> lines) {
    std::vector<std::vector<int> > all_game_data;
    for (int i = 0; i < lines.size(); i++) { 
        all_game_data.push_back(parse_game_info(lines[i]));
    }
    return all_game_data; 
}

// Determine if a game is possible if there are only 12 red cubes, 13 green cubes, 
// and 14 blue cubes in the bag
bool is_game_possible(std::vector<int> game_data) {
    bool is_possible = true; 
    if (game_data[0] > 12) {
        is_possible = false; 
    } else if (game_data[1] > 13) {
        is_possible = false;
    } else if (game_data[2] > 14) {
        is_possible = false;
    }
    return is_possible;
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
    std::vector<std::vector<int> > parsed_game_data = parse_input(lines);
    for (int i = 0; i < parsed_game_data.size(); i++) {
        if (is_game_possible(parsed_game_data[i])) {
            total_sum_part1 += i + 1; 
        }
    }
    std::cout << "The answer to part 1 is: " << total_sum_part1 << std::endl;

}