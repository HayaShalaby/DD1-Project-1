
#include "Library.h"
#include <stack>
#include <sstream>

Library::Library(string libFile, Circuit * circuit) {
    currCirc = circuit;
    readLib(libFile);
}

Library::~Library() {};

void Library::readLib(string libFile) {
    string line, name, opr, IS, D;
    int inputSize, delay;
    ifstream inFile(libFile);

    //Parsing .lib file line by line into gate attributes
    while (getline(inFile, line)) {
        stringstream ss(line);
        getline(ss, name, ',');
        getline(ss, IS, ',');
        inputSize = stoi(IS);
        getline(ss, opr, ',');
        getline(ss, D, ',');
        delay = stoi(D);

        //Creating a Gate object & adding it to the vector that stores the library components
        Gate g(name, inputSize, opr, delay);
        libComps.push_back(g);
    }

    inFile.close();
}

int Library::getDelay(string gateType) {
    //Looping through the library components until a match to the given gate is found
    for (int i = 0; i < libComps.size(); i++) {
        if (libComps[i].name == gateType)
            return libComps[i].delay;
    }
    return -1;
}

int Library::getInputSize(string gateType) {
    //Looping through the library components until a match to the given gate is found
    for (int i = 0; i < libComps.size(); i++) {
        if (libComps[i].name == gateType)
            return libComps[i].inputSize;
    }
    return -1;
}

string Library::getOperation(string gateType) {
    //Looping through the library components until a match to the given gate is found
    for (int i = 0; i < libComps.size(); i++) {
        if (libComps[i].name == gateType)
            return libComps[i].operation;
    }

    return "";
}

int Library::precedence(char op) {
    //NOT has the highest precedence in logic operations, followed by AND and OR
    if (op == '~')
        return 1;
    if (op == '&' || op == '|')
        return 2;
    return 0;
}

int Library::applyOp(bool in1, bool in2, char op) {
    //Applies the arithmetic operations equivalent to the logic operators
    switch (op) {
    case '&': return in1 * in2;
    case '|': return in1 + in2;
    }
    return -1;
}


int Library::operStack(string operation, int inputSize, vector<Signal> inputs) {
    //Index for loops
    int i;

    //Stack to store bool values
    stack <bool> values;

    //Stack to store operators
    stack <char> ops;

    int s = operation.length();

    //This flag is set to true if NOT is applied to an individual input not a bracket, is checked later on when pushing that input onto the 'values' stack
    //Needed for correct evaluation of the operation expression
    bool notFlag = false;

    //Looping over every character in the operation expression
    for (i = 0; i < operation.length(); i++) {

        //Current char is a whitespace, skip it
        if (operation[i] == ' ')
            continue;

        //Current char is an opening brace, push it to 'ops'
        else if (operation[i] == '(') {
            ops.push(operation[i]);
        }

        //Current char is 'i', substitute in an input value & push it to 'values'
        else if (operation[i] == 'i') {
            int index = int(operation.at(i + 1) - '0') - 1 ;
            bool val = inputs[index].value;
            //If a NOT was applied to this input
            if (notFlag)
            {
                values.push(!val);
                notFlag = false;
            }
            else
                values.push(val);


            i++;
        }

        //Closing brace encountered, solve entire brace
        else if (operation[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                char op = ops.top();
                ops.pop();

                if (op == '~') {
                    bool val = values.top();
                    values.pop();

                    values.push(!val);
                }
                else {
                    bool val2 = values.top();
                    values.pop();

                    bool val1 = values.top();
                    values.pop();

                    values.push(applyOp(val1, val2, op));
                }

            }

            //Pop opening brace
            if (!ops.empty())
                ops.pop();
        }

        //Current token is an operator
        else
        {
            if (operation[i] == '~') {
                if (operation[i + 1] == 'i')
                    notFlag = true;
                else
                    ops.push(operation[i]);
            }
            else
                ops.push(operation[i]);
        }
    }

    //Entire expression has been parsed at this point, apply remaining ops to remaining values
    while (!ops.empty()) {
        char op = ops.top();
        ops.pop();

        if (op == '~') {
            bool val = values.top();
            values.pop();

            values.push(!val);
        }
        else {
            bool val2 = values.top();
            values.pop();

            bool val1 = values.top();
            values.pop();

            values.push(applyOp(val1, val2, op));
        }
    }

    //Top of 'values' contains result, return it
    return values.top();
}



bool Library::logicChange(int index) {
    //Getting all needed info using Circuit and Library member functions
    string gateType = currCirc->getType(index);
    vector<Signal> inputs = currCirc->getInputs(index);
    Signal output = currCirc->getOutput(index);

    string exp = getOperation(gateType);
    int inputSize = getInputSize(gateType);
    //Calling operStack to get result of logic operation
    bool result = operStack(exp, inputSize, inputs);


    //Creating an instance of Signal to store result
    Signal changedOut;
    changedOut.name = "C_O";
    changedOut.value = result;
    //Checking if a logic change occurred and setting the new output value if yes
    if (output.value == result)
        return false;
    else
    {
        currCirc->setOutput(index, changedOut);
        return true;
    }
}

int Library::setLogic(int index) {
    string gateType = currCirc->getType(index);
    vector<Signal> inputs = currCirc->getInputs(index);
    Signal output = currCirc->getOutput(index);

    string exp = getOperation(gateType);
    //Gate not found error
    if(exp == "") {
        cout << gateType << " could not be found in the Library." <<endl;
        return -1;
    }    
    int inputSize = getInputSize(gateType);
    bool result = operStack(exp, inputSize, inputs);
    int delay = getDelay(gateType);
    currCirc->setDelay(index, delay);

    int numGates = currCirc->getGateNum();
    Signal changedOut;
    changedOut.name = "C_O";
    changedOut.value = result;
    //if the value of the output is different, change it
    if (output.value != result)
    {
        //set the output of the gate to the correct value
        currCirc->setOutput(index, changedOut);
        //variable to store the inputs if each gate
        vector<Signal>* inputsPtr;

        //loop over all the gates
        for (int i = 0; i < numGates; i++)
        {
            //get the inputs of each gate and the number of the inputs
            inputsPtr = currCirc->getInputsPtr(i);
            inputSize = inputsPtr->size();
            //for every input, if the name is the same as that of our output, change the value
                for (int j = 0; j < inputSize; j++)
                {
                    if (output.name == inputsPtr->at(j).name)
                        inputsPtr->at(j).value = result;
                }
        }
    }

    return 1;
}