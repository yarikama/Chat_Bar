#ifndef serverTCP_h
#define serverTCP_h
#include "header.h"
#include "elementTCP.h"
class sqlServer;
class serverTCP{
<<<<<<< HEAD
=======
    friend class sqlServer;
//share
>>>>>>> 2826b63ab8e52e0872f54197ac06026d1e0539a4
public:
//share
    serverTCP();
    static void sig_chld(int signo);
    void TCP_connect(int port);
    bool accept_client();
//login server
    void login_mainloop();
    void login_handle(sqlServer& sqlserver);
//game server
    void game_mainloop();
    void new_game_handle();
    void game_handle();
    void sendData(Packet packet, int sockfd);
    void broadcast_xy(Packet packet, int sockfd);
    Packet receiveData_login(int sockfd);
    Packet receiveData_game(int sockfd);
    Packet deserialize(std::string& json_string);
    std::string serialize(Packet packet);
    std::string get_player_name(int sockfd);
    
private:
	int			        n, listenfd, connfd, sockfd;
	socklen_t		    clilen;
	struct sockaddr_in	cliaddr, servaddr;
    int                 maxfd;
    fd_set              rset, allset;

    std::unordered_map<std::string, struct Player> players;
    std::unordered_map<int, std::string> disconnect_list; //name, sockfd
};

class sqlServer{
    friend class serverTCP;
public:
    sqlServer();
    ~sqlServer();
    int db_register();
    bool login_check();
    bool register_check();
    void db_information(std::string new_user_name, std::string new_user_password);
    void db_query();
    void db_clear();
    void db_pswd_select();
    void db_user_select();
    void db_user_insert();
private:
    int affectedRows;
    sql::ResultSet *res;
    std::string user_name;
    std::string user_password;
    sql::Connection *con;
    sql::mysql::MySQL_Driver *driver;
    sql::PreparedStatement *prep_stmt;
};
#endif