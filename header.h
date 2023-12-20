#ifndef header_h
#define header_h

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdexcept> 
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unordered_map>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>


#define MAPMODE     0
#define CHATMODE    1
#define REQMODE     2
#define INITMODE    3

#define GAMEPORT    11130
#define NAMELINE    128
#define MAXLINE     4096
#define LISTENQ     1024
#define SA          struct sockaddr
#define SERVERIP    "127.0.0.1"

#endif