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
#include <algorithm>
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
#ifndef PROXY_H
#define PROXY_H

struct IOException : exception {
    const char* what() const noexcept {
        return "io_error";
    }
};

#endif
