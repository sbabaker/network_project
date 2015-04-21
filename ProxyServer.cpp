/*
 * File name  : Proxyserver.cpp
 * Date		  : April 17, 2015
 * Author	  : Saeed Babaker, Joham Matos, Matthew Sherwood
 * Professor  : Dr. Enkh-Amgalan Baatarjav
 * Class	  : CSCE 3530
 * Semester	  : Spring 2015
 * Description: Creates a program that acts as a proxy server. Works for any
 *              device that knows the IP address of the computer this program
 *              is executed on.
 */
#include "ProxyServer.h"
//----
#define BUF_SIZE	1024
#define LISTEN_PORT	8888
#define CONNECT_PORT 80
#define PROXY 0
#define CLIENT 1
//----
void ProxyHandle(char**);
void ReadCacheFile(fstream&, vector<string>*);
void InitStructs(int, struct sockaddr_in*, char***);
string PrintInfo(char[][1024] , int);
void GetServerIP(string&, sockaddr_in*);
extern void Filter(char[][1024]);
extern bool Filter(string);


int
main(int argc, char** argv) {
    int i = 0;
    int count = 0;
    char *server_ip = new char(17);


    memset(server_ip, '\0', 17);
    if(argc < 2) {
        cout << "Usage : \"./ProxyServer server_ip\"" << endl;
        return 1;
    }
    else {
        memcpy(server_ip, argv[1], strlen(argv[1]));
        ProxyHandle(&server_ip);

		delete server_ip;
    }

    return 0;
}


void
ProxyHandle(char **server_ip) {
    struct sockaddr_in addr_proxy;
    struct sockaddr_in addr_client;
    struct sockaddr_in addr_server;
    socklen_t address_size = sizeof(struct sockaddr_in);
    int proxy_socket = 0;
    int client_socket = 0;
    int server_socket = 0;
    int message_size = 1;
    int string_split = 0;
    int iterator = 0;
    char buffer[BUF_SIZE] = {'\0'};
    bool file_exist = false;
    string file_name = "";
    string site_request = "";
    string temp_local_file = "temp";
    string host_name = "";
    fstream file_stream;
    fstream local_file_stream;
    vector<string> output_data;
    struct IOException ioError;


    InitStructs(PROXY, &addr_proxy, &server_ip);                                     //----Set up socket address structures
    InitStructs(CLIENT, &addr_client, &server_ip);
    InitStructs(-1, &addr_server, &server_ip);

    proxy_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                        //----Start up proxy server socket
    bind(proxy_socket, (struct sockaddr *)&addr_proxy, sizeof(addr_proxy));
    listen(proxy_socket, 5);

    while(1) {
        cout << "Ready to serve..." << endl;

    	client_socket = accept(proxy_socket, (struct sockaddr *)&addr_client, &address_size);       //----Get request from client
        cout << "Received a connection from: " << client_socket << endl;

        recv(client_socket, buffer, BUF_SIZE, 0);                                                   //----Print client request
        for(; buffer[iterator] != '\0'; iterator++) {
        }

        file_name = PrintInfo(&buffer, iterator);
        if(Filter(file_name)) {                                                          //----Check for blocked content
            file_name = "blocked_site.html";
        }

        try {
            file_stream.open(file_name, ios::in);                                         //----Try to grab file from cache
            if(file_stream.fail()) {
                throw ioError;
            }

            ReadCacheFile(file_stream, &output_data);                                               //----Read file in memory
            file_exist = true;

            send(client_socket, "HTTP/1.0 200 OK\r\n", 17, MSG_CONFIRM);                            //----Return file to client
            send(client_socket, "Content-Type:text/html\r\n", 24, MSG_CONFIRM);
            for(; iterator < output_data.size(); iterator++) {
                send(client_socket, output_data[iterator].c_str(), output_data[iterator].size(), MSG_CONFIRM);
            }
            cout << "Read from cache" << endl;
        }
        catch(const exception& e) {
            string_split = 0;
            site_request = "GET http://" + file_name + " HTTP/1.0\n\n";                             //----boilerplate
            string_split = file_name.find("www.");
            host_name = file_name.replace(string_split, 4, "");
            cout << host_name << endl;
            file_name = "./" + file_name;

            server_socket = socket(AF_INET, SOCK_STREAM, 0);                                        //----Retrieve site

            try {
                GetServerIP(host_name, &addr_server);
                connect(server_socket, (struct sockaddr *)&addr_server, sizeof(addr_server));
                send(server_socket, site_request.c_str(), site_request.size(), 0);

                file_name = file_name.substr(2, file_name.size() - 2);
                local_file_stream.open(file_name, ios::out);
                while(message_size > 0) {
                    memset(buffer, '\0', BUF_SIZE);
                    message_size = recv(server_socket, buffer, BUF_SIZE, 0);

                    Filter(&buffer);                                                                    //----Filter bad words

                    local_file_stream.write(buffer, message_size);                                      //----Cache site
                    send(client_socket, buffer, message_size, 0);                                       //----Send site to client
                }
                message_size = 1;
            }
            catch(...) {
                cout << "DEBUG: Server doesn't like our requests" << endl;
                cout << "Illegal request" << endl;
            }

            close(server_socket);
        }
        catch(...) {
            if(!file_exist) {
                cout << "DEBUG: Something wrong in ReadCacheFile" << endl;
            }
            else {
                send(client_socket, "HTTP/1.0 404 sendErrorErrorError\r\n", 34, MSG_CONFIRM);
                send(client_socket, "Content-Type:text/html\r\n", 24, MSG_CONFIRM);
                send(client_socket, "\r\n", 2, MSG_CONFIRM);
            }
        }

        close(client_socket);
    }

    close(proxy_socket);

    return;
}


