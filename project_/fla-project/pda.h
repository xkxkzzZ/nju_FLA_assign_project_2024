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

#include <stack>
#include "utils.h"


class PDA_Transition{
    public:
        PDA_Transition(std::string str);
        void log();

        std::string q1;
        char inputa;
        char stackz;
        std::string q2;
        std::string stackz1;

};

class PDA{
    public:
        PDA(std::string filename, bool verbose);
        void execute(std::string input); 

        void log();
        void print_state(int step, std::string input, int idx);

    private:
        std::set<std::string> states_Q;
        std::set<char> input_symbols_S;
        std::set<char> stack_symbols_G;
        std::string start_state_q0;
        char start_stack_symbol_z0;
        std::set<std::string> final_states_F;
        std::vector<PDA_Transition> transition_functions;

        bool is_verbose;
        std::stack<char> stack;
        std::string current_state;

        bool can_trans(PDA_Transition transition, char inputch);
        bool trans(PDA_Transition transition);

        void make_false(std::string message);
        
};

