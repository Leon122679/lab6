#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
#include <cassert>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iomanip>

using namespace std;
//Declare the Class
class Signal{
	//private part
	private:
		char filename[30];
		int length;
		double average;
		double scale, offset;
		double max,min;

	//public part
	public:
		Signal();
		Signal(int f_num);
		Signal(char* f_name);
		~Signal();
		int op_file;//flag
		void Offset(double val_offset);
		void Scale(double val_scale);
		void Center();
		void Normalize();
		void Statistic();
		void operation();
		void save_file(char* filename);
		void read(char* filename);
		void sig_info();
		void Max();
		void Min();
		void Average();
		int getLength() {return length;};
		void setLength(int len){length=len;};
		vector<double> sig_data;
		vector<double> up_data;
	    void operator+(const double temp);
		void operator*(const double temp);

};
//read from the file.
void Signal :: read(char* filename){
	ifstream in_file;
	in_file.open(filename);
	//check if it is correctly opened
	if(in_file.is_open()==true){
		in_file>>length;
		in_file>>max;
		cout<<"The numbers of the file is "<<length<<endl<<"The max of numbers is "<<max<<endl<<endl;
		//read data into vector.
		for(int i=0;i<length;i++){
			sig_data.resize(length);
            up_data.resize(length);
			in_file>>sig_data.at(i);
			up_data.at(i)=sig_data.at(i);
		}
		in_file.close();
		
		Max();
		Min();
		Average();
		op_file=0;
	}
	//check error.
	else{
		cerr<<"Error while opening the file"<<endl;
		op_file=1;//set flag
	}
}
//save and create new file
void Signal :: save_file(char* filename){
	ofstream output_file;
	output_file.open(filename);
	output_file<<length<<" "<<max<<"\n";
	//write data into new file.
	for(int i=0;i<up_data.size();i++){
		output_file<<up_data[i]<<"\n";
	}
	output_file.close();
}
//get the max
void Signal :: Max(){
	int i=0;
	max=up_data.at(i);
	for(i=0;i<up_data.size();i++){
		if(up_data[i]>max)
			max=up_data[i];
	}
}
//get the min
void Signal :: Min(){
	int i=0;
	min=up_data.at(i);
	for(i=0;i<up_data.size();i++){
		if(up_data[i]<min)
			min=up_data[i];
	}
}
//get the average
void Signal :: Average(){
	double sum=0;
	for(int i=0;i<length;i++)
		sum=sum+up_data[i];
	average=sum/length;
}
//display the four values
void Signal :: sig_info(){
	cout<< "Max:"<< setiosflags(ios::fixed)  << setprecision(4) << max <<endl;
	cout<< "Min:"<< setiosflags(ios::fixed)  << setprecision(4) << min <<endl;	
    cout<< "Length:"<< setiosflags(ios::fixed)  << setprecision(4) << length << endl;
    cout<< "Average:"<< setiosflags(ios::fixed)  << setprecision(4) << average << endl;
}
//----------------------------------------------------------------------------
//member operator +.
void Signal::operator+(const double temp){
	for(int i=0; i<length; i++)
	{
		up_data[i] += temp;
	}
}
//member operator *.
void Signal::operator*(const double temp){
	for(int i=0; i<length; i++)
		up_data[i] *= temp;
}
//----------------------------------------------------------------------------

//operating offset
void Signal :: Offset(double val_offset){
	offset=val_offset;
	this->operator+(offset);
	Max();
	Min();
}
//operating scale
void Signal :: Scale(double val_scale){
	scale=val_scale;
	this->operator*(scale);
	Max();
	Min();
}
//operating statistic
void Signal :: Statistic(){
	Max();
	Min();
	Average();
	sig_info();
}
//operating normalize
void Signal :: Normalize(){
	int i;
	Max();
	Min();
	//just like lab4.
	if(min==max){
		scale=0.5;
		this->operator*(scale);
	}
	else{
		scale = 1/max;
		this->operator*(scale);
	}
		
	Max();
	Min();
}
//operating center
void Signal :: Center(){
	Average();
	offset=-average;
	this->operator+(offset);
	Max();
	Min();
}
//default constructor
Signal :: Signal(){
	length=0;
	max=0;
	average=0;
}
//input integer parametric constructor
Signal :: Signal(int file_num){
	char filename[30];
	sprintf(filename, "Raw_data_%02d.txt",file_num);
	cout<<filename<<endl;
	read(filename);
}
//input filename parametric constructor
Signal :: Signal(char* filename){
	read(filename);
}
//destructor
Signal::~Signal(){
    
}
//help menu
void help(){
	cout<<"You can call the program like this :"<<endl<<" ./lab5 -n 12 or ./lab5 -f Raw_data_12.txt"<<endl;
}

