#include "Action.h"
#include "Volunteer.h"
#include "Order.h"
#include <iostream>
#ifndef main_cpp 
#define main_cpp

extern WareHouse* backup;

#endif 

std::string orderStatusToString(OrderStatus status);

std::string actionStatusToString(ActionStatus status) {
    switch (status) {
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        case ActionStatus::ERROR:
            return "ERROR";
        default:
            return "Unknown";
    }
}

std::string customerTypeToString(CustomerType status) {
    switch (status) {
        case CustomerType::Soldier:
            return "soldier";
        case CustomerType::Civilian:
            return "civilian";
        default:
            return "Unknown";
    }
}

CustomerType stringToCustomerType(const string& str) {
    CustomerType result = CustomerType::Civilian;  // Default value
    if (str == "soldier") {
        result = CustomerType::Soldier;
    } 
    return result;
}

// ---------------------- BaseAction class ----------------------

BaseAction:: BaseAction() : errorMsg("ERROR: ") ,status(ActionStatus::COMPLETED){}

ActionStatus BaseAction:: getStatus() const{
    // Returns the status of the Action
    return status;
}

void BaseAction:: complete(){
    // Set the Action to COMPLETED
    status = ActionStatus::COMPLETED;
}

void BaseAction:: error(string _errorMsg){
    // Set the Action to ERROR and change the error message
    status = ActionStatus::ERROR;
    errorMsg += _errorMsg;
}

string BaseAction:: getErrorMsg() const{
    return errorMsg;
}

BaseAction:: ~BaseAction(){}

// ---------------------- SimulateStep class ----------------------
SimulateStep:: SimulateStep(int _numOfSteps) : BaseAction(), numOfSteps(_numOfSteps){}

void SimulateStep:: act(WareHouse &wareHouse){
    // Simulates a step in the game
    for (int i = 0; i < numOfSteps; ++i) {
        wareHouse.step(); 
    }
}

string SimulateStep:: toString() const{
    return "step " + std::to_string(numOfSteps) + " " + actionStatusToString(getStatus());
}

SimulateStep* SimulateStep:: clone() const{
    // Clones the object
    return new SimulateStep(*this);
}

// ---------------------- AddOrder class ----------------------
AddOrder:: AddOrder(int id) : BaseAction(), customerId(id){}

void AddOrder:: act(WareHouse &wareHouse){
    //Add an order to the warehouse for a curtain customer
    Customer& customer = wareHouse.getCustomer(customerId);
    if(customer.getId() != -1 && customer.getNumOrders() < customer.getMaxOrders()){
        // if the customer is in warehouse and can place an order
        // then place a new order
        int orderId = wareHouse.getOrderCounter();
        Order* order = new Order(orderId, customerId, customer.getCustomerDistance());
        wareHouse.addOrder(order);
    }
    else{
        // free the memory of the default customer and state that the action accured an error
        if(customer.getId() == -1){
            delete &customer;
        }
        error("Cannot place this order");
        std::cout<< getErrorMsg() << std::endl;
    }
}

string AddOrder:: toString() const{
    return "order " + std::to_string(customerId) + " " + actionStatusToString(getStatus());
}

AddOrder* AddOrder:: clone() const{
    // Clones the object
    return new AddOrder(*this);
} 

// ---------------------- AddCustomer class ----------------------
AddCustomer:: AddCustomer(const string &_customerName, const string &_customerType, int _distance, int _maxOrders) : BaseAction(), customerName(_customerName), customerType(stringToCustomerType(_customerType)), distance(_distance), maxOrders(_maxOrders){}

void AddCustomer:: act(WareHouse &wareHouse){
    //Adds a new customer
    if(customerType == CustomerType::Soldier){
        SoldierCustomer* customer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(customer);
    }
    else{
        CivilianCustomer* customer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(customer);
    }
}

