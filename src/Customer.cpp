#include "Customer.h"
#include <iostream>
#include <string>
using std::string;


Customer::Customer(int id1, const string &name1, int locationDistance1, int maxOrders1):id(id1),name(name1),locationDistance(locationDistance1),maxOrders(maxOrders1),ordersId({}){}


const string &Customer::getName() const {
    // Returns the customer's name
    return name;
}

int Customer::getId() const{
    // Returns the customer's id
    return id;
}  

int Customer::getCustomerDistance() const{
    // Returns the customer's distance
    return locationDistance;
}     

int Customer::getMaxOrders() const{
    // Returns the customer's maxOrders
    return maxOrders;
}

int Customer::getNumOrders() const{
    // Returns the number of orders the customer placed
    return ordersId.size();
}   

bool Customer::canMakeOrder() const {
    // Checks if the customer can make a new order
    if((int)ordersId.size()<maxOrders)
       return true;
    return false;
}

const vector<int> &Customer::getOrdersIds() const {
    // Returns the vector of orders that the customer placed
    return ordersId;
}

int Customer::addOrder(int orderId) {
    // Place a new order if possible
    if(Customer::canMakeOrder())
    {
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
      
}

const string Customer::toString() const{ 
  return "Customer_id: " + std::to_string(getId()) + ", Customer_name:" + getName() + ", Location Distance:" + std::to_string(getCustomerDistance())+ ", Max Orders:" + std::to_string(getMaxOrders());
}

Customer::~Customer(){}


SoldierCustomer:: SoldierCustomer(int id1,const string &name1, int locationDistance1, int maxOrders1):Customer(id1,name1,locationDistance1,maxOrders1){}
SoldierCustomer *SoldierCustomer::clone() const {
    // Clones the object
    return new SoldierCustomer(*this);
}

CivilianCustomer:: CivilianCustomer(int id1,const string &name1, int locationDistance1, int maxOrders1):Customer(id1,name1,locationDistance1,maxOrders1){}
CivilianCustomer *CivilianCustomer::clone() const {
    // Clones the object
    return new CivilianCustomer(*this);
}
