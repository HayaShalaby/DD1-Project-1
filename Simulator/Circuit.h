#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

//figure out how to set inputs to gate initially to zerp
struct Signal {
	string name;
	bool value;

	bool operator == (Signal x)
	{
		if (x.name == name && x.value == value)
			return true;
		return false;
	}
};

class Circuit {

	//a component is a "gate"
	struct Component {

		string name;
		string type;
		int delay;
		Signal output;
		vector<Signal> inputs;
	};

	//Do we even need the vector of inputs?
	vector<Signal> Inputs;
	//vector of all the components in the circuit
	vector<Component> Gates;

public:
	//takes in the path for the circuit and calls readCircuit on it
	Circuit(string fileName);
	//destructor
	~Circuit();
	//reads file and populates inputs: uses populateComponent
	//note that when you read you need to store value in signal as bool
	void readCircuit(string);
	//takes string input, parses it then populates and sends back a component
	void populateComponent(string &);
	//returns the log from the components
	const vector<pair<Signal, int>>& returnLog();
	//function that returns the index of a gate and a signal and modifies the value of this signal in the input
	void setInput(int , const Signal& signal);
	//function that returns the index of a gate and a signal and modifies the value of the gate Output
	void setOutput(int, const Signal& signal);
	//function that returns name of a gate based on the index
	const string& getName(int);
	//function that returns type of a gate based on the index
	const string & getType(int);
	//function that returns delay of gate based on the index
	int getDelay(int);
	//function that returns the output of a gate based on the index
	Signal getOutput(int);
	//function that returns the inputs of a gate based on the index
	vector<Signal> getInputs(int);
	
};
