#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <math.h>

using namespace std;

struct trapOperations
{
    unsigned int aaa : 3;
    unsigned int trapInstruction : 5;
};

struct registerOperations
{
    unsigned int r : 1;
    unsigned int registerInstruction : 7;
};

struct regularOperations
{
    unsigned int aaa : 3;
    unsigned int r : 1;
    unsigned int opcode : 4;
};

union operations
{
    unsigned int singular : 8;
    trapOperations trapO;
    registerOperations registerO;
    regularOperations regularO;
};



string memory[1000];
int16_t registers[4] = {0, 0, 0, 0};

void printRegisters()
{
    cout << "accumulator: " << registers[0] << endl;
    cout << "index register: " << registers[1] << endl;
    cout << "program counter: " << registers[2] << endl;
    cout << "stack pointer: " << registers[3] << endl;
}

int convertToDecimal(string hexcode)
{
    // int result = 0;

    // map<char, int> values = {
    //     {'0', 0}, 
    //     {'1', 1}, 
    //     {'2', 2}, 
    //     {'3', 3},
    //     {'4', 4}, 
    //     {'5', 5}, 
    //     {'6', 6}, 
    //     {'7', 7},
    //     {'8', 8}, 
    //     {'9', 9}, 
    //     {'A', 10}, 
    //     {'B', 11},
    //     {'C', 12}, 
    //     {'D', 13}, 
    //     {'E', 14}, 
    //     {'F', 15},
    // };

    // for(int i = instruction.length() - 1; i >= 0; i++){
    //     result += values.at(instruction[i]) * pow(16, i);
    // }
    // return result;
    return stoul(hexcode, nullptr, 16);
}


void string_tokenizer(string s){
    stringstream ss(s);
    string word;
    
    int i = 0; 
    while (ss >> word) {
        if(word != "zz") {
            memory[i] = word;
            cout<<memory[i]<<endl;
        }
        i++;
    }
}

void invert(int r){
    registers[r] = ~(registers[r]);
}

void ASL(int r){
    registers[r] = registers[r] << 1;
}

void ASR(int r){
    registers[r] = registers[r] >> 1;
}

void LSL(int r){
    int16_t a = registers[r] << 1;
    int16_t b = registers[r] >> 15;

    registers[r] = a | b;
}

void LSR(int r){
    int16_t a = registers[r] >> 1;
    int16_t b = registers[r] << 15;

    registers[r] = a | b;
}



void compute (int PC, int opcode, int operand){
    operations operation; 
    operation.singular = opcode;
    registers[2] = PC;

    if(operation.registerO.registerInstruction >= 12 && operation.registerO.registerInstruction <= 17){
        int instruction = operation.registerO.registerInstruction;
        int r =  operation.registerO.r;

        switch(instruction){
            case 12: 
                invert(r);
                break;
            case 14:
                ASL(r);
                break;
            case 15:
                ASR(r);
                break;
            case 16:
                LSL(r);
                break;
            case 17: 
                LSR(r);
                break;
            default:
                cout<<"Some issues on register intructions"<<endl;
                break;
        }

    }
    else if(operation.trapO.trapInstruction >= 6 && operation.trapO.trapInstruction <= 10){

    }
    else {

    }
}


int main()
{
    registers[3] = 900;
    string input;
    cout<<"Please type in the commands separated by spaces. For Example: 1F2ED5 1CEE98 00 7 zz."<<endl;
    getline (cin, input);

    string_tokenizer(input);

    int i = 0; 

    while(memory[i] != "00"){
        string operation = memory[i];
        string opcodeString = operation.substr(0, 2);
        string operandString = operation.substr(2, 4);

        int opcode = convertToDecimal(opcodeString);
        int operand = convertToDecimal(operandString);

        compute(i, opcode, operand);
        printRegisters();
        i++;
    }

    return 0;
}
