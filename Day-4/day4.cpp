#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <set>
#include <math.h>

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

// Transform string number list into int vector
std::vector<int> transform_string_to_number_list(std::vector<std::string> string_num_list) {
    std::vector<int> num_list;
    for (int i = 0; i < string_num_list.size(); i++) {
        num_list.push_back(std::stoi(string_num_list[i]));
    }
    return num_list;
}

// Transform string number list into int set
std::set<int> transform_string_to_number_set(std::vector<std::string> string_num_list) {
    std::set<int> num_set;
    for (int i = 0; i < string_num_list.size(); i++) {
        num_set.insert(std::stoi(string_num_list[i]));
    }
    return num_set;
}

// struct containing info for each card 
struct CardInfo {
    std::set<int> winning_nums;
    std::vector<int> current_nums;
};

// Parse a given input line into a card info struct containing int vectors
// of the winning numbers and current numbers (i.e. numbers you have) 
struct CardInfo parse_card_info(std::string line) {
    // get rid of game number information 
    std::vector<std::string> parsed_card = split_with_deliminator(line, ':');

    // parse second part of line (containing numbers)
    std::vector<std::string> numbers = split_with_deliminator(parsed_card[1], '|');

    // create winning and current numbers list
    std::vector<std::string> winning_string_nums = split_with_deliminator(numbers[0], ' ');
    std::vector<std::string> current_string_nums = split_with_deliminator(numbers[1], ' ');

    // transform number lists from strings to ints 
    CardInfo card_info;

    // need to create set here
    card_info.winning_nums = transform_string_to_number_set(winning_string_nums);
    card_info.current_nums = transform_string_to_number_list(current_string_nums);
    return card_info;
}

// Given a CardInfo struct, determine the score for that card (matching numbers from
// current list to winning set). Score = 2^(# of matching nums - 1)
int calculate_card_score(CardInfo card_info) {
    // go through each of current nums to see if they are in the winning set 
    int counter = 0;
    for (int i = 0; i < card_info.current_nums.size(); i++) {
        counter += card_info.winning_nums.count(card_info.current_nums[i]);
    }

    // calculate card final score 
    int final_score = 0;
    if (counter > 0) {
        final_score = pow(2, counter - 1);
    }
    return final_score;
}

// Calculate the sum of all card scores 
int calculate_card_score_part_1(std::vector<std::string> lines) {
    int total_sum = 0;
    for (int i = 0; i < lines.size(); i++) {
        CardInfo card_info = parse_card_info(lines[i]);
        total_sum += calculate_card_score(card_info); 
    }
    return total_sum;
}

struct CardCountInfo {
    int num_matching;
    int num_count; 
};

// Parse card info into data structure for part two. It will output
// a CardInfo2 struct where num_matching is the number of current
// nums matching winning numbs, and num_count is total number of cards. 
CardCountInfo parse_card_count_info(CardInfo card_info) {
    // go through each of current nums to see if they are in the winning set 
    int counter = 0;
    CardCountInfo card_count_info;
    for (int i = 0; i < card_info.current_nums.size(); i++) {
        counter += card_info.winning_nums.count(card_info.current_nums[i]);
    }
    card_count_info.num_matching = counter;
    card_count_info.num_count = 1;
    return card_count_info;
}

// Create list of card count info for part 2
// Calculate the sum of all card scores 
std::vector<CardCountInfo> create_card_count_info(std::vector<std::string> lines) {
    std::vector<CardCountInfo> card_count_info_list;
    for (int i = 0; i < lines.size(); i++) {
        CardInfo card_info = parse_card_info(lines[i]);
        card_count_info_list.push_back(parse_card_count_info(card_info)); 
    }
    return card_count_info_list;
}

// Calculate score for part 2
int calculate_total_scratchcards(std::vector<CardCountInfo> card_count_info_list) {
    int total_cards = 0;
    for (int i = 0; i < card_count_info_list.size(); i++) {
        for (int j = 0; j < card_count_info_list[i].num_matching; j++) {
            // make sure j doesn't extend beyond bounds
            int next_index = i + j + 1;
            if (next_index < card_count_info_list.size()) {
                card_count_info_list[next_index].num_count += card_count_info_list[i].num_count;
            }
        }
        total_cards += card_count_info_list[i].num_count;
    }
    return total_cards;
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
    int total_sum_part1 = calculate_card_score_part_1(lines);
    std::cout << "The answer to part 1 is: " << total_sum_part1 << std::endl;

    // Calculate answer to part 2
    std::vector<CardCountInfo> card_count_info_list = create_card_count_info(lines);
    int total_sum_part2 = calculate_total_scratchcards(card_count_info_list);
    std::cout << "The answer to part 2 is: " << total_sum_part2 << std::endl;
}