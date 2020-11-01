CXX = g++
CXXFLAGS =  -Wall -g -std=c++17

main: main.o Message.o Portfolio.o
	$(CXX) $(CXXFLAGS) -o main main.o Message.o Portfolio.o

main.o: main.cpp message.h Portfolio.h
	$(CXX) $(CXXFLAGS) -c main.cpp

server: server.o Message.o Portfolio.o
	$(CXX) $(CXXFLAGS) -o server server.o Message.o Portfolio.o

server.o: server.cpp Message.h Portfolio.h
	$(CXX) $(CXXFLAGS) -c server.cpp

client: client.o Message.o
	$(CXX) $(CXXFLAGS) -o client client.o Message.o

client.o: client.cpp Message.h
	$(CXX) $(CXXFLAGS) -c client.cpp

Portfolio.o: Portfolio.h Message.h

Message.o: Message.h

clean:
	rm -f *.o main server client