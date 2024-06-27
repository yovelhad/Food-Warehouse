#include "Order.h"

//Order_cpp

    //constructor
    Order::Order(int id, int customerId, int distance) :
                id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING),
                collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER), isWaitingForDriver(false) {};

    //methods
    int Order::getId() const{
        return id;
    };

    int Order::getCustomerId() const{
        return customerId;
    };

    void Order::setStatus(OrderStatus status){
        this->status = status;
    };

    void Order::setCollectorId(int collectorId){
        this->collectorId = collectorId;
    };

    void Order::setDriverId(int driverId){
        this -> driverId = driverId;
    };

    int Order::getCollectorId() const{
        return collectorId;
    };

    int Order::getDriverId() const{
        return driverId;
    };

    OrderStatus Order::getStatus() const{
        return status;
    };

    const string Order::toString() const{
        string answer = "orderID: " + std::to_string(id) + ", customerID: " + std::to_string(customerId) + ", distance: " + std::to_string(distance) +
                        ", status: " +  statusToString(status) + ", collectorID: " + std::to_string(collectorId) + ", driverID: " + std::to_string(driverId);
        return answer;
    };

    //added methods
    bool Order::isPending() const{
        return status == OrderStatus::PENDING;
    };

    bool Order::isCollecting() const{
        return status == OrderStatus::COLLECTING;
    };

    bool Order::isDelivering() const{
        return status == OrderStatus::DELIVERING;
    };

    bool Order::isCompleted() const{
        return status == OrderStatus::COMPLETED;
    };

    string Order::statusToString(OrderStatus status) const{
        switch(status){
            case OrderStatus::PENDING: return "PENDING";
            case OrderStatus::DELIVERING: return "DELIVERING";
            case OrderStatus::COMPLETED: return "COMPLETED";
            case OrderStatus::COLLECTING: return "COLLECTING";
        }
        return "not a valid status";
    };
    
    int Order::getDistance() const{
        return this->distance;
    };

    void Order::finishCollecting(){
        isWaitingForDriver=true;
    };

    bool Order::getIsWaitingForDriver() const{
        return isWaitingForDriver;
    }
    void Order::setIsWaitingForDriver(bool isWaitingForDriver){
        this->isWaitingForDriver = isWaitingForDriver;
    }

