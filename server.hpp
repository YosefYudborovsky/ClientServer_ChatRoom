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


class Server
{
    
    //auto constructor
    //auto destructor

    
public:
    bool verifyUser();
    int createChat();
    
    void startServer(Server &mainServer);
    
    
};






#endif /* server_hpp */