string AddCustomer:: toString() const{
    return "customer " + customerName + " " + customerTypeToString(customerType) + " " + std::to_string(distance) + " " + std::to_string(maxOrders) + " " + actionStatusToString(getStatus());
}

AddCustomer* AddCustomer:: clone() const{
    // Clones the object
    return new AddCustomer(*this);
} 

// ---------------------- PrintOrderStatus class ----------------------
PrintOrderStatus:: PrintOrderStatus(int id) : BaseAction(), orderId(id) {}

void PrintOrderStatus:: act(WareHouse &wareHouse){
    // Prints the Status of the objects order
    Order& order = wareHouse.getOrder(orderId);
    if(order.getId()!=-1){
        // if the order was placed in the warehouse print its id,status,customer id, and collector id and driver id if was assigned to
        std::cout<< "OrderId: " + std::to_string(orderId) << std::endl;
        std::cout<< "OrderStatus: " + orderStatusToString(order.getStatus()) << std::endl;
        std::cout<< "CustomerId: " + std::to_string(order.getCustomerId()) << std::endl;
        if(order.getCollectorId()==-1){
            std::cout<< "Collector: None" << std::endl;
        }
        else{
            std::cout<< "Collector: " + std::to_string(order.getCollectorId()) << std::endl;
        }
        if(order.getDriverId()==-1){
            std::cout<< "Driver: None" << std::endl;
        }
        else{
            std::cout<< "Driver: " + std::to_string(order.getDriverId()) << std::endl;
        }
    }
    else{
        // No such order in warehouse so sets the action to be an error
        delete &order;
        error("Order doesn't exist");
        std::cout<< getErrorMsg() << std::endl;
    }
}

string PrintOrderStatus:: toString() const{
    return "orderStatus " + std::to_string(orderId) + " " + actionStatusToString(getStatus());
}

PrintOrderStatus* PrintOrderStatus:: clone() const{
    // Clones the object
    return new PrintOrderStatus(*this);
} 

// ---------------------- PrintCustomerStatus class ----------------------
PrintCustomerStatus:: PrintCustomerStatus(int id) : BaseAction(), customerId(id) {}

void PrintCustomerStatus:: act(WareHouse &wareHouse){
    // Prints the id,orders and orderLeft of the objects customer
    Customer& customer = wareHouse.getCustomer(customerId);
    if(customer.getId()!=-1){
        std::cout<< "CustomerID: " + std::to_string(customerId) << std::endl;
        vector<int> orderIds = customer.getOrdersIds();
        for(int id : orderIds){
            std::cout<< "OrderID: " + std::to_string(id) << std::endl;
            std::cout<< "OrderStatus: " + orderStatusToString(wareHouse.getOrder(id).getStatus()) << std::endl;
        }
        std::cout<< "numOrdersLeft: " + std::to_string(customer.getMaxOrders() - customer.getNumOrders()) << std::endl;
    }
    else{
        // No such customer in warehouse so sets the action to be an error
        delete &customer;
        error("Customer doesn't exist");
        std::cout<< getErrorMsg() << std::endl;
    }
}

string PrintCustomerStatus:: toString() const{
    return "customerStatus " + std::to_string(customerId) + " " + actionStatusToString(getStatus());
}

PrintCustomerStatus* PrintCustomerStatus:: clone() const{
    // Clones the object
    return new PrintCustomerStatus(*this);
} 

// ---------------------- PrintVolunteerStatus class ----------------------
PrintVolunteerStatus:: PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id) {}

