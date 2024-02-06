#include "WareHouse.h"
#include "Volunteer.h"
#include "Customer.h"
#include "Action.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

WareHouse:: WareHouse(const string& configFilePath) : isOpen(false), actionsLog({}), volunteers({}), pendingOrders({}), inProcessOrders({}), completedOrders({}), customers({}), customerCounter(0), volunteerCounter(0), orderCounter(0){
    // Open the file for reading
    std::ifstream inputFile(configFilePath);
    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return;
    }
    // Read and print the contents of the file line by line
    std::string line;
    std::vector<std::string> words;
    while (std::getline(inputFile, line)) {
        if(!line.empty() && line[0]!='#'){
            // Create a string stream from the line
            std::istringstream iss(line);

            // Iterate over the first 6 words in the line 
            std::string word;
            int i = 0;
            while (iss >> word && i<=5) {
                words.push_back(word);
                i+=1;
            }
            // Filtering the line to its object and creating one
            if(words[0]=="customer"){
                if(words[2] == "soldier"){
                    customers.push_back(new SoldierCustomer(customerCounter,words[1],std::stoi(words[3]),std::stoi(words[4])));
                }
                else{
                    customers.push_back(new CivilianCustomer(customerCounter,words[1],std::stoi(words[3]),std::stoi(words[4])));
                }           
                customerCounter+=1;
            }
            else{
                if(words[2] == "limited_collector"){
                    volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter,words[1],std::stoi(words[3]),std::stoi(words[4])));
                }
                else if(words[2] == "collector"){
                    volunteers.push_back(new CollectorVolunteer(volunteerCounter,words[1],std::stoi(words[3])));
                }
                else if(words[2] == "limited_driver"){
                    volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter,words[1],std::stoi(words[3]),std::stoi(words[4]),std::stoi(words[5])));
                }
                else if(words[2] == "driver"){
                    volunteers.push_back(new DriverVolunteer(volunteerCounter,words[1],std::stoi(words[3]),std::stoi(words[4])));
                }        
                volunteerCounter+=1;
            } 
            // emptying the words vector
            words.clear();
        }
    }
    // Close the file
    inputFile.close();
}

void WareHouse:: start(){
    // Opens the warehouse and start getting actions
    std:: cout << "WareHouse is open!" << std:: endl;
    open();
    while(isOpen){
        std::string userInput;
        std::getline(std::cin, userInput);

        //extracting words
        std::istringstream iss(userInput);
        std::vector<std::string> words = {};

        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }
        // filtering the action
        if(words[0] == "order"){
            AddOrder* addorder = new AddOrder(std:: stoi(words[1]));
            (*addorder).act(*this);
            actionsLog.push_back((BaseAction*)addorder);
        }
        else if(words[0] == "step"){
            SimulateStep* sim = new SimulateStep(std:: stoi(words[1]));
            (*sim).act(*this);
            actionsLog.push_back((BaseAction*)sim);
        }
        else if(words[0] == "orderStatus"){
            PrintOrderStatus* pos = new PrintOrderStatus(std:: stoi(words[1]));
            (*pos).act(*this);
            actionsLog.push_back((BaseAction*)pos);
        }
        else if(words[0] == "customer"){
            AddCustomer* addcustomer = new AddCustomer(words[1], words[2], std:: stoi(words[3]), std:: stoi(words[4]));
            (*addcustomer).act(*this);
            actionsLog.push_back((BaseAction*)addcustomer);
        }
        else if(words[0] == "customerStatus"){
            PrintCustomerStatus* pcs = new PrintCustomerStatus(std:: stoi(words[1]));
            (*pcs).act(*this);
            actionsLog.push_back((BaseAction*)pcs);
        }
        else if(words[0] == "volunteerStatus"){
            PrintVolunteerStatus* pvs = new PrintVolunteerStatus(std:: stoi(words[1]));
            (*pvs).act(*this);
            actionsLog.push_back((BaseAction*)pvs);
        }
        else if(words[0] == "log"){
            PrintActionsLog* pal = new PrintActionsLog();
            (*pal).act(*this);
            actionsLog.push_back((BaseAction*)pal);
        }
        else if(words[0] == "backup"){
            BackupWareHouse* back = new BackupWareHouse();
            (*back).act(*this);
            actionsLog.push_back((BaseAction*)back);
        }
        else if(words[0] == "restore"){
            RestoreWareHouse* res = new RestoreWareHouse();
            (*res).act(*this);
            actionsLog.push_back((BaseAction*)res);
        }
        else if(words[0] == "close"){
            Close close = Close();
            close.act(*this);
        }
    }
}

void WareHouse:: addOrder(Order* order){
    //Adds a new order to the warehouse
    pendingOrders.push_back(order);
    orderCounter+=1;
    int customerId = (*order).getCustomerId();
    int orderId = (*order).getId();
    getCustomer(customerId).addOrder(orderId);
}

