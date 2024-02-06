#include "Volunteer.h"
#include <string>
#include <iostream>


// Volunteer abstract class
Volunteer:: Volunteer(int _id, const string &_name) : completedOrderId(NO_ORDER) , activeOrderId(NO_ORDER) , id(_id) , name(_name){}

Volunteer:: ~Volunteer(){}

int Volunteer::getId() const{
    // Returns the Volunteers id
    return id;
}

const string& Volunteer::getName() const{
    // Returns the Volunteers name
    return name;
}

int Volunteer:: getActiveOrderId() const{
    // Returns the Volunteers active order id
    return activeOrderId;
}

int Volunteer:: getCompletedOrderId() const{
    // Returns the Volunteers completed order id
    return completedOrderId;
}

bool Volunteer:: isBusy() const{
    //Checks if the volunteer is busy processing and order or completed an order but didnt transfered to a driver
    return !(activeOrderId == NO_ORDER && completedOrderId == NO_ORDER);
} 

void Volunteer:: freeVolunteer(){
    //Sets the volunteer to be not busy
    completedOrderId = NO_ORDER;
}

// Collector Volunteer class
CollectorVolunteer:: CollectorVolunteer(int _id, const string &_name, int _coolDown) : Volunteer(_id,_name) , coolDown(_coolDown), timeLeft(NO_ORDER) {}

CollectorVolunteer:: ~CollectorVolunteer(){}

CollectorVolunteer* CollectorVolunteer:: clone() const{
    // Clones the object
    return new CollectorVolunteer(*this);
} 

void CollectorVolunteer:: step(){
    // Takes a step in the game: decreases time left by 1,
    // if the volunteer ended the proccess he resets active order id and time left to NO_ORDER and sets completed order id to the order that finished
    if(decreaseCoolDown()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        timeLeft = NO_ORDER;
    }
}

int CollectorVolunteer:: getCoolDown() const{
    // Returns coolDown
    return coolDown;
}

int CollectorVolunteer:: getTimeLeft() const{
    // Returns timeLeft
    return timeLeft;
}

bool CollectorVolunteer:: decreaseCoolDown(){
    // Decreases coolDown by 1 and returns if the volunteer has finished processing the order
    if(timeLeft>=1){
        timeLeft -= 1;
    }
    return (timeLeft==0);
}

bool CollectorVolunteer:: hasOrdersLeft() const{
    // Return if he has more orders to take
    return true;
}

bool CollectorVolunteer:: canTakeOrder(const Order& order) const{
    // Checks if he is busy processing an order or completed an order but didnt deliver to a driver
    return !(isBusy());
}

void CollectorVolunteer:: acceptOrder(const Order& order){
    // Accepts order: sets new active order id
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer:: toString() const{
    // prints collector volunteer
    return "Collector " + getName() + " " + std::to_string(getId()) + " " + std::to_string(getTimeLeft()) + " " + std::to_string(coolDown) + " " + std::to_string(getActiveOrderId()) + " " + std::to_string(getCompletedOrderId());
}



// LimitedCollector Volunteer class
LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int _id, const string &_name, int _coolDown, int _maxOrders) : CollectorVolunteer(_id,_name,_coolDown) , maxOrders(_maxOrders) , ordersLeft(_maxOrders){}

LimitedCollectorVolunteer* LimitedCollectorVolunteer:: clone() const{
    // Clones the object
    return new LimitedCollectorVolunteer(*this);
} 

bool LimitedCollectorVolunteer:: hasOrdersLeft() const{
    // Return if he has more orders to take
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer:: canTakeOrder(const Order& order) const{
    // Checks if he is busy processing an order or completed an order but didnt deliver to a driver
    return (hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order));
}

void LimitedCollectorVolunteer:: acceptOrder(const Order& order){
    // Accepts order: sets new active order id and decreases ordersLeft
    CollectorVolunteer::acceptOrder(order);
    ordersLeft -= 1;
}

int LimitedCollectorVolunteer:: getMaxOrders() const{
    // Returns maxOrders
    return maxOrders;
}

int LimitedCollectorVolunteer:: getNumOrdersLeft() const{
    // Returns NumOrdersLeft
    return ordersLeft;
}

