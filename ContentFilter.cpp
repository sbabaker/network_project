/*
 * File name  : Filter.cpp
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


void
Filter(char buffer[][1024]) {
    string line;
    ifstream inapLangFile;
    char *buffer_ptr = NULL;


    inapLangFile.open("inaplang.txt");                          //----gets and opens the bad language file to read it.
    if(inapLangFile.is_open()) {

        while(getline(inapLangFile, line)) {
            line.erase(line.length() - 1);
            buffer_ptr = buffer[0];
            while(buffer_ptr) {
                buffer_ptr = strstr(buffer_ptr, line.c_str());
                if(buffer) {
                    memset(buffer_ptr, '*', line.size());
                }
				transform(line.begin(), line.end(), line.begin(), ::tolower);         //----changes the string in line to all lowercase
				buffer_ptr = strstr(buffer_ptr, line.c_str());
				if (buffer) {
					memset(buffer_ptr, '*', line.size());
				}
            }
        }

    }
    else {
        cout << "Warning! Inappropriate language file missing, corrupted, or being used." << endl;
    }
    inapLangFile.close();


    return;
}


bool
Filter(string url_request) {
    ifstream blackListFile;
    string line;


    blackListFile.open("blackList.txt");                                    //----gets and opens to blacklist of websites.
    if(blackListFile.is_open()) {
        while(getline(blackListFile, line)) {
            line.erase(line.length() - 1);
            if(url_request.compare(line) == 0) {
                blackListFile.close();
                return true;
            }
        }
    }
    else {
        cout << "Warning! Blacklist file missing, corrupted, or being used." << endl;
    }
    blackListFile.close();

    return false;
}
