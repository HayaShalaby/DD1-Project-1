#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>
#include "Circuit.h"
#include "Library.h"
#include "Library.cpp"

using namespace std;


// this is used for the comparision to create the Minheap simOrder
auto cmp = [](const pair<int,Signal>& num_1, const pair<int,Signal>& num_2)
{
    return num_1.first > num_2.first;
};


int main()
{
    //int size;
    string Libfile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test Circuits/Library.lib";
    //cout<<"Enter the number of circuits that you will use"<<endl;
    //cin>>size;
    //cout<<"Enter the path of the library file"<<endl;
    //cin>>Libfile;

    //for(int cir=0; cir<size; cir++)
    //{
        ifstream read; // used to read from stimuli file
        ofstream write; // writes to the simulation file
        string timelapse, input, value;
        pair<int,Signal> element,test,output; // these are pairs of timelapse and the input with its value
        string stimulifile,simfile,circfile;

        circfile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test Circuits/Circuit 1/Circuit 1.circ";
        stimulifile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test Circuits/Circuit 1/Circuit 1.stim";
        simfile = "D:/University/5.Spring 2024/Digital Design/Digital Project/DD1-Project-1/Test Circuits/Circuit 1/Circuit 1.sim";

    //    cout<<"Enter the path of the first circuit file"<<endl;
    //    cin>>circfile;
    //    cout<<"Enter the path of the stimulifile for that circuit"<<endl;
    //    cin>>stimulifile;
    //    cout<<"Enter the path of the file you want to output the simulation to"<<endl;
    //    cin>>simfile;

        read.open(stimulifile); // this opens the stimulifile
        write.open(simfile); // this opens the simulation file
        priority_queue<pair<int,Signal>,vector<pair<int,Signal> >,decltype(cmp)> simOrder(cmp); // this is a minheap that will store all the signals with their timelapse in the ascending order of timelapse

        Circuit mycircuit(circfile); // creates the circuit using the circuit file


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

            //cout<<element.first<<endl;

            simOrder.push(element); // this pushes each element that will be outputted to the simulation file into the minHeap
        }
        read.close();


        

        //Call lib.setLogic on all the gates

        int gateNums = mycircuit.getGateNum();
        for (int i = 0; i < gateNums; i++) // will iterate through the log to check which gate each input affects
        {
            lib.setLogic(i);
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
                    mycircuit.setInput((*log)[i].second,test.second); // changes the input of the given gate
                    //lib.setLogic((*log)[i].second);
                    lib.logicChange((*log)[i].second); // calculates the output of the given gate with the new change in its input

                    output.first=(test.first+mycircuit.getDelay((*log)[i].second)); //the new timelapse of the changed output is calculated
                    output.second.name="W"+to_string((*log)[i].second); //the name of the wire taking the specific output is set
                    output.second.value=mycircuit.getOutput((*log)[i].second).value; // the new value of the output signal is calculated

                    simOrder.push(output); // pushes the new element which is the output of the given gate connected to the current input that changed its output so that changed output with its timelapse which is the timelapse of its input plus the delay of the given gate plus its new value will be pushed into the minHeap (sortedOrder)
                }
            }

        }
        write.close();

    //}

    return 0;
}
