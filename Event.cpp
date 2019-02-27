#include "Event.h"

Event::Event(int num, int time, EventType type, Passenger *_passenger,int _priorityForCompare){
    this->num=num;
    this->time=time;
    this->type=type;
    this->passenger=_passenger;
    this->priorityForCompare =_priorityForCompare;
}

Event::Event(const Event &other) {
    num=other.num;
    time=other.time;
    type=other.type;
    passenger=other.passenger;

}