
TARGET   = hypercli
CXXFLAGS = -g -Wall -Werror -pedantic-errors -fmessage-length=0
COBJ = Controller.o Parser.o

all: Controller Parser
	g++ $(CXXFLAGS) src/$(TARGET).cpp -o $(TARGET) $(COBJ)

Controller:
	g++ $(CXXFLAGS) src/Controller.cpp -c

Parser:
	g++ $(CXXFLAGS) src/Parser.cpp -c

clean:
	rm -f $(TARGET) *.o

test:
	echo "TODO"
