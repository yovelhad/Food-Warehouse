#include "Action.h"
extern WareHouse* backup;

//BaseAction_cpp

//methods
    BaseAction::BaseAction():errorMsg(""), status(ActionStatus(1)){}; //constructor

    //copy constructor
    BaseAction::BaseAction(const BaseAction &other):errorMsg(other.getErrorMsg()), status(other.getStatus()){};

    //virtual destructor
    BaseAction::~BaseAction(){};

    ActionStatus BaseAction::getStatus() const{
        return status;
    };

//protected methods
    void BaseAction::complete(){
        status = ActionStatus::COMPLETED;
    };

    void  BaseAction::error(string errorMsg){
        this->errorMsg = errorMsg;
    };

    string BaseAction::getErrorMsg() const{
        return errorMsg;
    };


    //added methods
    string BaseAction::statusToString(ActionStatus status) const{
        switch(status){
            case ActionStatus::COMPLETED: return "COMPLETED";
            case ActionStatus::ERROR: return " ERROR";
        }
        return "ERROR";//it will never get to this point, only for compilation purposes
    };

    string BaseAction::customerToString(CustomerType _customerType) const{
        switch(_customerType){
            case CustomerType::Civilian: return "CIVILIAN";
            case CustomerType::Soldier: return "SOLDIER";
        }
        return "ERROR";//it will never get to this point, only for compilation purposes
    };
    void BaseAction::setStatus(ActionStatus status){
        status = status;
    };


//SimulateStep_cpp

//methods
    SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {};

    //copy constructor
    SimulateStep::SimulateStep(const SimulateStep &other) : BaseAction(other), numOfSteps(other.getNumberOfSteps()){};
    
    void SimulateStep::act(WareHouse &wareHouse){
        for(int i = 0; i < numOfSteps; i++){ //does step as many times as the user asked for.
            vector<Volunteer*> warehouseVolunteers = wareHouse.getVolunteersVector();
            vector<Order*>& PendingOrders = wareHouse.getPendingOrders();
            vector<Order*>& inProgressOrders = wareHouse.getInProcessOrders();
            for(Order* order: PendingOrders){

                for(auto* volunteer: warehouseVolunteers){

                    if(!(volunteer->isBusy()) && !(volunteer->hasOrdersLeft()) &&
                     (volunteer->getType() == VolunteerType::LIMITEDCOLLECTOR || volunteer->getType() == VolunteerType::LIMITEDDRIVER)){                       

                        wareHouse.removeVolunteer(volunteer->getId());
                    }
                    else{

                        if(volunteer->canTakeOrder(*order)){

                            if(!(order->getIsWaitingForDriver()) && (volunteer->getType() == VolunteerType::COLLECTOR || volunteer->getType() == VolunteerType::LIMITEDCOLLECTOR)){

                                volunteer->acceptOrder(*order);
                                order->setStatus(OrderStatus::COLLECTING);
                                order->setCollectorId(volunteer->getId());
                                wareHouse.moveOrder(PendingOrders,inProgressOrders,order);                               

                            }
                            else if(order->getIsWaitingForDriver() && (volunteer->getType() == VolunteerType::DRIVER || volunteer->getType() == VolunteerType::LIMITEDDRIVER)){
                                volunteer->acceptOrder(*order);
                                order->setIsWaitingForDriver(false);
                                order->setStatus(OrderStatus::DELIVERING);
                                order->setDriverId(volunteer->getId());
                                wareHouse.moveOrder(PendingOrders,inProgressOrders,order);
                            }
                        }
                    }
                }
            }    
            for(Order* order : inProgressOrders){
                if(order->getStatus() == OrderStatus::COLLECTING && order->getIsWaitingForDriver() == false){
                    int collectorID = order->getCollectorId();
                    wareHouse.getVolunteer(collectorID).step(); //advance volunteer one time unit.
                    if(!wareHouse.getVolunteer(collectorID).isBusy()){
                        order->finishCollecting();
                        wareHouse.moveOrder(inProgressOrders,PendingOrders,order);
                    }
                }
                else if(order->getStatus() == OrderStatus::COLLECTING && order->getIsWaitingForDriver() == true){
                    for(auto* volunteer: warehouseVolunteers){
                        if(volunteer->canTakeOrder(*order) && (volunteer->getType() == VolunteerType::DRIVER || volunteer->getType() == VolunteerType::LIMITEDDRIVER)){
                            volunteer->acceptOrder(*order);
                            order->setIsWaitingForDriver(false);
                            order->setStatus(OrderStatus::DELIVERING);
                            order->setDriverId(volunteer->getId());
                            int driverID = order->getDriverId();
                            wareHouse.getVolunteer(driverID).step(); //advance volunteer one time unit.
                            if(!wareHouse.getVolunteer(driverID).isBusy()){
                                order->setStatus(OrderStatus::COMPLETED);
                                wareHouse.moveOrder(inProgressOrders,wareHouse.getCompletedOrders(),order);
                            }
                            break;
                        }
                    }

                }  
                else if(order->getStatus() == OrderStatus::DELIVERING){
                    int driverID = order->getDriverId();
                    wareHouse.getVolunteer(driverID).step(); //advance volunteer one time unit.
                    if(!wareHouse.getVolunteer(driverID).isBusy()){
                        order->setStatus(OrderStatus::COMPLETED);
                        wareHouse.moveOrder(inProgressOrders,wareHouse.getCompletedOrders(),order);
                    }
                }
           
            }
        }    
        complete();
    };

    std::string SimulateStep::toString() const{
        string answer;
        answer = "simulateStep "+ std::to_string(getNumberOfSteps()) + " " + statusToString(getStatus());
        return answer;
    };

    int SimulateStep::getNumberOfSteps() const{
        return numOfSteps;
    };

    SimulateStep* SimulateStep :: clone() const{
        return new SimulateStep(*this);
    };


