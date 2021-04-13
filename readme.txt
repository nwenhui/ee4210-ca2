This folder contains the server codes for AY20/21 S2 EE4210 CA2.

------------
INSTRUCTIONS
------------

TCP server S1:
1. In terminal/bash/command line, change directory to the folder using the command:
    cd <path/to/folder>
2. (OPTIONAL) To compile the TCP server S1, use the command:
    gcc -o s1 s1_tcp.c
3. To run the TCP server S1, use the command:
    ./s1 <port number>
4. Open up a browser of your choice and go to localhost:<port number> to start a HTTP connection with server.
5. Enter something in the textbox and watch it appear after clicking enter!
6. Close the client by closing the web browser.
7. To close the server, use CTRL + C in the terminal window that the server is running on.

UDP server S2:
1. In terminal/bash/command line, change directory to the folder using the command:
    cd <path/to/folder>
2. (OPTIONAL) To compile the TCP server S2, use the command:
    gcc -o s2 s2_udp.c
3. To run the TCP server S2, use the command:
    ./s2 <port number>
4. Open another terminal/bash/command line window and use netcat to start a connection with the server using the command:
    nc -u 127.0.0.1 <port number>
5. Once connected via netcat, send a basic HTTP/1.1 GET request to the server.  
    GET / HTTP/1.1
6. A webpage (HTTP Response) with the text "EE-4210: Continuous assessment" is expected.
7. To close the server/client, use CTRL + C in the terminal window that the server is running on.

-----
NOTES
-----
- Both servers (S1 & S2) only accept HTTP/1.1 GET requests.
- S1 is best used with a web browser so that the form can be seen visually.
- S2 uses UDP, and since UDP is not supported by web browsers, netcat must be used instead.
- Do avoid special characters (eg. !@#$%) for S1 as they will appear as their percent-encoded values instead.
- Do avoid enter multiple words for S1 in a single request as space (' ') will appear as '+' instead.