
TARGET   = hypercli
CXXFLAGS = -g -Wall -Werror -pedantic-errors -fmessage-length=0
OBJPATH = bin
COBJ = $(OBJPATH)/Controller.o $(OBJPATH)/ExecBuilder.o $(OBJPATH)/Lock.o $(OBJPATH)/Parser.o

all: init Controller ExecBuilder Lock Parser
	g++ $(CXXFLAGS) src/$(TARGET).cpp -o $(TARGET) $(COBJ)

Controller:
	g++ $(CXXFLAGS) src/Controller.cpp -c -o $(OBJPATH)/Controller.o

ExecBuilder:
	g++ $(CXXFLAGS) src/ExecBuilder.cpp -c -o $(OBJPATH)/ExecBuilder.o

Lock:
	g++ $(CXXFLAGS) src/Lock.cpp -c -o $(OBJPATH)/Lock.o

Parser:
	g++ $(CXXFLAGS) src/Parser.cpp -c -o $(OBJPATH)/Parser.o

clean:
	@rm -f $(TARGET) $(OBJPATH)/*.o

init:
	@mkdir -p $(OBJPATH)

install:
	@echo "TODO"

test:
	@bash ./tests/test_cli.sh
