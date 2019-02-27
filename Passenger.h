#ifndef PROJECT_2_PASSENGER_H
#define PROJECT_2_PASSENGER_H

#include <string>


class Passenger {
public:
    int arrivalTime = 0;
    int flightTime = 0;
    int waitLuggageTime = 0;
    int waitSecurityTime = 0;
    int waitingTime = 0;
    bool  isLate2Flight = false;
    std::string isVip = "";
    std::string haveLuggage = "";

    Passenger(int _arrivalTime,int _flightTime,int _waitLuggageTime,int _waitSecurityTime,std::string _isVip,std::string _haveLuggage);
    Passenger();
    ~Passenger();
};


#endif //PROJECT_2_PASSENGER_H
