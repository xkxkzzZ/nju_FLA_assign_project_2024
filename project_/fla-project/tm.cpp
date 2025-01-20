#include "tm.h"



TM::TM(std::string filename, bool verbose){
    is_verbose = verbose;
    std::ifstream file(filename);
    if (!file.is_open()){
        std::cerr<<"Error: Invalid file"<<std::endl;
        exit(1);       
    }
    std::string line;
    while(std::getline(file, line)){
        line = clean_line(line);
        if (line.size() == 0){
            continue;
        }
        else if (line[0] == '#'){
            std::string symbol = get_symbol_from_assignment(line);
            if(symbol == "Q"){
                states_Q = get_set_of_string(line);
            }
            else if(symbol == "S"){
                input_symbols_S = get_set_of_char(line);
            }
            else if(symbol == "G"){
                tape_symbols_G = get_set_of_char(line);
            }
            else if(symbol == "q0"){
                start_state_q0 = get_rvalue_from_assignment(line);
                if (!check_string_in_stringset(start_state_q0, states_Q)){
                    if(DEBUG) std::cerr<<"Error: Invalid start state"<<std::endl;
                    make_syntax_error();
                }
            }
            else if(symbol == "B"){
                blank_symbol_B = string_to_char(get_rvalue_from_assignment(line));
                if (!check_char_in_charset(blank_symbol_B, tape_symbols_G)){
                    if(DEBUG) std::cerr<<"Error: Invalid blank symbol"<<std::endl;
                    make_syntax_error();
                }
            }
            else if(symbol == "F"){
                final_states_F = get_set_of_string(line);
                if (!check_stringset_in_stringset(final_states_F, states_Q)){
                    if(DEBUG) std::cerr<<"Error: Invalid final states"<<std::endl;
                    make_syntax_error();
                }
            }
            else if(symbol == "N"){
                number_of_tapes_N = std::stoi(get_rvalue_from_assignment(line));
            }
            else{
                if(DEBUG) std::cerr<<"Error: Invalid assignment"<<std::endl;
                make_syntax_error();
            }
        }
        else{
            transition_functions.push_back(TM_Transition(line));
        }
    }
    file.close();
}

