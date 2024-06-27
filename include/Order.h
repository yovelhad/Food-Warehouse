#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;


class Volunteer;

enum class OrderStatus {
    PENDING, //0
    COLLECTING, //1
    DELIVERING, //2
    COMPLETED, //3
};

#define NO_VOLUNTEER -1

class Order {

    public:
        Order(int id, int customerId, int distance);
        int getId() const;
        int getCustomerId() const;
        void setStatus(OrderStatus status);
        void setCollectorId(int collectorId);
        void setDriverId(int driverId);
        int getCollectorId() const;
        int getDriverId() const;
        OrderStatus getStatus() const;
        const string toString() const;
        //added methods
        bool isPending() const;
        bool isCollecting() const;
        bool isDelivering() const;
        bool isCompleted() const;
        int getDistance() const;
        string statusToString(OrderStatus status) const;
        void finishCollecting();
        bool getIsWaitingForDriver() const;
        void setIsWaitingForDriver(bool isWaitingForDriver);

    private:
        const int id;
        const int customerId;
        const int distance;
        OrderStatus status;
        int collectorId; //Initialized to NO_VOLUNTEER if no collector has been assigned yet
        int driverId; //Initialized to NO_VOLUNTEER if no driver has been assigned yet
        //added fields
        bool isWaitingForDriver;
        
};