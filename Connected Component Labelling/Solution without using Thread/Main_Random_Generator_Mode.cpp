#include<iostream>
#include<fstream>
#include<time.h>

#include "Data_Filtering_and_Threshold.cpp"
#include "Buffer_Storing_and_Padding.cpp"

using namespace std;

int i = 0;          // i tells the number of columns that have been scanned till now for a particular row
int Row=0;          // Row tells the number of rows that have been scanned till now
ofstream obj;


void Data_Generation_Block(int limit, int m, int filter_window){   
    vector<int> Send;
    int j=0;
    while(i<m && j<limit){      // Performing iterations, in each iteration 'limit' number pixels would be generated from a row              
        i++; 
        j++;   
        int px = rand()%256;
        Send.push_back(px);
        if(i==m){                   // means all pixels have been scanned in the given row
            Row++;
            i=0;
            obj<<px<<"\n";
            Store_into_Buffer(Send, limit, m, filter_window);
            break;
        }
        else if(j==limit){              // means the given iteration has ended, now send scanned pixels to the processing block
            obj<<px<<",";
            Store_into_Buffer(Send, limit, m, filter_window);
        }
        else{
            obj<<px<<",";
        }
    }
    return;
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
    srand(time(0));     // To prevent getting same set of random values again and again
    int m;   
    double TV;
    int limit = 2;      int filter_window = 9;
    cout<<"Enter the value of 'm' (total no. of columns in each row)\n";
    cin>>m;
    cout<<"Enter the value of 'TV' (threshold value)\n";
    cin>>TV;
    
    obj.open("CSV Files/Random_generator_mode.csv" );          // To store the sequence of random numbers in this csv file             
    
    while(true){                // Performing iterations
        Data_Generation_Block(limit, m, filter_window);
        Filter_and_Threshold_Block(limit, TV, m, filter_window);
        if(Row==100){           // means Data_Generation_Block has completed its execution. We are considering that object has 100 rows 
            Add_Padding(filter_window);      // add padding of 0's in the end
            obj.close();
            break;
        }
    }
    
    while(buffer.size()>=filter_window){            // Processing the remaining pixels
        Filter_and_Threshold_Block(limit, TV, m, filter_window);
    }
}