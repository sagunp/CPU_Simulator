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

struct operands
{
    unsigned int rightHalf: 8; 
    unsigned int leftHalf: 8;
};

union operations
{
    unsigned int singular : 8;
    trapOperations trapO;
    registerOperations registerO;
    regularOperations regularO;
};

union operand
{
    unsigned int whole : 16;
    operands divisible;
};

string memory[1000];
int16_t registers[4] = {0, 0, 0, 0};

void printRegisters()
{
    cout << "accumulator: " << registers[0] << endl;
    cout << "index register: " << registers[1] << endl;
    cout << "program counter: " << registers[2] << endl;
    cout << "stack pointer: " << registers[3] << endl;
    cout<<endl;
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

string convertIntToHex(int candidate){
    int integer = candidate;

    char remainChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    string answer = "";

    while(integer > 0){
        int remainder = integer % 16;
        integer = integer / 16;

        answer.insert(0, 1, remainChars[remainder]);
    }

    return answer;
}


void string_tokenizer(string s){
    stringstream ss(s);
    string word;
    
    int i = 0; 
    while (ss >> word) {
        if(word != "zz") {
            memory[i] = word;
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

void rotateL(int r){
    int16_t a = registers[r] << 1;
    int16_t b = registers[r] >> 15;

    registers[r] = a | b;
}

void rotateR(int r){
    int16_t a = registers[r] >> 1;
    int16_t b = registers[r] << 15;

    registers[r] = a | b;
}

void deciTrap(int operandInt){
    int decimal;
    cout<<"Enter the decimal for input: "<<endl;
    cin>>decimal;

    memory[operandInt] = convertIntToHex(decimal);
}

void decoTrap(int addressing, int operandInt){
    if(addressing == 0) {
        cout<<"The immediate decimal output is: "<<operandInt<<endl;
    }else{
        int output = convertToDecimal(memory[operandInt]);
        cout<<"The direct decimal output is: "<<output<<endl;
    }
}

void chariTrap(int operandInt){
    char character; 
    cout<<"Enter the character for input: "<<endl;
    cin>>character;

    int ascii = character;
    memory[operandInt] = convertIntToHex(ascii);
}

void charoTrap(int addressing, int operandInt){
    if(addressing == 0){
        cout<<"The immediate character output is: "<<char(operandInt)<<endl;
    }else{
        string hexcode = memory[operandInt];
        int ascii = convertToDecimal(hexcode);
        
        cout<<"The direct character output is: "<<char(ascii)<<endl;
    }
}

void addToR(int r, int addressing, int16_t operandInt){
    if(addressing == 0){
        registers[r] += operandInt;
    }
    else {
        string hexcode = memory[operandInt];
        int16_t candidate = convertToDecimal(hexcode);
        registers[r] += candidate;
    }
}

void subtractFromR(int r, int addressing, int16_t operandInt){
    if(addressing == 0){
        registers[r] -= operandInt;
    }else{
        string hexcode = memory[operandInt];
        int16_t candidate = convertToDecimal(hexcode);
        registers[r] -= candidate;
    }
}

void bitwiseAndR(int r, int addressing, int16_t operandInt){
    if(addressing == 0){
        int16_t candidate = registers[r] & operandInt;
        registers[r] = candidate;
    }else{
        string hexcode = memory[operandInt];
        int16_t decimalValue = convertToDecimal(hexcode);
        int16_t candidate = registers[r] & decimalValue;
        registers[r] = candidate;
    }
}

void bitwiseOrR(int r, int addressing, int16_t operandInt){
    if(addressing == 0){
        int16_t candidate = registers[r] | operandInt;
        registers[r] = candidate;
    }else{
        string hexcode = memory[operandInt];
        int16_t decimalValue = convertToDecimal(hexcode);
        int16_t candidate = registers[r] | decimalValue;
        registers[r] = candidate;
    }
}

void loadR(int r, int addressing, int16_t operandInt){
    if(addressing == 0){
        registers[r] = operandInt;
    }else{
        string hexcode = memory[operandInt];
        registers[r] = convertToDecimal(hexcode);
    }
}

void storeR(int r, int16_t operandInt){
    int16_t decimal = registers[r];
    string candidate = convertIntToHex(decimal);
    memory[operandInt] = candidate;
}

void loadByteR (int r, int addressing, int16_t operandInt){
    operand forImmediate;
    operand forDirect;
    operand forRegister;

    if(addressing == 0){
        forImmediate.whole = operandInt;
        forRegister.whole = registers[r];

        forRegister.divisible.rightHalf = forImmediate.divisible.rightHalf;
        registers[r] = forRegister.whole; 
    }
    else{
        string memoryOutput = memory[operandInt];
        int16_t decimal = convertToDecimal(memoryOutput);

        forDirect.whole = decimal;
        forRegister.whole = registers[r];

        forRegister.divisible.rightHalf = forDirect.divisible.rightHalf;
        registers[r] = forRegister.whole;
    }
}

void storeByteR(int r, int16_t operandInt){
    operand forRegister;
    operand forMemory;

    string memoryOutput = memory[operandInt];
    int16_t decimal = convertToDecimal(memoryOutput);

    forMemory.whole = decimal;
    forRegister.whole = registers[r];

    forMemory.divisible.rightHalf = forRegister.divisible.rightHalf;
    
    memory[operandInt] = convertIntToHex(forMemory.whole);
}

void compute (int PC, int opcode, string operandString){
    operations operation; 
    operation.singular = opcode;
    registers[2] = PC;

    int16_t operandInt = convertToDecimal(operandString);

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
                rotateL(r);
                break;
            case 17: 
                rotateR(r);
                break;
            default:
                cout<<"Some issues on register intructions with program number: "<<PC<<endl;
                break;
        }

    }
    else if(operation.trapO.trapInstruction >= 6 && operation.trapO.trapInstruction <= 10){
        int instruction = operation.trapO.trapInstruction;
        int addressing = operation.trapO.aaa;
        
        switch(instruction){
            case 6: 
                deciTrap(operandInt);
                break;
            case 7:
                decoTrap(addressing, operandInt);
                break;
            case 9: 
                chariTrap(operandInt);
                break;
            case 10:
                charoTrap(addressing, operandInt);
                break;
            default:
                cout<<"Some issues on trap intructions with program number: "<<PC<<endl;
                break;
        }

    }
    else {
        int instruction = operation.regularO.opcode;
        int addressing = operation.regularO.aaa;
        int r = operation.regularO.r;

        switch(instruction){
            case 7:
                addToR(r, addressing, operandInt);
                break;
            case 8:
                subtractFromR(r, addressing, operandInt);
                break;
            case 9:
                bitwiseAndR(r, addressing, operandInt);
                break;
            case 10:
                bitwiseOrR(r, addressing, operandInt);
                break;
            case 12:
                loadR(r, addressing, operandInt);
                break;
            case 13: 
                loadByteR(r, addressing, operandInt);
                break;
            case 14:
                storeR(r, operandInt);
                break;
            case 15:
                storeByteR(r, operandInt);
                break;
            default:
                cout<<"Some issues on regular intructions with program number: "<<PC<<endl;
                break;
        }
    }
}


int main()
{
    registers[3] = 900;
    string input;
    cout<<"Please type in valid commands in this format:command1 commandN 00 hexData1 hexData2 hexDataN zz. For Example:C10003 1C 00 7 8 9 zz."<<endl;
    getline (cin, input);

    string_tokenizer(input);
    cout<<"instruction loop start"<<endl;
    

    int i = 0; 
    while(memory[i] != "00"){
        string operation = memory[i];
        string opcodeString = operation.substr(0, 2);
        string operandString = "0000";

        if(operation.length() == 6) operandString = operation.substr(2, 6);
        int opcode = convertToDecimal(opcodeString);

        compute(i, opcode, operandString);
        printRegisters();
        i++;
    }
    return 0;
}
