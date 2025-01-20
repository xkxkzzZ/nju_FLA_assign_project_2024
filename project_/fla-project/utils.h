// utils.h
#ifndef UTILS_H
#define UTILS_H

#define DEBUG 0

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

class Config{
public:
    enum Mode{
        PDA_MODE,
        TM_MODE
    };
    Mode mode;
    std::string file;
    std::string input;
    bool verbose;

    Mode get_mode(std::string str);
    void print_error(std::string str);

    Config(int argc, char* argv[]);
    void log();
};


std::vector<std::string> split(std::string str, char delimiter);
std::string clean_string(std::string str); 
char string_to_char(std::string str);
std::string char_to_string(char ch);
std::string clean_line(std::string str);

std::set<std::string> get_set_of_string(std::string str);
std::set<char> get_set_of_char(std::string str);
std::string get_symbol_from_assignment(std::string str);
std::string get_rvalue_from_assignment(std::string str);

bool check_string_in_stringset(std::string str, std::set<std::string> strset);
bool check_char_in_charset(char ch, std::set<char> chset);
bool check_stringset_in_stringset(std::set<std::string> strset1, std::set<std::string> strset2);

void make_syntax_error();


#endif