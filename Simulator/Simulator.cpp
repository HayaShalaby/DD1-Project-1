#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>
#include "Circuit.h"
#include "Library.h"
#include "Library.cpp"
#include <map>

using namespace std;


// this is used for the comparision to create the Minheap simOrder
auto cmp = [](const pair<int,Signal>& num_1, const pair<int,Signal>& num_2)
{
    return num_1.first > num_2.first;
};

struct sim {
    string output = "";
    int time = 0;
};

// Function to output the JSON
void JSON(string simFileName, string JSONfile) {

    ifstream read(simFileName);
    ofstream write(JSONfile);
    map<string, sim> waveform;
    string name, value, timeString, temp; int time, timeDiff, squareNum, lastTime;
    

    // typecast to int if it doesnt 
    while (read >> timeString)
    {
        read >> name >> value;

        timeDiff = stoi(timeString) - waveform[name].time;

        //if no value has been initialized to this map, add the opposite of the changed value in the beginning
        if (waveform[name].output == "" && timeString != "0")
        {
            if (value == "0")
                waveform[name].output += "1";
            else waveform[name].output += "0";
        }

        //our interval is 50, We minus one to exclude the last changed value
        squareNum = (timeDiff / 50) - 1;
        
        for (int i = 0; i < squareNum; i++)
        {
            waveform[name].output += ".";
        }

        waveform[name].output += value;

        waveform[name].time = stoi(timeString);

        lastTime = waveform[name].time;
    }


    // iterate over the map
    for (auto it = waveform.begin(); it != waveform.end(); it++)
    {
        //calculate the time difference between the lastTime and the current time of the signal
        timeDiff = lastTime - it->second.time;
        ///calculate the number of squares
        squareNum = (timeDiff / 50);
        
        if (squareNum > 0)
        {
            for (int i = 0; i < squareNum; i++)
            {
                it->second.output += ".";
            }
        }
    }

    //output the JSON
    write << "{ \"signal\" : [\n";
    for (auto it = waveform.begin(); it != waveform.end(); it++)
    {
        write << "{ \"name\": \"" << it->first << "\", \"wave\": \"" << it->second.output << "\" }";
        if (next(it) != waveform.end()) write << ",\n";
        else write << "] }";
    }

}


int main(int argc, char* argv[])
{
        ifstream read; // used to read from stimuli file
        ofstream write; // writes to the simulation file
        string timelapse, input, value;
        pair<int,Signal> element,test,output; // these are pairs of timelapse and the input with its value
        string stimulifile, simfile, circfile, JSONfile, Libfile;
        bool flag=false;
   

        if (argc == 1)
        {
            cout << "Enter the path of the library file" << endl;
            getline(cin, Libfile);
            cout << "\nEnter the path of the circuit file" << endl;
            getline(cin, circfile);
            cout << "\nEnter the path of the stimulifile for that circuit" << endl;
            getline(cin, stimulifile);
            cout << "\nEnter the path of the file you want to output the simulation to" << endl;
            getline(cin, simfile);
            cout << "\nEnter the path of the JSON file you want to output to" << endl;
            getline(cin, JSONfile);
        }
        // for testing purposes 
        else
        {
            Libfile = argv[1];
            circfile = argv[2];
            stimulifile = argv[3];
            simfile = argv[4];
            JSONfile = argv[5];
            cout << "\n";
        }
       


        //Libfile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test/cells.lib";
        //circfile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test/2.cir";
        //stimulifile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test/2.stim";
        //simfile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test/2.sim";
        //JSONfile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test/2.json";
        //
        //"D:\University\5.Spring 2024\Digital Design\Digital Project\DD1-Project-1\Test\1.cir"
        //Libfile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test Circuits/Library.lib";

        read.open(stimulifile); // this opens the stimulifile
        write.open(simfile); // this opens the simulation file
        priority_queue<pair<int,Signal>,vector<pair<int,Signal> >,decltype(cmp)> simOrder(cmp); // this is a minheap that will store all the signals with their timelapse in the ascending order of timelapse
        vector<Signal>inputs;

        Circuit mycircuit(circfile); // creates the circuit using the circuit file
        inputs=mycircuit.getCircuitInputs();
        //tests if an error was returned from the circuit
        if (circfile == "x") return -1;


        vector<pair<Signal,int> >* log; // log that will be used in the comparisions in the minheap
        //passing reference/pointer to library
        Library lib(Libfile,&mycircuit);

        string space;
        // in the following while loop it reads the stimuli file and inputs the timelapse and the signal which contains the name of the input and its value into the minHeap simOrder
        while (getline(read, timelapse, ',')) // here it seperates the three values by commas
        {
            getline(read, space, ' ');

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
                for(int i=0; i<inputs.size(); i++)
                {
                   if(inputs[i].name==input)
                       flag=true;
                }
                if(!flag)
                {
                    cout<<"This input does not exist in the given circuit"<<endl;
                    cout<<"Existing Program....."<<endl;
                    return -3;
                }
                
                getline(read, space, ' ');
                element.second.name = input;
                try {
                    getline(read, value,'\n'); // inputs the value of the given signal
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
        read.close();


        

        //Call lib.setLogic on all the gates

        int gateNums = mycircuit.getGateNum();
        int error;
        for (int i = 0; i < gateNums; i++) // will iterate through the log to check which gate each input affects
        {
            error = lib.setLogic(i);
            //Checking for a library error
            if(error == -2)
                return -2;
        }

        

        log = mycircuit.returnLog(); // obtains the log for the given circuit

        while(simOrder.size()!=0) // In this loop we will write the elements from the minheap to the simulation file
        {
            test=simOrder.top(); // reads the element with the shortest timelapse
            write<<test.first<<" "<<test.second.name<<" "<<test.second.value<<endl; // writes it here to the simulation file
            simOrder.pop(); // removes it from the heap


            for(int i=0; i<log->size();i++) // will iterate through the log to check which gate each input affects
            {
                if(test.second.name == (*log)[i].first.name) // checks if the current element in the log has the same signal as the current top in the minheap
                {
                    
                    bool change;
                        mycircuit.setInput((*log)[i].second,test.second); // changes the input of the given gate
                        change=lib.logicChange((*log)[i].second); // calculates the output of the given gate with the new change in its input

                        output.first=(test.first+mycircuit.getDelay((*log)[i].second)); //the new timelapse of the changed output is calculated
                        output.second.name=mycircuit.getOutputName((*log)[i].second); //the name of the wire taking the specific output is set
                        output.second.value=mycircuit.getOutput((*log)[i].second).value; // the new value of the output signal is calculated

                       if(change) simOrder.push(output); // pushes the new element which is the output of the given gate connected to the current input that changed its output so that changed output with its timelapse which is the timelapse of its input plus the delay of the given gate plus its new value will be pushed into the minHeap (sortedOrder)
                    

                }
            }

        }
        write.close();

        JSON(simfile, JSONfile);


    return 0;
}
