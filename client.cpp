//
//  client.cpp
//  Lab3
//
//  Created by Yosef Yudborovsky on 5/15/17.
//  Copyright © 2017 Yosef Yudborovsky. All rights reserved.
//
#ifndef client_cpp
#define client_cpp
#include "client.hpp"

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

//int Client::startClient()
//{

// Change activity status
 bool setActive(Client &client)
 {
    if (client.isActive==0) client.isActive=1;
    else client.isActive=0;
    return client.isActive;
 }

int Client::startClient()
{
     int client; // file descriptor to store the values returned by the socket system call and the accept system call.

    int portNum = 13607; // port number is same for all clients and the server
    bool exitChat = false;
    int bufsize = 1024;
    char buffer[bufsize];
    //char* ip = "127.0.0.1";
    string ip = "127.0.0.1", chatLine; // ip and each line of chat
    int clientNum = 1;

    // All similar to server settings
    struct sockaddr_in server_addr;
    
    client = socket(AF_INET, SOCK_STREAM, 0); // new socket for th client

    cout << "A socket is set for the client" << endl;
    
    if (client < 0)
    {
        cout << "Error while establishing client's socket" << endl;
        return 0;
    }

    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr); // Send to server's Ip

    
    /* client establishes connection to the server. The arguments are the socket file descriptor (FD), the address of the host (server) and the size of this address. The function returns 0 on success and -1 if it fails.
     */
    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "Connecting to the server at port number: " << portNum << endl;
 
    while (client > 0)
    {
        
        // receive connection confirmation in the form of a massage
        recv(client, buffer, bufsize, 0);
        // ?? //
        string input; // to hold received messages
        
        do { // client's  move
            
                bzero(buffer,bufsize);

                cout << "\n>  ";

                getline(cin, input); // to hold input fromthe user
                //cin >> buffer;
                
                // send input from client to server
                if (input.size() > 0)
                {
                    send(client, input.c_str(), input.size()+1, 0); // send it to the server
                    cout << endl;
                }
           

                // receive echo from server
                long receiveResults = recv(client, buffer, bufsize, 0); // receiveResults will indicate if receiving is successful. If == 0 client was disconnected

                if (receiveResults>0)
                {
                    cout << "Server (echo): " << buffer << " \n";
                   
                }
            
            
            
                /// client exist stage - by typing 'logout'
                if(buffer[0]=='l' && buffer[1]=='o' && buffer[2]=='g' && buffer[3]=='o' && buffer[4]=='u' && buffer[5]=='t')
                {
                    exitChat = true;
                    cout << clientNum << " left the Chat Room "<< endl; // client left the room
                    client = 0;
                }
                
            }while (!exitChat); // alternativlly - as long as input.size()>0


            
    
        close(client);

    }
    
 
    return 0;
}


int main()
{
    Client client1;
    client1.startClient();



   
}
    
//}

#endif /* server_cpp */

