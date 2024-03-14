#include "Circuit.h"

//takes in the path for the circuit and calls readCircuit on it
Circuit::Circuit(string fileName)
{
	readCircuit(fileName);
}

//destructor
Circuit::~Circuit() {};

//reads file and populates inputs: uses populateComponent and create Log
//note that when you read you need to store value in signal as bool
void Circuit::readCircuit(string fileName)
{
	ifstream read(fileName);
	//temp string to store input
	string temp;
	//ignore the "INPUTS" at the beginning of the file
	getline(read, temp);

	//note, cannot compare getline to string
	//loop that reads the inputs
	while (getline(read, temp))
	{
		if (temp == "COMPONENTS:") break;
		//pushes signals into inputs
		//or curly brackets instead?
		Inputs.push_back({ temp, 0 });
	}

	//while there are still gates, populate a new component
	while (getline(read, temp))
	{
		populateComponent(temp);
	}

}

//takes string input, parses it then populates a gate and adds it to the circuit
void Circuit::populateComponent(string & parseInput)
{
	//vector to store parsed substrings
	vector<string> values;
	//vector to store the inputs
	vector<Signal> compInputs;

	//variable to hold position of /,/
	int pos;

	//while string is not empty, get position of /,/
	while (pos = parseInput.find(',') != string::npos)
	{
		//push onto the values vector the substring
		values.push_back(parseInput.substr(0, pos));
		//delete the substring
		parseInput.erase(0, pos + 2);
	}

	//store the inputs (4th value and onwards) in a vector to be sent to the component
	for(auto it = values.begin()+3; it= values.end(); it++)
		compInputs.push_back(Signal{ it*, 0 });

	//get delay from library and put it in int delay
	//maybe static function for that?
	int delay;
	//= Library::getDelay(type)

	//create component using the name, type, delay, output and vector of inputs
	Gates.push_back({ values[0], values[1], delay, { values[2] }, compInputs });
}

//returns the log from the components
const vector<pair<Signal, int>>& Circuit::returnLog()
{
	//vector that stores log of signals and the index of the gates that it changes
	vector<pair<Signal, int>> Log;

	//for every gate in the circuit
	for (int i = 0; i < Gates.size(); i++)
		//for every input in each circuit
		for (int j = 0; j < Gates[i].inputs.size(); j++)
			//store a pair that has this input and the index of the gate that it affects
			Log.push_back({ Gates[i].inputs.at(j), i });
}

//function that returns the index of a gate and a signal and modifies the value of this signal in the input
void Circuit::setInput(int index, const Signal& signal)
{
	//go to the component at the index and loop over all of its inputs
	for (auto& S : Gates[index].inputs)
	{
		//if the name of the input signal matches the signal we want to change
		if (S.name == signal.name)
		{
			//modify the signal value
			S.value = signal.value;
		}
	}
}

//function that returns the index of a gate and a signal and modifies the value of the gate Output
void Circuit::setOutput(int index, const Signal& signal)
{
	//go to the component at the set its value to that of the signal
	Gates[index].output.value = signal.value;
}

//function that returns name of a gate based on the index
const string& Circuit::getName(int index)
{
	return Gates.at(index).name;
}
//function that returns type of a gate based on the index
const string& Circuit::getType(int index)
{
	return Gates.at(index).type;
}
//function that returns delay of gate based on the index
int Circuit::getDelay(int index) 
{
	return Gates.at(index).delay;
}

//function that returns the output of a gate based on the index
Signal Circuit::getOutput(int index)
{
	return Gates.at(index).output;
}
//function that returns the inputs of a gate based on the index
vector<Signal> Circuit::getInputs(int index)
{
	return Gates.at(index).inputs;
}