void
ReadCacheFile(fstream& file_stream, vector<string> *output_data) {
    char buffer[1024] = {'\0'};


    file_stream.getline(buffer, 50000);
    while(!file_stream.eof()) {
        (*output_data).push_back(buffer);
        file_stream.getline(buffer, 50000);
    }

    return;
}


void
InitStructs(int host, struct sockaddr_in *addr, char ***server_ip) {
    
    memset(addr, 0, sizeof(*addr));
    (*addr).sin_family = AF_INET;
    if(host == PROXY) {
        (*addr).sin_addr.s_addr = htonl(INADDR_ANY);
        (*addr).sin_port = htons((unsigned short)LISTEN_PORT);
    }
    else if(host == CLIENT) {
        (*addr).sin_addr.s_addr = inet_addr(**server_ip);
        (*addr).sin_port = htons((unsigned short)LISTEN_PORT);
    }
    else {
        (*addr).sin_port = htons((unsigned short)CONNECT_PORT);
    }

    return;
}


string
PrintInfo(char buffer[][1024], int iterator) {
    string buffer_print;
    string file_name;
    int string_split;


    buffer_print.assign(*buffer, iterator);
    cout << buffer_print << endl;

    string_split = buffer_print.find_first_of(' ');
    string_split = buffer_print.find_first_of(' ', string_split + 1);
    file_name = buffer_print.substr(buffer_print.find_first_of(' ') + 1, string_split - buffer_print.find_first_of(' ') - 1);
    cout << file_name << endl;

    string_split = file_name.find_first_of("/") + 1;
    file_name = file_name.substr(string_split, file_name.length() - 1);
    cout << file_name << endl;

    cout << "/" << file_name << endl;

    return file_name;
}


void
GetServerIP(string& host_name, sockaddr_in *server) {
    struct hostent *he;
    struct in_addr **addr_list;
    

    if((he = gethostbyname(host_name.c_str())) == NULL) {
        herror("gethostbyname");
        cout<<"Failed to resolve hostname\n";
        return;
    }

    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list = (struct in_addr **)he->h_addr_list;
    (*server).sin_addr = *addr_list[0];

    return;
}