void WareHouse:: addAction(BaseAction* action){
    // Adds a curtain action to the vector of actions
    actionsLog.push_back(action);
}

Customer& WareHouse:: getCustomer(int customerId) const{
    // Returns the customer which his id matches 
    // if there is no such customer returns the default customer
    for (Customer* customer : customers) {
        if ((*customer).getId() == customerId)
        {
            return (*customer);
        }
    }
    return (*(new SoldierCustomer(-1,"none", -1,-1)));
}

Volunteer& WareHouse:: getVolunteer(int volunteerId) const{
    // Returns the volunteer which his id matches 
    // if there is no such volunteer returns the default volunteer
    for (Volunteer* volunteer : volunteers) {
        if ((*volunteer).getId() == volunteerId)
        {
            return (*volunteer);
        }
    }
    return (*(new CollectorVolunteer(-1,"none", -1)));
}

Order& WareHouse:: getOrder(int orderId) const{
    // Returns the order which his id matches 
    // if there is no such order returns the default order
    for (Order* order : pendingOrders) {
        if ((*order).getId() == orderId)
        {
            return (*order);
        }
    }
    for (Order* order : inProcessOrders) {
        if ((*order).getId() == orderId)
        {
            return (*order);
        }
    }
    for (Order* order : completedOrders) {
        if ((*order).getId() == orderId)
        {
            return (*order);
        }
    }
    return (*(new Order(-1, -1, -1)));
}

const vector<BaseAction*>& WareHouse:: getActions() const{
    // Returns the actions log vector
    return actionsLog;
}

void WareHouse:: close(){
    // Closes the warehouse
    // printing all its orders status
    for(Order* order : pendingOrders){
       std::cout << (*order).toString() << std::endl;
    }
    for(Order* order : inProcessOrders){
       std::cout << (*order).toString() << std::endl;
    }
    for(Order* order : completedOrders){
       std::cout << (*order).toString() << std::endl;
    }
    isOpen = false;
}

void WareHouse:: open(){
    // Opens warehouse
    isOpen = true;
}

int WareHouse:: getOrderCounter() const{
    // Returns the orderCounter
    return orderCounter;
}

int WareHouse:: getCustomerCounter() const{
    // Returns the customerCounter
    return customerCounter;
}

int WareHouse:: getVolunteerCounter() const{
    // Returns the volunteerCounter
    return volunteerCounter;
}

void WareHouse:: addCustomer(Customer* customer){
    // Adds new customer to warehouse
    customers.push_back(customer);
    customerCounter+=1;
}

WareHouse* WareHouse:: clone() const{
    // Clones the object
    return new WareHouse(*this);
}

void WareHouse:: step(){
    //STEP 1: Go over all pending orders and assign a volunteer to them if possible
    bool found = false;
    vector<Volunteer*>::iterator iterCollectorVolunteer = volunteers.begin();
    vector<Volunteer*>::iterator iterDriverVolunteer = volunteers.begin();
    vector<Order*>::iterator iterPending = pendingOrders.begin();
    while (iterPending != pendingOrders.end()) {
        Order* order = *iterPending;
        if((*order).getCollectorId() == -1){
            //The order needs to be processed by a collector
            iterCollectorVolunteer = volunteers.begin();
            while (iterCollectorVolunteer != volunteers.end() && !found) {
                // Search for an available collector
                if (dynamic_cast<CollectorVolunteer*>(*iterCollectorVolunteer) && (**iterCollectorVolunteer).canTakeOrder(*order)) {
                    (**iterCollectorVolunteer).acceptOrder(*order);
                    (*order).setStatus(OrderStatus::COLLECTING);
                    (*order).setCollectorId((**iterCollectorVolunteer).getId());
                    inProcessOrders.push_back(*iterPending);
                    iterPending = pendingOrders.erase(iterPending);
                    found = true;
                }
                else{
                    ++iterCollectorVolunteer;
                }
            }
        }
        else{
            //The order needs to be processed by a driver
            iterDriverVolunteer = volunteers.begin();
            while (iterDriverVolunteer != volunteers.end() && !found) {
                // Search for an available driver
                if (dynamic_cast<DriverVolunteer*>(*iterDriverVolunteer) && (**iterDriverVolunteer).canTakeOrder(*order)) {
                    (**iterDriverVolunteer).acceptOrder(*order);
                    (*order).setStatus(OrderStatus::DELIVERING);
                    (*order).setDriverId((**iterDriverVolunteer).getId());
                    getVolunteer((*order).getDriverId()).freeVolunteer();
                    inProcessOrders.push_back(*iterPending);
                    iterPending = pendingOrders.erase(iterPending);
                    found = true;
                }
                else{
                    ++iterDriverVolunteer;
                }
            }
        }
        if(!found){
            ++iterPending;
        }
        else{
            found=false;
        }
    }

    // STEP 2: Go over inProcess and take a step
    vector<Order*>::iterator iterProcess = inProcessOrders.begin();
    while (iterProcess != inProcessOrders.end()) {
        Order* order = *iterProcess;
        if((*order).getDriverId() != -1){
            getVolunteer((*order).getDriverId()).step();
        }
        else{
            getVolunteer((*order).getCollectorId()).step();
        }
        ++iterProcess;
    }
    // Step 3: Go over volunteers and check if it completed an order
    vector<Volunteer*>::iterator iterVolunteer = volunteers.begin();
    while (iterVolunteer != volunteers.end()) {
        Volunteer* volunteer = *iterVolunteer;
        if((*volunteer).isBusy() && (*volunteer).getCompletedOrderId() != -1){
            Order order = getOrder((*volunteer).getCompletedOrderId());
            vector<Order*>::iterator iterProcess = inProcessOrders.begin();
            if (dynamic_cast<DriverVolunteer*>(*iterVolunteer)) {
                // Complete the order of the driver
                while (iterProcess != inProcessOrders.end() && !found) {
                    if((**iterProcess).getId() == order.getId()){
                        found = true;
                        (**iterProcess).setStatus(OrderStatus::COMPLETED);
                        completedOrders.push_back(*iterProcess);
                        iterProcess = inProcessOrders.erase(iterProcess);
                        (**iterVolunteer).freeVolunteer();
                    }
                    else{
                        ++iterProcess;
                    }
                }
                found = false;
            }
            else if(dynamic_cast<CollectorVolunteer*>(*iterVolunteer)){
                // Complete the order of the collector
                while (iterProcess != inProcessOrders.end() && !found) {
                    //found in process
                    if((**iterProcess).getId() == order.getId()){
                        found = true;
                    }
                    else{
                        ++iterProcess;
                    }
                }
                if(found){
                    pendingOrders.push_back(*iterProcess);
                    iterProcess = inProcessOrders.erase(iterProcess);
                    (**iterVolunteer).freeVolunteer();
                    found = false;
                }
            }
        }
        // Remove the volunteer if it completed its order and has no order left
        if(!(**iterVolunteer).hasOrdersLeft() && !(**iterVolunteer).isBusy()){
            delete *iterVolunteer;
            iterVolunteer = volunteers.erase(iterVolunteer);
        }
        else{
            ++iterVolunteer;
        }
    }
}

