#include<iostream>
#include<fstream>

#include "rs232.h"
//rename this function to main and compile it to test the serial communication.
int main_2()
{
	using namespace std;
	int COMPORT=0;
	for (int i=0;i<10;i++){
		COMPORT=i;
		if(RS232_OpenComport(COMPORT,9600) ){
			//cout<<"Error";
		}
		else{
			cout<<COMPORT;
		}

	}
    //sends string 'FFFR' byte by byte...
    const char* str = "FFFR";
    unsigned char* ptr = (unsigned char*)str;
    for(; *ptr ; ptr++){
          cout<<"Sending:"<<(*ptr)<<endl;
    RS232_SendByte(COMPORT,*ptr);
    //Now wait for return
    unsigned char c;
    while(1)
    {
            int n = RS232_PollComport(COMPORT, &c,1);
            if(n>0)
            {
                   cout<<"Received:"<<c<<endl;
                   break;
            }
    }
    if(c!=*ptr)
    {
               cout<<"Error:Sent:"<<(*ptr)<<" Received:"<<c<<endl;
               break;
    }
    }
	return 0;
}