void TM::log(){
    std::cout<<"--- TM ---"<<std::endl;
    std::cout<<"States Q: ";
    for(auto state: states_Q){
        std::cout<<state<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Input Symbols S: ";
    for(auto symbol: input_symbols_S){
        std::cout<<symbol<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Tape Symbols G: ";
    for(auto symbol: tape_symbols_G){
        std::cout<<symbol<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Start State q0: "<<start_state_q0<<std::endl;
    std::cout<<"Blank Symbol B: "<<blank_symbol_B<<std::endl;

    std::cout<<"Final States F: ";
    for(auto state: final_states_F){
        std::cout<<state<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Number of Tapes N: "<<number_of_tapes_N<<std::endl;

    std::cout<<"Transition Functions: \n";
    for(auto transition: transition_functions){
        transition.log();
    }
}


TM_Transition::TM_Transition(std::string str){
    std::vector<std::string> temp = split(str, ' ');
    if (temp.size() != 5){
        if(DEBUG) std::cerr<<"Error: Invalid transition"<<std::endl;
        make_syntax_error();
    }
    old_state = temp[0];
    old_symbols = temp[1];
    new_symbols = temp[2];
    directions = temp[3];
    new_state = temp[4];
}

void TM_Transition::log(){
    std::cout<<"Transition: ";
    std::cout<<"delta("<<old_state<<", "<<old_symbols<<") = ";
    std::cout<<"("<<new_symbols<<", "<<directions<<", "<<new_state<<")"<<std::endl;
}

Tape::Tape(std::string input, char blank_symbol){
    head = 0;
    for(int i = 0; i < input.size(); i++){
        tape.push_back(input[i]);
        index.push_back(i);
    }
    this->blank_symbol = blank_symbol;
}

char Tape::read(){
    if (head < 0 || head >= tape.size()){
        return blank_symbol;
    }
    return tape[head];
}

void Tape::write(char symbol){
    tape[head] = symbol;
}

void Tape::move(char direction){
    if (direction == 'l'){
        head--;
        if(head < 0){
            tape.insert(tape.begin(), blank_symbol);
            index.insert(index.begin(), index[0]-1);
            head = 0;
        }
        if(head+1 == tape.size()-1 && tape[tape.size()-1] == blank_symbol){
            tape.pop_back();
            index.pop_back();
        }
    }
    else if (direction == 'r'){
        head++;
        if(head >= tape.size()){
            tape.push_back(blank_symbol);
            index.push_back(index[index.size()-1]+1);
        }
        if (head-1 == 0 && tape[0] == blank_symbol){
            tape.erase(tape.begin());
            index.erase(index.begin()); 
            head = 0;
        }
    }
    // else * 不动
}

std::string Tape::get_tape_content(){
    std::string result = "";
    for(auto ch: tape){
        if(ch == blank_symbol){
            result += " ";
        }
        else{
            result += ch;
        }
    }
    return clean_string(result);
}

void Tape::log(int my_idx = 0){
    // index 超过10的部分需要对齐两个(或多个)空格
    std::cout<<"Index"<<my_idx<<" : ";
    for(auto idx: index){
        if(idx < 0){
            std::cout<<abs(idx)<<" ";
        }
        else{
            std::cout<<idx<<" ";
        }
    }
    std::cout<<std::endl;

    std::cout<<"Tape"<<my_idx<<"  : ";
    for(int i = 0; i < tape.size(); i++){
        std::cout<<tape[i];
        int tmp = abs(index[i]);
        while(tmp >= 10){
            std::cout<<" ";
            tmp /= 10;
        }
        std::cout<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Head"<<my_idx<<"  : ";
    // ^ 后面有空格
    for(int i = 0; i < tape.size(); i++){
        if(i == head){
            std::cout<<"^";
        }
        else{
            std::cout<<" ";
        }
        int tmp = abs(index[i]);
        while(tmp >= 10){
            std::cout<<" ";
            tmp /= 10;
        }
        std::cout<<" ";
    }
    std::cout<<std::endl;
}



bool TM::can_trans(TM_Transition transition){
    
    if (current_state != transition.old_state){
        return false;
    }
    for(int i = 0; i < number_of_tapes_N; i++){
        char tape_symbol = tapes[i].read();
        char old_symbol = transition.old_symbols[i];
        if (tape_symbol != old_symbol && !(old_symbol == '*' && tape_symbol != blank_symbol_B)){
            return false;
        }
    }
    return true;
} 

void TM::trans(TM_Transition transition){
    if(DEBUG) {
        transition.log();
    }
    current_state = transition.new_state;
    for(int i = 0; i < number_of_tapes_N; i++){
        
        char new_symbol = transition.new_symbols[i];
        if (new_symbol == '*'){
            new_symbol = tapes[i].read(); // 新符号为 *表示不改变
        }
        tapes[i].write(new_symbol);

        char direction = transition.directions[i];
        tapes[i].move(direction);
    }
}





void TM::execute(std::string input){
    if(is_verbose){
        std::cout<<"Input: "<<input<<std::endl;
    }
    for(int i=0; i<input.size(); i++){
        if (!check_char_in_charset(input[i], input_symbols_S)){
            if(is_verbose){
                std::cerr<<"==================== ERR ===================="<<std::endl;
                std::cerr<<"error: '"<<input[i]<<"' was not declared in the set of input symbols"<<std::endl;
                std::cerr<<"Input: "<<input<<std::endl;
                std::cerr<<"       ";
                for(int j=0; j<i; j++){
                    std::cerr<<" ";
                }
                std::cerr<<"^"<<std::endl;
                std::cerr<<"==================== END ===================="<<std::endl;
            }
            else{
                std::cerr<<"illegal input"<<std::endl;
            }
            exit(1);
        }
    }
    if(is_verbose){
        std::cout<<"==================== RUN ===================="<<std::endl;
    }

    current_state = start_state_q0;
    if(input == ""){
        input = char_to_string(blank_symbol_B);
    }
    tapes.push_back(Tape(input, blank_symbol_B));
    for(int i = 1; i < number_of_tapes_N; i++){
        tapes.push_back(Tape(char_to_string(blank_symbol_B), blank_symbol_B));
    }

    int step = 0;
    // 当且仅当找不到可用转移时停机
    bool found;
    do{
        if (is_verbose){
            print_state(step);
        }
        found = false;
        for(auto transition: transition_functions){
            if(can_trans(transition)){
                trans(transition);
                found = true;
                break;
            }
        }
        step++;
    }while(found);
    // 直接输出第一条纸带内容
    if (is_verbose){
        std::cout<<"Result: "<<tapes[0].get_tape_content()<<std::endl;
        std::cout<<"==================== END ===================="<<std::endl;
    }
    else{
        std::cout<<tapes[0].get_tape_content()<<std::endl;
    }
}

void TM::print_state(int step){
    std::cout<<"Step   : "<<step<<std::endl;
    std::cout<<"State  : "<<current_state<<std::endl;
    for(int i = 0; i < number_of_tapes_N; i++){
        tapes[i].log(i);
    }
    std::cout<<"---------------------------------------------"<<std::endl;
}