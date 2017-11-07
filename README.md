# ClientServer_ChatRoom via Sockets and Threads
A simple chat-room program to simulate communications handaling between multiple clients (3 in this simulation) and a server, implemented using sockets and thread.

The chat room follow the following protocol:
1. Access is limited to registered users only by verifying userName and password against a given .txt file
2. Upon successful verification, a client can choose between three functions:

	a. message all -> requires appending the massage with 'send all' (e.g. send all Hello people)
	
	b. message individual user -> requires appending the message with 'send UserName' (e.g. send Beth Hi Beth)
	
		Note: Names of current avaliable users provided in the credentials file (.txt)
		
	c. see who is currently loged-in the -> send the command 'who'
	
	d. logout -> send the command logout
	
To run the program:
1. Download all files and modify credentials (.txt) to your liking (each on an individual line)
2. Open four (4) terminal/CMD windows each will run an individual program
3. First window - compile and run the server:  clang++ -std=c++11 server.cpp (and run with) ./a.out
4. Second window - compile and run client:  clang++ -std=c++11 client.cpp (and run with) ./a.out
5. Third window - compile and run client2:  clang++ -std=c++11 client2.cpp (and run with) ./a.out
6. Forth window - compile and run client3:  clang++ -std=c++11 client3.cpp (and run with) ./a.out
  Note: Given that these processes run independently, exist no issue with similar naming (.out file)
7. All clients are logged-in -> Follow available chat functions

- All communications implemented using a socket per each client (and for the server). Handling each client is done via threads to allow parallel execution of all communications
- Current port address is 13607. Client IP is a loop-back IP (127.0.0.1).

Yosef Yudborovsky

