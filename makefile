all: clean compile link

link:
		g++ -o bin/warehouse bin/Action.o bin/Order.o bin/Customer.o bin/Volunteer.o bin/WareHouse.o bin/main.o

compile:
		g++ -g -Wall -Weffc++ -I include -c -o bin/Action.o src/Action.cpp
		g++ -g -Wall -Weffc++ -I include -c -o bin/Customer.o src/Customer.cpp
		g++ -g -Wall -Weffc++ -I include -c -o bin/Order.o src/Order.cpp
		g++ -g -Wall -Weffc++ -I include -c -o bin/Volunteer.o src/Volunteer.cpp
		g++ -g -Wall -Weffc++ -I include -c -o bin/WareHouse.o src/WareHouse.cpp
		g++ -g -Wall -Weffc++ -I include -c -o bin/main.o src/main.cpp

clean:

		rm -f bin/*
		
