/* ----------------------------------------------------------------------
    Program 4: Divvy Data
        Program 

    Course: CS 141, Fall 2021
    System: Windows using Visual Studio Code, repl.it
    Author: Nikos Kotsopulos, Matthew Danque
    
    Running the program looks like:
        . . .
Analyze Divvy bike ride information using September trip data.
First select option 1 to read and clean the data.
 
Select a menu option: 
   1. Select datafile, display rides totals and clean data
   2. Display overall trip information
   3. Display percentage of members vs. casual riders
   4. Display weekday vs weekend usage
   5. Extra Credit: find closest station
   6. Exit
Your choice -->

   ----------------------------------------------------------------------
*/
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
using namespace std;


struct divvyData{//to hold my values of the divvy and weekday csv values
  string rideID;
  string rideableTYPE;
  string startAT;
  string endAT;
  string startStationName;
  string endStationName;
  string start_lat;
  string start_long;
  string end_lat;
  string end_long;
  string member_cas;
  string temp;
};

vector<divvyData> unclean;
vector<divvyData> clean;

void displayMenu(){//displays the default menu options
  cout << "Select a menu option:\n"; 
  cout << "\t1. Select datafile, display rides totals and clean data\n";
  cout << "\t2. Display overall trip information\n";
  cout << "\t3. Display percentage of members vs. casual riders\n"; 
  cout << "\t4. Display weekday vs weekend usage\n";
  cout << "\t5. Extra Credit: find closest station\n";
  cout << "\t6. Exit\n";
}

//default code used calculate radians to get the miles over lat and longitudes
long double toRadians(const long double degree){
    // cmath library in C++
    // defines the constant
    // M_PI as the value of
    // pi accurate to 1e-30
    long double one_deg = (3.14159265358979323846) / 180;
    return (one_deg * degree);
}

long double distance(long double lat1, long double long1,
    long double lat2, long double long2)
{
    // Convert the latitudes
    // and longitudes
    // from degree to radians.
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);

    // Haversine Formula
    long double dlong = long2 - long1;
    long double dlat = lat2 - lat1;

    long double ans = pow(sin(dlat / 2), 2) +
        cos(lat1) * cos(lat2) *
        pow(sin(dlong / 2), 2);

    ans = 2 * asin(sqrt(ans));

    // Radius of Earth in
    // Kilometers, R = 6371
    // Use R = 3956 for miles
    long double R = 3956;

    // Calculate the result
    ans = ans * R;

    return ans;
}

//used to get the unclean version of the data of divvydata
void readUncleanDivvyData(string file, divvyData& dataUnclean){ 
  ifstream fileReader(file);

  string line;
  getline(fileReader, line);
  
  while (getline(fileReader, line) && !line.empty()){//grabs the datapoints in each file and stores them into a struct vector
    stringstream mystream(line);
    getline(mystream, dataUnclean.rideID, ',');
    getline(mystream, dataUnclean.rideableTYPE, ',');
    getline(mystream, dataUnclean.startAT, ',');
    getline(mystream, dataUnclean.endAT, ',');
    getline(mystream, dataUnclean.startStationName, ',');
    getline(mystream, dataUnclean.endStationName, ',');
    getline(mystream, dataUnclean.start_lat, ',');
    getline(mystream, dataUnclean.start_long, ',');
    getline(mystream, dataUnclean.end_lat, ',');
    getline(mystream, dataUnclean.end_long, ',');
    getline(mystream, dataUnclean.member_cas, '\n');
    unclean.push_back(dataUnclean);
  }
  fileReader.close();
}

//used to get the clean version of the data of divvy data
void readCleanDivvyData(string file, divvyData& dataClean){ 
  ifstream fileReader(file);

  string line;
  getline(fileReader, line);
  
  while (getline(fileReader, line) && !line.empty()){//grabs each datapoint and stores them into a struct then into a struct vector

    stringstream mystream(line);
    getline(mystream, dataClean.rideID, ',');
    getline(mystream, dataClean.rideableTYPE, ',');
    getline(mystream, dataClean.startAT, ',');
    getline(mystream, dataClean.endAT, ',');
    getline(mystream, dataClean.startStationName, ',');
    getline(mystream, dataClean.endStationName, ',');
    getline(mystream, dataClean.start_lat, ',');
    getline(mystream, dataClean.start_long, ',');
    getline(mystream, dataClean.end_lat, ',');
    getline(mystream, dataClean.end_long, ',');
    getline(mystream, dataClean.member_cas,'\n');
    clean.push_back(dataClean);
    //checks to see if any variable is empty, if it is, then it gets rid of the struct in the vector
    if(dataClean.rideID.empty()||
    dataClean.rideableTYPE.empty()||
    dataClean.startAT.empty()||
    dataClean.endAT.empty()||
    dataClean.startStationName.empty()||
    dataClean.endStationName.empty()||
    dataClean.start_lat.empty()||
    dataClean.start_long.empty()||
    dataClean.end_lat.empty()||
    dataClean.end_long.empty()||
    dataClean.member_cas.empty()){
      clean.pop_back();
    }
  }
  fileReader.close();
}

