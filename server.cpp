//
//  server.cpp
//  Lab3
//
//  Created by Yosef Yudborovsky on 5/15/17.
//  Copyright © 2017 Yosef Yudborovsky. All rights reserved.
//


#ifndef server_cpp
#define server_cpp

#include "server.hpp"
#include "client.hpp"
//#include "client.cpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <thread>

using namespace std;


bool Server::verifyUser()
{
    bool permited = 0;
    // check records for provided userID and password
    
    string userID, password;
    
    cout << "Hello, To Login Please provide Your UserID and Password \n";
    cout << "UserID: \n";
    cin >> userID;
    cout << "Password: \n";
    cin >> password;

    string line, nextLine = "";
    ifstream file;
    //ifstream file("loginDetails.txt", ios::in);
    file.open ("loginDetails.txt");
    //file.open();
    while(file.good())
    {
        getline(file,line);
        cout << "in file: " << line <<endl;

        //lineList.push(line);
    }
    
    
    if (file.is_open())
    {
        
        //while (!file.eof()) // not end of file
        //{
        
        cout << "in file: " << nextLine <<endl;
        
        while (getline(file,nextLine)) // read line by line
        {
            stringstream iss(nextLine); // get the next line and convert to stingStream
            cout << "line: " << nextLine <<endl;
            
            string first,second;
            //iss >> read next word
            
            /*while(iss >> first)
             {
             
             }
             */
        }
        
        //}// End while.
        file.close();

    }
    else cout << "Error opening file \n";
 
    return permited;
}

//int Server::createChat()
//{

int main()
{
    int socketFD, newSocketFD; // to hold the returned value from system calls -> change to clienFD serverFD
    int portNum = 13607; // 1 and last 4 ID digits
    bool exitChat = false; // to exit the listenig loop
    int bufsize = 1024; // to hold actual masseges
    char buffer[bufsize]; // to hold actual masseges
    int MAXCLIENTS = 3; // max for clients
    string ipaddress = "127.0.0.1", chatLine; // ip and each line of chat
    long numOfChar = 0; // num of characters in read and send massages
    //vector <Client> client(MAXCLIENTS); // to hold each client
    //thread newThread[MAXCLIENTS];
    
    
    /// Change name server_addr/// client
    
    struct sockaddr_in server_addr; // A socket struct which is defined in netinet/in.h and contains the address of the server -> change anme
    socklen_t size;  //
 
    // create a new socket with AF_INET = address domain of the socket, SOCK_STREAM = Type of socket - TCP and a protocol argument - 0. Returns an integer and is used for all references to this socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socketFD < 0) // the socket could not be constructed and client value is < 0
    {
        cout << "Could not create the socket " << endl;
        return 1;  //exit(1);
    }

    else cout << "Socket has been established" << endl;
  
    // define the details of the socket struct - server_addr.
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY); // IP address for the host machine server_addr.sin_addr.s_addr = inet_addr(ipaddress.c_str());

    server_addr.sin_port = htons(portNum); //htnos converts the port number from host byte order to a port number in network byte order.
    
    /*
     binds a socket to an address of the current host and port number
     on which the server will run. It takes three arguments, the socket file descriptor, a pointer to structure of type sockaddr, which, must be cast to the correct type
     */
    if ((::bind(socketFD, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
    {
        cout << "Error: the socket has already been established " << endl;
        return 0;
    }
    //else cout << "bind is good" <<endl;
    

    // listen for connectionsup for up to 3 clients
    listen(socketFD, MAXCLIENTS);
    int clientCount = 1;

    size = sizeof(server_addr);
 


    // wake up the process when a connection from a client has been successfully established. It returns a new file descriptor.
    // The other two arguments are reference pointer to the address of the client and the size of this structure.
    newSocketFD = accept(socketFD,(struct sockaddr *)&server_addr,&size);
    
    // At this point, any new connection has to be verifyied with the credentails store in loginDetails.txt

    // Each new connection receives eachown FD variable and handled seperatly.
    // For communications b/w clients, we usesend and recive acording to send requirements

    
    if (newSocketFD < 0) // check if connection is valid
        cout << "Error on accept: Connection faild to established " << endl;
    //else cout << "accept is good. ServerFD value is:" << newSocketFD << endl;

    bzero(buffer,bufsize); // set buffer to zero
    strcpy(buffer, "Server connected and waiting: \n"); //make a string mesaage into the buffer to be sent first
    send(newSocketFD, buffer, bufsize, 0); // send the first inviting mesage as a massage


    while (newSocketFD > 0) //FD value is good
    {
        //cout << "Waiting "<< endl;
        //strcpy(buffer, "Server connected and waiting: \n"); //make a string mesaage into the buffer to be sent first
        
        // send first message from server to client
        //cout << "message 1 sent s->c "<< endl;

        //cout << clientCount << "is connected "<< endl; // clients in the room
        //cout << clientCount << ": "<< endl; // clients turn
        
        //recive message
        bzero(buffer,bufsize); // set buffer to zero
        numOfChar = read(newSocketFD,buffer,bufsize); // get the number of byts received
        if (numOfChar < 0)
        {

            cout <<"Empty Connection"<< endl;
            newSocketFD = 0;
        }
        else
        {
            cout <<"\n" << clientCount << ": " << buffer << endl; // use consule to asses status

            send(newSocketFD, buffer, bufsize, 0); // echo it back to the connection

        }

          
        //getline(cin, chatLine);
        
        
    } // end of while
    cout << "Chat is Over " << endl;
    close(newSocketFD);
    
    cout << "\nGoodbye..." << endl;
    exitChat = false;
    
    close(socketFD);

    return 0;
}
    
//}




#endif /* server_cpp */







/*void Server::startServer()
 {
 cout << "ChatRoom Server Is Running \n";
 
 string userID, password;
 bool permition = 0;
 
 cout << "To Login Please provide Your UserID and Password \n";
 cout << "UserID: \n";
 cin >> userID;
 cout << "Password: \n";
 cin >> password;
 
 permition = mainServer.verifyUser(userID, password);
 if (permition == 0) cout <<"Permission is denied \n";
 else // Aproved User - crate sockets on server end end
 {
 
 }
 }
 */