//AddOrder_cpp

//methods
    //constructor
    AddOrder::AddOrder(int id) : BaseAction(), customerId(id){};
    //copy constructor
    AddOrder::AddOrder( const AddOrder &other) : BaseAction(), customerId(other.getCustomerId()){};

    void AddOrder::act(WareHouse &wareHouse){

        if((wareHouse.getCustomer(getCustomerId()).getId() == -1 || (wareHouse.getCustomer(getCustomerId())).addOrder(wareHouse.getOrderCounter())<0)){ //check if customer doesnt exists in vector || if the customer has reached his limit of orders
            setStatus(ActionStatus::ERROR);
            error("Error: Cannot place this order");
            cout<< getErrorMsg() << endl;
        }
        else{
            int currentCustomerDistance = wareHouse.getCustomer(getCustomerId()).getCustomerDistance();
            Order* newOrder = new Order(wareHouse.getOrderCounter(),getCustomerId(),currentCustomerDistance);
            wareHouse.addOrder(newOrder);
            complete(); 
        }
    };

    string AddOrder::toString() const{
        return "Order " + std::to_string(getCustomerId()) + " " + statusToString(getStatus());
    }
    AddOrder* AddOrder::clone() const {
        return new AddOrder(*this);
    };

    //added methods
    int AddOrder::getCustomerId() const{
        return customerId;
    };


//AddCustomer_cpp
    //constructor
    AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):BaseAction(), customerName(customerName),
     customerType(stringToCustomerType(customerType)), distance(distance), maxOrders(maxOrders){};
    //copy constructor
    AddCustomer::AddCustomer(const AddCustomer &other): BaseAction(), customerName(other.getCustomerName()), customerType(other.getCustomerType()),
                                                        distance(other.getDistance()),maxOrders(other.getMaxOrders()){};

    void AddCustomer::act(WareHouse &wareHouse){
        if(typeid(customerType) == typeid(CustomerType::Civilian)){
            CivilianCustomer * newCustomer = new CivilianCustomer(wareHouse.getCustomerCounter(),customerName, distance, maxOrders);
            wareHouse.AddCustomerToVector(newCustomer);
            complete();
        }
        else{
            SoldierCustomer* newCustomer = new SoldierCustomer(wareHouse.getCustomerCounter(),customerName, distance, maxOrders);
            wareHouse.AddCustomerToVector(newCustomer);
            complete();
        }
    };

    AddCustomer* AddCustomer::clone() const{
        return new AddCustomer(*this);
    };

    string AddCustomer:: toString() const{
        return "customer " + getCustomerName() + " " + customerToString(getCustomerType()) + " "  +
                 std::to_string(getDistance()) + " " + std::to_string(getMaxOrders()) + " " + statusToString(getStatus());
    };


     //added methods
     CustomerType AddCustomer::stringToCustomerType(const string &type){
        if(type == "soldier" || type == "Soldier" || type == "SOLDIER"){
            return CustomerType::Soldier;
        }
        else{
            return CustomerType::Civilian;
        }
     };

     string AddCustomer::getCustomerName() const{
        return customerName;
     };

     int AddCustomer::getDistance() const{
        return distance;
     };

     CustomerType AddCustomer::getCustomerType() const{
        return customerType;
     };

    int AddCustomer::getMaxOrders() const{
        return maxOrders;
    };  

