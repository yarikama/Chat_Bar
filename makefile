CXX=g++
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lmysqlcppconn

LOGIN_OBJS=login.o elementTCP.o readline.o clientTCP.o

CLIENT_OBJS=readline.o clientTCP.o elementTCP.o clientMain.o

SERVER_OBJS=readline.o elementTCP.o game.o server.o

SERVERLOGIN_OBJS=readline.o elementTCP.o serverLOGIN.o serverTCP.o 

all: main server login

login: login.o
	$(CXX) $(CXXFLAGS) -o login $(LOGIN_OBJS) $(LDFLAGS)

main: $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o main $(CLIENT_OBJS) $(LDFLAGS)

server: $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o server $(SERVER_OBJS) $(LDFLAGS)

serverLOGIN: $(SERVERLOGIN_OBJS)
	$(CXX) $(CXXFLAGS) -o serverLOGIN $(SERVERLOGIN_OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o main server

.PHONY: clean
