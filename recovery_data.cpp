// NAME:	Jonathan Wasserman
// ASUID:	1206062165
// DATE: 	11/23/2016
// COURSE:	IEE 426
// TITLE:	Recovery Area Statistical Analysis Tool (recovery_data.cpp)

using namespace std;

// INCLUDE THE FOLLOWING C++ LIBRARIES

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <iomanip>

// DATA STRUCTURES

typedef struct {
	string date;
	int service_time;
	int discharge_time;
	int recovery_time;
} patient_interaction;

typedef struct {
	int id;
	int beds_in_use;
} minute;

typedef struct {
	string date;
	minute * daily_minutes;
	vector<patient_interaction> daily_interactions;
	int max_beds;
	int max_duration_max_beds;
	int total_duration_max_beds;
	double * utilization;
	double arrival_rate;
} day;

// GLOBAL VARIABLES

int n; // Number of patient interactions
char * filename; // Name of input file

// MACRO DEFINITIONS

#define WORKDAY_MINUTES 600 // Number of minutes in the workday (7am - 5pm)

// FUNCTIONS

patient_interaction * readFile(char * filename); 
vector<day *> sortByDay(patient_interaction * full_list);
int num_beds_in_use(vector<day*> month);
int maximum_beds(vector<day*> month);
int total_duration_maximum_beds(vector<day*> month);
int maximum_duration_maximum_beds(vector<day*> month);
double * utilization(vector<day*> month);
double rate_of_arrival(vector<day*> month); 
int print(vector<day*> month);

// BODY

// Reads in text file containing patient interaction information 
patient_interaction * readFile() 
{
	// Prompt user
	cout << "Please enter name of data file (including .txt extension)" << endl;

	// Read in filename from standard input
	filename = new char[100];
	cin >> filename;

	// Set up input stream, open file
	ifstream file;
	file.open(filename);
	
	// Read in number of patient interactions
	string word;
	getline(file, word, '\t');
	//cout << word << endl; // TESTING
	n = stoi(word); // Previously declared as global variable
	word.clear();
	getline(file, word);	
	word.clear();

	// Allocate array of patient interactions of size n
	patient_interaction * full_list = new patient_interaction[n];

	// Read in patient interactions
	for(int i = 0; i < n; i++)
	{
		// Read in date
		getline(file, full_list[i].date, '\t');
		//get();
		// Read in service time
		getline(file, word, '\t');
		full_list[i].service_time = stoi(word);
		word.clear();
		// Read in recovery time
		getline(file, word);
		full_list[i].recovery_time = stoi(word);
		word.clear();
		// Calculate discharge time
		full_list[i].discharge_time = full_list[i].service_time + full_list[i].recovery_time;
	}

	/*// TESTING
	for(int i = 0; i < n; i++)
	{
		cout << i << "\t" << full_list[i].date << "\t\t" << full_list[i].service_time << "\t\t" << full_list[i].recovery_time << endl; 
	}*/
	return full_list;
}

// Creates array of days, sorts patient interactions into their respective days
vector<day *> sortByDay(patient_interaction * full_list)
{
	// Create vector of unique days
	vector<day *> month;
	// Add first unique day to vector
	day * first = new day;
	first->date = full_list[0].date;
	month.push_back(first);
	// Add remaining unique days to vector
	for(int i = 1; i < n; i++)
	{
		if((full_list[i].date).compare(month.at(month.size()-1)->date) != 0)
		{
			day * next = new day;
			next->date = full_list[i].date;
			month.push_back(next);
		}
	}

	/*// TESTING
	for(int i = 0; i < month.size(); i++)
	{
		cout << month.at(i)->date << endl;
	}*/

	// Add patient_interactions to respective days
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < month.size(); j++)
		{
			if(full_list[i].date.compare(month.at(j)->date) == 0)
			{
				(month.at(j)->daily_interactions).push_back(full_list[i]);
			}
		}
	}
	return month;
}

// Calculates the number of beds in use at each minute of each day
int num_beds_in_use(vector<day*> month)
{
	// For each date, iterate through minutes of the day and analyze how many beds are in use during that minute
	for(int i = 0; i < month.size(); i++)
	{
		for(int j = 0; j < 1440; j++)
		{
			month.at(i)->daily_minutes[j].beds_in_use = 0;
			for(int k = 0; k < month.at(i)->daily_interactions.size(); k++)
			{
				if((month.at(i)->daily_minutes[j].id >= month.at(i)->daily_interactions.at(k).service_time) && (month.at(i)->daily_minutes[j].id < month.at(i)->daily_interactions.at(k).discharge_time))
				{
					month.at(i)->daily_minutes[j].beds_in_use++;
				}
			}
			//cout << month.at(i)->daily_minutes[j].beds_in_use << " "; // TESTING
		}
	}
	return 0;
}

// Calculates the maximum number of beds in use at one time for each day
int maximum_beds(vector<day*> month)
{
	for(int i = 0; i < month.size(); i++)
	{
		month.at(i)->max_beds = 0;
		for(int j = 420; j < 1020; j++)
		{
			if(month.at(i)->daily_minutes[j].beds_in_use > month.at(i)->max_beds)
				month.at(i)->max_beds = month.at(i)->daily_minutes[j].beds_in_use;
		}
		//cout << month.at(i)->date << ": " << month.at(i)->max_beds << endl; // TESTING
	}
	return 0;
}

