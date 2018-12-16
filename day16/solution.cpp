#include <array>
#include <iostream>
#include <regex>
#include <vector>
#include <functional>
#include <set>

typedef std::array<int, 4> registers;
typedef std::array<int, 4> opcode;

std::array<std::function<registers(registers, opcode)>, 16> opcodes = {
    [](auto r, auto o){ r[o[3]] = r[o[1]] + r[o[2]]; return r;},  // addr
    [](auto r, auto o){ r[o[3]] = r[o[1]] + o[2]; return r;},     // addi
    [](auto r, auto o){ r[o[3]] = r[o[1]] * r[o[2]]; return r;},  // mulr
    [](auto r, auto o){ r[o[3]] = r[o[1]] * o[2]; return r;},     // muli
    [](auto r, auto o){ r[o[3]] = r[o[1]] & r[o[2]]; return r;},  // banr
    [](auto r, auto o){ r[o[3]] = r[o[1]] & o[2]; return r;},     // bani
    [](auto r, auto o){ r[o[3]] = r[o[1]] | r[o[2]]; return r; }, // borr
    [](auto r, auto o){ r[o[3]] = r[o[1]] | o[2]; return r;},     // bori
    [](auto r, auto o){ r[o[3]] = r[o[1]]; return r;},            // setr
    [](auto r, auto o){ r[o[3]] = o[1]; return r; },              // seti
    [](auto r, auto o){ r[o[3]] = o[1] > r[o[2]]; return r;},     // gtir
    [](auto r, auto o){ r[o[3]] = r[o[1]] > o[2]; return r;},     // gtri
    [](auto r, auto o){ r[o[3]] = r[o[1]] > r[o[2]]; return r;},  // gtrr
    [](auto r, auto o){ r[o[3]] = o[1] == r[o[2]]; return r;},    // eqir
    [](auto r, auto o){ r[o[3]] = r[o[1]] == o[2]; return r;},    // eqri
    [](auto r, auto o){ r[o[3]] = r[o[1]] == r[o[2]]; return r;}  // eqrr
};



// parse first part of data as vector of
// [registers_before[4], opcode[4], reg_after[4]]
std::vector<std::array<std::array<int, 4>, 3> > parsePart1(){
    std::vector<std::array<std::array<int, 4>, 3> > res;
    std::string line;
    std::regex re_reg("\\[(.*), (.*), (.*), (.*)\\]");
    std::smatch matches;
    while(true){
            // registers before
            std::getline(std::cin, line);
            if(line == "")
                break;
            std::regex_search(line, matches, re_reg);
            res.emplace_back();
            for(int i = 0; i < 4; i++){
                res.back()[0][i] = std::atoi(matches[i+1].str().c_str());
            }
            // opcode
            for(int i = 0; i < 4; i++)
                std::cin >> res.back()[1][i];
            std::getline(std::cin, line);

            // registers after
            std::getline(std::cin, line);
            std::regex_search(line, matches, re_reg);
            for(int i = 0; i < 4; i++){
                res.back()[2][i] = std::atoi(matches[i+1].str().c_str());
            }
            // empty line
            std::getline(std::cin, line);
    }
    return res;
}

// parse program as vector of opcodes[4]
std::vector<opcode> parsePart2(){
    std::vector<opcode> res;
    int code;
    while(std::cin >> code){
        res.emplace_back();
        res.back()[0] = code;
        for(int i = 1; i < 4; i++)
            std::cin >> res.back()[i];
    }
    return res;
}


// recursively erase option from all sets
void eraseOptions(std::array<std::set<int>, 16>& options, int i){
    if(options[i].size() == 1){
        int uniq = *options[i].begin();
        for(int j = 0; j < options.size(); j++){
            if(j != i){
                if(options[j].erase(uniq))
                    eraseOptions(options, j);
            }
        }
    }
}

// deduce lookup table
std::array<int, 16>
getLookupTable(std::vector<std::array<std::array<int, 4>, 3>> data){
    std::array<std::set<int>, 16> options;
    for(auto& s: options)
        for(int i =0; i < 16 ;i++)
            s.emplace(i);

    while(std::any_of(options.begin(), options.end(),
            [](auto& x){return x.size() > 1;}))
    {
        for(auto state : data){
            int opcode = state[1][0];
            if(options[opcode].size()  == 1)
                continue; // opcode already known

            for(int i = 0; i < opcodes.size(); i++){
                auto res = opcodes[i](state[0], state[1]);
                if(res != state[2]){
                    options[opcode].erase(i);
                    eraseOptions(options, opcode);
                }
            }
        }
    }

    std::array<int, 16> table;
    for(int i = 0; i < table.size(); i++){
        table[i] = *options[i].begin();
    }
    return table;
}

// get count of instructions that behave like three or more opcodes
int test_opcode(std::array<std::array<int, 4>, 3>& states){
    int count = 0;
    for(auto fun : opcodes){
        auto res = fun(states[0], states[1]);
        if(res == states[2])
            count++;
    }

    return count;
}

// run the program and return state of registers
registers runProgram(std::vector<opcode> program, std::array<int, 16> table){
    registers state = {0,0,0,0};
    for(auto instruction : program){
        state = opcodes[table[instruction[0]]](state, instruction);
    }
    return state;
}


int main() {
    auto data = parsePart1();
    int count = 0;
    for(auto state : data){
        if(test_opcode(state) >= 3)
            count++;

    }
    std::cout << "Part 1: " << count << std::endl;
    auto table = getLookupTable(data);
    auto opcodes = parsePart2();
    auto res = runProgram(opcodes, table);
    std::cout << "Part 2" << res[0] << std::endl;
    return 0;
}
