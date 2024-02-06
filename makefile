
all: clean compile link

link:
	g++ -o bin/warehouse bin/main.o bin/action.o bin/customer.o bin/order.o bin/volunteer.o bin/warehouse.o

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/order.o src/order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/warehouse.o src/WareHouse.cpp

clean:
	rm -f bin/*