//PrintOrderStatus_cpp
    //constructor
    PrintOrderStatus::PrintOrderStatus(int id):BaseAction(), orderId(id){};
    //copy constructor
    PrintOrderStatus::PrintOrderStatus(const PrintOrderStatus &other):BaseAction(), orderId(other.getOrderId()){};

    void PrintOrderStatus:: act(WareHouse &wareHouse) {

        Order currentOrder = wareHouse.getOrder(orderId);
        if(currentOrder.getId()==-1){
            error("Error: Order doesnt exist");
            cout<< getErrorMsg() << endl;
        }
        else{
            cout<< "OrderId: " + std::to_string(orderId) << endl;
            cout<< "OrderStatus: " + currentOrder.statusToString(currentOrder.getStatus()) << endl;
            cout<< "CustomerID: " + std::to_string(currentOrder.getCustomerId()) << endl;
            //a situation where both collectorId and driverId are -1 is impossible, because if the order is pending, it has no volunteers assigned to it.
            int CurrentCollectorID = currentOrder.getCollectorId();
            if(CurrentCollectorID == NO_VOLUNTEER){
                cout<< "Collector: None" << endl;                 
            }
            else{
                cout<<"Collector: " + std::to_string(CurrentCollectorID)<< endl;
            }
            int CurrentDriverID = currentOrder.getDriverId();
            if(CurrentDriverID == NO_VOLUNTEER){
                cout<< "Driver: None" << endl;                 
            }
            else{
                cout<<"Driver: " + std::to_string(CurrentDriverID)<< endl;
            }
            complete();
        }
        
    };
    PrintOrderStatus* PrintOrderStatus::clone() const{
        return new PrintOrderStatus(*this);
    };
    string PrintOrderStatus:: toString() const{
        return "orderStatus " + std::to_string(getOrderId()) + " " + statusToString(getStatus());
    }

    int PrintOrderStatus::getOrderId() const
    {
        return orderId;
    };

//PrintCustomerStatus_cpp
    //constructor
    PrintCustomerStatus:: PrintCustomerStatus(int customerId):BaseAction(), customerId(customerId){};
    //copy constructor
    PrintCustomerStatus:: PrintCustomerStatus(const PrintCustomerStatus &other): BaseAction(), customerId(other.getCustomerId()){};

    void PrintCustomerStatus:: act(WareHouse &wareHouse){
        Customer& currentCustomer = wareHouse.getCustomer(customerId); //saving the customer because we call it more than once. returns & because we do not
        //want to duplicate it, only use getter functions
        if(currentCustomer.getId()==-1){
            error("Error: Customer doesnt exist");
            cout<< getErrorMsg()<<endl;
        }
        else{
            cout<< "CustomerID: " + std::to_string(customerId)<< endl;
            for(Order* order: wareHouse.getCompletedOrders()){
                if(order->getCustomerId()==customerId){
                    cout<< "OrderID: " + std::to_string(order->getId())<<endl;
                    cout<< "OrderStatus: " << order->statusToString(order->Order::getStatus())  <<endl;
                }
            }

            for(Order* order: wareHouse.getInProcessOrders()){
                if(order->getCustomerId()==customerId){
                    cout<< "OrderID: " + std::to_string(order->getId())<<endl;
                    if(order->getStatus() == OrderStatus::COLLECTING){
                        cout<< "OrderStatus: " << order->statusToString(order->Order::getStatus())  << endl; 
                    }
                    else{
                        cout<< "OrderStatus: " << order->statusToString(order->Order::getStatus())  << endl; 
                    }
                }
            }

            for(Order* order: wareHouse.getPendingOrders()){
                if(order->getCustomerId()==customerId){
                    cout<< "OrderID: " + std::to_string(order->getId())<<endl;
                    cout<< "OrderStatus: " << order->statusToString(order->Order::getStatus()) << endl; 
                }
            }//iterating this way because using getOrder(int id) is far less efficient

            cout<< "numOrdersLeft: " + std::to_string(currentCustomer.getOrdersLeft())<< endl;
        };
        complete();
    };

    PrintCustomerStatus* PrintCustomerStatus::clone() const{
        return new PrintCustomerStatus(*this);
    };

    string PrintCustomerStatus:: toString() const{
        return "customerStatus " + std::to_string(getCustomerId()) +" " + statusToString(getStatus());
    };

    //added methods
    int PrintCustomerStatus:: getCustomerId() const{
        return customerId;
    };


