/*
 * FruitSortingApp.cpp
 *
 *  Created on: 31-Oct-2013
 *      Author: prpankajsingh
 */

#include "FruitSortingApp.h"
#include "GroupManager.h"
#include <unistd.h>
#include "constants.h"
#include "rs232.h"
#include "cv.hpp"
#include "highgui/highgui.hpp"

FruitSortingApp::FruitSortingApp(){
	this->alpha=0;
	this->beta=1;
	if(RS232_OpenComport(COMPORT,BAUDRATE) ){
				cout<<"Error in opening comport";
				exit(1);
			}

	cout<<"Successfully opened comport "<<COMPORT<<"\n";

}

FruitSortingApp::FruitSortingApp(int n):lemonSortingVision(n){
	this->alpha=0;
	this->beta=1;
	if(RS232_OpenComport(COMPORT,BAUDRATE) ){
					cout<<"Error in opening comport";
					exit(1);
				}

		cout<<"Successfully opened comport "<<COMPORT<<"\n";

}

FruitSortingApp::~FruitSortingApp(){
	this->alpha=0;
	this->beta=1;
	RS232_CloseComport(COMPORT);
}


void FruitSortingApp::loadProfile(string & prof_file_path, FILE *& prof_file)
{
	//char *prof_file_path;
	cout << "enter profile name which must be present in profiles folder...\n";
	cin >> prof_file_path;
	prof_file_path.insert(0, "profiles/");
	//prof_file_path=strcat("profiles/",prof_file_path);
	prof_file = fopen(prof_file_path.data(), "r");
	if(prof_file!=NULL)
	{
		fscanf(prof_file,"%d",&num_of_groups);
		int i;//,hue[num_of_groups],area[num_of_groups];
		for(i=0;i<num_of_groups;i++)
		{
			fscanf(prof_file,"%d %d\n",&hue[i],&area[i]);
			//cout<<hue[i]<<" "<<area[i];
		}

		fscanf(prof_file,"%lf\n",&alpha);
	}
	else
	{
		cout<<"Cannot open profile..exiting\n";
		exit(1);
	}
	cout << "Value has been read successfully from profile\n";
}

void FruitSortingApp::createProfile(string & prof_file_path, FILE *& prof_file)
{
	cout << "Enter Name of Profile....\n";
	FILE *file;
	while(1)
	{

		cin>>prof_file_path;
		prof_file_path.insert(0,"profiles/");
		//prof_file_path=strcat("profiles/",prof_file_path);
		//cout<<"We are trying to open file in read mode.........";
		file = fopen(prof_file_path.data(), "r");
		//cout << prof_file_path<< "And we are trying to check whether this file is already there or not.....";
		if (file!=NULL)
		{
			fclose(file);
			cout<<"This profile is already present..\n";
			cout<<"Enter another name for your profile...\n";
		}
		else
		{
			//fclose(file);
			cout<<"Profile Name is looking Good... :)\n";
			break;
		}
	}
	prof_file = fopen(prof_file_path.data(), "w");
	while(1){
		cout << "enter number of groups\n";
		cin >> num_of_groups;
		if(!(num_of_groups == int(num_of_groups))){
			cout << "Number of groups should be an integer value.\n";
			continue;
		}else
			if(num_of_groups < 2 || num_of_groups > 5){
				cout << "Number of groups should be greater than 1 and less than 6\n";
				continue;
			}else{
				break;
			}

	}

	fprintf(prof_file, "%d\n", num_of_groups);
	int i;
	cout << "Do you know the values of hue and area, press Y/N\n";
	char hue_area_know;
	cin >> hue_area_know;
	if(hue_area_know == 'N' || hue_area_know == 'n'){
		for(i = 0;i < num_of_groups;i++){
			cout<<"Training of group "<<i+1<<" started....\n\n";
			cout<<"Please place a sample fruit in front of camera that belongs to this GROUP";
			cout<<"Press 'Y' when you are ready....and any other key to exit";
			char c;
			cin>>c;
			if (c=='Y'||c=='y')
			{
				FruitAttribute f=lemonSortingVision.getAttributeFromCamera();
				fprintf(prof_file,"%d %d\n",f.hueVal,f.area);
				cout<<"Hue Value is "<<f.hueVal<<"And area is "<<f.area;
				hue[i]=f.hueVal;
				area[i]=f.area;

			}
			else
			{
				exit(0);
			}
			//place fruit infront of camera to extract feature;
			//and extract the values hue, area for ith group
			//fprintf(prof_file,"%d %d\n",hue[i],area[i]);
		}
	}
	else
		if(hue_area_know == 'Y' || hue_area_know == 'y'){
			for(i = 0;i < num_of_groups;i++){
				cout << "enter hue value for " << i + 1 << "th group\n";
				cin >> hue[i];
				//check if entered hue is a proper value or not......
				cout << "enter area value for " << i + 1 << "th group\n";
				cin >> area[i];
				//check if entered area is a proper value or not.....
				fprintf(prof_file, "%d %d\n", hue[i], area[i]);
			}
		}


	cout << "enter alpha value (0 for hue only and 1 for area only) \n";
	while(1){
		cin >> alpha;
		if(alpha >= 0 && alpha <= 1){
			break;
		}
		cout << "Enter proper value of alpha. The value should be between 0 and 1\n";
	}

	fprintf(prof_file, "%lf", alpha);
	cout << "Thanks for providing this input. Now we are trying to create a profile....\n";
	try {
		fclose(prof_file);
	}
	catch(exception e){
		cout << "Ooopss.....\nNot able to write in a file... :(\n";
	}
	cout << "Congratulation your file has been created successfully...!!! :)\n";
	cout << "Now you can use it.......\n";
}

