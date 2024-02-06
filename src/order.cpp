#include "Order.h"

std::string orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING:
            return "PENDING";
        case OrderStatus::COLLECTING:
            return "COLLECTING";
        case OrderStatus::DELIVERING:
            return "DELIVERING";
        case OrderStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "Unknown";
    }
}

Order:: Order(int _id, int _customerId, int _distance) : id(_id) , customerId(_customerId) , distance(_distance) , status(OrderStatus::PENDING) , collectorId(NO_VOLUNTEER) , driverId(NO_VOLUNTEER){}

int Order::getId() const{
    //Returns the order's id
    return id;
}

int Order::getCustomerId() const{
    //Returns the order's customer id
    return customerId;
}

int Order::getCollectorId() const{
    //Returns the order's collector id
    return collectorId;
}

int Order::getDriverId() const{
    //Returns the order's driver id
    return driverId;
}

OrderStatus Order::getStatus() const{
    //Returns the order's status
    return status;
}

int Order::getDistance() const{
    //Returns the order's disance
    return distance;
}

void Order::setCollectorId(int _collectorId){
    //Sets a collector to the order
    collectorId = _collectorId;
}

void Order::setDriverId(int _driverId){
    //Sets a driver to the order
    driverId = _driverId;
}

void Order::setStatus(OrderStatus _status){
    //Sets a new status to the order
    status = _status;
}

const string Order::toString() const{
    return "OrderID: " + std::to_string(id) + " , CustomerID: " + std::to_string(customerId) + " , OrderStatus: " + orderStatusToString(status);
}

Order* Order:: clone() const{
    // Clones the object
    return new Order(*this);
} 
