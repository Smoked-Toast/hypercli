
TARGET   = hypercli
CXXFLAGS = -g -Wall -Werror -pedantic-errors -fmessage-length=0
COBJ = Controller.o Parser.o

all: Controller
	g++ $(CXXFLAGS) $(TARGET).cpp -o $(TARGET) $(COBJ)

Controller:
	g++ $(CXXFLAGS) Controller.cpp -c

Parser:
	g++ $(CXXFLAGS) Parser.cpp -c
clean:
	rm -f $(TARGET) *.o