void FruitSortingApp::run()
{
	string prof_file_path;
	FILE *prof_file;
	namedWindow("Video");
	Mat m;
	m=Mat::zeros(300,300,CV_32F);
	imshow("Video",m);

	/*
	 * Initialize FBV
	 */
	if(int status=initFB()){
		cout<<"Not able to Initialize FireBird.....";
		cout<<"Error Signal code is "<<status;
		exit(status);
	}

	/**
	 * Initializing IP module
	 */
	if(int status=initIPModule()){
			cout<<"Not able to Initialize IP module.....";
			cout<<"Error Signal code is "<<status;
			exit(status);
		}
	;


	cout<<"Do you want to load existing profile Y/N\n";
	char prof_info='Y';
	cin>>prof_info;
	if (prof_info == 'Y' || prof_info == 'y' )
	{
		//It takes the input from command line.......
		loadProfile(prof_file_path, prof_file);

	}
	else if (prof_info == 'N' || prof_info == 'n' )
	{
		/**
		 *It takes the input from command line.
		 *It It also loads the profile
		 */
		createProfile(prof_file_path, prof_file);
	}
	cout<<"Initializing Group Manager....\n";
	//send data to Group Manager
	//try{

		GroupManager gm(num_of_groups);

	//}
	//catch(exception e){
	//	cout<<"Error while initializing Group Manager.....\n";

	//}
	cout<<"Training the group ";
	try{
		int ac=gm.getActiveGroupCount();
		for(int i =1;i<=ac;i++)
		{
			gm.TrainGroup(i,FruitAttribute(hue[i-1],area[i-1]));

		}

	}
	catch(exception e){
		cout<<"Error occurred while training the data....\n";
	}

	cout<<"Starting Working.....\n";

	if(sendSignalToFB(START_FB))
	{
		cout<<"Error Ocurred while starting the FB...\n";
		exit(1);
	}


	//try{

	/*
	 * Un comment below region to test classification by cli..
	 */
	/*
	char c=getchar();
			while(c!=27)
			{
				int h, a;
			cout<<"Testing.....\n\n\n";
			cout<<"Enter Hue";
			cin>>h;
			cout<<"Enter Area\n";
			cin>>a;
			cout<<"Assigned Group is : "<<gm.AssignGroup(FruitAttribute(h,a),0,1);
			c=getchar();
			}
//int c = waitKey(10);
////If 'ESC' is pressed, break the loop
//if((char)c==27 ) break;
	*/

	while(receiveSignalFromFB()==GET_LABEL){

		//cout<<"Press 'ESC' to Stop and Exit\n";
		//by default number of frame per decision is 10 if requires soeething else pass it as a integer parameter.
		FruitAttribute f=lemonSortingVision.getAttributeFromCamera();
		//default value of alpha is 0 and beta is 1..In load profile function alpha is being loaded from file...while beta will be 1 always for now...
		int grp=gm.AssignGroup(f,this->alpha,this->beta);
		int confidence=0;
		confidence=gm.calculateConfidence(f,this->alpha,this->beta);
		cout<<"current Group Assignment is "<<grp<<"with confidence level "<<confidence<<"%";
		cout<<"TODO...Display Group wise Total....";
		if(sendGroupInfoToFB(grp)){
			cout<<"Error occurred while stopping the Firebird";
			exit(1);
		}
		int c = waitKey(50);
		//If 'ESC' is pressed, break the loop
		if((char)c==27 ) break;

//		c = getch();
	}
	//STOP mechanism not ready...................
//	if(sendSignalToFB(STOP_FB)){
//		cout<<"Error occurred while stopping the Firebird";
//		exit(1);
//	}

};

int main(){
	//argument is camera number
	FruitSortingApp s(1);
	s.run();
	return 0;
}
/**
     * Initialize IP module with camera number n and returns 0 for sucessfull initialization
     * else return appropriate signal defined in mySig.h
     */

int FruitSortingApp::initIPModule(int n)
{
	try{
		//Currently IP is already initialized as camera 1 as a part of constructor of This class
		//this->lemonSortingVision(n);

	}
	catch(exception e){
		cout <<e.what();
		return IP_NOT_READY;
	}
	return IP_READY;
}

/**
     * Send Init signal to Firebird
     * and wait for ACK signal to recieve for atmost timeout of 5 sec period
     * This function returns 0 on success(READY) else return appropriate signal value defined in mySig.h
     */
int FruitSortingApp::initFB()
{
	cout<<"TODO"<<"sush::initFB()";
	if(RS232_SendByte(COMPORT,INIT_FB)){
		cout<<"Not able to send INIT_FB signal";
		exit(1);
		};
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
	return FB_READY;
}


int FruitSortingApp::sendSignalToFB(unsigned char signal)
{
	if(RS232_SendByte(COMPORT,signal)){
		return 1;
	}
	cout<<"sush::sendSignalToFB("<<signal<<")";
	return 0;
}



unsigned char FruitSortingApp::receiveSignalFromFB()
{
	cout<<"TODO"<<"sush::receiveSignalFromFB()";
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

	return c;
}

int FruitSortingApp::sendGroupInfoToFB(int groupNo)
{
	unsigned char c=groupNo+48;
	sendSignalToFB(c);
	std::cout<<"pending.......int sush::sendGroupInfoToFB(int "<<groupNo<<")"<<"and character "<<c<<"...";
	return 0;
}