vector<divvyData> uncleanDivvyData(string file, divvyData readUncleanData){
  readUncleanDivvyData(file, readUncleanData);
  return unclean;
}

vector<divvyData> cleanDivvyData(string file, divvyData readCleanData){
  readCleanDivvyData(file, readCleanData);
  return clean;
}

double totalMiles(){ //returns the total amount of miles
  double count = 0;
  for(int i = 0;i<clean.size();i++){
    count += distance(stold(clean.at(i).start_lat),stold(clean.at(i).start_long),stold(clean.at(i).end_lat),stold(clean.at(i).end_long));
  }
  return round(count);
}

long double findLongestTripDist(){//finds the longest trip by calculating the total miles of each trip and comparing each
  long double lastDist = distance(stold(clean.at(0).start_lat),stold(clean.at(0).start_long),stold(clean.at(0).end_lat),stold(clean.at(0).end_long));
  for(int i = 0;i<clean.size();i++){
    long double currentDist = distance(stold(clean.at(i).start_lat),stold(clean.at(i).start_long),stold(clean.at(i).end_lat),stold(clean.at(i).end_long));
    if(currentDist>lastDist){
      lastDist=currentDist;
    }
  }
  return lastDist;
}

double averageTripDist(){//finds the average of each trip distance
  return 1e-1*round(totalMiles()/clean.size()*1e1);
}

int findIndex(){//finds the index of the highest distance
  double lastDist = distance(stold(clean.at(0).start_lat),stold(clean.at(0).start_long),stold(clean.at(0).end_lat),stold(clean.at(0).end_long));
  int highest = 0;
  for(int i = 0;i<clean.size();i++){
    double currentDist = distance(stold(clean.at(i).start_lat),stold(clean.at(i).start_long),stold(clean.at(i).end_lat),stold(clean.at(i).end_long));
    if(currentDist>lastDist){
      lastDist=currentDist;
      highest = i;
    }
  }
  return highest;
}


string findLongestTripID(){//returns the trip at the index of the longest trip
  return clean.at(findIndex()).rideID;
}

string findLongestStartLoc(){ //returns the starting location of the longest trip
  return clean.at(findIndex()).startStationName;
}

string findLongestEndLoc(){ //returns the ending location of the longest trip
  return clean.at(findIndex()).endStationName;
}

void memberPercentage(){//calculates the percentage of the members to casuals
  int memberCount = 0;
  int casualCount = 0;
  for (int i = 0; i<clean.size();i++){
    if(clean.at(i).member_cas == "member\r" ) {
      memberCount++;
    } else{
      casualCount++;
    }
  }
  double casualRides = casualCount / double(clean.size()) * 100;
  double memberRides = memberCount / double(clean.size()) * 100;
  cout << "\tCasual Rider Percentage: " <<fixed<<setprecision(1)<< casualRides << "%" << endl;
  cout << "\tMember Rider Percentage: " <<fixed<<setprecision(1)<< memberRides << "%" << endl;
}

