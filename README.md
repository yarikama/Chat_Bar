# Network Programming Final Project Report

**Language / 語言選擇:**
- **[English](#english-version)** (Default)
- **[繁體中文](#中文版本)**

---

<a name="english-version"></a>
# English Version

> ***Recommended to view the web-friendly version: [https://highfalutin-biplane-60c.notion.site/Network-Programming-Final-Project-Report-23f8b6780f8344d78265ce883fab6135?pvs=4](https://www.notion.so/Network-Programming-Final-Project-Report-23f8b6780f8344d78265ce883fab6135?pvs=21)***

# 0. Introduction

## 0.0 Game Overview

This Network Programming final project features a **MUD real-time map game combined with communication and chat functionality**. It aims to create an experience where users can freely move around a map, meet new friends, invite others to chat, and view real-time personal and best scores. Developed and designed by **Sun Yu-Han and Hsu Heng-Jui**.

## 0.1 Tools Overview

Languages: C++, MySQL

APIs: OpenSSH, JSON, SQL, SFML, GDB, Valgrind

Support Tools: ChatGPT, Github, VSCode, Copilot

Graphics: DALL-E

Music: [Medieval Royalty Free Music](https://www.youtube.com/watch?v=SyzE2mZZvks)

Fonts: [Breathe Fire](https://www.fontspace.com/breathe-fire-iii-font-f69367)

VPN: Hamachi

(Due to router restrictions in our rental housing, we used a free but unstable VPN to set up the SERVER)

## 0.2 Division of Labor

All project content was designed, completed, and adapted through discussions between the two team members. While there are no completely independent parts done by a single person, the work can be roughly divided as: **Server - Sun Yu-Han, Client - Hsu Heng-Jui**

# 1. Development, Execution Environment, Special Requirements, Adaptations

## 1.0 Prerequisites

This section mainly covers the game development overview, and if you want to compile or use our Github repository, here's the guide on how to implement it, especially regarding special requirements. Here's our file structure and corresponding implementation content.

```verilog
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

```markdown
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

```jsx
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

```jsx
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

In the Code, we use `Class Character` to handle initialization related to Character movement, initialization (such as graphics, names, etc.), character position, and display functions. `Class OtherCharacter` handles other **single players**, inheriting Character functions and adding distance to the user's character and update function parts. Additionally, there is `Class OtherCharacter**s**` which uses unordered_map structure to store **all online player characters**, and provides functions to update all people's positions at once, add other characters, delete other characters, and record the nearest character.

`Class ChatRoomIcon` handles chat room-related parts, with data parts including coordinates, images, etc., and functions including display and initialization. `Class ChatRoomIcon**s**` records data structures of different chat rooms, also with functions to update minimum position.

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

```jsx
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

```jsx
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

1. [https://youtube.com/playlist?list=PLvv0ScY6vfd95GMoMe2zc4ZgGxWYj3vua&si=NgLTCKtw6o9yv19p](https://youtube.com/playlist?list=PLvv0ScY6vfd95GMoMe2zc4ZgGxWYj3vua&si=NgLTCKtw6o9yv19p)
2. [https://youtube.com/playlist?list=PL21OsoBLPpMOO6zyVlxZ4S4hwkY_SLRW9&si=82lu6UkTRmb0fHot](https://youtube.com/playlist?list=PL21OsoBLPpMOO6zyVlxZ4S4hwkY_SLRW9&si=82lu6UkTRmb0fHot)
3. [https://www.sfml-dev.org/tutorials/2.6/](https://www.sfml-dev.org/tutorials/2.6/)
4. [https://www.youtube.com/@CodeAesthetic](https://www.youtube.com/@CodeAesthetic)
5. [https://man7.org/tlpi/code/online/dist/sockets/read_line.c.html](https://man7.org/tlpi/code/online/dist/sockets/read_line.c.html)
6. ChatGPT

# 6. Our Source Code in Github Repository

[https://github.com/yuhansun33/Chat_Bar](https://github.com/yuhansun33/Chat_Bar)

# 7. Others

```markdown
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
9. Does this project allow participants to use custom id/nickname? Allowed, and password verification required
10. If this project proceeds in turn-based mode, is there time calculation/limitation for user actions?
    Has timing but no limitation on action time. No timing
11. If this project proceeds in turn-based mode, are users allowed to send special commands to the Server when it's not their turn to act, such as exit game or admit defeat? Allowed. Not allowed (client program blocks). Not allowed (server program blocks). Others
12. Are ANSI escape sequences or **other methods** used for advanced control of screen output? Yes
13. When a game/round of this project ends, the client program also ends, the server program continues to execute and can handle new clients
14. What technologies/system calls does this project use? (Multiple choice) TCP select fork
    non-blocking accept/connect
15. After a game/round/match of this project starts/opens, are new users allowed to join already opened/established games/rounds/matches? Allowed
```

---

<a name="中文版本"></a>
# 中文版本

> ***建議以網頁好讀版來看：[https://highfalutin-biplane-60c.notion.site/Network-Programming-Final-Project-Report-23f8b6780f8344d78265ce883fab6135?pvs=4](https://www.notion.so/Network-Programming-Final-Project-Report-23f8b6780f8344d78265ce883fab6135?pvs=21)***
> 

# 0. 簡介

## 0.0 遊戲簡介

這次NP期末專題的主題為使用**Mud即時性地圖遊戲混合通訊聊天**的方式，旨為使用者創造一個可在地圖上自由移動，遇上其他新朋友，邀請他人進行聊天通訊，並且可看到即時個人與最佳成績的遊戲聊天程式。—由**孫于涵、許恒睿**兩人共同開發與設計。

## 0.1 工具簡介

語言：C++, MySQL

API ：OpenSSH, JSON, SQL, SFML, GDB, Valgrind

輔助：ChatGPT, Github, VSCode, Copilot

圖片：DALL-E 

音樂：[中古世紀免費音樂](https://www.youtube.com/watch?v=SyzE2mZZvks)

字體：[Breathe Fire](https://www.fontspace.com/breathe-fire-iii-font-f69367)

VPN：Hamachi

（因租屋處的路由器不能設定，使用免費但不穩的 VPN 架設 SERVER）

## 0.2 分工簡介

這裡所有專案內容都是在兩人討論下一步步設計、完成和調適，沒有完全獨立的部份是由單個人來做的，但大致上可以看作 ⇒ **Server - 孫于涵 、Client - 許恒睿**

# 1. 開發、執行環境、特殊需求、調適

## 1.0 須知

這裡主要是講遊戲開發的概述，以及如果要使用我們的 Github 進行編譯或是使用的話，該如何實現的指南，尤其是特殊需求這一塊很重要。這裡提供我們的檔名以及其對應的實現內容。

```verilog
├── Assets                       （字體、圖片、音樂檔案）
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
├── cleanSQL.sql                （刪除 Database 的腳本）
├── clientMain.cpp              （Client 遊戲模式)
├── clientTCP.cpp               （Client 處理 Packet 傳遞）
├── clientTCP.h             
├── elementTCP.cpp              （Packet 實現）
├── elementTCP.h            
├── header.h                    （include 所有必要函式庫、Define 變數）
├── initSQL.sql                 （建立 Database 的腳本）
├── login.cpp                   （Client 登入與註冊）
├── makefile                    （Compile）
├── readline.cpp                 (Readline 實做)
├── readline.h          
├── README.md                   （Preinstall information & specification)
├── serverGame.cpp              （Server 處理遊戲模式）
├── serverLOGIN.cpp              (Server 處理登入模式)
├── serverTCP.cpp               （Server 處理 Packet 傳遞）
└── serverTCP.h
```

在我們的設計中，我們盡量以物件導向的方式來實做這次的專案。

## 1.1 開發

本程式基於 C++ 開發，而遊戲 GUI 則是使用 SFML 進行存取圖片、字體、音樂（這些資源非此 Library 提供）等配置，而網路連線雖然 SFML 有提供 API 可供遊戲連線操作但我們認為本次作業關於網路部份應該由自己動手做，因此使用的都是上課所學和 Unix Socket Programming 這本書的內容。

而本遊戲基於 TCP 連線，在 login, register 時採用 Blocking I/O 處理，同時也用到 fork() 進行開啟遊戲模式的中介，另外進入到遊戲時則是採用 Nonblocking I/O 以處理即時性的玩家動作。

此外，我們在 Server 端有使用 MySQL 建立資料庫，以紀錄並歸納玩家的資訊，其中包括帳號、密碼、遊玩時間等等，也使用 OpenSSH 中的 library 使用 SHA1 加密玩家的註冊的密碼。

而當 Client 與 Server 交換訊息的時候則是用 JSON 的格式來傳遞，其中是考量到若是使用 char 或是 string 等基本資料傳送的過程中，出現錯誤是很難去發現的，使用 JSON 的格式傳送時，若是 Parse 的部份出錯，系統會拋出 ERROR，另一方面則是可以保證資料的格式，而不會造成混淆。

對於其他的圖源，都是使用 ChatGPT 中的 DALL-E 來產生的，風格的方向大致上為 RPG、Pixel 的提字詞來產生圖片，而字體跟音樂的部份都是確認過 License 才使用的。

## 1.2 執行環境

SFML的 Library 官方文檔表示可供 **Linux, Windows, MacOS** 使用，但我們主要是以 WSL, Linux 來做為測試的平台，而 WSL 有時會顯示因為 Buffer 太小，無法處理短時間大量的資料傳遞而出現 Error Message，這方面我們已經透過 Socket Option 來嘗試加大 Socket Buffer 至 8K ，但在  WSL 偶爾還是會出現這樣的問題。

## 1.3 特殊需求 (**執行前必看** Compile, Library Installation, Database Setup)

編譯前到進入遊戲設定的時候主要有以下三格項目需要注意：
**安裝 Library、直接使用 Make 進行編譯、建立 Database、宣告自用 IP address 和 Port、開始`./serverGAME、./serverLOGIN`之後直接使用`./login` 來開始遊戲**

編譯前需要安裝這些 Library：（我們的 GitHub Repository Readme 中有提供指令安裝）

```markdown
sudo apt install nlohmann-json3-dev
sudo apt-get install libsfml-dev
sudo apt-get install libmysqlcppconn-dev
sudo apt-get install libssl-dev
sudo apt install mysql-server
```

安裝後，還須建立 System Database：（包括建立這個 Server 的使用者和 Database Frame）

```sql
mysql -u root -p
[your passwd]

SOURCE initSQL.sql
```

若是要刪除 Database

```jsx
SOURCE cleanSQL.sql
```

而遊戲中我們將 Server IP, Server Port 定義在 header.h 中，若是要使用此 Server 得先更改

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

```jsx
sudo ufw allow 11131/tcp
sudo ufw allow 11130/tcp
sudo ufw reload
```

## 1.4 調適

大部分情況下我們使用 GDB 來 Debug，少數情況直接 cout 或是用 Valgrind 來進行 Debug。 

# 2. 研究方法與設計：

## 2.0 寫作風格初探

我們將 Function 宣告在 `.h` 檔中，而實做在 `.cpp`（除了 `./clientMain` 以外都是這樣），並且大部份都以抽象化的方式建立物件，如：角色的實現就對應到 `Class Character` 在其中實現有關角色的功能和定義角色的素材。我們在保持 Code 的整潔下了不少功夫，畢竟 Code 太亂在這麼龐大的結構中，會導致後面要生產其他功能上會有很大的困難。

## 2.1 Server 與 Client 程式功能

### 2.1.1 Login 功能

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled.png)

    **遊戲畫面**

首先，使用者會利用 `./login` 向 `./serverLOGIN` 註冊，而 Client 則要處理使用者的輸入並轉成 Packet，此時 Client 會利用傳送 LOGINMODE 的Packet 給 Server 會檢查是否有註冊重複、註冊失效，抑或註冊成功等情況，並同樣使用 Packet 將結果判定回傳給 Client 來做進一步的 GUI 顯示通知使用者結果。

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%201.png)

而若註冊成功，使用者即可透過 Client 再次輸入帳號密碼來登入，成功後 Client 會透過`fork()`和`exec()`從 `./login` 轉至 `./main` 來和 Game Server 進行連線並且實現遊戲互動。在此過程之中不需要使用者輸入任何 IP 或是 address。

### 2.1.2 Game 功能

Server (`./serverGAME`)和 Client (`./main`) 內主要實現的模式為**地圖模式**與**聊天模式：**

**地圖模式** 

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%202.png)

**地圖模式遊戲畫面**

使用者可按下鍵盤`WASD`來移動，使 Client 偵測與處理，其中有以下功能須實現：

- 使用者角色座標移動
- 將自身座標傳給 Server（使用 `MAPMODE` 的 Packet）
- 螢幕中心視角鎖定於使用者
- 更新其他角色、聊天室的相對距離
- 紀錄最近的角色、聊天室
- 判斷最近的角色、聊天室是否在聊天半徑內，能夠傳遞聊天邀請，並提示使用者可發送邀請

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%203.png)

**座標更新動作示意圖**

此時 Server 則會對 Client 所寄送的 Packet 進行下列處理：

- 將從 Client 收到的 Packet Broadcast 給所有在線（地圖、聊天室）的 Clients

而其他 Client 接受到 Server 傳來的 Packet 則須完成：

- 忽略 Sender 為自己的 Packet
- 為 Packet 的 Sender 更新位置
- 為 Packet 的 Sender 更新最短距離
- 判定是否有更短距離的角色存在
- 判斷是否能夠傳遞聊天邀請給最近的角色、聊天室，並提示使用者可發送邀請

![螢幕擷取畫面 2024-01-12 152909.png](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/%25E8%259E%25A2%25E5%25B9%2595%25E6%2593%25B7%25E5%258F%2596%25E7%2595%25AB%25E9%259D%25A2_2024-01-12_152909.png)

**聊天邀請畫面**

在 Code 中我們以 `Class Character` 來處理初始化有關 Character 的移動、初始化（如圖像、名稱等等）、角色位置、以及顯示等功能。而 `Class OtherCharacter`則是處理其他**單一玩家**，它繼承了 Character 的功能，並新增對於使用者角色的距離，以及更新功能的部份。另外還有一個`Class OtherCharacter**s**`則是用 unordered_map 的結構儲存**所有在線的玩家角色**，並提供一次更新全部人位置，增加其他角色、刪除其他角色、紀錄距離最近角色的功能。

而`Class ChatRoomIcon`則是處理聊天室相關的部份，一樣資料部份有座標、圖片等，功能有顯示、初始化，`Class ChatRoomIcon**s**`紀錄不同聊天室的資料結構，同樣也有更新最小位置的功能。

因為發出邀請時，要決定最小距離的物件是誰（可能是其他玩家或是聊天室），再依據最小物件為何提示使用者使用 Ctrl + Z / Ctrl + X 

**聊天模式**

![螢幕擷取畫面 2024-01-12 153111.png](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/%25E8%259E%25A2%25E5%25B9%2595%25E6%2593%25B7%25E5%258F%2596%25E7%2595%25AB%25E9%259D%25A2_2024-01-12_153111.png)

         **聊天遊戲畫面**

會先以**邀請**（ `REQMODE` 的 Packet）寄給另一位使用者，Peer Client 會以訊息提示使用者收到邀請了，請按下Y/N來決定接受或是拒絕邀請，並讓 Server 建立聊天室以紀錄聊天內容和傳遞 Packet 的聊天室中的 Clients。

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%204.png)

**聊天邀請動作示意圖**

而邀請請求需要則是要在聊天半徑內有角色或是聊天室存在時才可發送。並且邀請會發送給距離最近的角色或是聊天室。此外，聊天室建立則以最低兩人為限，而後續加入人數並無限制。

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%205.png)

**Client & Server 互動示意圖**

當玩家進入聊天室後右上角計時器會開始計時。此時 Client 輸入訊息送給 Server 時，Server 會 broadcast 給聊天室內其他所有 Client，以達到玩家聊天功能。而玩家可以按下 Esc  離開聊天室，聊天室計時器結束，並且聊天室內其他玩家會收到有人離開的通知，此時玩家可以選擇繼續留在聊天室內聊天或按下 Esc 離開聊天室回到地圖。一旦聊天室因玩家離開或 Client 斷線導致人數為0時，聊天室則會自動被 Server 所清除。

## 2.2 資料傳輸格式

Server 與 Client 間統一以 Packet 的資料結構來初始化資料，格式內容包含 mode, sender_name, receiver_name, x, y, message，適用於各個模式中傳遞的 float 與字串資料。 Packet 的mode 分為12類，以利於 Server 與 Client 間溝通，能夠清楚地表達與判斷各個 Packet 須採取的動作。

其中傳遞時，會經過 Serialize 將 Packet 變成 JSON 檔傳給對端，而對端收到時則會在將此 JSON 檔 Deseriailze 成 Packet 進行下一步處理，如此一來可以偵測出若是 Packet 重疊所造成的問題，JSON Library 進行 Parse 的過程產生錯誤就會拋出錯誤。 

```jsx
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

格式和設計使用方法息息相關，我們將有關的格式都定義在`header.h`中（如：MODE, STATE, SPECAIL VALUE）。

## 2.3 玩家資料儲存方式

每個玩家在程式中的個人資料以Player structure來包裝，包含playerID與sockfd儲存玩家暱稱與socket number，roomID則儲存玩家所進入的聊天室號碼，mode顯示玩家當前屬於地圖或聊天模式，以及儲存玩家當前的x、y座標。

```jsx
struct Player{
	std::string playerID;
	int sockfd;
	int roomID;
  int mode_player; //MAP , CHAT
	float x_player;
	float y_player;
};
```

而玩家的帳號密碼與聊天時數則是儲存於MySQL資料庫上，並分為兩個 Tables。User Table 採用 SHA1 雜湊函數代換原密碼字串來加密，將帳號與加密密碼存入資料庫，避免惡意植入程式或後台資料外洩的不安全性。而 ChatTime Table 則儲存玩家每次的聊天時數，用於個人與最佳總計時數統計。

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%206.png)

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%207.png)

**資料庫 ER diagram**

**資料庫更新動作示意圖**

## 2.4 聊天室儲存方式

在 Server 中若是玩家們產生了聊天室則需要將他們儲存到以 RoomID 為 Index 玩家為 Element 的 2D array，而 Server 會在玩家離開（Esc or 強制關閉遊戲時），刪除玩家的 Element，而同時若是聊天室如果已經無人存在，便會清除掉此聊天室，並且給所有人發送取消聊天室的 Packet。 

## 2.5 例外狀況之分析與處理

當玩家關閉視窗、不正常斷線、離線、結束程式等例外狀況離開遊戲時，對於位於地圖模式或聊天模式有不同處理方式。地圖模式時，Server除了在unorder_map移除斷線玩家，也會broadcast給所有剩餘Client一則packet，內部包含斷線玩家名稱，並且x與y數值為NOWHERE，接著Client在內部unorder_map移除斷線玩家，此時便不會再繪製斷線玩家於地圖上了。而聊天模式時，Server會將斷線玩家移出原先所在的聊天室，並且broadcast給該聊天室的所有Client，此時聊天室內其他玩家會收到有人離線的通知。透過以上方式來處理玩家不正常斷線狀況，使得遊戲不但能繼續運行外，聊天中的玩家也能得知當下情形。

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%208.png)

**聊天例外處理畫面**

# 3. 成果：最後成果的主要功能與特色

![Untitled](Network%20Programming%20Final%20Project%20Report%2023f8b6780f8344d78265ce883fab6135/Untitled%209.png)

- **人物移動**
    - 玩家中心：視窗畫面視角以玩家為中心，隨著玩家移動，地圖視角也隨之移動。
    - 接近聊天：若移動接近其他玩家，並且兩者距離於100f以內，則可以發送或接收邀請，進入聊天室聊天
- **普通聊天**
    - 時間統計：一旦進入聊天室起，會開始計算聊天時間，直到按下 Esc 離開聊天室後，計時結束。
- **多人聊天**
    - 群發訊息：若地圖上顯示已存在的聊天室，玩家可以接近其他玩家並按下 X ，或是接近聊天室圖示並按下Z，來加入聊天室。加入聊天室後，便可以與聊天室內所有玩家傳訊息聊天。
    - 時間統計：多人聊天的時間計算方式與普通雙人聊天相同，於進入聊天室起算時間，若其他人都離開聊天室，則同樣會繼續計時直到玩家離開聊天室。
- **排行榜**
    - 聊天時數：玩家離開聊天室後，由 Client 端向 Server 傳送此次聊天時間，Server 將資料放入資料庫，接著回傳 Client 端玩家的個人累積時數以及最佳聊天時數，供 Client 端更新排行榜玩家名稱與時數。
- **資料庫**
    - 登入資訊：註冊時，玩家輸入帳號與密碼後，Client 將玩家所輸密碼以 sha1 加密字串代換後，傳送帳號與加密密碼給 Server 確認是否有重複註冊的情形，若沒有則註冊成功，可以進行登入。登入時，Client 同樣將玩家所輸密碼以 sha1 加密字串代換後，傳送給 Server 確認資料是否正確，若正確則會跳轉至 Game Server。
    - 時數清單：當玩家離開聊天室後，由 Client 端向 Server 傳送此次聊天時間，Server 將資料放入資料庫，接著回傳 Client 端玩家的個人累積時數以及最佳聊天時數，供 Client 更新排行榜內個人時數，及最佳成績的玩家名稱與時數。

# 4. 結論：

## 4.0 專題製作心得

針對這次專題製作，是我們小組兩個人第一次接觸到 Socket Programming 的專案，對我們來說是很有挑戰性、成就感的嘗試，在開發時期很常遇到問題須解決，或是對於使用者體驗可以更好的部分，我們都不斷的修改與優化，也在實作中了解到課堂上的功能與觀念如何實際的應用。而以下是一些製作期間曾遭遇的困難與解決經過：

## 4.1 遭遇困難及解決經過

1. **Packet 傳遞設計：**
不同於作業在 Client、Server 間以字串傳遞資訊，我們希望加入 float, string 資料型態，並且能在單一次傳遞的資訊中包含更多資訊，如 sender name、receiver name 等。因此採用自行設計資料結構 Packet 來包裝需要的資料型態(`./elementTCP.cpp`, `./elementTCP.h`)，而這在傳送與接收時，都需要在字串與 JSON 格式中依照傳送或是接收進行序列化或反序列化，花了不少時間嘗試與 debug。
2. **Buffer Overflow：**
在專題製作的最後，我們仍發現在 WSL 使用 Client 進行連線時，會有Kernel 反應 Buffer 被塞爆的錯誤，我們有嘗試過以 Socket Options 來解決，但似乎沒用，而使用虛擬機則不會有錯誤，這也是未來還需要再延伸研究的部分。
3. **Blocking / Non-blocking 選擇：**
由於 Login 功能在登入與註冊時，需等待玩家輸入後，Client 將資料傳送至 Server ，並於 MySQL 驗證，因此採用 Blocking I/O 處理。Game 功能由於採用 SFML ，並設定幀率為 60 Hz，在 1 秒內會跑 60 次 while 迴圈，而遊戲設計不限使用者人數，也無法確定每次迴圈內是否有 Client 的動作回傳而導致遊戲卡住，為確保遊戲的即時性，因而採用 Non-blocking 的方式。
同時，在此專案中我們才發現原來 Readline 的功能不是 C standard library 的內容，而是 Unix Socket Programming 這本書中，作者做的，因為收取資料需要以 `\n` 來判斷，所以我們有實做 Readline 出來。
4. **架設 VPN 以測試遠搬連線：**
由於租屋處的路由器不給設定，我們使用 Hamachi 建立 Group 來進行連線，要不然連線會被路由器擋掉，防火牆的 Ports 也要關閉。但缺點是免費版連線相對不穩，以及只能使用 5 台主機。
5. **Packet 接收重疊問題：**
有時會有 Packet 重疊的問題，這是要設定 Server 忽略此錯誤，要不然會整個 Crash 掉。
6. **重複角色問題：**
重複角色最容易發生在自己的角色上面，包括離開後重新登入時，Server會誤認成兩個人，這裡我們在 Client 端會對收到 Packet Sender Name 與自己 ID 重疊的 Packet 進行 2 次 Check。
7. **模式隔離問題：**
原本在我們的設計上面，地圖模式和聊天模式是隔離開的，但是依照此設計會發生幾項問題，像是聊天時，有新的玩家加入到地圖中，而此時自己離開聊天模式，就看不到該使用者，除非它進行移動，另外還有，當自己發起聊天並建立聊天室後，離開聊天不會看到聊天室的圖樣還在（裡面還有其他的聊天室），所以之後我們改善成在聊天的過程中，依舊可以處理其他 Mode 的Packet。
8. **從兩人聊天變成多人聊天：**
兩人聊天的實做非常簡單，只須 Server 讀取 Packet 的接受與傳送兩方並負責轉發消息就好了，但是多聊天室且要多人聊天就必須維護聊天室的資料結構，來對聊天室的紀錄，玩家有誰進行存取，有也必須處理聊天室的新增與移除。

## 4.2 成果未來改進或延伸方向等

這裡提供紀錄我們設計和實做到一半的功能：

**好友功能：**
我們已經設計好友功能和圖片，以及 Client 端的程式部份，但還未有足夠時間完成 Server 的維護，以及最難的部份是 Debug。我們設計聊天時，經過 60 秒之後便可以打開聊天室選單，勾選申請加好友的對象，而對象那邊則會收到邀請，回覆同意與否，同時 Server 要維護 Database 來紀錄好友關係並顯示好友清單在旁邊。

**好友聊天：**
好友聊天一樣不用透過距離，在好友清單中選取聊天就可以發出邀請，進行密聊功能（不會出現聊天室）。

**註冊選角：**

我們原本預想可以有選角色樣式的功能，並依照選的角色來創建遊戲帳號，同樣 Server 也要紀錄此帳號使用的角色是誰，但礙於產生同樣風格的圖片有一定難度，還要調正大小，以及要做更多的 GUI 因此我們將這個功能擱置在後面。

# 5. 參考文獻與附錄

1. [https://youtube.com/playlist?list=PLvv0ScY6vfd95GMoMe2zc4ZgGxWYj3vua&si=NgLTCKtw6o9yv19p](https://youtube.com/playlist?list=PLvv0ScY6vfd95GMoMe2zc4ZgGxWYj3vua&si=NgLTCKtw6o9yv19p)
2. [https://youtube.com/playlist?list=PL21OsoBLPpMOO6zyVlxZ4S4hwkY_SLRW9&si=82lu6UkTRmb0fHot](https://youtube.com/playlist?list=PL21OsoBLPpMOO6zyVlxZ4S4hwkY_SLRW9&si=82lu6UkTRmb0fHot)
3. [https://www.sfml-dev.org/tutorials/2.6/](https://www.sfml-dev.org/tutorials/2.6/)
4. [https://www.youtube.com/@CodeAesthetic](https://www.youtube.com/@CodeAesthetic)
5. [https://man7.org/tlpi/code/online/dist/sockets/read_line.c.html](https://man7.org/tlpi/code/online/dist/sockets/read_line.c.html)
6. ChatGPT

# 6. Our Souce Code in Github Repository

[https://github.com/yuhansun33/Chat_Bar](https://github.com/yuhansun33/Chat_Bar)

[https://github.com/yuhansun33/Chat_Bar](https://github.com/yuhansun33/Chat_Bar)

# 7. Others

```markdown
1. 此期末專題的性質為 
    遊戲/牌戲類/通訊/社交
2. 此專題程式的進行方式為 
    即時制 
3. 此專題程式允許一局/牌/回合參與的使用者數目為 
    不限(2 至多人)
4. 此專題程式支援最多同時進行的局/盤/聊天室數目為 
    4 及以上
5. 遊戲/應用進行過程中，是否允許使用者退出? (有支援此命令/選擇) 
    有 
6. 如 5 的答案為有，則使用者退出後，遊戲/應用後續如何進行? 
    此回合/盤/局之遊戲/應用正常結束，且 client/server 程式繼續正常工作 
7. 如 6 的答案為或，則其它使用者是否會收到“此使用者已退出”或類似的訊息? 
    會立即收到 
8. 遊戲/應用進行過程中，若某個使用者不正常斷線/離線/結束程式，則會發生何種結果? 
    此回合/盤/局之遊戲/應用正常結束，且 server/其它 client 程式繼續正常執行，但其它使用者不會收到任何訊息 
    同，但其它使用者會收到相關訊息 剩餘使用者繼續進行此回合/盤/局之遊戲/應用，但不會收到任何
相關訊息剩餘使用者收到相關訊息後繼續進行此回合/盤/局之遊戲/應用
server/其它 client 程式當機/不正常結束 其它
9. 此專題程式是否允許參與者使用自訂 id/暱稱? 允許，且須密碼驗證
10. 若此專題程式的進行方式為回合制，是否有計算/限制使用者進行動作的時間?
有計時但無限制動作時間 沒有計時
11. 若此專題程式的進行方式為回合制，是否允許使用者在尚未輪到他進行動作
時，對 Server 送出特別的指令，例如退出遊戲或認輸等? 允許 不允許
(client 程式阻擋) 不允許(server 程式阻擋) 其它
12. 是否使用 ANSI escape sequence 或**其它方式**對螢幕輸出進行進階的控制? 有
13. 當此專題程式的一局/盤結束後 client 程式也隨之結束，server 程式繼續執行且可處理新的 client
14. 此專題程式使用到何種技術/system call? (多選) TCP select fork
non-blocking accept/connect
15. 當此專題程式的一局/盤/配對開始/開設後，是否允許新的使用者加入已開啟/
開設的一局/盤/配對? 允許
```