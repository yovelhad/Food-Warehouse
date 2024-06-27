#include "Volunteer.h"

//Volunteer_cpp

    //methods
        // construcor
        Volunteer::Volunteer(int id, const string &name) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER) ,id(id), name(name), type(VolunteerType::VOLUNTEER){};

        // copyConstructor
        Volunteer::Volunteer(const Volunteer &other) : completedOrderId(other.completedOrderId), activeOrderId(other.activeOrderId),
                                                        id(other.id), name(other.name), type(other.type){};

        //virtual destructor
        Volunteer::~Volunteer(){};
        
        int Volunteer::getId() const{
            return id;
        };

        const string& Volunteer::getName() const{
            return name;
        };

        int Volunteer::getActiveOrderId() const{
            return activeOrderId;
        };

        int Volunteer::getCompletedOrderId() const{
             return completedOrderId;
        };

        bool Volunteer::isBusy() const{
            return activeOrderId >= 0;
        }; // Signal whether the volunteer is currently processing an order   

        //added methods

        string Volunteer::volunteerToString(VolunteerType type) const{
            switch (type){
                case VolunteerType::VOLUNTEER:
                    return "volunteer";
                case VolunteerType::COLLECTOR:
                    return "collector";
                case VolunteerType::LIMITEDCOLLECTOR:
                    return "limited_collector";
                case VolunteerType::DRIVER:
                    return "driver";
                case VolunteerType::LIMITEDDRIVER:
                    return "limited_driver";
                default:
                    return "volunteer";
            }
        };

//CollectorVolunteer_cpp

    // construcor
        CollectorVolunteer::CollectorVolunteer(int id,const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0), type(VolunteerType::COLLECTOR) {};

    // copyConstructor
        CollectorVolunteer::CollectorVolunteer(const CollectorVolunteer &other) : Volunteer(other), coolDown(other.coolDown), timeLeft(other.timeLeft),type(other.type) {};

        CollectorVolunteer* CollectorVolunteer::clone() const{
            return new CollectorVolunteer(*this);
        };

        void CollectorVolunteer::step(){
            bool isFinished = decreaseCoolDown();
            if(isFinished){
                completedOrderId = activeOrderId;
                activeOrderId = NO_ORDER;
            }

        };

        int CollectorVolunteer::getCoolDown() const{
            return coolDown;
        };

        int CollectorVolunteer::getTimeLeft() const{
            return timeLeft;
        };

    //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
        bool CollectorVolunteer::decreaseCoolDown(){
            timeLeft = timeLeft -1 ;
            return timeLeft == 0;
        };

        bool CollectorVolunteer::hasOrdersLeft() const{
            return activeOrderId > 0;
        };

        bool CollectorVolunteer::canTakeOrder(const Order &order) const{
            return (order.isPending() && timeLeft<1 && activeOrderId < 0);
        };

        void CollectorVolunteer::acceptOrder(const Order &order){
            if(canTakeOrder(order)){
                timeLeft = coolDown ;
                activeOrderId = order.getId();
            
            }
        };

        string CollectorVolunteer::toString() const{
            string CollectorVolunteerString = "volunteer " + getName() + " collector " + std::to_string(getCoolDown());
            return CollectorVolunteerString;
        };
        VolunteerType CollectorVolunteer::getType() const{
            return type;
        };

//LimitedCollectorVolunteer_cpp

        //constructor
        LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders):
                                         CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders), type(VolunteerType::LIMITEDCOLLECTOR){};

        //copy constructor
        LimitedCollectorVolunteer::LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other):
                                        CollectorVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft),type(other.type){};

        LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const{
            return new LimitedCollectorVolunteer(*this);
        };

        bool LimitedCollectorVolunteer::hasOrdersLeft() const{
            return ordersLeft>0;
        };
        
        bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
            return hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order);
        };

        void LimitedCollectorVolunteer::acceptOrder(const Order &order){
            if(canTakeOrder(order)){
                ordersLeft = ordersLeft-1;
                CollectorVolunteer::acceptOrder(order);

            }
        };

        int LimitedCollectorVolunteer::getMaxOrders() const{
            return maxOrders;
        };

        int LimitedCollectorVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        };

        string LimitedCollectorVolunteer::toString() const {
            string LimitedCollectorVolunteerString = "volunteer " + getName() + " limited_collector " + std::to_string(getCoolDown()) + std::to_string(getMaxOrders());
            return LimitedCollectorVolunteerString;
        };
        VolunteerType LimitedCollectorVolunteer::getType() const{
            return type;
        };
    
