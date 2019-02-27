#ifndef PROJECT_2_EVENT_H
#define PROJECT_2_EVENT_H

#include "Passenger.h"

enum class EventType{
    arrival,
    luggageControl,
    securityControl,
};

class Event {
public:
    int num  ;
    int time ;
    Passenger* passenger = nullptr;
    EventType type ;
    int priorityForCompare;
    Event(const Event & other);
    Event(int num,int time,EventType type,Passenger* passe,int priorityForCompare);
};


#endif //PROJECT_2_EVENT_H