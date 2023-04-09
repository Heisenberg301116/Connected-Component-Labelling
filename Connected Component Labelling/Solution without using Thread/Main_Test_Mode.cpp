#include<iostream>
#include<fstream>
#include<time.h>

#include "Data_Generation.cpp"
#include "Data_Filtering_and_Threshold.cpp"
#include "Buffer_Storing_and_Padding.cpp"

using namespace std;

int Row=0;          // Row tells the number of rows that have been scanned till now
string line;        // to store the scanned line


void Data_Generation_Block(int limit, int m, int filter_window){   
    Pixel_Generator(line, limit, m, filter_window);    // Scanning of next 'limit' pixels present in the csv file
}

void Filter_and_Threshold_Block(int limit, int TV, int m, int filter_window){
    if(buffer.size()<filter_window){      // means insufficient storage in the buffer to initiate filtering
        return;
    }
    else{
        Processing(limit, TV, m, filter_window);
    }
    return;
}

int main(){
    int m;   
    double TV;
    int limit = 2;      int filter_window = 9;
    cout<<"Enter the value of 'm' (total no. of columns in each row)\n";
    cin>>m;
    cout<<"Enter the value of 'TV' (threshold value)\n";
    cin>>TV;
    
    ifstream obj;
    obj.open("CSV Files/Testing_mode.csv");            
    
    while(obj.good() || check==1){   // Performing iterations. Note: If obj.good() is false => we have scanned the last row and if check==-1 => we have scanned the last element of the current row. So, if obj.good() is false and check==-1 => all pixels have been scanned.
        if(check==-1){       // means no more pixels to scan in the given row, so fetch new row
            getline(obj,line); 
            check=1;  
        }
        Data_Generation_Block(limit, m, filter_window);
        Filter_and_Threshold_Block(limit, TV, m, filter_window);
    }
    
    Add_Padding(filter_window);       // add padding of four 0's at the end of buffer
    obj.close();
    
    while(buffer.size()>=filter_window){            // Processing the remaining pixels
        Filter_and_Threshold_Block(limit, TV, m, filter_window);
    }
}