//DriverVolunteer_cpp


        //constructor
        DriverVolunteer:: DriverVolunteer(int id, string name, int maxDistance, int distancePerStep):
                                 Volunteer(id,name) ,maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0), type(VolunteerType::DRIVER){};
        //copy constructor
        DriverVolunteer:: DriverVolunteer(const DriverVolunteer &other):
                                 Volunteer(other), maxDistance(other.maxDistance), distancePerStep(other.distancePerStep), distanceLeft(other.distanceLeft),type(other.type){};

        DriverVolunteer* DriverVolunteer:: clone() const{
            return new DriverVolunteer(*this);
        };

        int DriverVolunteer:: getDistanceLeft() const{
            return distanceLeft;
        };

        int DriverVolunteer::getMaxDistance() const{
            return maxDistance;
        };

        int DriverVolunteer::getDistancePerStep() const{
            return distancePerStep;
        };

        bool DriverVolunteer:: decreaseDistanceLeft(){
            distanceLeft = distanceLeft - distancePerStep;
            return distanceLeft<=0;
        }; //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise

        bool DriverVolunteer:: hasOrdersLeft() const {
            return activeOrderId > 0;
        };

        bool DriverVolunteer::canTakeOrder(const Order &order) const{
            return (order.isCollecting() && order.getIsWaitingForDriver() && activeOrderId < 0 && order.getDistance()<= maxDistance);
        }; // Signal if the volunteer is not busy and the order is within the maxDistance

        void DriverVolunteer:: acceptOrder(const Order &order){
            if(canTakeOrder(order)){
                distanceLeft = order.getDistance();//why is it supposd to be max distnace?
                activeOrderId = order.getId();
            }
        }; // Reset distanceLeft to maxDistance

        void DriverVolunteer::step(){
            bool isFinished = decreaseDistanceLeft();
            if(isFinished){
                completedOrderId = activeOrderId;
                activeOrderId = NO_ORDER;
            }
        }; // Decrease distanceLeft by distancePerStep

        string DriverVolunteer:: toString() const{
             string DriverVolunteerString = "volunteer " + getName() + " driver " + std::to_string(getMaxDistance()) + std::to_string(getDistancePerStep());
            return DriverVolunteerString;
        };
        VolunteerType DriverVolunteer::getType() const{
            return type;
        };

//LimitedDriverVolunteer_cpp

        //constructor
        LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) :
                                DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders), type(VolunteerType::LIMITEDDRIVER){};
        
        //copy constructor
        LimitedDriverVolunteer::LimitedDriverVolunteer(const LimitedDriverVolunteer &other) :
                                 DriverVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft),type(other.type){};

        LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const{
            return new LimitedDriverVolunteer(*this);
        };

        int LimitedDriverVolunteer::getMaxOrders() const{
            return maxOrders;
        };

        int LimitedDriverVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        };

        bool LimitedDriverVolunteer::hasOrdersLeft() const{
            return ordersLeft>0;
        };

        bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
            return hasOrdersLeft() && DriverVolunteer::canTakeOrder(order);
        };// Signal if the volunteer is not busy, the order is within the maxDistance and have orders left

        void LimitedDriverVolunteer::acceptOrder(const Order &order){
            if(canTakeOrder(order)){
                ordersLeft = ordersLeft-1;
                DriverVolunteer::acceptOrder(order);
            }
        }; // Reset distanceLeft to maxDistance and decrease ordersLeft

        string LimitedDriverVolunteer::toString() const{
            string LimitedDriverVolunteerString = "volunteer " + getName() + " limited_driver " + std::to_string(getMaxDistance()) + std::to_string(getDistancePerStep()) + 
            std::to_string(getMaxOrders());
            return LimitedDriverVolunteerString;
        };
        VolunteerType LimitedDriverVolunteer::getType() const{
            return type;
        };
