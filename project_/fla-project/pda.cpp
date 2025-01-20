// ; This example program checks if the input string is in \(L = \{a^nb^n | n \ge 1\}\).
// ; Input: a string of a's and b's, e.g. 'aabb'

// ; the finite set of states
// #Q = {q0,q1,q2,accept}

// ; the finite set of input symbols
// #S = {a,b}

// ; the complete set of stack symbols
// #G = {0,1,z}

// ; the start state
// #q0 = q0

// ; the start stack symbol
// #z0 = z

// ; the set of final states
// #F = {accept}

// ; the transition functions

// q0 a z q1 1z
// q1 a 1 q1 11
// q1 b 1 q2 _
// q2 b 1 q2 _
// q2 _ z accept _

// ';' 为注释，忽略
// '#' 为定义，分别为 #Q, #S, #G, #q0, #z0, #F
// 最后几行为转移函数，为五元组，分别为 q1, inputa, stackz, q2, stackz1

#include "pda.h"


PDA::PDA(std::string filename, bool verbose){

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
                stack_symbols_G = get_set_of_char(line);
            }
            else if(symbol == "q0"){
                start_state_q0 = get_rvalue_from_assignment(line);
                if (!check_string_in_stringset(start_state_q0, states_Q)){
                    if(DEBUG) std::cerr<<"Error: Invalid start state"<<std::endl;
                    make_syntax_error();
                }
            }
            else if(symbol == "z0"){
                start_stack_symbol_z0 = string_to_char(get_rvalue_from_assignment(line));
                if (!check_char_in_charset(start_stack_symbol_z0, stack_symbols_G)){
                    if(DEBUG) std::cerr<<"Error: Invalid start stack symbol"<<std::endl;
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
            else{
                if(DEBUG) std::cerr<<"Error: Invalid assignment"<<std::endl;
                make_syntax_error();
            }
        }
        else{
            transition_functions.push_back(PDA_Transition(line));
        }
    }

    file.close();

}


void PDA::log(){
    std::cout<<"--- PDA ---"<<std::endl;
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

    std::cout<<"Stack Symbols G: ";
    for(auto symbol: stack_symbols_G){
        std::cout<<symbol<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Start State q0: "<<start_state_q0<<std::endl;
    std::cout<<"Start Stack Symbol z0: "<<start_stack_symbol_z0<<std::endl;

    std::cout<<"Final States F: ";
    for(auto state: final_states_F){
        std::cout<<state<<" ";
    }
    std::cout<<std::endl;

    std::cout<<"Transition Functions: \n";
    for(auto transition: transition_functions){
        transition.log();
    }

}

PDA_Transition::PDA_Transition(std::string str){
    std::vector<std::string> temp = split(str, ' ');
    if (temp.size() != 5){
        if(DEBUG) std::cerr<<"Error: Invalid transition"<<std::endl;
        make_syntax_error();
    }
    q1 = temp[0];
    inputa = string_to_char(temp[1]);
    stackz = string_to_char(temp[2]);
    q2 = temp[3];
    stackz1 = temp[4];
}

void PDA_Transition::log(){
    std::cout<<"Transition: ";
    std::cout<<"delta("<<q1<<", "<<inputa<<", "<<stackz<<") = ";
    std::cout<<"("<<q2<<", "<<stackz1<<")"<<std::endl;
}

void PDA::print_state(int step, std::string input, int idx){
    std::cout<<"Step: "<<step<<std::endl;

    std::cout<<"Input: "<<input<<std::endl;
    std::cout<<"       ";
    for (int i = 0; i < idx; i++){
        std::cout<<" ";
    }
    std::cout<<"^"<<std::endl;

    std::cout<<"State: "<<current_state<<std::endl;

    std::cout<<"Stack: ";
    std::stack<char> temp = stack;
    while(!temp.empty()){
        std::cout<<temp.top();
        temp.pop();
    }
    std::cout<<std::endl;

    std::cout<<"---------------------------------------------"<<std::endl;    
}


bool PDA::can_trans(PDA_Transition transition, char inputch){
    return transition.q1 == current_state && transition.inputa == inputch && transition.stackz == stack.top();
}
bool PDA::trans(PDA_Transition transition){
    if(stack.empty()){
        return false;
    }
    stack.pop();
    current_state = transition.q2;
    if (!(transition.stackz1.size()==1 && transition.stackz1[0] == '_')){
        for(int i = transition.stackz1.size()-1; i >= 0; i--){
            stack.push(transition.stackz1[i]);
        }
    }
    if (is_verbose){
        transition.log();
    }
    return true;
}

void PDA::make_false(std::string message){
    if(is_verbose){
        std::cout<<"Result: false"<<std::endl;
        std::cout<<"==================== END ===================="<<std::endl;
    }
    else{
        std::cout<<"false"<<std::endl;
    }
    if(DEBUG) std::cout<<"Reason: "<<message<<std::endl;
}


void PDA::execute(std::string input){
    int step = 0;
    if(is_verbose){
        std::cout<<"Input: "<<input<<std::endl;
    }
    for(auto ch : input){
        if (!check_char_in_charset(ch, input_symbols_S)){
            if(is_verbose){
                std::cerr<<"==================== ERR ===================="<<std::endl;
                std::cerr<<"error: '"<<ch<<"' was not declared in the set of input symbols"<<std::endl;
                std::cerr<<"Input: "<<input<<std::endl;
                std::cerr<<"       ";
                for(int j=0; j<input.find(ch); j++){
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
    
    current_state = start_state_q0;
    stack.push(start_stack_symbol_z0);

    if(is_verbose){
        std::cout<<"==================== RUN ===================="<<std::endl;
        print_state(step, input, 0);
    }

    // 输入符号为_表示空转移，栈符号为_表示空栈符号，空转移和正常转移只能找到一个
    int idx = 0;
    while(idx < input.size()){
        char ch = input[idx];
        bool found = false;
        for(auto transition: transition_functions){
            if((can_trans(transition, ch) && ++idx) || can_trans(transition, '_')){
                if(!trans(transition)){
                    make_false("stack is empty");
                    return;
                }
                found = true;
                if(is_verbose){
                    step++;
                    print_state(step, input, idx);
                }
                break;
            }
        }
        if (!found){
            make_false("can not find transition");
            return;
        }
    }
    while(!check_string_in_stringset(current_state, final_states_F)){
        
        bool found = false;
        for(auto transition: transition_functions){
            if (can_trans(transition, '_')){
                if(!trans(transition)){
                    make_false("stack is empty with final transition");
                    return;
                }
                found = true;
                if (is_verbose){
                    step++;
                    print_state(step, input, idx);
                }
                break;
            }
        }
        if(!found){
            make_false("input end with no-final state");
            return;
        }
    }
    if(is_verbose){
        std::cout<<"Result: true"<<std::endl;
        std::cout<<"==================== END ===================="<<std::endl;
    }
    else{
        std::cout<<"true"<<std::endl;
    }
}