// Calculates the total duration of time each day in which that day's maximum bed number was used
int total_duration_maximum_beds(vector<day*> month)
{
	for(int i = 0; i < month.size(); i++)
	{
		month.at(i)->total_duration_max_beds = 0;
		for(int j = 420; j < 1020; j++)
		{
			if(month.at(i)->daily_minutes[j].beds_in_use == month.at(i)->max_beds)
				month.at(i)->total_duration_max_beds++;
		}
		//cout << month.at(i)->date << ": " << month.at(i)->total_duration_max_beds << endl; // TESTING
	}
	return 0;
}

// Calculates the longest duration of time each day in which that day's maximum bed number was used
int maximum_duration_maximum_beds(vector<day*> month)
{
	for(int i = 0; i < month.size(); i++)
	{
		month.at(i)->max_duration_max_beds = 0;
		int current_duration = 0;
		for(int j = 420; j < 1020; j++)
		{
			if(current_duration > month.at(i)->max_duration_max_beds)
				month.at(i)->max_duration_max_beds = current_duration;
			if(month.at(i)->daily_minutes[j].beds_in_use == month.at(i)->max_beds)
				current_duration++;
			else if(month.at(i)->daily_minutes[j].beds_in_use != month.at(i)->max_beds)
				current_duration = 0;
		}
		//cout << month.at(i)->date << ": " << month.at(i)->max_duration_max_beds << endl; // TESTING
	}
	return 0;
}

// Calculates number of minutes in each workday that each number of rooms is in use (inclusive of lower numbers, so if 5 rooms are in use, this will count 1-4 rooms as also in use)
double * utilization(vector<day*> month)
{
	for(int i = 0; i < month.size(); i++)
	{
		// Allocate utilization variables
		month.at(i)->utilization = new double[6];
		for(int j = 0; j < 6; j++)
		{
			month.at(i)->utilization[j] = 0;
		}

		// Iterate through workday minutes (7am-5pm)
		for(int j = 420; j < 1020; j++)
		{
			for(int k = 1; k <= 6; k++)
			{
				if(month.at(i)->daily_minutes[j].beds_in_use >= k)
				{
					month.at(i)->utilization[k-1] = month.at(i)->utilization[k-1] + 1;
				}
			}
		}
	}
}

// Calculates daily average of how often a patient is likely to arrive in the recovery area
double rate_of_arrival(vector<day*> month)
{
	for(int i = 0; i < month.size(); i++)
	{
		month.at(i)->arrival_rate = WORKDAY_MINUTES/(month.at(i)->daily_interactions.size());
	}
}

// Prints values for each day: maximum beds used, total duration of maximum beds used, and maximum duration of maximum beds used
int print(vector<day*> month)
{
	string output_filename = filename;

	output_filename.erase(output_filename.find_last_of("."), string::npos);
	output_filename.append("_output.txt");

	ofstream output;
	output.open(output_filename);

	for(int i = 0; i < month.size(); i++)
	{
		output << month.at(i)->date << ":" << endl;
		output << "Max beds used: " << month.at(i)->max_beds << endl;
		output << "Total duration of max beds used: " << month.at(i)->total_duration_max_beds << " minutes" << endl;
		output << "Max duration of max beds used: " << month.at(i)->max_duration_max_beds << " minutes" << endl;
		output << "Average rate of patient arriving in recovery area: One patient per " << fixed << setprecision(3) << month.at(i)->arrival_rate << " minutes" << endl;
		output << "Utilization of one room: " << fixed << setprecision(3) << month.at(i)->utilization[0]/6 << "%" << endl; 
		output << "Utilization of two rooms: " << fixed << setprecision(3) << month.at(i)->utilization[1]/6 << "%" << endl; 
		output << "Utilization of three rooms: " << fixed << setprecision(3) << month.at(i)->utilization[2]/6 << "%" << endl; 
		output << "Utilization of four rooms: " << fixed << setprecision(3) << month.at(i)->utilization[3]/6 << "%" << endl; 
		output << "Utilization of five rooms: " << fixed << setprecision(3) << month.at(i)->utilization[4]/6 << "%" << endl; 
		output << "Utilization of six rooms: " << fixed << setprecision(3) << month.at(i)->utilization[5]/6 << "%" << endl; 
		output << endl;
	}
	return 0;
}

// Driver
int main() {
	
	// Read in text file, create array of patient interactions
	patient_interaction * full_list = readFile();

	// Create array of dates, sort patient interactions into dates
	vector <day *> month = sortByDay(full_list);

	// Initiatalize minute array for each date, set id
	for(int i = 0; i < month.size(); i++)
	{
		month.at(i)->daily_minutes = new minute[1440]; // Total minutes in a day
		for(int j = 0; j < 1440; j++)
		{
			month.at(i)->daily_minutes[j].id = j+1;
		}
	}

	// Calculate umber of beds in use each minute
	num_beds_in_use(month);
	
	// Calculate maximum number of beds used at once each day
	maximum_beds(month);

	// Calculates the total duration of time each day in which that day's maximum bed number was used
	total_duration_maximum_beds(month);

	// Calculates the longest duration of time each day in which that day's maximum bed number was used
	maximum_duration_maximum_beds(month);

	// Calculates daily percentage utilization for each number of rooms
	utilization(month);

	// Calculates average daily rate of patient arrival in recovery area
	rate_of_arrival(month);

	// For each date, output all relevant values to a text file
	print(month);

	// De-allocate memory
	for(int i = 0; i < month.size(); i++)
	{
		delete month.at(i)->daily_minutes;
		delete month.at(i)->utilization;
		delete month.at(i);
	}
	delete filename;
	//delete full_list;
	return 0;
}


