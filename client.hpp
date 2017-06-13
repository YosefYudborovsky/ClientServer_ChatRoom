//
//  client.hpp
//  Lab3
//
//  Created by Yosef Yudborovsky on 5/15/17.
//  Copyright © 2017 Yosef Yudborovsky. All rights reserved.
//

#ifndef client_hpp
#define client_hpp

//#include "server.cpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
class client_type
{

public:
    int socketFD;
    int id;
    char received_message[1024];
    string clientName;
};
/*
class Client
{
    
    //auto constructor
    //auto destructor
   
    
public:
    //bool verifyUser();
    //int createChat();
    int clientID;
    bool isActive;
    int startClient();
    //bool setActive();
    
    
};

*/



#endif /* client_hpp */
