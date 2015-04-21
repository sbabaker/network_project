/*
 * File name  : Proxyserver.h
 * Date		  : April 17, 2015
 * Author	  : Saeed Babaker, Joham Matos, Matthew Sherwood
 * Professor  : Dr. Enkh-Amgalan Baatarjav
 * Class	  : CSCE 3530
 * Semester	  : Spring 2015
 * Description: Creates a program that acts as a proxy server. Works for any
 *              device that knows the IP address of the computer this program
 *              is executed on.
 */
#include <algorithms>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <thread>
#include <string>
#include <cstring>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
//----
using namespace std;
//----
enum message_type {
    BOOT_ERR,
    MENU_MSG,
    NEW_CONNECT,
    QUIT_MSG,
};
string interface_messages[] = {
    "Usage : \"./ProxyServer server_ip\"",
    "Ready to serve...",
    "Received a connection from: ",
    "Exiting program. Goodbye!\n",
};
struct IOException : exception {
    const char* what() const noexcept {
        return "io_error";
    }
};
