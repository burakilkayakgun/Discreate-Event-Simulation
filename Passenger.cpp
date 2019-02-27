#include "Passenger.h"

Passenger::Passenger(int _arrivalTime, int _flightTime, int _waitLuggageTime, int _waitSecurityTime, std::string _isVip,
                     std::string _haveLuggage) {
        this->arrivalTime=_arrivalTime;
        this->flightTime=_flightTime;
        this->waitLuggageTime=_waitLuggageTime;
        this->waitSecurityTime=_waitSecurityTime;
        this->isVip=std::move(_isVip);
        this->haveLuggage= std::move(_haveLuggage);
}

Passenger::Passenger() {}

Passenger::~Passenger() {}