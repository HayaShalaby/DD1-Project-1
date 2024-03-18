#pragma once
#include "Circuit.h"
#include "Circuit.cpp"
#include <stack>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Gate {
    string name;
    int inputSize;
    string operation;
    int delay;

    Gate(string gateName = "", int inSize = 0, string opr = "", int gateDelay = 0) : name(gateName), inputSize(inSize), operation(opr), delay(gateDelay) {}
};

class Library {
    vector<Gate> libComps;
    //turned this into a pointer
    Circuit* currCirc;
    string thisLibFile;
public:
    //ADD A CONSTRUCTOR WHICH CALLS READLIB AND POPULATES LIBCOMPS
    //takes 
    Library(string libFile, Circuit* circuit);
    ~Library();
    void readLib(string libFile);
    int getInputSize(string gateType);
    int getDelay(string gateType);
    string getOperation(string gateType);
    int precedence(char op);
    int applyOp(bool in1, bool in2, char op);
    int operStack(string operation, int inputSize, vector<Signal> inputs);
    //    bool operStack(string operation, int inputSize, vector<Signal> inputs);
    bool logicChange(int index);
    //sets the logic of gates at the beginning
    void setLogic(int index);
};