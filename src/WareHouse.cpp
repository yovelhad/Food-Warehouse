#include "WareHouse.h"

    //constructor
    WareHouse::WareHouse(const string &configFilePath)  : isOpen(false), actionsLog(),volunteers(), pendingOrders(), inProcessOrders(),
     completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0){
        ifstream configFile(configFilePath);

        if (!configFile.is_open()) {
            cerr << "Error: Unable to open configuration file: " << configFilePath << endl;
            return;
        }

        string line;
        while (getline(configFile, line)) {
            istringstream iss(line);

            string entityType;
            string semiType;
            string name;
            string first; // Used for the first number in the line
            string second; // Used for the second number in the line
            string third; // Used for the third number in the line

            // Skip the first two words
            iss >>semiType >> name >> entityType;
            // Check if the stream is in a good state and read the third word
            if (iss) {
                if (entityType == "collector") {
                    iss >> first;
                    // Create a new Collector object and add it to the Volunteers vector
                    volunteers.push_back(new CollectorVolunteer(volunteerCounter++, name, stoi(first)));
                } 
                else if (entityType == "driver") {
                    iss >> first >> second;
                    // Create a new Driver object and add it to the Volunteers vector
                    volunteers.push_back(new  DriverVolunteer(volunteerCounter++, name, stoi(first), stoi(second)));                      
                } 
                else if (entityType == "limited_collector") {
                    iss >> first >> second;
                    // Create a new LimitedCollector object and add it to the Volunteers vector
                    volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter++, name, stoi(first), stoi(second)));
                } 
                else if (entityType == "limited_driver") {
                    iss >> first >> second >> third;
                    // Create a new LimitedDriver object and add it to the Volunteers vector
                    volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter++, name, stoi(first), stoi(second), stoi(third))); 
                } 
                else if (entityType == "soldier") {
                    iss >> first >> second;
                    // Create a new Soldier object and add it to the Customers vector
                    customers.push_back(new SoldierCustomer(customerCounter++, name, stoi(first), stoi(second))); 
                } 
                else if (entityType == "civilian") {
                    iss >> first >> second;
                    // Create a new Civilian object and add it to the Customers vector
                    customers.push_back(new CivilianCustomer(customerCounter++, name, stoi(first), stoi(second))); 
                }
                else {
                    // Handle the case where the third word doesn't match any entity type
                    cerr << "Error: Unknown entity type in configuration file." << endl;
                }
            } else {
                // Handle the case where there are not enough words
                cerr << "Error: Not enough words in the line in configuration file." << endl;
            }
        }

        configFile.close();
    };

    // Copy constructor
    WareHouse:: WareHouse(const WareHouse& other) : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(),
    completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
        // Copy the actions log
        for (auto action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        // Copy the volunteers
        for (auto volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }

        // Copy the pending orders
        for (auto order : other.pendingOrders) {
            pendingOrders.push_back(new Order(*order));
        }

        // Copy the in process orders
        for (auto order : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*order));
        }

        // Copy the completed orders
        for (auto order : other.completedOrders) {
            completedOrders.push_back(new Order(*order));
        }

        // Copy the customers
        for (auto customer : other.customers) {
            customers.push_back(customer->clone());
        }
    };

    // Copy assignment operator
    WareHouse& WareHouse::operator=(const WareHouse& other) {
        // Check for self assignment
        if (this != &other) {
            // Delete the current actions log
            for (auto action : actionsLog) {
                delete action;
            }
            actionsLog.clear();

            // Delete the current volunteers
            for (auto volunteer : volunteers) {
                delete volunteer;
            }
            volunteers.clear();

            // Delete the current pending orders
            for (auto order : pendingOrders) {
                delete order;
            }
            pendingOrders.clear();

            // Delete the current in process orders
            for (auto order : inProcessOrders) {
                delete order;
            }
            inProcessOrders.clear();

            // Delete the current completed orders
            for (auto order : completedOrders) {
                delete order;
            }
            completedOrders.clear();

            // Delete the current customers
            for (auto customer : customers) {
                delete customer;
            }
            customers.clear();

            // Copy the actions log
            for (auto action : other.actionsLog) {
                actionsLog.push_back(action->clone());
            }

            // Copy the volunteers
            for (auto volunteer : other.volunteers) {
                volunteers.push_back(volunteer->clone());
            }

            // Copy the pending orders
            for (auto order : other.pendingOrders) {
                pendingOrders.push_back(new Order(*order));
            }

            // Copy the in process orders
            for (auto order : other.inProcessOrders) {
                inProcessOrders.push_back(new Order(*order));
            }

            // Copy the completed orders
            for (auto order : other.completedOrders) {
                completedOrders.push_back(new Order(*order));
            }

            // Copy the customers
            for (auto customer : other.customers) {
                customers.push_back(customer->clone());
            }

            // Copy the other fields
            isOpen = other.isOpen;
            customerCounter = other.customerCounter;
            volunteerCounter = other.volunteerCounter;
            orderCounter = other.orderCounter;
        }

        return *this;
    };

    // Move constructor
    WareHouse::WareHouse(WareHouse&& other) noexcept : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(),
     completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
        // Move the actions log
        actionsLog = std::move(other.actionsLog);
        other.actionsLog.clear();

        // Move the volunteers
        volunteers = std::move(other.volunteers);
        other.volunteers.clear();

        // Move the pending orders
        pendingOrders = std::move(other.pendingOrders);
        other.pendingOrders.clear();

        // Move the in process orders
        inProcessOrders = std::move(other.inProcessOrders);
        other.inProcessOrders.clear();

        // Move the completed orders
        completedOrders = std::move(other.completedOrders);
        other.completedOrders.clear();

        // Move the customers
        customers = std::move(other.customers);
        other.customers.clear();
    };

    // Move assignment operator
    WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
        // Check for self assignment
        if (this != &other) {
            // Delete the current actions log
            for (auto action : actionsLog) {
                delete action;
            }
            actionsLog.clear();

            // Delete the current volunteers
            for (auto volunteer : volunteers) {
                delete volunteer;
            }
            volunteers.clear();

            // Delete the current pending orders
            for (auto order : pendingOrders) {
                delete order;
            }
            pendingOrders.clear();

            // Delete the current in process orders
            for (auto order : inProcessOrders) {
                delete order;
            }
            inProcessOrders.clear();

            // Delete the current completed orders
            for (auto order : completedOrders) {
                delete order;
            }
            completedOrders.clear();

            // Delete the current customers
            for (auto customer : customers) {
                delete customer;
            }
            customers.clear();

            // Move the actions log
            actionsLog = std::move(other.actionsLog);
            other.actionsLog.clear();

            // Move the volunteers
            volunteers = std::move(other.volunteers);
            other.volunteers.clear();

            // Move the pending orders
            pendingOrders = std::move(other.pendingOrders);
            other.pendingOrders.clear();

            // Move the in process orders
            inProcessOrders = std::move(other.inProcessOrders);
            other.inProcessOrders.clear();

            // Move the completed orders
            completedOrders = std::move(other.completedOrders);
            other.completedOrders.clear();

            // Move the customers
            customers = std::move(other.customers);
            other.customers.clear();

            // Move the other fields
            isOpen = other.isOpen;
            customerCounter = other.customerCounter;
            volunteerCounter = other.volunteerCounter;
            orderCounter = other.orderCounter;
        }

        return *this;
    };

    // Destructor
    WareHouse::~WareHouse() {
        this->close();

    };
    
    //methhod
    
    void WareHouse :: start(){
        open();        
        cout << "Waiting for input!" << endl;
        string nextUserInput;
        while(isOpen){
            cin >> nextUserInput;
            if(nextUserInput == "menu"){
                cout << "--------------------------" << endl;
                cout << "close" << endl;
                cout << "backup" << endl;
                cout << "restore" << endl;
                cout << "log" << endl;
                cout << "volunteerStatus" << endl;
                cout << "customerStatus" << endl;
                cout << "orderStatus" << endl;
                cout << "customer" << endl;
                cout << "order" << endl;
                cout << "step" << endl;
            }
            else if(nextUserInput == "close"){
                Close* close = new Close();
                actionsLog.push_back(close);

                close->act(*this);

            }
            else if(nextUserInput == "backup"){
                BackupWareHouse* backupWareHouse = new BackupWareHouse();
                backupWareHouse->act(*this);       

                actionsLog.push_back(new BackupWareHouse());         
            }        
            else if(nextUserInput == "restore"){
                RestoreWareHouse* restoreWareHouse = new RestoreWareHouse();
                restoreWareHouse->act(*this);

                actionsLog.push_back(restoreWareHouse);
            }
            else if(nextUserInput == "log"){
                PrintActionsLog* printActionsLog = new PrintActionsLog();
                printActionsLog->act(*this);
                actionsLog.push_back(printActionsLog);
            }
            else if(nextUserInput =="volunteerStatus"){
                cin >> nextUserInput;
                int volunteerId = stoi(nextUserInput);
                PrintVolunteerStatus* printVolunteerStatus = new PrintVolunteerStatus(volunteerId);
                printVolunteerStatus->act(*this);

                actionsLog.push_back(printVolunteerStatus);
            }
            else if(nextUserInput == "customerStatus"){
                cin >> nextUserInput;
                int customerId = stoi(nextUserInput);
                PrintCustomerStatus* printCustomerStatus = new PrintCustomerStatus(customerId);
                printCustomerStatus->act(*this);

                actionsLog.push_back(printCustomerStatus);
            }
            else if(nextUserInput == "orderStatus"){
                cin >> nextUserInput;
                int orderId = stoi(nextUserInput);
                PrintOrderStatus* printOrderStatus = new PrintOrderStatus(orderId);
                printOrderStatus->act(*this);

                actionsLog.push_back(printOrderStatus);
            }

            else if(nextUserInput == "customer"){
                cin >> nextUserInput;
                string customerName = nextUserInput;
                cin >> nextUserInput;
                string customerType = nextUserInput;
                cin >> nextUserInput;
                int customerDistance = stoi(nextUserInput);
                cin >> nextUserInput;
                int customerMaxOrders = stoi(nextUserInput);
                AddCustomer* addCustomer = new AddCustomer(customerName, customerType, customerDistance, customerMaxOrders);
                                addCustomer->act(*this);

                actionsLog.push_back(addCustomer);

            }

            else if(nextUserInput == "order"){
                cin >> nextUserInput; 
                int customerId = stoi(nextUserInput);
                AddOrder* addOrder = new AddOrder(customerId);
                                addOrder->act(*this);

                actionsLog.push_back(addOrder);
            }
            else if(nextUserInput == "step"){
                cin >> nextUserInput;
                int numberOfSteps = stoi(nextUserInput);
                SimulateStep* simulateStep = new SimulateStep(numberOfSteps);
                simulateStep->act(*this);

                actionsLog.push_back(simulateStep);
            }
            else{
                cout << "Invalid input!" << endl;
            }
                
        }
    };

    const vector<BaseAction*> & WareHouse :: getActions() const{
        return actionsLog;
    };

    void WareHouse :: addOrder(Order* order){
        pendingOrders.push_back(order);
        orderCounter++;
    };

    void WareHouse :: addAction(BaseAction* action){
        actionsLog.push_back(action);
    };


    Customer & WareHouse :: getCustomer(int customerId) const{
        for(const auto& customer: customers){
            if(customer->getId() == customerId){
                return *customer;
            }
        }
        static SoldierCustomer dummyCustomer(-1, "Not found", -1,-1);
        return dummyCustomer;
    };

    Volunteer & WareHouse :: getVolunteer(int volunteerId) const{
        for(const auto& volunteer: volunteers){
            if(volunteer->getId() == volunteerId){
                return *volunteer;
            }
        }
        static CollectorVolunteer dummyVolunteer(-1, "Not found", -1);
        return dummyVolunteer;        
        
    };

    Order & WareHouse :: getOrder(int orderId) const{
        for(const auto& order: pendingOrders){
            if(order->getId() == orderId){
                return *order;
            }
        }
        for(const auto& order: inProcessOrders){
            if(order->getId() == orderId){
                return *order;
            }
        }

        for(const auto& order: completedOrders){
            if(order->getId() == orderId){
                return *order;
            }
        }

        static Order dummyOrder(-1, -1, -1);
        return dummyOrder;  
    };

    void  WareHouse :: close(){
        if(isOpen){        // Delete the actions log
            for (auto action : actionsLog) {
                delete action;
            }
            actionsLog.clear();

            // Delete the volunteers
            for (auto volunteer : volunteers) {
                delete volunteer;
            }
            volunteers.clear();

            // Delete the pending orders
            for (auto order : pendingOrders) {
                delete order;
            }
            pendingOrders.clear();

            // Delete the in process orders
            for (auto order : inProcessOrders) {
                delete order;
            }
            inProcessOrders.clear();

            // Delete the completed orders
            for (auto order : completedOrders) {
                delete order;
            }
            completedOrders.clear();

            // Delete the customers
            for (auto customer : customers) {
                delete customer;
            }
            customers.clear();

            cout << "Warehouse is closed!" << endl;
            isOpen = false;
        }
    };

    void WareHouse :: open(){
        isOpen = true;
        cout << "Warehouse is open!" << endl;
    };



    //added methods
    vector<Order*>& WareHouse :: getPendingOrders() { 
        return pendingOrders;
    };

    vector<Order*>& WareHouse :: getInProcessOrders() {
        return inProcessOrders;
    };

    vector<Order*>& WareHouse :: getCompletedOrders() {
        return completedOrders;
    };

    vector<Volunteer*> WareHouse :: getVolunteersVector() const {
        return volunteers;
    };

    void WareHouse :: moveOrder(vector<Order*>& sourceVector, vector<Order*>& destinationVector, Order* OrderToTransfer) {
        // Find the order in the source vector
        for(auto iter = sourceVector.begin(); iter != sourceVector.end(); iter++){
            if((*iter)->getId() == OrderToTransfer->getId()){
                // Add the order to the destination vector
                destinationVector.push_back(*iter);
                // Remove the order from the source vector
                sourceVector.erase(iter);
                return;
            }
        }
    };

    int WareHouse::getOrderCounter() const{
        return orderCounter;
    };

    int WareHouse:: getCustomerCounter() const{
        return customerCounter;
    };

    int WareHouse:: getVolunteerCounter() const{
        return volunteerCounter;
    };
    void WareHouse:: AddCustomerToVector(Customer* customer){
        customers.push_back(customer);
        customerCounter++;
    };
    void WareHouse::AddVolunteerToVector(Volunteer* volunteer){
        volunteers.push_back(volunteer);
        volunteerCounter++;
    };
    void WareHouse::removeVolunteer(int volunteerId){
        for(auto iter = volunteers.begin(); iter != volunteers.end(); iter++){
            if((*iter)->getId() == volunteerId){
                delete *iter;
                volunteers.erase(iter);
                break;
            }
        }
    };