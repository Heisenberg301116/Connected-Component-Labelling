#pragma once

#include "Buffer_Storing_and_Padding.cpp"

int Col_Processed=-1;      // Col_Processed is the Column no. (0 based indexing) of the last pixel that has been processed by the Processing() module.
                            // Col_Processed = 5 means I have done filtering from 0 to 5th index in a row

double Compute(){
    double result = 0.00025177*buffer[0] + 0.008666992*buffer[1] + 0.078025818*buffer[2] + 0.24130249*buffer[3] + 
    0.343757629*buffer[4] + 0.24130249*buffer[5] + 0.078025818*buffer[6] + 0.008666992*buffer[7] + 0.000125885*buffer[8];
    return result;
}

void Processing(int limit, double TV, int m, int filter_window){       // This will process 'limit' number of pixels in each iteration
    int buffer_index = (filter_window-1)/2;   // buffer_index tells us the index in the buffer where we have to apply filtering. For filter_window = 9, buffer_index = 4.
    for(int i=1; i<=limit; i++){
        if(Col_Processed==m-1){     
            Remove_Padding(filter_window);      // Perform 4 pop() operations to remove padding of 4 0's that you previously added to the buffer
            Col_Processed=-1;      // set count = -1 for the next row
            cout<<endl;     // display new line to mark the new row
        }
        if(buffer.size() >= filter_window){        // For processing in the buffer, the limit no. of elements inside the buffer must be >= 9
            double result = Compute();
            if(result>=TV){
                //cout<<buffer[buffer_index]<<" ";
                //cout<<result<<" ";
                cout<<1<<" ";
            }
            else{
                //cout<<buffer[buffer_index]<<" ";
                //cout<<result<<" ";
                cout<<0<<" ";
            }
            buffer.pop_front();  
            Col_Processed++;
        }
        else{
            return;   
        }                     
    }
    return;
}