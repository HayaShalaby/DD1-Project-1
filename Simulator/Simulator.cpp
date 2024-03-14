#include "Library.h"
#include "Library.cpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>
#include "Circuit.h"
using namespace std;

// ## HAYA BEGIN
// Input Library Path
// Read the library file
// Store contents in Library "as 2D array"
// Split the input according to different values
//   Library has : gateType, inputsNum, outputExpressions, delay

// Write a stack function that translates the expression & | ~
//## HAYA END

//## YASMINA BEGIN
// Create a class / struct called 'Components'
// Name, Type, Output, Inputs[list of structs]

// struct A { 
//    "Value": 0
// }
	// A["Value"] = 1
	// print(A["Value"])

// ## YASMINA END

//##YASMINA START
	// Create a class / struct called 'Circuit'
	//    it has inputs[], Gates[], 
	//		as function that gets a log and returns it to main program
	//       log file of what output go into which GATES(to look up when we modify the wires nd delay)
	//       use the list.append() method(as if it's a vector)

		// Input Circuit Path
		// Read Circuit from File and update the data and send the log to the main file
		// Create gate instances/objects



		// Log file vector<vector>:
//  Input, components[]
//##YASMINA END


// // // // KEEP TRACK OF TIME // // // //
// // // At the beginning all wires and inputs are set to zero
// Time = 0
// Input Stimuli Path


//## DOES NOT MAKE SENSE OUR MINHEAP EXPERT RANA WILL FIX IT WITH A MIN HEAP
// while loop till minheap size = 0
// 
// // Read stimuli file and Create a list of objects "name" of the format[time, signalName, value]
// Loop increment by 50 ps
//    until reaches time in the first input
//    checks the log for the value of the input
//    if the value of the input doesn't change, nothing
//    if it changes,
//       Loop through the gates
//          We compare componentType to gateName(library)
//          And them figure out the change that will happen to which outputs
//			if it doesn't change, ignore
//			else put it in a similar object[time, signal, value]
//          Do a depth first update

// POSSIBLE ISSUE // // // Check if this might put issues with chronological order

//       Create a minheap of the time objects

//       Output it to file

//  GUIs

// Note to self : always try to use data structure we learned

// Compare the currentTime + Delay and the time of the next gate



// Extra feature : Real time simulation

// this is used for the comparision to create the Minheap simOrder
auto cmp = [](const pair<int,Signal>& a, const pair<int,Signal>& b)
{
    return a.first > b.first;
};


int main()
{
    ifstream read; // used to read from stimuli file
    ofstream write; // writes to the simulation file
    string timelapse,input,value;
    pair<int,Signal> element,test; // these are pairs of timelapse and the input with its value
    string stimulifile="circuit 3 stim.txt",simfile;
    read.open(stimulifile); // this opens the stimulifile
    write.open(simfile); // this opens the simulation file
    priority_queue<pair<int,Signal>,vector<pair<int,Signal>>,decltype(cmp)> simOrder(cmp); // this is a minheap that will store all the signals with their timelapse in the ascending order of timelapse
    Circuit mycircuit("Circuit 3.txt"); // creates the circuit using the circuit file
    vector<pair<Signal,int>> log; // log that will be used in the comparisions in the minheap

    // in the following while loop it reads the stimuli file and inputs the timelapse and the signal which contains the name of the input and its value into the minHeap simOrder
    while (getline(read, timelapse, ',')) // here it seperates the three values by commas
    {
        try {
            size_t pos;
            element.first = stoi(timelapse, &pos); // inputs the timelapse
            if (pos < timelapse.length()) {
                throw invalid_argument("Invalid integer format");
            }
        } catch (const exception& e) {
            cout << "Error converting string to integer: " << e.what() << endl;
            cout << "String value: " << timelapse << endl;
            return 1;
        }

        try {
            getline(read, input, ','); // inputs the name of the signal
            element.second.name = input;
            try {
                getline(read, value, '\r'); // inputs the value of the given signal
                element.second.value = (stoi(value) != 0) ? true : false;
            } catch (const exception& e) {
                cout << "Error converting string to integer: " << e.what() << endl;
                cout << "String value: " << value << endl;
                return 1;
            }
        } catch (const exception& e) {
            cout << "Error reading input: " << e.what() << endl;
            return 1;
        }

        simOrder.push(element); // this pushes each element that will be outputted to the simulation file into the minHeap
    }

    log=mycircuit.returnLog(); // obtains the log for the given circuit


    while(simOrder.size()!=0) // In this loop we will write the elements from the minheap to the simulation file
    {
        test=simOrder.top(); // reads the element with the shortest timelapse
        write<<test.first<<" "<<test.second.name<<" "<<test.second.value<<endl; // writes it here to the simulation file
        simOrder.pop(); // removes it from the heap

        for(int i=0; i<log.size();i++) // will iterate through the log to check which gate each input affects
        {
            if(test.second==log[i].first) // checks if the current element in the log has the same signal as the current top in the minheap
            {
                mycircuit.setInput(log[i].second,test.second); // changes the input of the given gate
                // output(log[i].second); // calculates the output of the given gate with the new change in its input
                //simOrder.push({test.first+one.getDelay(log[i].second),signal recieved from output}); // pushes the new element which is the output of the given gate connected to the current input that changed its output so that changed output with its timelapse which is the timelapse of its input plus the delay of the given gate plus its new value will be pushed into the minHeap (sortedOrder)
            }
        }

    }

    return 0;


}