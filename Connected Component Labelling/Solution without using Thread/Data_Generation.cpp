#pragma once            // To prevent loading the same header file again and again that already has been loaded in the main.cpp

#include "Buffer_Storing_and_Padding.cpp"

using namespace std;

int check=-1;     // check = 1 means scanning of current row hasn't been completed yet and check = -1 means current row scan is been completed
int next_index =0;   // next_index= 5 means in my next iteration I have to start scanning elements from index = 5 of the string (row)

void Pixel_Generator(string line, int limit, int m, int filter_window){
    vector<int> Send;
    int curr=0;     // curr represents how many pixels have been scanned till now. If curr = limit => stop further scanning and further scanning will take place in next iteration.
    string val = "";    // px1 = value of 1st pixel and px2 = value of 2nd pixel retrieved from the given row in the current iteration
    for(int i=next_index; i<=line.size(); i++){    
        if(curr==limit){        // means limit of processing pixels in each iteration reached
            curr = 0;      
            val="";   
            check = 1;        // to signal main.cpp file that scanning row hasn't yet ended
            next_index=i;        // In the next iteration, pixel retrieval will take place starting from index = next_indexin the same given 'line' or row.
            Store_into_Buffer(Send, limit, m, filter_window);
            return;
        }
        else if(i==line.size()){      // => Row has ended and no further elements to process in the given row. Also, the total no. of columns are odd
            Send.push_back(stoi(val));  // The last element in the row does not have ',' succeeding it, so do store it in the buffer.
            curr = 0;
            val="";
            check=-1;      // check=-1 means to signal end of the row to main.cpp file.
            next_index=0;  // for new row set next_indexto 0 as searching will take place from 0th index.
            Store_into_Buffer(Send, limit, m, filter_window);
            return;
        }
        else if(line[i]==','){       // => The first pixel value has been retrieved 
            Send.push_back(stoi(val)); 
            curr++;
            val="";
            next_index=i+1;
        }
        else{
            val+=line[i];
        }
    }
    return;
}