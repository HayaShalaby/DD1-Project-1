#include "Component.h"

// Input Library Path
// Read the library file
// Store contents in Library "as 2D array"
// Split the input according to different values
//   Library has : gateType, inputsNum, outputExpressions, delay

// Write a stack function that translates the expression & | ~

// ### Create a class / struct called 'Components'
// ###  Name, Type, Output, Inputs[list of structs]

// struct A { 
//    "Value": 0
// }
	// A["Value"] = 1
	// print(A["Value"])


	// Create a class / struct called 'Circuit'
	//    it has inputs[], Gates[], 
	//		as function that gets a log and returns it to main program
	//       log file of what output go into which GATES(to look up when we modify the wires nd delay)
	//       use the list.append() method(as if it's a vector)

		// Input Circuit Path
		// Read Circuit from File and update the data and send the log to the main file
		// Create gate instances/objects

		// Log file 2D array:
//  Input, components

// // // // KEEP TRACK OF TIME // // // //
// // // At the beginning all wires and inputs are set to zero
// Time = 0
// Input Stimuli Path


// Read stimuli file and Create a list of objects "name" of the format[time, signalName, value]
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

//  GUI

// Note to self : always try to use data structure we learned

// Compare the currentTime + Delay and the time of the next gate



// Extra feature : Real time simulation

int main()
{

}