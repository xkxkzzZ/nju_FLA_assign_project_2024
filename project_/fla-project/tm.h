
#include "utils.h"

class TM_Transition{
    public:
    // <旧状态> <旧符号组> <新符号组> <⽅向组> <新状态>
        std::string old_state;
        // 组均有 size = N
        std::string old_symbols; 
        std::string new_symbols; 
        // 通配符 * 匹配除空格符号外的任意符号，旧*新y为y, 旧x新*为x

        std::string directions; // 由 l, r, * 组成, 左移，右移，不动
        std::string new_state;

        TM_Transition(std::string str);
        void log();
};


class Tape{
    public:
        Tape(std::string input, char blank_symbol);
        char read();
        void write(char symbol);
        void move(char direction);
        std::string get_tape_content();
        void log(int my_idx);

        std::vector<char> tape;
        int head;
        char blank_symbol;
        std::vector<int> index;
};

class TM{
    public:
        TM(std::string filename, bool verbose);
        void execute(std::string input); 
        void log();
    private:
        std::set<std::string> states_Q;
        std::set<char> input_symbols_S;
        std::set<char> tape_symbols_G;
        std::string start_state_q0;
        char blank_symbol_B;    //在本实验中，规定 #B = _
        std::set<std::string> final_states_F;
        int number_of_tapes_N;
        std::vector<TM_Transition> transition_functions;

        bool is_verbose;
        std::string current_state;
        std::vector<Tape> tapes;

        bool can_trans(TM_Transition transition);
        void trans(TM_Transition transition);
        void print_state(int step);

};