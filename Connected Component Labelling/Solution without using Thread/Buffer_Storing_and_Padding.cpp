#pragma once
#include<iostream>
#include<deque>
#include<vector>

using namespace std;

deque<int> buffer;
int Col_Stored = -1;       // Col_Stored is the Column no. (0 based indexing) of the last pixel that was stored in the buffer

void Add_Padding(int filter_window){
    int num_of_zeroes = (filter_window-1)/2;        //  For filter_window = 9, we have to add padding of four 0's.
    for(int x=1; x<=num_of_zeroes; x++){            // to provide filter padding for the last elements of the last row of the object
        buffer.push_back(0);
    }
}

void Remove_Padding(int filter_window){
    int num_of_zeroes = (filter_window-1)/2;        //  For filter_window = 9, we have to add padding of four 0's.
    for(int x=1; x<=num_of_zeroes; x++){            // to provide filter padding for the last elements of the last row of the object
        buffer.pop_front();       // Perform 4 pop() operations to remove padding of 4 0's that you previously added to the buffer
    }
}

void Store_into_Buffer(vector<int> Send, int limit, int m, int filter_window){      // Col_Stored is the column no. of the 2nd pixel(ele2) and Col_Stored-1 is col no. for ele1.
    for(int x=0; x<Send.size(); x++){
        int ele = Send[x];
        if(Col_Stored==m-1){        // Col_Stored = Column no. (0 based indexing) of the last buffer that was stored in the buffer
            Col_Stored=-1;
        }
        if(Col_Stored==-1){
            Add_Padding(filter_window);
        }
        Col_Stored++;          // At this point, Row = Row no. and Col_Stored = Col_Stored no. of the pixel that is to be stored in the buffer
        buffer.push_back(ele);                           
    }
    return;
}