// The rule of 5
WareHouse:: ~WareHouse(){
    for(BaseAction* action : actionsLog){
        delete action;
    }
    for(Volunteer* volunteer : volunteers){
        delete volunteer;
    }
    for(Order* order : pendingOrders){
        delete order;
    }
    for(Order* order : inProcessOrders){
        delete order;
    }
    for(Order* order : completedOrders){
        delete order;
    }
    for(Customer* customer : customers){
        delete customer;
    }
}

WareHouse:: WareHouse(const WareHouse &other) : isOpen(false), actionsLog({}), volunteers({}), pendingOrders({}), inProcessOrders({}), completedOrders({}), customers({}), customerCounter(0), volunteerCounter(0), orderCounter(0){
    *this = other;
}


WareHouse& WareHouse::operator=(const WareHouse& other) {
    if (this != &other) {
        // Release existing resources
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        for(Order* order : pendingOrders){
            delete order;
        }
        for(Order* order : inProcessOrders){
            delete order;
        }
        for(Order* order : completedOrders){
            delete order;
        }
        for (Customer* customer : customers) {
            delete customer;
        }

        // Copy resources from 'other' to 'this'
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        // Copy vectors
        actionsLog.clear();
        volunteers.clear();
        customers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();

        for (const BaseAction* action : other.actionsLog) {
            actionsLog.push_back((*action).clone());
        }
        for (const Volunteer* volunteer : other.volunteers) {
            volunteers.push_back((*volunteer).clone());
        }
        for (const Order* order : other.pendingOrders) {
            pendingOrders.push_back((*order).clone());
        }
        for (const Order* order : other.inProcessOrders) {
            inProcessOrders.push_back((*order).clone());
        }
        for (const Order* order : other.completedOrders) {
            completedOrders.push_back((*order).clone());
        }
        for (const Customer* customer : other.customers) {
            customers.push_back((*customer).clone());
        }
    }
    return *this;
}

WareHouse::WareHouse(WareHouse&& other) 
    : isOpen(other.isOpen),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter){
    // Set the source object to a valid but unspecified state
    other.isOpen = false;
    other.customerCounter = 0;
    other.volunteerCounter = 0;
    other.orderCounter = 0;
}

WareHouse& WareHouse::operator=(const WareHouse&& other) {
    if (this != &other) {
        // Release existing resources
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        for(Order* order : pendingOrders){
            delete order;
        }
        for(Order* order : inProcessOrders){
            delete order;
        }
        for(Order* order : completedOrders){
            delete order;
        }
        for (Customer* customer : customers) {
            delete customer;
        }

        // Transfer ownership from 'other' to 'this'
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        // Move resources from 'other' to 'this'
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
    }
    return *this;
}