#include<iostream>
#include<fstream>
#include <chrono>       // To make a Filtering block sleep in case buffer has insufficient elements to perform Filtering
#include<time.h>      // To use srand() for seeding rand()
//#include <thread>     // Don't use this inbuilt header file. Use header file provided in 'Dependencies' folder.
#include "Dependencies/mingw.thread.h"

#include "Data_Filtering_and_Threshold.cpp"
#include "Buffer_Storing_and_Padding.cpp"

using namespace std;

int done = 0;

void Data_Generation_Block(int limit, int m, int filter_window){
    srand(time(0));     // To prevent getting same set of random values again and again
    ofstream obj;
    obj.open("CSV Files/Random_generator_mode.csv" );          // To store the sequence of random numbers in this csv file
    int Row=0;                  // Row tells the number of rows that have been scanned till now
    while(Row<100){            // Scanning each row. We have set the total rows to be 100.                
        //printf("Generating\n");
        Row++;                  // Increment Row number
        int i=0;               // i means total number of pixels currently scanned in the row
        int j=0;                // j means total number of pixels currently scanned in each iteration
        vector<int> Send;
        while(i<m && j<limit){      // Performing iterations, in each iteration 'limit' number pixels would be generated from a row              
            i++; 
            j++;   
            int px = rand()%256;
            Send.push_back(px);
            if(i==m){                   // means all pixels have been scanned in the given row
                obj<<px<<"\n";
                Store_into_Buffer(Send, limit, m, filter_window);
                break;
            }
            else if(j==limit){              // means the given iteration has ended, now send scanned pixels to the processing block
                obj<<px<<",";
                j=0;
                Store_into_Buffer(Send, limit, m, filter_window);
                Send.clear();
                continue;
            }
            else{
                obj<<px<<",";
                continue;
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
        //printf("Processing\n");
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
    // srand(clock(NULL));     don't use it here as srand() does not work with threads. Instead initialize it inside the thread function.
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