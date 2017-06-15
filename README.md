# ClientServer_ChatRoom
A chat-room program which simulates communications between multiple clients (3 in this case) and a server using sockets and thread.

The chat room follow the following protocol:
1. Access is limited to registered users by verifying userName and password against a given .txt file
2. Upon successful verification a client can choose between three functions:
	a. message all -> requires appending the massage with 'send all' (e.g. send all Hello people)
	b. message individual user -> requires appending the message with 'send UserName' (e.g. send Beth Hi Beth)
		Note: Current individual messaging users the provided naming in the credentials file (.txt)
	c. see how is currently in the chat room -> send the command 'who'
	d. logout of the room -> send the command logout

To run the program, follow these steps:
1. Download all files and modify credentials (.txt) to your liking (each should exist on an individual line)
2. Open four (4) terminal/CMD windows
3. First window - compile and run server:  clang++ -std=c++11 server.cpp (and run with) ./a.out
4. Second window - compile and run client:  clang++ -std=c++11 client.cpp (and run with) ./a.out
5. Third window - compile and run client2:  clang++ -std=c++11 client2.cpp (and run with) ./a.out
6. Forth window - compile and run client3:  clang++ -std=c++11 client3.cpp (and run with) ./a.out
  Note: Given that these processes run independently exist no issue with similar naming for all .out files
7. All clients are logged-in -> Follow available chat functions

- All communications implemented using a socket per each client (and for the server). Handling each client is done via threads to allow parallel execution of all communications
- Current port address is 13607. Client IP is a loop back IP (127.0.0.1).

Yosef Yudborovsky

