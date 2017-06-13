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
#include <thread>


using namespace std;

const int MAXCLIENTS = 3;
const int BUFFSIZE = 1024; // to hold actual masseges

int handleNewClient(client_type &new_client); // For each client

int main()
{
     int clientFD; // file descriptor to store the values returned by the socket system call and the accept system call.

    int portNum = 13607; // port number is same for all clients and the server
    bool exitChat = false, connected=false;
    int BUFFSIZE = 1024;
    char buffer[BUFFSIZE];
    string ip = "127.0.0.1"; // loopback ip and each line of chat
    int clientNum = 1;
    string sent_message = "";
    client_type client = { 0, -1, "","" }; // initilazing data
    int connectionFD = 0; //FD to hold the result of a connection
    string message= "";

    // All basic client settings follow the logic of server socket settings
    struct sockaddr_in server_addr;
    client.socketFD = socket(AF_INET, SOCK_STREAM, 0); // new socket for th client
    cout << "A socket is set for the client" << endl;
    
    if (client.socketFD < 0)
    {
        cout << "Error while establishing client's socket" << endl;
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr); // Send to server's Ip

    //client establishes connection to the server. The arguments are the socket file descriptor (FD), the address of the host (server) and the size of this address.
    //The function returns 0 on success and -1 if it fails.
    while (connected==false)
    {   
        // successfull connect() returns 0;
        connectionFD = connect(client.socketFD,(struct sockaddr *)&server_addr, sizeof(server_addr)); // connect and asign the result
        if (connectionFD <0) // Error in connection
        {
            cout << "Error in connection" <<endl;
            close (client.socketFD); // no connection yet, close and try in the next loop
            continue;
        }
        
      cout << "Connecting to the server at port number: " << portNum << endl;
        connected=true;
 
    }
    if (client.socketFD < 0)
    {
        cout << "Connection to Server Has Faild" << endl;
        return 1;
    }
    
    // if we didnt return we assume connection
    else cout << "Successfull Connection" << endl; //string input; // to hold received messages

    //Obtain id from server for this client;
    recv(client.socketFD, client.received_message, BUFFSIZE, 0);
    message = client.received_message;


    if (message != "Server is full")
    {
        client.id = atoi(client.received_message);

        thread my_thread(handleNewClient, ref(client));

        while (1) // keep sending option 
        {
            
            cout << "> ";
            getline(cin, sent_message);
            connectionFD = send(client.socketFD, sent_message.c_str(), strlen(sent_message.c_str()), 0);
 
            if (connectionFD < 0)
            {
                cout << "Error at send function " << endl;
                break;
            }
        }
 
         my_thread.join();
    }
    else
        cout << client.received_message << endl; // Show the message
 
    cout << "Socket is terminated" << endl;
    
    if (connectionFD == 0) 
    {
        cout << "Error at shutdown " << endl;
        close(client.socketFD);
        return 1;
    }
    close(client.socketFD);
    return 0;
}

int handleNewClient(client_type &new_client)
{
    while (1)
    {
        memset(new_client.received_message, 0, BUFFSIZE);
 
        if (new_client.socketFD != 0) // connection is alive
        {
            int connectionFD = recv(new_client.socketFD, new_client.received_message, BUFFSIZE, 0);
 
            if (connectionFD != 0)
            {
                cout << new_client.received_message << endl; // show the received message
                
            }
               
            else // no connection
            {
                cout << "Connection is  Terminated " << endl;
                return 0;
            }
        }
    }
 
    return 0;
}

#endif /* server_cpp */

