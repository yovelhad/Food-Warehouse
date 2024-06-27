#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs


    public:

        WareHouse(const string &configFilePath);

        //rule of five
        WareHouse(const WareHouse& other); // Copy constructor
        WareHouse& operator=(const WareHouse& other); // Copy assignment operator
        WareHouse(WareHouse&& other) noexcept; // Move constructor
        WareHouse& operator=(WareHouse&& other) noexcept; // Move assignment operator
        ~WareHouse(); // Destructor

        void start();
        const vector<BaseAction*> &getActions() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();
        //added methods
        vector<Order*>& getPendingOrders();
        vector<Order*>& getInProcessOrders();
        vector<Order*>& getCompletedOrders();
        vector<Volunteer*> getVolunteersVector() const;
        void moveOrder(std::vector<Order*>& sourceVector, std::vector<Order*>& destinationVector, Order* OrderToTransfer);    
        int getOrderCounter() const;
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        void AddCustomerToVector(Customer* customer);
        void AddVolunteerToVector(Volunteer* volunteer);
        void removeVolunteer(int volunteerId);
};
