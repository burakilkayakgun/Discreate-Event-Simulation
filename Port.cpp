#include "Port.h"

Port::Port(int L, int S) {
    this->L=L;
    this->S=S;
    luggageChecker.reserve(L);
    for(int i=0;i< L;i++){
        Employee newEmployee ;
        luggageChecker.push_back(newEmployee);
    }
    securityChecker.reserve(S);
    for(int i =0 ; i<S;i++){
        Employee newEmployee;
        securityChecker.push_back(newEmployee);
    }
}

void Port::runAllCases(vector<Passenger> &passenger) {
    runForCase1(passenger);
    runForCase2(passenger);
    runForCase3(passenger);
    runForCase4(passenger);
    runForCase5(passenger);
    runForCase6(passenger);
    runForCase7(passenger);
    runForCase8(passenger);
}



// case1 that contains not PQ ( priority queue ) , not VIP , not OT ( online ticket )
void Port::runForCase1(vector<Passenger> &passenger) {
    eventQueueGenerator(passenger);
    while( !eventQueue.empty() ){
        //read info from the eventQueue's first element
        Event currentEvent = eventQueue.top();
        Passenger* currentPassenger = currentEvent.passenger;
        int currentNum = currentEvent.num;
        currentTime=currentEvent.time; // updating the time
        eventQueue.pop();

        if (currentEvent.type== EventType::securityControl){
            securityChecker[currentNum].isBusy=false; //this busy security counter is free right now
            //time between the arrival of a passenger to the terminal and her  departure from the security check.
            currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
            totalWaitingTimeCase1+= currentPassenger->waitingTime;
            if(currentTime > currentPassenger->flightTime){  // if passenger miss her flight update her info
                currentPassenger->isLate2Flight = true;
                ++missFlightCase1;
            }
            /* after this situation there is a free security counter, if securityQueue is not empty
             * read info of the first element of it and create a new securityControl event then pop
             * the first element.
            */
            if(!securityQueue.empty()){
                securityChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = securityQueue.front();
                Event newEvent(currentNum,currentTime + newPassenger->waitSecurityTime,EventType::securityControl,newPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                securityQueue.pop();
            }
        } else if (currentEvent.type == EventType::luggageControl){
            luggageChecker[currentNum].isBusy = false; // this busy luggage counter is free right now
            int freeSecurityChecker = getFreeSecurityChecker(); // check is there any free security counter
            if (freeSecurityChecker == -5){  // there is no free security counter
                securityQueue.push(currentPassenger);  // push passenger to the first-come-first-serve security queue
            } else {
                // there is a free security counter
                securityChecker[freeSecurityChecker].isBusy = true;  // this free counter is busy right now
                Event newEvent(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType::securityControl,currentPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
            }
            /* after this situation there is a free luggage counter, if luggageQueue is not empty
             * read info of the first element of it and create a new luggageControl event then pop
             * the first element.
            */
            if(!luggageQueue.empty()){
                luggageChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = luggageQueue.front();
                Event newEvent(currentNum,currentTime + newPassenger->waitLuggageTime,EventType::luggageControl,newPassenger,1); // create new event with type luggageControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                luggageQueue.pop();
            }
        } else if (currentEvent.type == EventType::arrival){
            int freeLuggageChecker = getFreeLuggageChecker(); // check is there any free luggage counter
            if(freeLuggageChecker == -5){  // there is no free luggage counter
                luggageQueue.push(currentPassenger); // push passenger to the first-come-first-serve luggage queue
            } else {
                // there is a free luggage counter
                luggageChecker[freeLuggageChecker].isBusy = true; // this free counter is busy right now
                Event newEvent(freeLuggageChecker,currentTime + currentPassenger->waitLuggageTime,EventType::luggageControl,currentPassenger,1); // create new event with type luggageControl
                eventQueue.push(newEvent); // push this event to the eventQueue
            }
        }
    }
}

// case2 that contains PQ ( priority queue ) , not VIP , not OT ( online ticket )
void Port::runForCase2(vector<Passenger> &passenger) {
    runCase0();
    eventQueueGenerator(passenger);
    while( !eventQueue.empty() ){
        Event currentEvent = eventQueue.top();
        Passenger* currentPassenger = currentEvent.passenger;
        int currentNum = currentEvent.num;
        currentTime=currentEvent.time;
        eventQueue.pop();

        if (currentEvent.type == EventType::securityControl){
            securityChecker[currentNum].isBusy = false; //this busy security counter is free right now
            //time between the arrival of a passenger to the terminal and her  departure from the security check.
            currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime ;
            totalWaitingTimeCase2 += (double) currentPassenger->waitingTime;
            if(currentTime > currentPassenger->flightTime){ // if passenger miss her flight update her info
                currentPassenger->isLate2Flight = true;
                missFlightCase2++;
            }
            /* after this situation there is a free security counter, if securityQueue is not empty
             * read info of the first element of it and create a new securityControl event then pop
             * the first element.
            */
            if( !securityPriQueue.empty() ){
                securityChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = securityPriQueue.top();
                Event event(currentNum,currentTime + newPassenger->waitSecurityTime, EventType::securityControl,newPassenger,0); // create new event with type securityControl
                eventQueue.push(event);  // push this event to the eventQueue
                securityPriQueue.pop();
            }
        } else if (currentEvent.type == EventType::luggageControl){
            luggageChecker[currentNum].isBusy= false; // this busy luggage counter is free right now
            int freeSecurityChecker = getFreeSecurityChecker(); // check is there any free security counter
            if(freeSecurityChecker == -5){  // there is no free security counter
                securityPriQueue.push(currentPassenger);  // push passenger to the first-to-flight-first-serve security queue
            } else {
                // there is a free security counter
                securityChecker[freeSecurityChecker].isBusy=true;  // this free counter is busy right now
                Event event(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType ::securityControl,currentPassenger,0); // create new event with type securityControl
                eventQueue.push(event); // push this event to the eventQueue
            }
            /* after this situation there is a free luggage counter, if luggageQueue is not empty
             * read info of the first element of it and create a new luggageControl event then pop
             * the first element.
            */
            if( !luggagePriQueue.empty()){
                luggageChecker[currentNum].isBusy= true;  // this free counter is busy right now
                Passenger* newPassenger = luggagePriQueue.top();
                Event event(currentNum,currentTime + newPassenger->waitLuggageTime,EventType::luggageControl,newPassenger,1); // create new event with type luggageControl
                eventQueue.push(event); // push this event to the eventQueue
                luggagePriQueue.pop();
            }
        } else if (currentEvent.type == EventType::arrival){
            int freeLuggageChecker = getFreeLuggageChecker(); // check is there any free luggage counter
            if (freeLuggageChecker == -5){  // there is no free luggage counter
                luggagePriQueue.push(currentPassenger); // push passenger to the first-to-flight-first-serve luggage queue
            } else {
                luggageChecker[freeLuggageChecker].isBusy = true;  // this free counter is busy right now
                Event event(freeLuggageChecker,currentTime + currentPassenger->waitLuggageTime,EventType::luggageControl,currentPassenger,1);  // create new event with type luggageControl
                eventQueue.push(event); // push this event to the eventQueue
            }
        }
    }
}

// case3 that contains not PQ ( priority queue ) , VIP , not OT ( online ticket )
void Port::runForCase3(vector<Passenger> &passenger) {
    runCase0();
    eventQueueGenerator(passenger);
    while( !eventQueue.empty() ){
        Event currentEvent = eventQueue.top();
        Passenger* currentPassenger = currentEvent.passenger;
        int currentNum = currentEvent.num;
        currentTime = currentEvent.time;
        eventQueue.pop();

        if ( currentEvent.type == EventType::securityControl ){
            securityChecker[currentNum].isBusy=false; //this busy security counter is free right now
            //time between the arrival of a passenger to the terminal and her  departure from the security check.
            currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
            totalWaitingTimeCase3 += (double) currentPassenger->waitingTime;
            if(currentTime > currentPassenger->flightTime){  // if passenger miss her flight update her info
                currentPassenger->isLate2Flight = true;
                missFlightCase3++;
            }
            /* after this situation there is a free security counter, if securityQueue is not empty
             * read info of the first element of it and create a new securityControl event then pop
             * the first element.
            */
            if(!securityQueue.empty()){
                securityChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = securityQueue.front();
                Event newEvent(currentNum,currentTime + newPassenger->waitSecurityTime,EventType::securityControl,newPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                securityQueue.pop();
            }
        } else if ( currentEvent.type == EventType::luggageControl ){
            luggageChecker[currentNum].isBusy = false;
            int freeSecurityChecker = getFreeSecurityChecker();
            if( currentPassenger->isVip == "V"){  // skip the security counter and go out the system
                currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime ;
                totalWaitingTimeCase3 += (double) currentPassenger->waitingTime;
                if(currentTime > currentPassenger->flightTime){ // if passenger miss her flight update her info
                    currentPassenger->isLate2Flight = true;
                    missFlightCase3++;
                }
            } else if ( freeSecurityChecker == -5 ){
                securityQueue.push(currentPassenger);
            } else {
                securityChecker[freeSecurityChecker].isBusy = true ;
                Event event(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType ::securityControl,currentPassenger,0); // create new event with type securityControl
                eventQueue.push(event); // push this event to the eventQueue
            }
            /* after this situation there is a free luggage counter, if luggageQueue is not empty
             * read info of the first element of it and create a new luggageControl event then pop
             * the first element.
            */
            if(!luggageQueue.empty()){
                luggageChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = luggageQueue.front();
                Event newEvent(currentNum,currentTime + newPassenger->waitLuggageTime,EventType::luggageControl,newPassenger,1); // create new event with type luggageControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                luggageQueue.pop();
            }
        } else if ( currentEvent.type == EventType::arrival ){
            int freeLuggageChecker = getFreeLuggageChecker();
            if( freeLuggageChecker == -5 ){
                luggageQueue.push(currentPassenger);
            } else {
                luggageChecker[freeLuggageChecker].isBusy= true;
                Event event(freeLuggageChecker,currentTime + currentPassenger->waitLuggageTime,EventType::luggageControl,currentPassenger,1);
                eventQueue.push(event);
            }
        }
    }
}

// case4 that contains PQ ( priority queue ) , VIP , not OT ( online ticket )
void Port::runForCase4(vector<Passenger> &passenger) {
    runCase0();
    eventQueueGenerator(passenger);
    while ( !eventQueue.empty() ){
        Event currentEvent = eventQueue.top();
        Passenger* currentPassenger = currentEvent.passenger;
        currentTime=currentEvent.time;
        int currentNum = currentEvent.num;
        eventQueue.pop();

        if ( currentEvent.type == EventType::securityControl ){
            securityChecker[currentNum].isBusy = false; //this busy security counter is free right now
            //time between the arrival of a passenger to the terminal and her  departure from the security check.
            currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime ;
            totalWaitingTimeCase4 += (double) currentPassenger->waitingTime;
            if(currentTime > currentPassenger->flightTime){ // if passenger miss her flight update her info
                currentPassenger->isLate2Flight = true;
                missFlightCase4++;
            }
            /* after this situation there is a free security counter, if securityQueue is not empty
             * read info of the first element of it and create a new securityControl event then pop
             * the first element.
            */
            if( !securityPriQueue.empty() ){
                securityChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = securityPriQueue.top();
                Event event(currentNum,currentTime + newPassenger->waitSecurityTime, EventType::securityControl,newPassenger,0); // create new event with type securityControl
                eventQueue.push(event);  // push this event to the eventQueue
                securityPriQueue.pop();
            }
        } else if ( currentEvent.type == EventType::luggageControl){
            luggageChecker[currentNum].isBusy = false;
            int freeSecurityChecker = getFreeSecurityChecker();
            if (currentPassenger->isVip == "V"){
                currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
                totalWaitingTimeCase4 += (double) currentPassenger->waitingTime;
                if(currentTime > currentPassenger->flightTime){
                    currentPassenger->isLate2Flight = true;
                    missFlightCase4++;
                }
            } else if (freeSecurityChecker == -5){
                securityPriQueue.push(currentPassenger);
            } else {
                securityChecker[freeSecurityChecker].isBusy = true;
                Event event(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType ::securityControl,currentPassenger,0); // create new event with type securityControl
                eventQueue.push(event); // push this event to the eventQueue
            }
            /* after this situation there is a free luggage counter, if luggageQueue is not empty
             * read info of the first element of it and create a new luggageControl event then pop
             * the first element.
            */
            if( !luggagePriQueue.empty()){
                luggageChecker[currentNum].isBusy= true;  // this free counter is busy right now
                Passenger* newPassenger = luggagePriQueue.top();
                Event event(currentNum,currentTime + newPassenger->waitLuggageTime,EventType::luggageControl,newPassenger,1); // create new event with type luggageControl
                eventQueue.push(event); // push this event to the eventQueue
                luggagePriQueue.pop();
            }
        } else if ( currentEvent.type == EventType::arrival){
            int freeLuggageChecker = getFreeLuggageChecker();
            if( freeLuggageChecker == -5 ){
                luggagePriQueue.push(currentPassenger);
            } else {
                luggageChecker[freeLuggageChecker].isBusy = true;
                Event event(freeLuggageChecker,currentTime + currentPassenger->waitLuggageTime,EventType::luggageControl,currentPassenger,1);
                eventQueue.push(event);
            }
        }
    }
}

// case5 that contains not PQ ( priority queue ) , not VIP , OT ( online ticket )
void Port::runForCase5(vector<Passenger> &passenger) {
    runCase0();
    eventQueueGenerator(passenger);
    while ( !eventQueue.empty() ){
        Event currentEvent = eventQueue.top();
        Passenger* currentPassenger = currentEvent.passenger;
        currentTime=currentEvent.time;
        int currentNum = currentEvent.num;
        eventQueue.pop();

        if ( currentEvent.type == EventType::securityControl){
            securityChecker[currentNum].isBusy=false; //this busy security counter is free right now
            //time between the arrival of a passenger to the terminal and her  departure from the security check.
            currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
            totalWaitingTimeCase5 += (double) currentPassenger->waitingTime;
            if(currentTime > currentPassenger->flightTime){  // if passenger miss her flight update her info
                currentPassenger->isLate2Flight = true;
                missFlightCase5++;
            }
            /* after this situation there is a free security counter, if securityQueue is not empty
             * read info of the first element of it and create a new securityControl event then pop
             * the first element.
            */
            if(!securityQueue.empty()){
                securityChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = securityQueue.front();
                Event newEvent(currentNum,currentTime + newPassenger->waitSecurityTime,EventType::securityControl,newPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                securityQueue.pop();
            }
        } else if ( currentEvent.type == EventType::luggageControl){
            luggageChecker[currentNum].isBusy = false; // this busy luggage counter is free right now
            int freeSecurityChecker = getFreeSecurityChecker(); // check is there any free security counter
            if (freeSecurityChecker == -5){  // there is no free security counter
                securityQueue.push(currentPassenger);  // push passenger to the first-come-first-serve security queue
            } else {
                // there is a free security counter
                securityChecker[freeSecurityChecker].isBusy = true;  // this free counter is busy right now
                Event newEvent(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType::securityControl,currentPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
            }
            /* after this situation there is a free luggage counter, if luggageQueue is not empty
             * read info of the first element of it and create a new luggageControl event then pop
             * the first element.
            */
            if(!luggageQueue.empty()){
                luggageChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = luggageQueue.front();
                Event newEvent(currentNum,currentTime + newPassenger->waitLuggageTime,EventType::luggageControl,newPassenger,1); // create new event with type luggageControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                luggageQueue.pop();
            }
        } else if ( currentEvent.type == EventType::arrival ){
            int freeLuggageChecker = getFreeLuggageChecker();
            if(currentPassenger->haveLuggage == "N" ){
                int freeSecurityChecker = getFreeSecurityChecker(); // check is there any free security counter
                if (freeSecurityChecker == -5){  // there is no free security counter
                    securityQueue.push(currentPassenger);  // push passenger to the first-come-first-serve security queue
                } else {
                    // there is a free security counter
                    securityChecker[freeSecurityChecker].isBusy = true;  // this free counter is busy right now
                    Event newEvent(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType::securityControl,currentPassenger,0); // create new event with type securityControl
                    eventQueue.push(newEvent); // push this event to the eventQueue
                }
            } else if ( freeLuggageChecker == -5 ){
                luggageQueue.push(currentPassenger);
            } else {
                luggageChecker[freeLuggageChecker].isBusy= true;
                Event event(freeLuggageChecker,currentTime + currentPassenger->waitLuggageTime,EventType::luggageControl,currentPassenger,1);
                eventQueue.push(event);
            }
        }
    }
}

// case6 that contains PQ ( priority queue ) , not VIP , OT ( online ticket )
void Port::runForCase6(vector<Passenger> &passenger) {
    runCase0();
    eventQueueGenerator(passenger);
    while ( !eventQueue.empty() ) {
        Event currentEvent = eventQueue.top();
        Passenger* currentPassenger = currentEvent.passenger;
        currentTime=currentEvent.time;
        int currentNum = currentEvent.num;
        eventQueue.pop();

        if ( currentEvent.type == EventType::securityControl ){
            securityChecker[currentNum].isBusy=false; //this busy security counter is free right now
            //time between the arrival of a passenger to the terminal and her  departure from the security check.
            currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
            totalWaitingTimeCase6 += (double) currentPassenger->waitingTime;
            if(currentTime > currentPassenger->flightTime){  // if passenger miss her flight update her info
                currentPassenger->isLate2Flight = true;
                missFlightCase6++;
            }
            /* after this situation there is a free security counter, if securityQueue is not empty
             * read info of the first element of it and create a new securityControl event then pop
             * the first element.
            */
            if(!securityPriQueue.empty()){
                securityChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = securityPriQueue.top();
                Event newEvent(currentNum,currentTime + newPassenger->waitSecurityTime,EventType::securityControl,newPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                securityPriQueue.pop();
            }
        } else if ( currentEvent.type == EventType::luggageControl ){
            luggageChecker[currentNum].isBusy = false; // this busy luggage counter is free right now
            int freeSecurityChecker = getFreeSecurityChecker(); // check is there any free security counter
            if (freeSecurityChecker == -5){  // there is no free security counter
                securityPriQueue.push(currentPassenger);  // push passenger to the first-come-first-serve security queue
            } else {
                // there is a free security counter
                securityChecker[freeSecurityChecker].isBusy = true;  // this free counter is busy right now
                Event newEvent(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType::securityControl,currentPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
            }
            /* after this situation there is a free luggage counter, if luggageQueue is not empty
             * read info of the first element of it and create a new luggageControl event then pop
             * the first element.
            */
            if(!luggagePriQueue.empty()){
                luggageChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = luggagePriQueue.top();
                Event newEvent(currentNum,currentTime + newPassenger->waitLuggageTime,EventType::luggageControl,newPassenger,1); // create new event with type luggageControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                luggagePriQueue.pop();
            }
        } else if ( currentEvent.type == EventType::arrival ){
            int freeLuggageChecker = getFreeLuggageChecker();
            if(currentPassenger->haveLuggage == "N" ){
                int freeSecurityChecker = getFreeSecurityChecker(); // check is there any free security counter
                if (freeSecurityChecker == -5){  // there is no free security counter
                    securityPriQueue.push(currentPassenger);  // push passenger to the first-come-first-serve security queue
                } else {
                    // there is a free security counter
                    securityChecker[freeSecurityChecker].isBusy = true;  // this free counter is busy right now
                    Event newEvent(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType::securityControl,currentPassenger,0); // create new event with type securityControl
                    eventQueue.push(newEvent); // push this event to the eventQueue
                }
            } else if ( freeLuggageChecker == -5 ){
                luggagePriQueue.push(currentPassenger);
            } else {
                luggageChecker[freeLuggageChecker].isBusy= true;
                Event event(freeLuggageChecker,currentTime + currentPassenger->waitLuggageTime,EventType::luggageControl,currentPassenger,1);
                eventQueue.push(event);
            }
        }
    }
}

// case7 that contains not PQ ( priority queue ) , VIP , OT ( online ticket )
void Port::runForCase7(vector<Passenger> &passenger) {
    runCase0();
    eventQueueGenerator(passenger);
    while ( !eventQueue.empty() ) {
        Event currentEvent = eventQueue.top();
        Passenger* currentPassenger = currentEvent.passenger;
        currentTime=currentEvent.time;
        int currentNum = currentEvent.num;
        eventQueue.pop();

        if ( currentEvent.type == EventType::securityControl) {
            securityChecker[currentNum].isBusy=false; //this busy security counter is free right now
            //time between the arrival of a passenger to the terminal and her  departure from the security check.
            currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
            totalWaitingTimeCase7 += (double) currentPassenger->waitingTime;
            if(currentTime > currentPassenger->flightTime){  // if passenger miss her flight update her info
                currentPassenger->isLate2Flight = true;
                missFlightCase7++;
            }
            /* after this situation there is a free security counter, if securityQueue is not empty
             * read info of the first element of it and create a new securityControl event then pop
             * the first element.
            */
            if(!securityQueue.empty()){
                securityChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = securityQueue.front();
                Event newEvent(currentNum,currentTime + newPassenger->waitSecurityTime,EventType::securityControl,newPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                securityQueue.pop();
            }
        } else if ( currentEvent.type == EventType::luggageControl){
            luggageChecker[currentNum].isBusy = false;
            int freeSecurityChecker = getFreeSecurityChecker();
            if (currentPassenger->isVip == "V"){
                currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
                totalWaitingTimeCase7 += (double) currentPassenger->waitingTime;
                if(currentTime > currentPassenger->flightTime){
                    currentPassenger->isLate2Flight = true;
                    missFlightCase7++;
                }
            } else if (freeSecurityChecker == -5){
                securityQueue.push(currentPassenger);
            } else {
                securityChecker[freeSecurityChecker].isBusy = true;
                Event event(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType ::securityControl,currentPassenger,0); // create new event with type securityControl
                eventQueue.push(event); // push this event to the eventQueue
            }
            /* after this situation there is a free luggage counter, if luggageQueue is not empty
             * read info of the first element of it and create a new luggageControl event then pop
             * the first element.
            */
            if( !luggageQueue.empty()){
                luggageChecker[currentNum].isBusy= true;  // this free counter is busy right now
                Passenger* newPassenger = luggageQueue.front();
                Event event(currentNum,currentTime + newPassenger->waitLuggageTime,EventType::luggageControl,newPassenger,1); // create new event with type luggageControl
                eventQueue.push(event); // push this event to the eventQueue
                luggageQueue.pop();
            }
        } else if ( currentEvent.type == EventType::arrival ){
            int freeLuggageChecker = getFreeLuggageChecker();
            if(currentPassenger->haveLuggage == "N" ) {
                if(currentPassenger->isVip == "V"){
                    currentPassenger->waitingTime = 0;
                    totalWaitingTimeCase7 += (double) currentPassenger->waitingTime;
                    if(currentTime > currentPassenger->flightTime){
                        currentPassenger->isLate2Flight = true;
                        missFlightCase7++;
                    }
                } else {
                    int freeSecurityChecker = getFreeSecurityChecker(); // check is there any free security counter
                    if (freeSecurityChecker == -5){  // there is no free security counter
                        securityQueue.push(currentPassenger);  // push passenger to the first-come-first-serve security queue
                    } else {
                        // there is a free security counter
                        securityChecker[freeSecurityChecker].isBusy = true;  // this free counter is busy right now
                        Event newEvent(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType::securityControl,currentPassenger,0); // create new event with type securityControl
                        eventQueue.push(newEvent); // push this event to the eventQueue
                    }
                }
            } else if ( freeLuggageChecker == -5 ){
                luggageQueue.push(currentPassenger);
            } else {
                luggageChecker[freeLuggageChecker].isBusy= true;
                Event event(freeLuggageChecker,currentTime + currentPassenger->waitLuggageTime,EventType::luggageControl,currentPassenger,1);
                eventQueue.push(event);
            }
        }
    }
}

// case8 that contains PQ ( priority queue ) , VIP , OT ( online ticket )
void Port::runForCase8(vector<Passenger> &passenger) {
    runCase0();
    eventQueueGenerator(passenger);
    while ( !eventQueue.empty() ) {
        Event currentEvent = eventQueue.top();
        Passenger* currentPassenger = currentEvent.passenger;
        currentTime=currentEvent.time;
        int currentNum = currentEvent.num;
        eventQueue.pop();

        if ( currentEvent.type == EventType::securityControl) {
            securityChecker[currentNum].isBusy=false; //this busy security counter is free right now
            //time between the arrival of a passenger to the terminal and her  departure from the security check.
            currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
            totalWaitingTimeCase8 += (double) currentPassenger->waitingTime;
            if(currentTime > currentPassenger->flightTime){  // if passenger miss her flight update her info
                currentPassenger->isLate2Flight = true;
                missFlightCase8++;
            }
            /* after this situation there is a free security counter, if securityQueue is not empty
             * read info of the first element of it and create a new securityControl event then pop
             * the first element.
            */
            if(!securityPriQueue.empty()){
                securityChecker[currentNum].isBusy = true; // this free counter is busy right now
                Passenger* newPassenger = securityPriQueue.top();
                Event newEvent(currentNum,currentTime + newPassenger->waitSecurityTime,EventType::securityControl,newPassenger,0); // create new event with type securityControl
                eventQueue.push(newEvent); // push this event to the eventQueue
                securityPriQueue.pop();
            }
        } else if ( currentEvent.type == EventType::luggageControl){
            luggageChecker[currentNum].isBusy = false;
            int freeSecurityChecker = getFreeSecurityChecker();
            if (currentPassenger->isVip == "V"){
                currentPassenger->waitingTime = currentTime - currentPassenger->arrivalTime;
                totalWaitingTimeCase8 += (double) currentPassenger->waitingTime;
                if(currentTime > currentPassenger->flightTime){
                    currentPassenger->isLate2Flight = true;
                    missFlightCase8++;
                }
            } else if (freeSecurityChecker == -5){
                securityPriQueue.push(currentPassenger);
            } else {
                securityChecker[freeSecurityChecker].isBusy = true;
                Event event(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType ::securityControl,currentPassenger,0); // create new event with type securityControl
                eventQueue.push(event); // push this event to the eventQueue
            }
            /* after this situation there is a free luggage counter, if luggageQueue is not empty
             * read info of the first element of it and create a new luggageControl event then pop
             * the first element.
            */
            if( !luggagePriQueue.empty()){
                luggageChecker[currentNum].isBusy= true;  // this free counter is busy right now
                Passenger* newPassenger = luggagePriQueue.top();
                Event event(currentNum,currentTime + newPassenger->waitLuggageTime,EventType::luggageControl,newPassenger,1); // create new event with type luggageControl
                eventQueue.push(event); // push this event to the eventQueue
                luggagePriQueue.pop();
            }
        } else if ( currentEvent.type == EventType::arrival ){
            int freeLuggageChecker = getFreeLuggageChecker();
            if(currentPassenger->haveLuggage == "N" ) {
                if(currentPassenger->isVip == "V"){
                    currentPassenger->waitingTime = 0;
                    totalWaitingTimeCase8 += (double) currentPassenger->waitingTime;
                    if(currentTime > currentPassenger->flightTime){
                        currentPassenger->isLate2Flight = true;
                        missFlightCase8++;
                    }
                } else {
                    int freeSecurityChecker = getFreeSecurityChecker(); // check is there any free security counter
                    if (freeSecurityChecker == -5){  // there is no free security counter
                        securityPriQueue.push(currentPassenger);  // push passenger to the first-come-first-serve security queue
                    } else {
                        // there is a free security counter
                        securityChecker[freeSecurityChecker].isBusy = true;  // this free counter is busy right now
                        Event newEvent(freeSecurityChecker,currentTime + currentPassenger->waitSecurityTime,EventType::securityControl,currentPassenger,0); // create new event with type securityControl
                        eventQueue.push(newEvent); // push this event to the eventQueue
                    }
                }
            } else if ( freeLuggageChecker == -5 ){
                luggagePriQueue.push(currentPassenger);
            } else {
                luggageChecker[freeLuggageChecker].isBusy= true;
                Event event(freeLuggageChecker,currentTime + currentPassenger->waitLuggageTime,EventType::luggageControl,currentPassenger,1);
                eventQueue.push(event);
            }
        }
    }
}

// get information from the passenger vector then create event to push eventQueue
void Port::eventQueueGenerator(vector<Passenger> &passenger) {
    for(int i=0; i<passenger.size(); i++){
        Event event(-3,passenger[i].arrivalTime,EventType::arrival,&passenger[i],2);
        eventQueue.push(event);
    }
}

// case0 that set the luggage counters' and security counters' information and also currentTime as default
void Port::runCase0() {
    for(int i=0;i<L;i++){
        luggageChecker[i].isBusy = false;
    }
    for(int i=0;i<S;i++){
        securityChecker[i].isBusy=false;
    }
    currentTime = 0;
}

int Port::getFreeLuggageChecker() const {
    for(int i =0;i<L;i++){
        if(!luggageChecker[i].isBusy){
            return i;
        }
    }
    return -5;
}

int Port::getFreeSecurityChecker() const {
    for(int i=0;i<S;i++){
        if(!securityChecker[i].isBusy){
            return i;
        }
    }
    return  -5;
}

Port::~Port() {}

double Port::getTotalWaitingTimeCase1() const { return totalWaitingTimeCase1;}

double Port::getTotalWaitingTimeCase2() const { return totalWaitingTimeCase2;}

double Port::getTotalWaitingTimeCase3() const { return totalWaitingTimeCase3;}

double Port::getTotalWaitingTimeCase4() const { return totalWaitingTimeCase4;}

double Port::getTotalWaitingTimeCase5() const { return totalWaitingTimeCase5;}

double Port::getTotalWaitingTimeCase6() const { return totalWaitingTimeCase6;}

double Port::getTotalWaitingTimeCase7() const { return totalWaitingTimeCase7;}

double Port::getTotalWaitingTimeCase8() const { return totalWaitingTimeCase8;}

int Port::getMissFlightCase1() const {return missFlightCase1;}

int Port::getMissFlightCase2() const {return missFlightCase2;}

int Port::getMissFlightCase3() const {return missFlightCase3;}

int Port::getMissFlightCase4() const {return missFlightCase4;}

int Port::getMissFlightCase5() const {return missFlightCase5;}

int Port::getMissFlightCase6() const {return missFlightCase6;}

int Port::getMissFlightCase7() const {return missFlightCase7;}

int Port::getMissFlightCase8() const {return missFlightCase8;}