void help1(){
cout<<"Please enter correct operator!!"<<endl;
}
//operation to decide the function we need
void Signal :: operation(){
	char input=0;
	double v_offset, v_scale;
	char filename[30];
	

	while(input!='e')
	{
	cout<<"Please enter the function you want!"<<endl<<endl;
	cout<<"	o: offset."<<endl;
	cout<<"	s: scale."<<endl;
	cout<<"	C: Center."<<endl;
	cout<<"	N: Normalize."<<endl;
	cout<<"	S: Statistic."<<endl;
	cout<<"	e: End and save file."<<endl;
	cin>>input;
	switch(input){
			case 'o':{
				cout<<"Please enter offset value:";
				cin>>v_offset;
				cout<<endl;
				Offset(v_offset);
				break;
			}
			case 's':{
				cout<<"Please enter scale value:";
				cin>>v_scale;
				cout<<endl;
				Scale(v_scale);
				break;
			}
			case 'C':{
				Center();
				break;
			}
			case 'N':{
				Normalize();
				break;
			}
			case 'S':{
				Statistic();
				break;
			}
			case 'e':{
				cout<<"Please enter the file name:";
				cin>>filename;
				save_file(filename);
				break;
			}
			default:{
				help1();
			}

		}
	}
}
//non-member operator
Signal operator+(Signal &lhs, Signal &rhs){
	Signal sum;
	//check error.
	if(lhs.getLength() != rhs.getLength()){
		cout<<"The length of two signals are not equal!!"<<endl;
		return 0;
	}
	//add two signal.
	else{
		for(int i=0; i<lhs.getLength(); i++){
			sum.up_data.push_back(rhs.sig_data[i]);
			sum.up_data[i] += lhs.sig_data[i];
		}
		//update information
		int len;
		len=lhs.getLength();
		sum.setLength(len);
		sum.Average();
		sum.Max();
		return sum;
	}
}



int main(int argc, char* argv[])
{
	//variables for different constructor
	char filename[30];
	int file_num;
	
	switch (argc)
	{
		case 1:{
			//case of default constructor.
			Signal defal;
			if(defal.op_file==1)
			{return EXIT_FAILURE;}
			defal.sig_info();
			break;
		}
		case 3:{
			//case of input integer parametric constructor
			if (argv[1][1]=='n')
			{
			if(argv[2] == NULL){
                    printf("Please enter a file number!!\n");
                    return EXIT_FAILURE;
                }
				file_num=strtod(argv[2],NULL);
			Signal run1(file_num);
			
			if(run1.op_file==1)
			{return EXIT_FAILURE;}
			
			run1.sig_info();
			run1.operation();
			
			cout<<endl<<endl<<"------------------------------------------------------"<<endl;
			cout<<endl<<endl<<"Lab 6 Part!!"<<endl<<endl;
			
			Signal lab6;
			//add two signals
			lab6 = operator+(run1, run1);
			//print out
			for(int i=0; i<lab6.getLength(); i++){
				cout<<lab6.up_data[i]<<endl;
			}
			cout<<"Information:"<<endl;
			lab6.sig_info();
			
			
			
			}
			//case of input filename parametric constructor
			else if(argv[1][1]=='f')
			{
				strcpy(filename, argv[2]);
				Signal run1(filename);
				if(run1.op_file==1)
					{return EXIT_FAILURE;}
				run1.sig_info();
				run1.operation();
				
			cout<<endl<<endl<<"------------------------------------------------------"<<endl;
			cout<<endl<<endl<<"Lab 6 Part!!"<<endl<<endl;
			
			Signal lab6;
			lab6 = operator+(run1, run1);
			for(int i=0; i<lab6.getLength(); i++){
				cout<<lab6.up_data[i]<<endl;
			}
			cout<<"Information:"<<endl;
			lab6.sig_info();
			}
			else{
				help();
				return EXIT_FAILURE;
			}
			break;
		}
		//default error input.
		default:{
			cerr<<"Error Input!"<<endl;
			help();
			break;
		}
		
	}
	return 0;
}
