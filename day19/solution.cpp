#include <array>
#include <iostream>
#include <regex>
#include <vector>
#include <functional>
#include <set>
#include <unordered_map>
#include <iomanip>

using registers = std::array<int, 6>;
using parameters = std::array<int, 3>;

struct Opcode{
    std::string instruction;
    parameters param;
};

std::unordered_map<std::string, std::function<registers(registers, parameters)>> opcodes ({
    {"addr", [](auto r, auto o){ r[o[2]] = r[o[0]] + r[o[1]]; return r;}},  // addr
    {"addi", [](auto r, auto o){ r[o[2]] = r[o[0]] + o[1]; return r;}},     // addi
    {"mulr", [](auto r, auto o){ r[o[2]] = r[o[0]] * r[o[1]]; return r;}},  // mulr
    {"muli", [](auto r, auto o){ r[o[2]] = r[o[0]] * o[1]; return r;}},     // muli
    {"banr", [](auto r, auto o){ r[o[2]] = r[o[0]] & r[o[1]]; return r;}},  // banr
    {"bani", [](auto r, auto o){ r[o[2]] = r[o[0]] & o[1]; return r;}},     // bani
    {"borr", [](auto r, auto o){ r[o[2]] = r[o[0]] | r[o[1]]; return r;}},  // borr
    {"bori", [](auto r, auto o){ r[o[2]] = r[o[0]] | o[1]; return r;}},     // bori
    {"setr", [](auto r, auto o){ r[o[2]] = r[o[0]]; return r;}},            // setr
    {"seti", [](auto r, auto o){ r[o[2]] = o[0]; return r; }},              // seti
    {"gtir", [](auto r, auto o){ r[o[2]] = o[0] > r[o[1]]; return r;}},     // gtir
    {"gtri", [](auto r, auto o){ r[o[2]] = r[o[0]] > o[1]; return r;}},     // gtri
    {"gtrr", [](auto r, auto o){ r[o[2]] = r[o[0]] > r[o[1]]; return r;}},  // gtrr
    {"eqir", [](auto r, auto o){ r[o[2]] = o[0] == r[o[1]]; return r;}},    // eqir
    {"eqri", [](auto r, auto o){ r[o[2]] = r[o[0]] == o[1]; return r;}},    // eqri
    {"eqrr", [](auto r, auto o){ r[o[2]] = r[o[0]] == r[o[1]]; return r;}}  // eqrr
});

int parseData(std::vector<Opcode>& result){
    std::string line;
    std::getline(std::cin, line);
    int ip = line[line.size() - 1] - 48;
    result.emplace_back();
    while(std::cin >> result.back().instruction){
        for(int i = 0; i < 3; i++)
            std::cin >> result.back().param[i];
        result.emplace_back();
    }
    return ip;
}

void printRegister(registers& reg){
    for(int i = 0; i < reg.size(); i++){
        std::cout << "[" << i << "] " << std::setfill('0') << std::setw(8) << reg[i] << " "; 
    }
    std::cout << std::endl;
}

registers runProgram(std::vector<Opcode>& program, int ip){
    registers reg = {1,0,0,0,0,0};
    while(reg[ip] < program.size() && reg[ip] >= 0){
        printRegister(reg);
        auto& line = program[reg[ip]];
        reg = opcodes[line.instruction](reg, line.param); 
        reg[ip]++;
        
    }
    return reg;

}


int main(){
    std::vector<Opcode> opcodes;
    int ip = parseData(opcodes);
    auto reg = runProgram(opcodes, ip);
    std::cout << "Part 1: " << reg[0] << std::endl;
} 