//PrintVolunteerStatus_cpp
    //constructor
        PrintVolunteerStatus::PrintVolunteerStatus(int id):BaseAction(),volunteerId(id){};

    //copy constructor
        PrintVolunteerStatus::PrintVolunteerStatus(const PrintVolunteerStatus &other): BaseAction(), volunteerId(other.getVolunteerId()){};

        void PrintVolunteerStatus::act(WareHouse &wareHouse){
            Volunteer& currentVolunteer = wareHouse.getVolunteer(volunteerId);
            if(currentVolunteer.getId()==-1){
                error("Error: Volunteer doesnt exist");
                cout<< getErrorMsg()<<endl;
            }
            else{
                cout<< "VolunteerID: " + std::to_string(volunteerId) << endl;
                string isBusyString;
                if(currentVolunteer.isBusy()){
                    isBusyString = "true";
                }
                else{
                    isBusyString = "false";
                }
                cout<< "isBusy: " << isBusyString << endl;

                if(!currentVolunteer.isBusy()){
                    cout<< "OrderID: None" <<endl;
                    if(currentVolunteer.getType() == VolunteerType::COLLECTOR){
                        cout<< "TimeLeft: None" << endl;
                        cout<<"OrdersLeft: No Limit" << endl;
                    }
                    else if(currentVolunteer.getType() == VolunteerType:: LIMITEDCOLLECTOR){
                        cout<< "TimeLeft: None" << endl;
                        cout<< "Orders Left: " << std::to_string((dynamic_cast<LimitedCollectorVolunteer*>(&currentVolunteer))->getNumOrdersLeft()) << endl;
                        
                    }
                    else if(currentVolunteer.getType() == VolunteerType::LIMITEDDRIVER){
                        cout<< "DistanceLeft: None" << endl;
                        cout<<"OrderLeft: " << std::to_string((dynamic_cast<LimitedDriverVolunteer*>(&currentVolunteer))->getNumOrdersLeft()) << endl;
                    }
                    else{
                        cout<< "DistanceLeft: None" << endl;
                        cout<<"OrderLeft: No Limit" << endl;
                    }
                }
                else{
                    cout<< "OrderID: " <<currentVolunteer.getActiveOrderId() <<endl;
                    if(currentVolunteer.getType() == VolunteerType::COLLECTOR){
                        cout<< "TimeLeft: " << std::to_string((dynamic_cast<CollectorVolunteer*>(&currentVolunteer))->getTimeLeft()) << endl; //cast is to make volunteer a collector
                        cout<<"OrderLeft: No Limit" << endl;
                    }
                    else if(currentVolunteer.getType() == VolunteerType::LIMITEDCOLLECTOR){
                        cout<< "TimeLeft: " << std::to_string((dynamic_cast<LimitedCollectorVolunteer*>(&currentVolunteer))->getTimeLeft()) << endl;
                        cout<<"OrderLeft: " << std::to_string((dynamic_cast<LimitedCollectorVolunteer*>(&currentVolunteer))->getNumOrdersLeft()) << endl;
                    }
                    else if(currentVolunteer.getType() == VolunteerType::LIMITEDDRIVER){
                        cout<< "DistanceLeft: " << std::to_string((dynamic_cast<LimitedDriverVolunteer*>(&currentVolunteer))->getDistanceLeft()) << endl;
                        cout<<"OrderLeft: " << std::to_string((dynamic_cast<LimitedDriverVolunteer*>(&currentVolunteer))->getNumOrdersLeft()) << endl;
                    }
                    else{
                        cout<< "DistanceLeft: " <<  std::to_string((dynamic_cast<DriverVolunteer*>(&currentVolunteer))->getDistanceLeft()) << endl;
                        cout<<"OrderLeft: No Limit" << endl;
                    }
                }   
                complete();
            }
        };

        PrintVolunteerStatus* PrintVolunteerStatus::clone() const{
            return new PrintVolunteerStatus(*this);
        };

        string PrintVolunteerStatus::toString() const{
            return "volunteerStatus " + std::to_string(getVolunteerId()) + " " + statusToString(getStatus()); 
        };

        //added methods
        int  PrintVolunteerStatus::getVolunteerId() const{
            return volunteerId;
        };