void checkOption(bool& check, int option){//checks the options to see if the first menu choice was chosen at any point.
  if (check == false){
    
      if (option != 1){
        cout << "Your choice --> First read in data by selecting menu option 1" << endl;
        check = false;
      } else {
        cout << "Your choice --> Select datafile:" << endl;
        check = true;
      }
  }
}
int findLargestNumberOfRides(){//finds the largest number of rides
  int weekdayCount = 0;
  int weekendCount = 0;
  int time_int;
  int date_int;
  vector<int> timeVector;
  vector<int> dateVector;
  vector<int> weekdayVector;
  vector<int> weekendVector;

  for(int i = 0; i < clean.size(); i++){//inserts the trips into the correspoding hour slots in the vector
    
    string time = ((clean.at(i)).startAT).substr(9, clean.at(i).startAT.find(":"));
    int time_int = stoi(time);
    timeVector.push_back(time_int);
    string date = clean.at(i).startAT.substr(2,1);
    int date_int = stoi(date);
    if(date_int == 1){
      weekdayVector.push_back(time_int);
    }
    else if (date_int == 4){
      weekendVector.push_back(time_int);
    }
  }
  int largestTrip = 0;
  for(int k = 0; k <= 23; k++){//checks each vector to calculate the highest trip
    for(int i = 0; i < weekdayVector.size(); i++){//checks the days
      if(k == weekdayVector.at(i)){
        weekdayCount++;
        if(weekdayCount>largestTrip){
          largestTrip=weekdayCount;
        }
      }
    }
    for (int i = 0; i < weekendVector.size(); i++){//checks the weekends
      if (k == weekendVector.at(i)){
        weekendCount++;
        if(weekendCount>largestTrip){
          largestTrip=weekendCount;
        }
      }
    }
    weekdayCount = 0;
    weekendCount = 0;
  }
  timeVector.clear();
  dateVector.clear();
  weekdayVector.clear();
  weekendVector.clear();
  return largestTrip;
}

void displayRides(){//displays the rides per hour
  int weekdayCount = 0;
  int weekendCount = 0;
  int time_int;
  int date_int;
  vector<int> timeVector;
  vector<int> dateVector;
  vector<int> weekdayVector;
  vector<int> weekendVector;

  for(int i = 0; i < clean.size(); i++){//inserts each instance a ride is found with the hour it was found
    
    string time = ((clean.at(i)).startAT).substr(9, clean.at(i).startAT.find(":"));
    int time_int = stoi(time);
    timeVector.push_back(time_int);
    string date = clean.at(i).startAT.substr(2,1);
    int date_int = stoi(date);

    if(date_int == 1){
      weekdayVector.push_back(time_int);
    } else if (date_int == 4){
      weekendVector.push_back(time_int);
    }
  }
  for(int k = 0; k <= 23; k++){//puts all instances of an hour into a vector and at a certain location, adding them together.
    cout << k <<":\t";
    for(int i = 0; i < weekdayVector.size(); i++){
      if(k == weekdayVector.at(i)){
        weekdayCount++;
      }
    }
    for (int i = 0; i < weekendVector.size(); i++){
      if (k == weekendVector.at(i)){
        weekendCount++;
      }
    }
    cout << weekdayCount << "\t" << weekendCount << endl;
    weekdayCount=0;
    weekendCount=0;
  }
  
}

