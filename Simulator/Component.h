#pragma once
#include<string>
#include <vector>
using namespace std;

struct Signal {
	string name = "";
	bool value = 0;
};

class Component {

	string name;
	string type;
	Signal output;
	//Would it be better to use a dynamic array instead?
	vector<Signal> inputs;

public:
	Component(string value)	{};
	~Component() {};
	// takes string input and populates the gate
	void parseLine(string value);
};