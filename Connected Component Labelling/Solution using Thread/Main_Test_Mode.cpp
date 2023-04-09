#include<iostream>
#include<fstream>
#include <chrono>       // To make a Filtering block sleep in case buffer has insufficient elements to perform Filtering
//#include <thread>     // Don't use this inbuilt header file. Use header file provided in 'Dependencies' folder.
#include "Dependencies/mingw.thread.h"

#include "Data_Generation.cpp"
#include "Data_Filtering_and_Threshold.cpp"
#include "Buffer_Storing_and_Padding.cpp"

using namespace std;

int done = 0;       // done = 1 means Data_Generation_Block has completed its task

void Data_Generation_Block(int limit, int m, int filter_window){
    ifstream obj;
    obj.open("CSV Files/Testing_mode.csv");
    int Row = 0;        // means 0 rows have been scanned till now
    while(obj.good()){
        Row++;
        string line;
        getline(obj,line);       
        while(true){        // Performing iterations
            Pixel_Generator(line, limit, m, filter_window);      // Scanning at max 'limit' number of pixels in each iteration 
            if(check==-1){        // means no more pixels to scan in the given row
                break;
            }
        }
    }
    Add_Padding(filter_window);       // add padding of four 0's at the end of buffer
    obj.close();
    done = 1;                           // Means this block has completed its execution
    return;
}

void Filter_and_Threshold_Block(int limit, double TV, int m, int T, int filter_window){
    while(true){                            // Performing iterations
        if(buffer.size()<9 && done==1){      // means both the blocks have fully executed
            break;
        }
        else if(buffer.size()<9 && done==0){     // means insufficient elements in buffer to initiate filtering, wait for certain time so that Data_Generation_Block may insert elements into buffer
            std::chrono::nanoseconds timespan(T);      // waiting time = 1ms (assuming 1 Time period lasts for 1ms)
            std::this_thread::sleep_for(timespan);
            continue;
        }
        else{
            Processing(limit, TV, m, filter_window);
        }
    }
    return;
}

int main(){
    int m, T;   
    double TV;
    int limit = 2;      int filter_window = 9;
    cout<<"Enter the value of 'm' (total no. of columns in each row)\n";
    cin>>m;
    cout<<"Enter the value of 'TV' (threshold value)\n";
    cin>>TV;
    cout<<"Enter the value of 'T' (process time)\n";
    cin>>T;

    thread t1(Data_Generation_Block, limit, m, filter_window);
    thread t2(Filter_and_Threshold_Block, limit, TV, m, T, filter_window);
    t1.join();
    t2.join();
}