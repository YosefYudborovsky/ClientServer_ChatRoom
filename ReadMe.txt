Wellcome to the Server-multiple clients chatRoom.
This progarm simulates the interaction between multiple clients (3 in this case) and a server in the structure of a chat room. 
The chat room follow the following protocol:
1. Access is limited to registered useres by verifying userName and password against a given .txt file
2. Upon successful verification a client can choose between three functions:
	a. mesaage all -> requires 
	b. message individual user -> requires
	c. see how is currently in the chat romm -> send the command 'who'
	d. logout of the room -> send the command logout





To run the program please follow these steps:\
\
1. Download all files and modify credentials (.txt) to your likeing (each should exist on an individual line)\
2. Open four (4) terminal/CMD windows\
3. First window - compile and run server:  clang++ -std=c++11 server.cpp (and run with) ./a.out\
4. Second window - compile and run client:  clang++ -std=c++11 client.cpp (and run with) ./a.out\
5. Third window - compile and run client2:  clang++ -std=c++11 client2.cpp (and run with) ./a.out\
6. Forth window - compile and run client3:  clang++ -std=c++11 client3.cpp (and run with) ./a.out\
 Note: Given that these processes run independendatly exist no issue with simislar naming for all .out files
 7. All clients are loged-in -> Follow avaliable chat functions\
\
# All communications are implemented using a socket for each client and server. Handaling each client is done via threads to allow parallel execution of all communications. 
# Current port address is 13607. Client IP is a loop back IP (127.0.0.1).\

\
Yosef Yudborovsky\
\
}