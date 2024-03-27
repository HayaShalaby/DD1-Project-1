#pragma once
#include "Circuit.h"
#include "Circuit.cpp"
#include <stack>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Struct to create a type Gate and store all gate attributes
struct Gate {
    string name;
    //No. of inputs
    int inputSize;
    string operation;
    int delay;

    //Gate struct constructor
    //Initializes member variables with zeros/empty strings unless values are provided
    Gate(string gateName = "", int inSize = 0, string opr = "", int gateDelay = 0) : name(gateName), inputSize(inSize), operation(opr), delay(gateDelay) {}
};


//Class to create a type Library and manage all .lib file attributes
class Library {
    //Vector that will store all the library components
    vector<Gate> libComps;
    //An object of class Circuit (a circuit)
    Circuit* currCirc;
public:
    //Constuctor
    //Takes the .lib file name and calls the readLib function to read and parse it
    Library(string libFile, Circuit* circuit);
    //Destructor
    ~Library();
    //Parses data from .lib file
    void readLib(string libFile);
    //Getter for a gate's num. of inputs
    int getInputSize(string gateType);
    //Getter for a gate's delay
    int getDelay(string gateType);
    //Getter for a gate's operation expression
    string getOperation(string gateType);
    //Determines precedence of given operator
    int precedence(char op);
    //Applies given operator on the two given bool values
    int applyOp(bool in1, bool in2, char op);
    //Evaluates a given gate operation expression and returns result
    //Uses the functions precedence and applyOp
    int operStack(string operation, int inputSize, vector<Signal> inputs);
    //Uses operStack to update a circuit's logic values and checks if a logic change occured
    bool logicChange(int index);
    //Sets the logic of gates at the beginning
    int setLogic(int index);
};