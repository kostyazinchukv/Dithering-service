HEADERS = include
SOURCE = src
# Compiler
CXX = g++

#Compiler flags
CXXFLAGS += -g -std=gnu++11 -Wall \
			 -lpthread

all: service

clean: 
	rm -fr *.o *.a

basesocket.o: $(SOURCE)/basesocket.cpp $(HEADERS)/basesocket.h
	$(CXX) $(CXXFLAGS) -c $< 

server.o: basesocket.o $(SOURCE)/server.cpp $(HEADERS)/server.h 
	$(CXX) $(CXXFLAGS) -c $(SOURCE)/server.cpp 

dithering.o: $(SOURCE)/dithering.cpp $(HEADERS)/dithering.h
	$(CXX) $(CXXFLAGS) -c $(SOURCE)/dithering.cpp

service: basesocket.o server.o dithering.o $(SOURCE)/main.cpp
	$(CXX) $(CXXFLAGS) \
			$^ -I $(HEADERS) -o negrila -lSDL2 -lSDL2_image 