//PrintActionsLog_cpp

        //constructor
        PrintActionsLog::PrintActionsLog():BaseAction(){};

        //copy constructor
        PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) : BaseAction(){};

        void PrintActionsLog::act(WareHouse &wareHouse){
            for(auto* action: wareHouse.getActions()){
                cout<< action->toString() << endl;
            }
            complete();
        };
    
        PrintActionsLog* PrintActionsLog:: clone() const{
            return new PrintActionsLog(*this);
        };

        string PrintActionsLog::toString() const {
            return "log " + statusToString(getStatus());
         };



//Close_cpp

        //constructor
        Close::Close():BaseAction(){};

        //copy constructor
        Close::Close(const Close &other): BaseAction(){};

        void Close::act(WareHouse &wareHouse){
            backup = new WareHouse(wareHouse);
            for(auto* order : wareHouse.getPendingOrders()){
                cout<< "OrderID: "<< order->getId() << ",CustomerID: " << order->getCustomerId() << ",Status: " << order->statusToString(order->getStatus()) << endl;
            }
            for(auto* order : wareHouse.getInProcessOrders()){
                cout<< "OrderID: "<< order->getId() << ",CustomerID: " << order->getCustomerId() << ",Status: " << order->statusToString(order->getStatus()) << endl;
            }
            for(auto* order : wareHouse.getCompletedOrders()){
                cout<< "OrderID: "<< order->getId() << ",CustomerID: " << order->getCustomerId() << ",Status: " << order->statusToString(order->getStatus()) << endl;
            }
            wareHouse.close();
        };

        Close* Close::clone() const{
            return new Close(*this);
        };

        string Close::toString() const{
            return "close " + statusToString(getStatus());
        };


//BackupWareHouse_cpp

        //constructor
        BackupWareHouse::BackupWareHouse():BaseAction(){};
        //copy constructor
        BackupWareHouse::BackupWareHouse(const BackupWareHouse &other) : BaseAction(){};

        void BackupWareHouse::act(WareHouse &wareHouse){
            backup = new WareHouse(wareHouse);
            complete();
        };

        BackupWareHouse* BackupWareHouse::clone() const {
            return new BackupWareHouse(*this);
        };

        string BackupWareHouse::toString() const{
            return "backupWareHouse " + statusToString(getStatus());
        };

//RestoreWareHouse_cpp
        //constrcutor
        RestoreWareHouse::RestoreWareHouse():BaseAction(){};
        //copy constructor
        RestoreWareHouse::RestoreWareHouse(const RestoreWareHouse &other): BaseAction(){};

        void RestoreWareHouse::act(WareHouse &wareHouse){
            if(backup == nullptr){
                error("No backup available");
                cout<< getErrorMsg() << endl;
            }
            else{
            wareHouse = *backup;
            complete();
            }
        };

        RestoreWareHouse* RestoreWareHouse::clone() const{
            return new RestoreWareHouse(*this);
        };

        string RestoreWareHouse::toString() const {
            return "restoreWareHouse " + statusToString(getStatus());
        };


