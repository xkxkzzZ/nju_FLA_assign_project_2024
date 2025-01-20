// utils.cpp

#include "utils.h"

void Config::print_error(std::string message){
    // 用于参数格式不正确的提示
    std::cerr<<"Argument error: "<<message<<std::endl;
    std::cerr<<"Please use -h or --help to get help"<<std::endl;
}

Config::Mode Config::get_mode(std::string filename){
    if (filename.size() < 4){
        print_error("Invalid file name");
        exit(1);
    }
    if (filename.substr(filename.size()-3) == ".tm"){
        return TM_MODE;
    }
    if (filename.substr(filename.size()-4) == ".pda"){
        return PDA_MODE;
    }
    print_error("Invalid file name");
    exit(1);
}

Config::Config(int argc, char* argv[]){
    if (argc == 1){
        print_error("Missing arguments");
        exit(1);
    }
    verbose = false;
    for(int i = 0; i < argc; i++){
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help"){
            std::cout<<"usage:   fla [-h|--help] <pda> <input>\n"
            <<"         fla [-v|--verbose] [-h|--help] <tm> <input>\n";
            exit(0);
        }
        if (std::string(argv[i]) == "-v" || std::string(argv[i]) == "--verbose"){
            verbose = true;
        }
    }
    if (verbose == false && argc == 3){
        mode = get_mode(argv[1]);
        file = argv[1];
        input = argv[2];
    }
    else if (verbose == true && argc == 4){
        mode = get_mode(argv[2]);
        file = argv[2];
        input = argv[3];
    }
    else{
        print_error("Invalid arguments");    
        exit(1);
    }
}

void Config::log(){
    std::cout<<"--- Config ---"<<std::endl;
    if (mode == PDA_MODE){
        std::cout<<"mode: PDA"<<std::endl;
    }
    else if (mode == TM_MODE){
        std::cout<<"mode: TM"<<std::endl;
    }
    if(verbose){
        std::cout<<"verbose: true"<<std::endl;
    }
    else{
        std::cout<<"verbose: false"<<std::endl;
    }
    std::cout<<"file: "<<file<<std::endl;
    std::cout<<"input: "<<input<<std::endl;
}


void make_syntax_error(){
    std::cerr<<"syntax error"<<std::endl;
    exit(1);
}

std::vector<std::string> split(std::string str, char delimiter){
    std::vector<std::string> result;
    std::string temp = "";
    for(int i = 0; i < str.size(); i++){
        if (str[i] == delimiter){
            result.push_back(temp);
            temp = "";
        }
        else{
            temp += str[i];
        }
    }
    if (temp != ""){
        result.push_back(temp);
    }
    return result;
}

std::string clean_string(std::string str){
    // 去掉首尾空格
    std::string result = "";
    int begin = 0, end = str.size()-1;
    while(str[begin] == ' '){
        begin++;
    }
    while(str[end] == ' '){
        end--;
    }
    for(int i = begin; i <= end; i++){
        result += str[i];
    }
    return result;
}

char string_to_char(std::string str){
    str = clean_string(str);
    if (str.size() != 1){
        if(DEBUG) std::cerr<<"Error: Invalid char"<<std::endl;
        make_syntax_error();
    }
    return str[0];
}

std::string char_to_string(char ch){
    return std::string(1, ch);
}

std::string clean_line(std::string str){
    //去掉行末/r（如果有）
    if (str.size() > 0 && str[str.size()-1] == '\r'){
        str = str.substr(0, str.size()-1);
    }
    // 去掉行末注释 ，注释以;开头，去掉首尾空格
    std::string result = "";
    int i = str.find(';');
    if (i == std::string::npos){
        result = str;
    }
    else{
        result = str.substr(0, i);
    }
    return clean_string(result);
}

std::set<std::string> get_set_of_string(std::string str){
    std::set<std::string> result;
    int begin = str.find('{');
    int end = str.find('}');
    if (begin == std::string::npos || end == std::string::npos){
        if(DEBUG) std::cerr<<"Error: Invalid set of string"<<std::endl;
        make_syntax_error();    
    }
    str = str.substr(begin+1, end-begin-1); 
    std::vector<std::string> temp = split(str, ',');
    for(int i = 0; i < temp.size(); i++){
        result.insert(clean_string(temp[i]));
    }
    return result;
}

std::set<char> get_set_of_char(std::string str){
    std::set<std::string> temp = get_set_of_string(str);
    std::set<char> result;
    for(std::set<std::string>::iterator it = temp.begin(); it != temp.end(); it++){
        result.insert(string_to_char(*it));
    }
    return result;
}

std::string get_symbol_from_assignment(std::string str){
    // #q0 = q0
    // 输出在 # 和 = 之间的字符
    // #一定在第一个

    if (str[0] != '#'){
        if(DEBUG) std::cerr<<"Error: Invalid assignment"<<std::endl;
        make_syntax_error();
    }
    int assignop = str.find('=');
    if (assignop == std::string::npos){
        if(DEBUG) std::cerr<<"Error: Invalid assignment"<<std::endl;
        make_syntax_error();
    }
    std::string result = "";
    // str[1 - assignop-1]
    for(int i = 1; i < assignop; i++){
        result += str[i];
    }
    result = clean_string(result);
    return result;
}


std::string get_rvalue_from_assignment(std::string str){
    // #q0 = q0
    // 输出在 = 之后的字符，去掉前后空格
    int assignop = str.find('=');
    if (assignop == std::string::npos){
        if(DEBUG) std::cerr<<"Error: Invalid assignment"<<std::endl;
        make_syntax_error();
    }
    std::string result = "";
    for (int i = assignop+1; i < str.size(); i++){
        result += str[i];
    }
    result = clean_string(result);
    return result;
}

bool check_string_in_stringset(std::string str, std::set<std::string> strset){
    return strset.find(str) != strset.end();
}

bool check_char_in_charset(char ch, std::set<char> chset){
    return chset.find(ch) != chset.end();
}

bool check_stringset_in_stringset(std::set<std::string> strset1, std::set<std::string> strset2){
    for(std::set<std::string>::iterator it = strset1.begin(); it != strset1.end(); it++){
        if (strset2.find(*it) == strset2.end()){
            return false;
        }
    }
    return true;
}