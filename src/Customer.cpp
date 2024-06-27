#include "Customer.h"


//Customer_cpp

        Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance),
                            maxOrders(maxOrders), ordersId(),ordersLeft(maxOrders){
            std::vector<int> ordersId;
        };

        //copy constructor
        Customer:: Customer(const Customer &other) : id(other.id), name(other.name), locationDistance(other.locationDistance),
         maxOrders(other.maxOrders), ordersId(),ordersLeft(maxOrders){
            for(int orderId : other.ordersId){
                ordersId.push_back(orderId);
            }
         };

        //virtual destructor
        Customer::~Customer(){};
        
        const string& Customer::getName() const{
            return name;
        };
        
        int Customer::getId() const{
            return id;
        };

        int Customer::getCustomerDistance() const{
            return locationDistance;
        };

        int Customer::getMaxOrders() const{
            return maxOrders;
        }; //Returns maxOrders

        int Customer::getNumOrders() const{
            return ordersId.size();
        }; //Returns num of orders the customer has made so far

        bool Customer::canMakeOrder() const{
            return ordersId.size() < static_cast<std::vector<int>::size_type>(maxOrders);//using because maxOrders is int and size() returns size_t
        }; //Returns true if the customer didn't reach max orders

        vector<int>& Customer::getOrdersIds() const{
            return const_cast<vector<int>&>(ordersId);
        };

        int Customer::addOrder(int orderId){
            if(!canMakeOrder()){
                return -1;
            }
            ordersId.push_back(orderId);
            return orderId;
        }; //return OrderId if order was added successfully, -1 otherwise

//SoldierCustomer_cpp

    //constructor
        SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){};

    //copy constructor
        SoldierCustomer::SoldierCustomer(const SoldierCustomer &other) : Customer(other) {};

        SoldierCustomer* SoldierCustomer::clone() const{
            return new SoldierCustomer(*this);
        };

        int SoldierCustomer:: getOrdersLeft() const{
            return getMaxOrders() - getNumOrders();
        };


//CivilianCustomer_cpp

    //constructor
        CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){};
    
    //copy constructor
        CivilianCustomer::CivilianCustomer(const CivilianCustomer &other) : Customer(other) {};

        CivilianCustomer*CivilianCustomer::clone() const{
            return new CivilianCustomer(*this);
        };

        int CivilianCustomer:: getOrdersLeft() const{
            return getMaxOrders() - getNumOrders();
        };