//for reference for this function https://piazza.com/class/ksgmk54b32v3xu?cid=960
void displayColumnGraph(){//for menu option 4, option
  int weekdayCount = 0;
  int weekendCount = 0;
  int time_int;
  int date_int;
  vector<int> timeVector;
  vector<int> dateVector;
  vector<int> weekdayVector;
  vector<int> weekendVector;

//finds the instances of the time and trips and puts each of the times per weekday or weekend into two seperate vectors
  for(int i = 0; i < clean.size(); i++){
    
    string time = ((clean.at(i)).startAT).substr(9, clean.at(i).startAT.find(":"));
    int time_int = stoi(time);
    timeVector.push_back(time_int);
    string date = clean.at(i).startAT.substr(2,1);
    int date_int = stoi(date);

    if(date_int == 1){
      weekdayVector.push_back(time_int);
    }
    else if (date_int == 4){
      weekendVector.push_back(time_int);
      
    }
  }

  vector<int> weekdayHours;
  vector<int> weekendHours;
  double largestTrip = 0;
  //adds up each instance of the trip per hour into one number and correlates that into each its own seperate vector per hour
  for(int k = 0; k <= 23; k++){
    for(int i = 0; i < weekdayVector.size(); i++){
      if(k == weekdayVector.at(i)){
        weekdayCount++;
      }
    }
    weekdayHours.push_back(weekdayCount);//collects the number of trips for the hour and stores them for weekday
    for (int i = 0; i < weekendVector.size(); i++){
      if (k == weekendVector.at(i)){
        weekendCount++;
      }
    }
    weekendHours.push_back(weekendCount);//collects the number of trips for the hour and stores them for weekend
    //sets the largest trip to highest along with the day with the highest
    if (weekdayCount > weekendCount){
      if (weekdayCount > largestTrip){
        largestTrip = weekdayCount;
      }
    } else {
       if (weekendCount > largestTrip){
        largestTrip = weekendCount;
      }
    }
    weekdayCount = 0;
    weekendCount = 0;
  }
  //formula for the characters (numberOfRidesAtASpecificHour / #highestInfluence) *50
  for (int i = 0; i < 24; i++){
    cout << i << ":";
    int weekdayIteration = (weekdayHours.at(i)/largestTrip) * 50;
    for (int j = 0; j < weekdayIteration; j++){//this one is for the @'s the weekdays
      cout << "@";
    }
    cout << endl;
    int weekendIteration = (weekendHours.at(i)/largestTrip) * 50;
    for (int j = 0; j < weekendIteration; j++){//this one is for the +'s the weekdays
      cout << "+";
    }
    cout << endl;
  }
  return;
}
const void cases(int& option, bool &ifOne){//goes through the effort of getting the user input and initiating what case is to appear depending on that option
  divvyData data;
  displayMenu();
  cin >> option;
  checkOption(ifOne, option);
  if (ifOne == false){
    cases(option, ifOne);
    return;
  }

  vector<divvyData> divData;
  vector<divvyData> cleanDivData;
  switch(option){
      case 1: 
        int dataFileOption;
        cout << "\t1. Small subset of data with 14 rides to help create your program" << endl;
        cout << "\t2. Week day vs weekend rides" << endl;
        cout << "\t3. All September 2021 data (not tested in Zybooks)" << endl;
        cin >> dataFileOption;
          if (dataFileOption == 1) {
            cout << "Your selection -->" << endl;
            divData = uncleanDivvyData("divvyridesampledata.csv",data);
            cleanDivData = cleanDivvyData("divvyridesampledata.csv", data);
            cout << "Total # of trips found in datafile: " << divData.size() << endl; //should be 13
            cout << "Total # of trips in clean data: " << cleanDivData.size() << endl; //should be 10
            cases(option, ifOne);
            return;
          } else if (dataFileOption == 2) {
            cout << "Your selection --> " << endl;
            divData = uncleanDivvyData("weekdayweekend.csv",data);
            cleanDivData = cleanDivvyData("weekdayweekend.csv",data);
            cout << "Total # of trips found in datafile: " << divData.size() <<endl;
            cout << "Total # of trips in clean data: " << cleanDivData.size()<<endl;
            cases(option, ifOne);
            return;
          }
      case 2:
        cout << "Your choice -->\n" << endl;
        cout << "\tTotal # of miles traveled: " << totalMiles() <<endl;
        cout << "\tAverage length of trips in miles: " << fixed<<setprecision(1)<<averageTripDist() <<endl<<endl;
        cout<< "\tLongest trip information below: "<<endl;
        cout<< "\t-------------------------------"<<endl;
        cout<< "\tTrip ID: " << findLongestTripID() << endl;
        cout<< "\tTrip start location: "<< findLongestStartLoc() << endl;
        cout<< "\tTrip end location: " << findLongestEndLoc() << endl;
        cout<< "\tTrip distance in miles: " << findLongestTripDist() << endl;
        cases(option, ifOne);
        return;
      case 3: 
        cout << "Your choice -->\n" << endl;
        memberPercentage();
        cases(option, ifOne);
        return;
      case 4:
        int displayType;
        cout << "Your choice --> ";
        cout << "Select type of display: " << endl;
        cout << "\t1. Counts of rides per hour in the day"<<endl;
        cout << "\t2. Proportional 50 column graph with @ for weekday and + for weekend" << endl;
        cin >> displayType;
        if(displayType==1){
          cout<< "Your selection--> " << "LargestNumberOfRides is: " << findLargestNumberOfRides() << endl;
          cout<< "\tRides per hour for weekday and weekend: "<<endl;
          displayRides();
          cases(option, ifOne);
          return;
        } else if(displayType==2){
          cout<< "Your selection-->\t" << endl;
          displayColumnGraph();
          cases(option, ifOne);
          return;
        }
      case 6:
        cout << "Your choice -->";
        exit(0);
    }
}

int main() {
  cout << "Analyze Divvy bike ride information using September trip data.\nFirst select option 1 to read and clean the data." << endl << endl;
  //cases
  int option = 0;
  bool ifOne = false;
  cases(option, ifOne);
  return 0;
} 