void PrintVolunteerStatus:: act(WareHouse &wareHouse){
    // Prints the id,isBusy,orderId, TimeLeft/DistanceLeft and ordersLeft of the objects volunteer
    Volunteer& volunteer = wareHouse.getVolunteer(volunteerId);
    if(volunteer.getId()!=-1){
        std::cout<< "VolunteerID: " + std::to_string(volunteerId) << std::endl;
        Volunteer* cloned = volunteer.clone();
        if(volunteer.isBusy()){
            std::cout<< "isBusy: True" << std::endl;
            std::cout<< "OrderID: " + std::to_string(volunteer.getActiveOrderId()) << std::endl;
            //prints timeleft
            if(CollectorVolunteer* collector = dynamic_cast<CollectorVolunteer*>(cloned)){
                std::cout<< "TimeLeft: " + std::to_string((*collector).getTimeLeft()) << std::endl;
            }
            else if (DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(cloned)){
                std::cout<< "DistanceLeft: " + std::to_string((*driver).getDistanceLeft()) << std::endl;
            }
        }
        else{
            std::cout<< "isBusy: False" << std::endl;
            std::cout<< "OrderID: None" << std::endl;
            std::cout<< "TimeLeft: None" << std::endl;
        }
        //prints ordersleft
        if(LimitedCollectorVolunteer* limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(cloned)){
            std::cout<< "OrdersLeft: " + std::to_string((*limitedCollector).getNumOrdersLeft()) << std::endl;
        }
        else if (LimitedDriverVolunteer* limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(cloned)){
            std::cout<< "OrdersLeft: " + std::to_string((*limitedDriver).getNumOrdersLeft()) << std::endl;            }
        else{
            std::cout<< "OrdersLeft: No Limit"<< std::endl;
        }
        delete cloned;
    }
    else{
        // No such volunteer in warehouse so sets the action to be an error
        delete &volunteer;
        error("Volunteer Doesn't exist");
        std::cout<< getErrorMsg() << std::endl;
    }
}

string PrintVolunteerStatus:: toString() const{
    return "volunteerStatus " + std::to_string(volunteerId) + " " + actionStatusToString(getStatus());
}

PrintVolunteerStatus* PrintVolunteerStatus:: clone() const{
    // Clones the object
    return new PrintVolunteerStatus(*this);
}

// ---------------------- PrintActionsLog class ----------------------
PrintActionsLog:: PrintActionsLog() : BaseAction(){}

void PrintActionsLog:: act(WareHouse &wareHouse){
    // Prints the actions log of warehouse
    vector<BaseAction*> actions = wareHouse.getActions();
    for(BaseAction* action : actions){
       std::cout << (*action).toString() << std::endl;
    }
}

string PrintActionsLog:: toString() const{
    return "log " + actionStatusToString(getStatus());
}

PrintActionsLog* PrintActionsLog:: clone() const{
    // Clones the object
    return new PrintActionsLog(*this);
}

// ---------------------- Close class ----------------------
Close:: Close() : BaseAction(){}

void Close:: act(WareHouse &wareHouse){
    // Closes warehouse
    wareHouse.close();
}

string Close:: toString() const{
    return "log " + actionStatusToString(getStatus());
}

Close* Close:: clone() const{
    // Clones the object
    return new Close(*this);
}

// ---------------------- BackupWareHouse class ----------------------
BackupWareHouse:: BackupWareHouse() : BaseAction(){}

void BackupWareHouse:: act(WareHouse &wareHouse){
    // Save a backup of the warehouse
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    backup = new WareHouse(wareHouse);
}

string BackupWareHouse:: toString() const{
    return "backup " + actionStatusToString(getStatus());
}

BackupWareHouse* BackupWareHouse:: clone() const{
    // Clones the object
    return new BackupWareHouse(*this);
}

// ---------------------- RestoreWareHouse class ----------------------
RestoreWareHouse:: RestoreWareHouse() : BaseAction(){}

void RestoreWareHouse:: act(WareHouse &wareHouse){
    //Restore the warehouse to its last saved backup
    if(backup!=nullptr){
        wareHouse = (*backup);
    }
    else{
        error("No backup available");
        std::cout<< getErrorMsg() << std::endl;
    }
}

string RestoreWareHouse:: toString() const{
    return "restore " + actionStatusToString(getStatus());
}

RestoreWareHouse* RestoreWareHouse:: clone() const{
    // Clones the object
    return new RestoreWareHouse(*this);
}