string LimitedCollectorVolunteer:: toString() const{
    // prints limited collector volunteer
    return "LimitedCollector " + getName() + " " + std::to_string(getId()) + " " + std::to_string(getTimeLeft()) + " " + std::to_string(getCoolDown()) + " " + std::to_string(getActiveOrderId()) + " " + std::to_string(getCompletedOrderId()) + " " + std::to_string(maxOrders) + " " + std::to_string(ordersLeft);
}


// DriverVolunteer class
DriverVolunteer:: DriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep) : Volunteer(_id, _name), maxDistance(_maxDistance), distancePerStep(_distancePerStep), distanceLeft(0){}

DriverVolunteer:: ~DriverVolunteer(){}

DriverVolunteer* DriverVolunteer:: clone() const{
    // Clones the object
    return new DriverVolunteer(*this);
} 

int DriverVolunteer:: getDistanceLeft() const {
    // Returns distanceLeft
    return distanceLeft;
}

int DriverVolunteer:: getMaxDistance() const {
    // Returns maxDistance
    return maxDistance;
}

int DriverVolunteer:: getDistancePerStep() const {
    // Returns distancePerStep
    return distancePerStep;
}

bool DriverVolunteer:: decreaseDistanceLeft(){
    // Decreases distance by distancePerStep and returns if the order was delivered
    // if order was delivered it resets distance left and active order id, and sets completed order id 
    if(distanceLeft>0){
        distanceLeft -= distancePerStep;
    }
    return (distanceLeft<=0);
}

bool DriverVolunteer:: hasOrdersLeft() const{
    // Return if he has more orders to take
    return true;
}

bool DriverVolunteer:: canTakeOrder(const Order &order) const{
    // Checks if he is busy delivering an order
    return (order.getDistance() <= maxDistance && !isBusy());
}

void DriverVolunteer:: acceptOrder(const Order &order){
    // Accepts order: sets new active order id and distance left
    distanceLeft = order.getDistance();
    activeOrderId = order.getId();
}

void DriverVolunteer:: step(){
    // Takes a step in the game: decreases DistanceLeft left by distancePerStep,
    // if the volunteer ended the proccess he resets active order id and distanceLeft to NO_ORDER and sets completed order id to the order that finished
    if(decreaseDistanceLeft()){
        distanceLeft=0;
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer:: toString() const{
    // prints driver volunteer
    return "DriverVolunteer " + getName() + " " + std::to_string(getId()) + " " + std::to_string(maxDistance) + " " + std::to_string(distancePerStep) + " " + std::to_string(distanceLeft) + " " + std::to_string(getActiveOrderId()) + " " + std::to_string(getCompletedOrderId());
}

// LimitedDriver Volunteer class
LimitedDriverVolunteer:: LimitedDriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep, int _maxOrders) : DriverVolunteer(_id, _name , _maxDistance, _distancePerStep) , maxOrders(_maxOrders) , ordersLeft(_maxOrders){}

LimitedDriverVolunteer* LimitedDriverVolunteer:: clone() const{
    // Clones the object
    return new LimitedDriverVolunteer(*this);
} 

int LimitedDriverVolunteer:: getMaxOrders() const{
    // Returns maxOrders
    return maxOrders;
}

int LimitedDriverVolunteer:: getNumOrdersLeft() const{
    // Returns NumOrdersLeft
    return ordersLeft;
}

bool LimitedDriverVolunteer:: hasOrdersLeft() const{
    // Return if he has more orders to take
    return (ordersLeft!=0);
}

bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const{
    // Checks if he is busy delivering an order
    return (DriverVolunteer:: canTakeOrder(order) && hasOrdersLeft());
}

void LimitedDriverVolunteer:: acceptOrder(const Order &order){
    // Accepts order: sets new active order id and distanceLeft, decreases ordersLeft
    ordersLeft -= 1;
    DriverVolunteer::acceptOrder(order);
}

string LimitedDriverVolunteer:: toString() const{
    // prints limited driver volunteer
    return "DriverVolunteer " + getName() + " " + std::to_string(getId()) + " " + std::to_string(getMaxDistance()) + " " + std::to_string(getDistancePerStep()) + " " + std::to_string(getDistanceLeft()) + " " + std::to_string(getActiveOrderId()) + " " + std::to_string(getCompletedOrderId()) + " " + std::to_string(maxOrders) + " " + std::to_string(ordersLeft);
}