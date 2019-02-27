#ifndef PROJECT_2_PORT_H
#define PROJECT_2_PORT_H

#include <vector>
#include <string>
#include <queue>
#include "Event.h"
#include "Employee.h"
#include "Passenger.h"

using namespace std;

// comparator for first to fly, first serve priority queue
struct Comparator1{
    bool operator()(const Passenger* left , const Passenger* right){
        if (left->flightTime == right->flightTime)
            return left->arrivalTime > right->arrivalTime;
        else
            return left->flightTime > right->flightTime;
    }
};

//comparator for event queue that are sorted in term of time
struct Comparator2{
    bool operator()(const Event& left, const Event& right){
        if(left.time == right.time){
            return left.passenger->arrivalTime > right.passenger->arrivalTime ;
        }else{
            return left.time > right.time;
        }
    }
};


class Port {
private:
    int L,S; // number of luggage counter and security counter
    int currentTime = 0;  // time counter for eventQueue
    vector<Employee> luggageChecker;  // luggage counter vector
    vector<Employee> securityChecker;  //  security counter vector
    queue<Passenger*> luggageQueue;   // for case 1,3,5,7
    queue<Passenger*> securityQueue;    // for case 1,3,5,7
    priority_queue<Passenger*,vector<Passenger*>,Comparator1> luggagePriQueue; // for case 2,4,6,8
    priority_queue<Passenger*,vector<Passenger*>,Comparator1> securityPriQueue;  // for case 2,4,6,8
    priority_queue<Event,vector<Event>,Comparator2> eventQueue;
    double totalWaitingTimeCase1 =0.0;
    double totalWaitingTimeCase2 =0.0;
    double totalWaitingTimeCase3 =0.0;
    double totalWaitingTimeCase4 =0.0;
    double totalWaitingTimeCase5 =0.0;
    double totalWaitingTimeCase6 =0.0;
    double totalWaitingTimeCase7 =0.0;
    double totalWaitingTimeCase8 =0.0;
    int missFlightCase1 =0;
    int missFlightCase2 =0;
    int missFlightCase3 =0;
    int missFlightCase4 =0;
    int missFlightCase5 =0;
    int missFlightCase6 =0;
    int missFlightCase7 =0;
    int missFlightCase8 =0;
    int getFreeLuggageChecker() const ;
    int getFreeSecurityChecker() const ;
    void runCase0();
    void eventQueueGenerator(vector<Passenger> & passenger);

public:
    Port(int L,int S);
    ~Port();
    void runAllCases(vector<Passenger> & passenger);
    void runForCase1(vector<Passenger> & passenger);
    void runForCase2(vector<Passenger> & passenger);
    void runForCase3(vector<Passenger> & passenger);
    void runForCase4(vector<Passenger> & passenger);
    void runForCase5(vector<Passenger> & passenger);
    void runForCase6(vector<Passenger> & passenger);
    void runForCase7(vector<Passenger> & passenger);
    void runForCase8(vector<Passenger> & passenger);
    double getTotalWaitingTimeCase1() const ;
    double getTotalWaitingTimeCase2() const ;
    double getTotalWaitingTimeCase3() const ;
    double getTotalWaitingTimeCase4() const ;
    double getTotalWaitingTimeCase5() const ;
    double getTotalWaitingTimeCase6() const ;
    double getTotalWaitingTimeCase7() const ;
    double getTotalWaitingTimeCase8() const ;
    int getMissFlightCase8() const ;
    int getMissFlightCase7() const ;
    int getMissFlightCase6() const ;
    int getMissFlightCase5() const ;
    int getMissFlightCase4() const ;
    int getMissFlightCase3() const ;
    int getMissFlightCase2() const ;
    int getMissFlightCase1() const ;



};


#endif //PROJECT_2_PORT_H