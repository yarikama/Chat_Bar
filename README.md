# Network Programming Final Project Report

**Languages / 語言：**
[English](README.md) | [繁體中文](README.zh-TW.md)

---

> ***Recommended to view the web-friendly version: [https://highfalutin-biplane-60c.notion.site/Network-Programming-Final-Project-Report-23f8b6780f8344d78265ce883fab6135?pvs=4](https://www.notion.so/Network-Programming-Final-Project-Report-23f8b6780f8344d78265ce883fab6135?pvs=21)***

## Table of Contents
- [0. Introduction](#0-introduction)
- [1. Development Environment](#1-development-execution-environment-special-requirements-adaptations)
- [2. Research Methods and Design](#2-research-methods-and-design)
- [3. Results](#3-results-main-functions-and-features-of-final-results)
- [4. Conclusion](#4-conclusion)
- [5. References](#5-references-and-appendix)
- [6. GitHub Repository](#6-our-source-code-in-github-repository)
- [7. Others](#7-others)

---

# 0. Introduction

## 0.0 Game Overview

This Network Programming final project features a **MUD real-time map game combined with communication and chat functionality**. It aims to create an experience where users can freely move around a map, meet new friends, invite others to chat, and view real-time personal and best scores. Developed and designed by **Sun Yu-Han and Hsu Heng-Jui**.

## 0.1 Tools Overview

**Languages:** C++, MySQL

**APIs:** OpenSSH, JSON, SQL, SFML, GDB, Valgrind

**Support Tools:** ChatGPT, Github, VSCode, Copilot

**Graphics:** DALL-E

**Music:** [Medieval Royalty Free Music](https://www.youtube.com/watch?v=SyzE2mZZvks)

**Fonts:** [Breathe Fire](https://www.fontspace.com/breathe-fire-iii-font-f69367)

**VPN:** Hamachi

(Due to router restrictions in our rental housing, we used a free but unstable VPN to set up the SERVER)

## 0.2 Division of Labor

All project content was designed, completed, and adapted through discussions between the two team members. While there are no completely independent parts done by a single person, the work can be roughly divided as: **Server - Sun Yu-Han, Client - Hsu Heng-Jui**

# 1. Development, Execution Environment, Special Requirements, Adaptations

## 1.0 Prerequisites

This section mainly covers the game development overview, and if you want to compile or use our Github repository, here's the guide on how to implement it, especially regarding special requirements. Here's our file structure and corresponding implementation content.

```
├── Assets                       (Font, image, music files)
│   ├── Fonts
│   │   ├── chat_font.ttf
│   │   └── login_font.ttf
│   ├── Musics
│   │   └── bgm.ogg
│   └── Pictures
│       ├── boy.png
│       ├── characterBackground.png
│       ├── chatclock.png
│       ├── chatroomicon.png
│       ├── chatroom.png
│       ├── friendrequest.png
│       ├── login.png
│       ├── map.png
│       └── request.png
├── cleanSQL.sql                (Script to delete Database)
├── clientMain.cpp              (Client game mode)
├── clientTCP.cpp               (Client handles Packet transmission)
├── clientTCP.h             
├── elementTCP.cpp              (Packet implementation)
├── elementTCP.h            
├── header.h                    (Include all necessary libraries, Define variables)
├── initSQL.sql                 (Script to create Database)
├── login.cpp                   (Client login and registration)
├── makefile                    (Compile)
├── readline.cpp                (Readline implementation)
├── readline.h          
├── README.md                   (Preinstall information & specification)
├── serverGame.cpp              (Server handles game mode)
├── serverLOGIN.cpp             (Server handles login mode)
├── serverTCP.cpp               (Server handles Packet transmission)
└── serverTCP.h
```

In our design, we implement this project in an object-oriented manner as much as possible.

## 1.1 Development

This program is developed based on C++, and the game GUI uses SFML for accessing images, fonts, music (these resources are not provided by this Library), etc. While SFML provides APIs for game networking operations, we believe that the networking part of this assignment should be done by ourselves, so we use what we learned in class and content from the Unix Socket Programming book.

This game is based on TCP connection, using Blocking I/O during login and registration, and also uses fork() to open game mode as an intermediary. When entering the game, it uses Nonblocking I/O to handle real-time player actions.

Additionally, on the Server side, we use MySQL to establish a database to record and organize player information, including accounts, passwords, play time, etc., and use the library in OpenSSH with SHA1 encryption for player registration passwords.

When Client and Server exchange messages, they use JSON format for transmission. This is because if basic data types like char or string are used during transmission, errors are hard to detect. When using JSON format for transmission, if the Parse part goes wrong, the system will throw an ERROR. On the other hand, it can guarantee the data format without causing confusion.

For other image sources, we use DALL-E in ChatGPT to generate them, with the style direction generally being RPG and Pixel keywords to generate images. The fonts and music parts are all used after confirming the License.

## 1.2 Execution Environment

SFML Library official documentation states it can be used on **Linux, Windows, MacOS**, but we mainly use WSL and Linux as testing platforms. WSL sometimes displays Error Messages because the Buffer is too small to handle a large amount of data transmission in a short time. We have tried to increase the Socket Buffer to 8K through Socket Options, but occasionally this problem still occurs on WSL.

## 1.3 Special Requirements (**Must Read Before Execution** - Compile, Library Installation, Database Setup)

There are three main items to note from compilation to entering the game settings:
**Install Libraries, directly use Make to compile, establish Database, declare your own IP address and Port, after starting `./serverGAME` and `./serverLOGIN`, directly use `./login` to start the game**

Libraries need to be installed before compilation: (We provide installation commands in our GitHub Repository Readme)

```bash
sudo apt install nlohmann-json3-dev
sudo apt-get install libsfml-dev
sudo apt-get install libmysqlcppconn-dev
sudo apt-get install libssl-dev
sudo apt install mysql-server
```

After installation, you also need to create System Database: (including creating users and Database Frame for this Server)

```sql
mysql -u root -p
[your passwd]

SOURCE initSQL.sql
```

To delete Database

```bash
SOURCE cleanSQL.sql
```

In the game, we define Server IP and Server Port in header.h. If you want to use this Server, you must first change:

```cpp
#define INFINDISTANCE       1000000
#define GAMEPORT            11130
#define LOGINPORT           11131
#define NAMELINE            128
#define MAXLINE             4096
#define LISTENQ             1024
#define MOVEDISTANCE        37
#define SA                  struct sockaddr
#define SERVERIP            "127.0.0.1"
#define SQLPASSWD           "Np2023@finalproject"
```

Firewall Configuration

```bash
sudo ufw allow 11131/tcp
sudo ufw allow 11130/tcp
sudo ufw reload
```

## 1.4 Adaptation

In most cases, we use GDB to Debug, and in a few cases, we use cout directly or Valgrind for debugging.

# 2. Research Methods and Design

## 2.0 Preliminary Writing Style

We declare Functions in `.h` files and implement them in `.cpp` (except for `./clientMain`), and most are built in an abstract way to create objects, such as: the implementation of characters corresponds to `Class Character`, implementing character-related functions and defining character materials. We put a lot of effort into keeping the Code clean, because Code that is too messy in such a large structure will cause great difficulties in producing other functions later.

## 2.1 Server and Client Program Functions

### 2.1.1 Login Function

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled.png)

**Game Screen**

First, users will use `./login` to register with `./serverLOGIN`, and the Client must process user input and convert it into Packets. At this time, the Client will send a LOGINMODE Packet to the Server, which will check for duplicate registration, invalid registration, or successful registration, and similarly use Packets to return the result to the Client for further GUI display to notify the user of the result.

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%201.png)

If registration is successful, users can log in again by entering their account and password through the Client. After success, the Client will use `fork()` and `exec()` to switch from `./login` to `./main` to connect with the Game Server and implement game interaction. Users do not need to enter any IP or address during this process.

### 2.1.2 Game Functions

Server (`./serverGAME`) and Client (`./main`) mainly implement **Map Mode** and **Chat Mode:**

**Map Mode**

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%202.png)

**Map Mode Game Screen**

Users can press keyboard `WASD` to move, allowing the Client to detect and process. The following functions must be implemented:

- User character coordinate movement
- Send own coordinates to Server (using `MAPMODE` Packet)
- Lock screen center view on user
- Update relative distances of other characters and chat rooms
- Record nearest character and chat room
- Determine if nearest character or chat room is within chat radius, can send chat invitation, and prompt user that invitation can be sent

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%203.png)

**Coordinate Update Action Diagram**

At this time, the Server will process the Packet sent by the Client as follows:

- Broadcast the Packet received from the Client to all online Clients (map, chat room)

Other Clients receiving Packets from the Server must complete:

- Ignore Packets where the Sender is themselves
- Update position for the Packet's Sender
- Update shortest distance for the Packet's Sender
- Determine if there is a character at a shorter distance
- Determine if chat invitation can be sent to the nearest character or chat room, and prompt user that invitation can be sent

![螢幕擷取畫面 2024-01-12 152909.png](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/%25E8%259E%25A2%25E5%25B9%2595%25E6%2593%25B7%25E5%258F%2596%25E7%2595%25AB%25E9%259D%25A2_2024-01-12_152909.png)

**Chat Invitation Screen**

In the Code, we use `Class Character` to handle initialization related to Character movement, initialization (such as graphics, names, etc.), character position, and display functions. `Class OtherCharacter` handles other **single players**, inheriting Character functions and adding distance to the user's character and update function parts. Additionally, there is `Class OtherCharacters` which uses unordered_map structure to store **all online player characters**, and provides functions to update all people's positions at once, add other characters, delete other characters, and record the nearest character.

`Class ChatRoomIcon` handles chat room-related parts, with data parts including coordinates, images, etc., and functions including display and initialization. `Class ChatRoomIcons` records data structures of different chat rooms, also with functions to update minimum position.

When sending an invitation, it's necessary to determine who the object at minimum distance is (could be another player or a chat room), then prompt the user to use Ctrl + Z / Ctrl + X based on the minimum object.

**Chat Mode**

![螢幕擷取畫面 2024-01-12 153111.png](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/%25E8%259E%25A2%25E5%25B9%2595%25E6%2593%25B7%25E5%258F%2596%25E7%2595%25AB%25E9%259D%25A2_2024-01-12_153111.png)

**Chat Game Screen**

It will first send an **invitation** (`REQMODE` Packet) to another user. The Peer Client will prompt the user with a message that an invitation has been received, asking them to press Y/N to accept or reject the invitation, and have the Server create a chat room to record chat content and transmit Packets to Clients in the chat room.

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%204.png)

**Chat Invitation Action Diagram**

Invitation requests can only be sent when there is a character or chat room within the chat radius. And invitations will be sent to the nearest character or chat room. Additionally, chat room creation requires a minimum of two people, and there is no limit on the number of people joining subsequently.

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%205.png)

**Client & Server Interaction Diagram**

When a player enters a chat room, the timer in the upper right corner will start timing. At this time, when the Client sends a message to the Server, the Server will broadcast to all other Clients in the chat room to achieve player chat functionality. Players can press Esc to leave the chat room, the chat room timer ends, and other players in the chat room will receive notification that someone has left. At this time, players can choose to continue staying in the chat room to chat or press Esc to leave the chat room and return to the map. Once the chat room has 0 people due to players leaving or Client disconnection, the chat room will be automatically cleared by the Server.

## 2.2 Data Transmission Format

Server and Client uniformly use the Packet data structure to initialize data. The format content includes mode, sender_name, receiver_name, x, y, message, suitable for transmitting float and string data in various modes. Packet mode is divided into 12 categories to facilitate communication between Server and Client, and can clearly express and judge the actions to be taken for each Packet.

During transmission, it will go through Serialize to convert the Packet into a JSON file and send it to the peer, and when the peer receives it, it will Deserialize this JSON file into a Packet for further processing. In this way, problems caused by overlapping Packets can be detected. If errors occur during the Parse process of the JSON Library, errors will be thrown.

```cpp
struct Packet{
	json packet_to_json();
	Packet json_to_packet(json& json_packet);
	int mode_packet; 
	char sender_name[NAMELINE]; 
	char receiver_name[NAMELINE]; 
	float x_packet;
	float y_packet;
	char message[MAXLINE];
	void printPacket();
};
```

```cpp
#define EMPTYMODE       -1
#define MAPMODE         0
#define CHATMODE        1
#define REQMODE         2
#define INITMODE        3
#define LOGINMODE       4
#define REGISTERMODE    5
#define ESCMODE         6
#define TIMEMODE        7
#define ROOMMODE        8
#define RANKMODE        9
#define JOINMODE        10
```

The format is closely related to the design and usage method. We define all related formats in `header.h` (such as: MODE, STATE, SPECIAL VALUE).

## 2.3 Player Data Storage Method

Each player's personal information in the program is packaged in the Player structure, including playerID and sockfd to store player nickname and socket number, roomID stores the chat room number the player enters, mode displays whether the player is currently in map or chat mode, and stores the player's current x, y coordinates.

```cpp
struct Player{
	std::string playerID;
	int sockfd;
	int roomID;
	int mode_player; //MAP , CHAT
	float x_player;
	float y_player;
};
```

Player accounts, passwords, and chat time are stored in the MySQL database, divided into two Tables. The User Table uses the SHA1 hash function to replace the original password string for encryption, storing accounts and encrypted passwords in the database to avoid insecurity from malicious program injection or background data leakage. The ChatTime Table stores each player's chat time for personal and best total time statistics.

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%206.png)

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%207.png)

**Database ER Diagram**

**Database Update Action Diagram**

## 2.4 Chat Room Storage Method

In the Server, if players create a chat room, they need to be stored in a 2D array with RoomID as Index and players as Elements. The Server will delete player Elements when players leave (Esc or force close game), and at the same time, if the chat room has no one, it will clear this chat room and send a cancel chat room Packet to everyone.

## 2.5 Exception Analysis and Handling

When players leave the game due to exceptional circumstances such as closing the window, abnormal disconnection, offline, or ending the program, there are different processing methods for map mode or chat mode. In map mode, the Server removes the disconnected player from the unordered_map and broadcasts a packet to all remaining Clients, including the disconnected player's name, with x and y values as NOWHERE. Then the Client removes the disconnected player from the internal unordered_map, and the disconnected player will no longer be drawn on the map. In chat mode, the Server removes the disconnected player from the original chat room and broadcasts to all Clients in that chat room. At this time, other players in the chat room will receive notification that someone has gone offline. Through the above methods to handle player abnormal disconnection situations, not only can the game continue to run, but players in chat can also know the current situation.

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%208.png)

**Chat Exception Handling Screen**

# 3. Results: Main Functions and Features of Final Results

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%209.png)

- **Character Movement**
    - Player-centered: Window screen view is centered on the player, and as the player moves, the map view also moves accordingly.
    - Approach to chat: If moving close to other players and the distance between the two is within 100f, invitations can be sent or received to enter the chat room to chat
- **Normal Chat**
    - Time statistics: Once entering the chat room, it will start calculating chat time until pressing Esc to leave the chat room, then timing ends.
- **Multi-person Chat**
    - Group messaging: If the map shows an existing chat room, players can approach other players and press X, or approach the chat room icon and press Z to join the chat room. After joining the chat room, they can send messages and chat with all players in the chat room.
    - Time statistics: Multi-person chat time calculation is the same as normal two-person chat, time is counted from entering the chat room. If others all leave the chat room, it will continue timing until the player leaves the chat room.
- **Leaderboard**
    - Chat time: After players leave the chat room, the Client sends this chat time to the Server, the Server puts the data into the database, then returns the player's personal accumulated time and best chat time to the Client, for the Client to update the leaderboard player name and time.
- **Database**
    - Login information: During registration, after players enter account and password, the Client encrypts the password entered by the player with sha1 string replacement, sends the account and encrypted password to the Server to confirm if there is duplicate registration. If not, registration is successful and login can proceed. During login, the Client similarly encrypts the password entered by the player with sha1 string replacement, sends to the Server to confirm if the data is correct. If correct, it will jump to the Game Server.
    - Time list: After players leave the chat room, the Client sends this chat time to the Server, the Server puts the data into the database, then returns the player's personal accumulated time and best chat time to the Client, for the Client to update personal time in the leaderboard, and the player name and time of the best score.

# 4. Conclusion

## 4.0 Project Creation Experience

For this project creation, it was our team's first time encountering Socket Programming projects. For us, it was a very challenging and fulfilling attempt. During the development period, we often encountered problems that needed to be solved, or parts where user experience could be better. We continuously modified and optimized, and also understood in practice how to actually apply the functions and concepts from class. Below are some difficulties encountered during creation and the resolution process:

## 4.1 Difficulties Encountered and Resolution Process

1. **Packet Transmission Design:**
Unlike homework where information is transmitted between Client and Server as strings, we wanted to add float and string data types, and be able to include more information in a single transmission, such as sender name, receiver name, etc. Therefore, we designed our own data structure Packet to package the required data types (`./elementTCP.cpp`, `./elementTCP.h`). This requires serialization or deserialization according to sending or receiving in string and JSON formats during transmission and reception, which took a lot of time to try and debug.

2. **Buffer Overflow:**
At the end of project creation, we still found that when using Client on WSL for connection, there would be errors from Kernel responding that Buffer was blown up. We tried to solve it with Socket Options, but it didn't seem to work. Using virtual machines doesn't have errors. This is also a part that needs further research in the future.

3. **Blocking / Non-blocking Choice:**
Because the Login function needs to wait for player input during login and registration, the Client sends data to the Server and verifies in MySQL, so Blocking I/O is used. The Game function uses SFML and sets the frame rate to 60 Hz. It will run the while loop 60 times in 1 second. The game design has no limit on the number of users, and it's also impossible to determine whether there are Client actions returned in each loop, which could cause the game to freeze. To ensure the real-time nature of the game, Non-blocking is used.
At the same time, in this project, we discovered that the Readline function is not part of the C standard library, but was made by the author in the Unix Socket Programming book. Because receiving data needs to be judged by `\n`, we implemented Readline.

4. **Setting up VPN to Test Remote Connection:**
Because the router in our rental housing doesn't allow settings, we use Hamachi to create a Group for connection, otherwise the connection will be blocked by the router, and the firewall Ports also need to be closed. But the disadvantage is that the free version connection is relatively unstable, and only 5 hosts can be used.

5. **Packet Reception Overlap Problem:**
Sometimes there are Packet overlap problems. The Server needs to be set to ignore this error, otherwise it will crash completely.

6. **Duplicate Character Problem:**
Duplicate characters are most likely to occur on one's own character, including when logging in again after leaving. The Server will mistakenly think there are two people. Here we perform 2 Checks on the Client side for Packets where the received Packet Sender Name overlaps with one's own ID.

7. **Mode Isolation Problem:**
Originally in our design, map mode and chat mode were isolated, but according to this design, several problems would occur, such as when chatting, new players join the map, and when you leave chat mode, you can't see that user unless they move. Also, when you initiate chat and create a chat room, leaving chat doesn't show the chat room icon still there (there are other chat rooms inside). So later we improved it so that during the chat process, other Mode Packets can still be processed.

8. **From Two-person Chat to Multi-person Chat:**
Two-person chat implementation is very simple, the Server just needs to read the Packet receiver and sender and be responsible for forwarding messages. But multi-chat room and multi-person chat must maintain the chat room data structure to record the chat room and access which players there are. It's also necessary to handle chat room addition and removal.

## 4.2 Future Improvements or Extensions for Results

Here we provide records of functions we designed and implemented halfway:

**Friend Function:**
We have designed friend functions and images, as well as the Client-side program part, but haven't had enough time to complete Server maintenance, and the most difficult part is Debug. We designed that after 60 seconds of chatting, the chat room menu can be opened, check the object to apply to add as a friend, and the other side will receive an invitation to reply agree or not. At the same time, the Server needs to maintain the Database to record friend relationships and display the friend list on the side.

**Friend Chat:**
Friend chat also doesn't need distance. Select chat in the friend list to send an invitation for private chat function (no chat room will appear).

**Registration Character Selection:**

We originally envisioned having a character style selection function and creating a game account according to the selected character. The Server also needs to record which character this account uses. But due to the difficulty of generating images of the same style, and also needing to adjust the size, as well as doing more GUI, we put this function aside for later.

# 5. References and Appendix

1. [SFML Network Programming Tutorial](https://youtube.com/playlist?list=PLvv0ScY6vfd95GMoMe2zc4ZgGxWYj3vua&si=NgLTCKtw6o9yv19p)
2. [C++ Socket Programming Tutorial](https://youtube.com/playlist?list=PL21OsoBLPpMOO6zyVlxZ4S4hwkY_SLRW9&si=82lu6UkTRmb0fHot)
3. [SFML Documentation](https://www.sfml-dev.org/tutorials/2.6/)
4. [Code Aesthetic](https://www.youtube.com/@CodeAesthetic)
5. [Unix Socket Programming - readline implementation](https://man7.org/tlpi/code/online/dist/sockets/read_line.c.html)
6. ChatGPT

# 6. Our Source Code in Github Repository

🔗 [https://github.com/yuhansun33/Chat_Bar](https://github.com/yuhansun33/Chat_Bar)

# 7. Others

```
1. The nature of this final project is 
    Game/Card game/Communication/Social
2. The way this project proceeds is 
    Real-time 
3. This project allows the number of users participating in one game/round is 
    Unlimited (2 to multiple people)
4. This project supports the maximum number of simultaneous games/rounds/chat rooms 
    4 and above
5. During the game/application process, are users allowed to exit? (This command/option is supported) 
    Yes 
6. If the answer to 5 is yes, how does the game/application proceed after the user exits? 
    This round/game ends normally, and client/server programs continue to work normally 
7. If the answer to 6 is as above, will other users receive a message like "this user has exited"? 
    Will receive immediately 
8. During the game/application process, if a user abnormally disconnects/goes offline/ends the program, what will happen? 
    This round/game ends normally, and server/other client programs continue to execute normally, but other users will not receive any message 
    Same, but other users will receive related messages. Remaining users continue this round/game, but will not receive any related messages. Remaining users receive related messages and continue this round/game.
    Server/other client programs crash/abnormally end. Others
9. Does this project allow participants to use custom id/nickname? 
    Allowed, and password verification required
10. If this project proceeds in turn-based mode, is there time calculation/limitation for user actions?
    Has timing but no limitation on action time. No timing
11. If this project proceeds in turn-based mode, are users allowed to send special commands to the Server when it's not their turn to act, such as exit game or admit defeat? 
    Allowed. Not allowed (client program blocks). Not allowed (server program blocks). Others
12. Are ANSI escape sequences or other methods used for advanced control of screen output? 
    Yes
13. When a game/round of this project ends 
    Client program also ends, server program continues to execute and can handle new clients
14. What technologies/system calls does this project use? (Multiple choice) 
    TCP, select, fork, non-blocking accept/connect
15. After a game/round/match of this project starts/opens, are new users allowed to join already opened/established games/rounds/matches? 
    Allowed
```

---

**Developers:** Sun Yu-Han (孫于涵) & Hsu Heng-Jui (許恒睿)

**Year:** 2024
