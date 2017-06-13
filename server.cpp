//
//  server.cpp
//
//  Created by Yosef Yudborovsky on 5/15/17.
//  Copyright © 2017 Yosef Yudborovsky. All rights reserved.
//


#ifndef server_cpp
#define server_cpp

#include "server.hpp"
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
#include <string.h>

using namespace std;


int newClient(Client &new_client, vector<Client> &client_array, thread &thread);

const int MAXCLIENTS = 3; // set in advance
const int bufsize = 1024; // to hold actual masseges

bool verifyUser(char tempmsg[bufsize], string &name); // to veirfy a user


int main()
{
    int socketFD, newSocketFD, newSocketFD1, newSocketFD2, newSocketFD3; // to hold the returned value from system calls -> change to clienFD serverFD
    int portNum = 13607; // A random port
    bool exitChat = false; // to exit the listenig loop
    char buffer[bufsize]; // to hold actual masseges
    bool chatIsOver = false;
    long numOfLogin = 0; // num of characters in read and send massages
    vector <Client> client(MAXCLIENTS); // to hold each client
    thread newThread[MAXCLIENTS];
    string msg = "";
    
    struct sockaddr_in server_addr; // A socket struct which is defined in netinet/in.h and contains the address of the server -> change anme
    socklen_t size;  //
 
    // create a new socket with AF_INET = address domain of the socket, SOCK_STREAM = Type of socket - TCP and a protocol argument - 0. Returns an integer and is used for all references to this socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socketFD < 0) // the socket could not be constructed and client value is < 0
    {
        cout << "Could not create a socket for connection" << endl;
        return 1;  
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
        cout << "Error: the socket seem to be in use (try recompiling) " << endl;
        return 0;
    }    

    // listen for connections up to 3 clients
    listen(socketFD, MAXCLIENTS);
    int num_clients = -1, temp_id;
    cout << "listening..." <<endl;

    size = sizeof(server_addr);
 
    //Initialize each of the client in the client vector
    for (int i = 0; i < MAXCLIENTS; i++)
    {
        client[i].id = -1;
        client[i].clientSocketFD = 0;
        client[i].connected=false;
    }
 
    while (1) // keep looping to accept new connection
    {
 
        int incomingFD = 0; // to indicate reult of connection
        incomingFD = accept(socketFD, NULL, NULL); // accept new connection
 
        if (incomingFD == 0) continue; // if false, skip forward
 
        //Reset the number of clients
        num_clients = -1;
 
        //Create a temporary id for the next client this will allow to identify each before recognizing as client
        temp_id = -1;
        for (int i = 0; i < MAXCLIENTS; i++) // loop through each
        {
            if (client[i].clientSocketFD == 0 && temp_id == -1) // good (new) connection
            {
                client[i].clientSocketFD = incomingFD; // assign the established connection to the new client
                client[i].id = i;
                temp_id = i;
            }
 
            if (client[i].clientSocketFD != 0)
                num_clients++; // new client
 
        }
 

        if (temp_id != -1) 
        {

            //Send the id to that client
            cout << "Client " << client[temp_id].id << ": Requiers Login" << endl;
            msg = to_string(client[temp_id].id);
            send(client[temp_id].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
 
            //Create a thread process for that client
            newThread[temp_id] = thread(newClient, ref(client[temp_id]), ref(client), ref(newThread[temp_id]));

        }
        else  
        {
            msg = "Server is full"; // - No More Avaliable Connections";
            send(incomingFD, msg.c_str(), strlen(msg.c_str()), 0);
            cout << msg << endl;
        }
    } //end while
 
    //Close listening socket
    close(socketFD);
 
    //Close client socket
    for (int i = 0; i < MAXCLIENTS; i++)
    {
        newThread[i].join();
    }
    return 0;

}
 
// the process to handle each client 
int newClient(Client &new_client, vector<Client> &client_array, thread &thread)
{
    string msg = "", clientName = "";
    char tempmsg[bufsize] = "";
    bool legit=false;
    vector <string> connectedClients; // to hold the name of all connected
    
    // First: check if a legit client
    msg = "Please provide Your UserID and Password (userID password)"; // ask for credentials
    send(new_client.clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0); // send the request
    
    // get login details
    memset(tempmsg, 0, bufsize); // set all to zero to later hold the response
    recv(new_client.clientSocketFD, tempmsg, bufsize, 0); // receive the response
    legit = verifyUser(tempmsg, clientName); // send the buffer with the credentials to verify correctness
    if (legit)
    {
        msg = "" + clientName + " Joined the Room";
        connectedClients.push_back (clientName); // store the name of the new connection
        new_client.nameOfClient = clientName;
        
        //Broadcast the new connection to the other clients
        for (int i = 0; i < MAXCLIENTS; i++)
        {
            if (client_array[i].clientSocketFD != 0)
                send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
        }
    }
  
    while (1) // keep looping to be avaliable for all received messages
    {
        // if legitimate client provide all the required functions 
        if (legit)
        {

            memset(tempmsg, 0, bufsize); // set message buffer to zero
 
            if (new_client.clientSocketFD != 0) // 
            {
                int resultedFD = recv(new_client.clientSocketFD, tempmsg, bufsize, 0); // FD to evalute a new message
                string temp; // for message manipulation
                // Logout option
                if (tempmsg[0]=='l' && tempmsg[1]=='o' && tempmsg[2]=='g' && tempmsg[3]=='o'&& tempmsg[4]=='u' && tempmsg[5]=='t')
                {
                    msg = "" + clientName + " Left the Room";
                    
                    // delete the name 
                    for (int i = 0; i < connectedClients.size() ; i++)
                    {
                        if (connectedClients.at(i) ==  clientName) connectedClients.erase(connectedClients.begin()+i);
                    }
 
                    cout << msg << endl;
 
                    close(new_client.clientSocketFD); //
                    close(client_array[new_client.id].clientSocketFD);
                    client_array[new_client.id].clientSocketFD = 0;
 
                    //Broadcast the disconnection message to the other clients
                    for (int i = 0; i < MAXCLIENTS; i++)
                    {
                        if (client_array[i].clientSocketFD != 0)
                        resultedFD = send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                    }
 
                    break;
                }

                // Who option
                if (tempmsg[0]=='w' && tempmsg[1]=='h' && tempmsg[2]=='o') // Who is connected
                {
                    cout << " At Who" << endl;
                    //respond with all that connected by loopin through the names vector
                    if (connectedClients.size()==1) msg = connectedClients.at(0); // if only one - avoid ','
                    else 
                    {
                        for (int i = 0; i < connectedClients.size() ; i++)
                        {
                            msg += connectedClients.at(i) + ", ";
                        }

                    }
                    cout << msg.c_str() << endl;
                    for (int i = 0; i < MAXCLIENTS; i++)
                    {   
                        if (client_array[i].clientSocketFD != 0)
                        resultedFD = send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                    }
                    
                    continue;
                }

                // Send All Option 
                if (resultedFD >0 && (tempmsg[0]=='s' && tempmsg[1]=='e' && tempmsg[2]=='n' && tempmsg[3]=='d' && tempmsg[4]==' ' && tempmsg[5]=='a' && tempmsg[6]=='l' && tempmsg[6]=='l'))
                {
                    if (strcmp("", tempmsg))
                    {
                        temp = tempmsg;
                        msg = temp.substr(9,temp.size()); // manipulate the meassge to avoid 'send all'
                        msg = "" + clientName + ": " + msg;
                        
                    }
 
                    cout << msg.c_str() << endl;
 
                    //Broadcast that message to the other clients
                    for (int i = 0; i < MAXCLIENTS; i++)
                    {
                        if (client_array[i].clientSocketFD != 0)
                            if (new_client.id != i)
                                resultedFD = send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                    }

                }

                 // Send individual Option 
                if (resultedFD >0 && (tempmsg[0]=='s' && tempmsg[1]=='e' && tempmsg[2]=='n' && tempmsg[3]=='d')) // && tempmsg[4]==' ' && tempmsg[5]=='a' && tempmsg[6]=='l' && tempmsg[6]=='l'))
                {
                    if (tempmsg[5]=='T') // send to Tom
                    {
                        
                        if (strcmp("", tempmsg))
                        {   
                            temp = tempmsg;
                            msg = temp.substr(9,temp.size()); // manipulate the meassge to avoid 'send all'
                            msg = "" + clientName + " (to Tom): " + msg;
                            cout << msg  << endl;
                        
                        }
                    
                        //Unicast the message to the relevant client
                        for (int i = 0; i < MAXCLIENTS; i++)
                        {
                            if (client_array[i].nameOfClient == "Tom")
                                resultedFD = send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                        }

                    }

                    else if (tempmsg[5]=='D') // send to David
                    {
                        if (strcmp("", tempmsg))
                        {   
                            temp = tempmsg;
                            msg = temp.substr(11,temp.size()); // manipulate the meassge to avoid 'send all'
                            msg = "" + clientName + " (to David): " + msg;
                            cout << msg  << endl;
                        
                        }
                        
                        //Unicast the message to the relevant client
                        for (int i = 0; i < MAXCLIENTS; i++)
                        {
                            if (client_array[i].nameOfClient == "David")
                                resultedFD = send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                        }

                    }

                    else if (tempmsg[5]=='B') // send to Beth
                    {
                        if (strcmp("", tempmsg))
                        {   
                            temp = tempmsg;
                            msg = temp.substr(10,temp.size()); // manipulate the meassge to avoid 'send all'
                            msg = "" + clientName + " (to Beth): " + msg;
                            cout << msg  << endl;
                        
                        }
 
                        //Unicast the message to the relevant client
                        for (int i = 0; i < MAXCLIENTS; i++)
                        {
                            if (client_array[i].nameOfClient == "Beth")
                                resultedFD = send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                        }


                    }

                    else if (tempmsg[5]=='J') // send to John
                    {
                        if (strcmp("", tempmsg))
                        {   
                            temp = tempmsg;
                            msg = temp.substr(10,temp.size()); // manipulate the meassge to avoid 'send all'
                            msg = "" + clientName + " (to John): " + msg;
                            cout << msg  << endl;
                        
                        }
 
                        //Unicast the message to the relevant client
                        for (int i = 0; i < MAXCLIENTS; i++)
                        {
                            if (client_array[i].nameOfClient == "John")
                                resultedFD = send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                        }

                    }

                }

                if (resultedFD <=0) // end of connection
                {
                    
                    msg = "" + clientName + " Left the Room";

                    // delete the name 
                    for (int i = 0; i < connectedClients.size() ; i++)
                    {
                        if (connectedClients.at(i) ==  clientName) connectedClients.erase(connectedClients.begin()+i);
                    }
                    cout << msg << endl;
 
                    close(new_client.clientSocketFD); //
                    close(client_array[new_client.id].clientSocketFD);
                    client_array[new_client.id].clientSocketFD = 0;
 
                    //Sent the disconnection message to all  other clients
                    for (int i = 0; i < MAXCLIENTS; i++)
                    {
                        if (client_array[i].clientSocketFD != 0)
                        resultedFD = send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                    }
 
                    break;
                }
            }

        }
        else // if not a legit user ask again and test again
        {
            msg = "Client " + to_string(new_client.id) + " Could Not Be verifyied";
            send(new_client.clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
            cout << msg << endl;
            msg = "Please provide Your UserID and Password (userID password)"; // ask for credentials
            send(new_client.clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0); // send the request
            
            memset(tempmsg, 0, bufsize); // set all to zero to later hold the response
            recv(new_client.clientSocketFD, tempmsg, bufsize, 0); // receive the response
            legit = verifyUser(tempmsg, clientName); // send the buffer with the credentials to verify correctness
            
            if (legit)
            {
                msg = "" + clientName + " Joined the Room";
                connectedClients.push_back (clientName); // store the name of the new connection
                new_client.nameOfClient = clientName; // add the name to the class


                //Broadcast the new connection to the other clients
                for (int i = 0; i < MAXCLIENTS; i++)
                {
                    if (client_array[i].clientSocketFD != 0)
                    send(client_array[i].clientSocketFD, msg.c_str(), strlen(msg.c_str()), 0);
                }
            }
            continue;

        }

    } //end while
    close(new_client.clientSocketFD); //
    close(client_array[new_client.id].clientSocketFD);
    client_array[new_client.id].clientSocketFD = 0;
 
    connectedClients.clear(); // empty the vector
    return 0;
}



bool verifyUser(char tempmsg[bufsize], string &clientName)
{       
    bool permited = false;
    
    // check records for provided userID and password
    string line, nextLine = "";
    ifstream file;
    file.open ("loginDetails.txt");
    
    while(file.good())
    {        
        while (getline(file,line))
        {
            if (strcmp(line.c_str(), tempmsg)==0)
            {
                cout << line << " Matched for Login" <<endl;
                istringstream iss(line);
                string word;
                iss >> word;
                clientName = word;
                permited=true;
                file.close();  
                memset(tempmsg, 0, bufsize); // set all to zero to later hold the response
                return permited;

            }
      
        }
        file.close();  

    }
    
    return permited;
 
}

#endif /* server_cpp */
