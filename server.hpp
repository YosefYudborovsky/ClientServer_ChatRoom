//
//  server.hpp
//  Lab3
//
//  Created by Yosef Yudborovsky on 5/15/17.
//  Copyright © 2017 Yosef Yudborovsky. All rights reserved.
//

#ifndef server_hpp
#define server_hpp
//#include "server.cpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Client
{
    public:
        Client(){};
        ~Client(){};

    int id;
    int clientSocketFD;
    bool connected;
    string nameOfClient;


};

#endif /* server_hpp */
