
#include "Library.h"
#include <stack>
#include <sstream>

//static vector<Gate> libComps;

Library::Library(string libFile){
    readLib(libFile);
}

Library::~Library() {};

void Library::readLib(string libFile){
    string line, name, opr, IS, D;
    int inputSize, delay;
//    vector<Gate> libComps;
    ifstream inFile(libFile);

    while(getline(inFile, line)){
//        getline(inFile, line);
        stringstream ss(line);
        getline(ss, name, ',');
        getline(ss, IS, ',');
        inputSize = stoi(IS);
        getline(ss, opr, ',');
        getline(ss, D, ',');
        delay = stoi(D);

        Gate g(name, inputSize, opr, delay);
        libComps.push_back(g);
    }

    inFile.close();
}

int Library::getDelay(string gateType){
    for(int i = 0; i<libComps.size(); i++){
        if(libComps[i].name == gateType)
            return libComps[i].delay;
    }
    return -1;
}

int Library::getInputSize(string gateType){
    for(int i = 0; i<libComps.size(); i++){
        if(libComps[i].name == gateType)
            return libComps[i].inputSize;
    }
    return -1;
}

string Library::getOperation(string gateType){
    for(int i = 0; i<libComps.size(); i++){
        if(libComps[i].name == gateType)
            return libComps[i].operation;
    }
    return "";
}

int Library::precedence(char op){
    if(op == '~')
        return 1;
    if(op == '&'||op == '|')
        return 2;
    //might need to add a third return
    return 0;
}

int Library::applyOp(bool in1, bool in2, char op){
    switch(op){
        case '&': return in1 * in2;
        case '|': return in1 + in2;
    }
    return -1;
}

//should return value be bool or would it not matter??
int Library::operStack(string operation, int inputSize, vector<Signal> inputs){
    int i;
    //not sure if should subtract 1 or not
    int in = inputSize - 1;

    // stack to store bool values.
    stack <bool> values;

    // stack to store operators.
    stack <char> ops;

    for(i = 0; i < operation.length(); i++){

        // Current token is a whitespace,
        // skip it.
        if(operation[i] == ' ')
            continue;

            // Current token is an opening
            // brace, push it to 'ops'
        else if(operation[i] == '('){
            ops.push(operation[i]);
        }

            // Current token is a number, push
            // it to stack for numbers.
        else if(operation[i] == 'i'){
            bool val = inputs[in].value; //sub in input
            values.push(val);
            in--;

            i += 2;
            // right now the i points to
            // the character next to the digit,
            // since the for loop also increases
            // the i, we would skip one
            //  token position; we need to
            // decrease the value of i by 1 to
            // correct the offset.
            i--;
        }

            // Closing brace encountered, solve
            // entire brace.
        else if(operation[i] == ')')
        {
            while(!ops.empty() && ops.top() != '(')
            {   char op = ops.top();
                ops.pop();

                if(op == '~'){
                    bool val = values.top();
                    values.pop();

                    values.push(!val);
                }
                else{
                    bool val2 = values.top();
                    values.pop();

                    bool val1 = values.top();
                    values.pop();

                    values.push(applyOp(val1, val2, op));
                }

            }

            // pop opening brace.
            if(!ops.empty())
                ops.pop();
        }

            // Current token is an operator.
        else
        {
            // While top of 'ops' has same or greater
            // precedence to current token, which
            // is an operator. Apply operator on top
            // of 'ops' to top two elements in values stack.
            while(!ops.empty() && precedence(ops.top())
                                  >= precedence(operation[i])){
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }

            // Push current token to 'ops'.
            ops.push(operation[i]);
        }
    }

    // Entire expression has been parsed at this
    // point, apply remaining ops to remaining
    // values.
        while(!ops.empty()){
            char op = ops.top();
            ops.pop();

            if(op == '~'){
                bool val = values.top();
                values.pop();

                values.push(!val);
            }
            else{
                bool val2 = values.top();
                values.pop();

                bool val1 = values.top();
                values.pop();

                values.push(applyOp(val1, val2, op));
            }
        }

    // Top of 'values' contains result, return it.
    return values.top();
}

//URE TREATING IT AS A POSTFIX EXPRESSION CHANGE INFIX FIRST TO EVALUATE
//ADD ~ IN AN IF BEOFRE THE SWITCH?
//if this character is the first character and also ~ then flag = true and after the brackets are opened and closed flip the result
//bool Library::operStack(string operation, int inputSize, vector<Signal> inputs){
//    //ADD PRECEDENCE ESP BRACKETS
//    stack<string> oper;
//    bool operand, operand1, operand2, result;
//
//
//
//    for (int i = 0; i < operation.length(); i++) {
//        if (operation[i] == 'i') {
//            operand = operation[i] + operation[i + 1];
//            oper.push(operand);
//            i++;
//        } else {
//            switch (operation[i]) {
//                case '&':
//                    operand1 * operand2;
//                    //multiply their logic values
//                    break;
//                case '|':
//                    operand1 + operand2;
//                    //add their logic values
//                    break;
//                case '~':
//                    operand = !operand;
//                    //if operand1 = 1 then 0 else 1
//                    break;
//            }
//        }
//
//    }
//}

bool logicChange(int index){
    string libFile;
    Library lib(libFile);

    string circFile;
    Circuit currCirc(circFile);
    string gateType = currCirc.getType(index);
    vector<Signal> inputs= currCirc.getInputs(index);
    Signal output = currCirc.getOutput(index);

    string exp = lib.getOperation(gateType);
    int inputSize = lib.getInputSize(gateType);
    bool result = lib.operStack(exp, inputSize, inputs);
    int delay = lib.getDelay(gateType);
    currCirc.setDelay(index, delay);

    Signal changedOut;
    changedOut.name = "C_O";
    changedOut.value = result;
    if(output.value == result)
        return false;
    else
    {
        currCirc.setOutput(index, changedOut);
        return true;
    }
}

