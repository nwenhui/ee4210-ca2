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

UDP server S2:
1. In terminal/bash/command line, change directory to the folder using the command:
    cd <path/to/folder>
2. (OPTIONAL) To compile the TCP server S2, use the command:
    gcc -o s2 s2_udp.c
3. To run the TCP server S2, use the command:
    ./s2 <port number>
4. Open another terminal/bash/command line window and use echo to start a connection with the server using the command:
    echo "GET / HTTP/1.1\r\n" > /dev/udp/127.0.0.1/<port number>

-----
NOTES
-----
- S1 is best used with a web browser so that the form can be seen visually.
- S2 uses UDP, and since UDP is not supported by web browsers, echo command must be used instead.
- Echo command can be used for S1 too, but the form will not be shown.
- Do avoid special characters (eg. !@#$%) for S1 as they will appear as their percent-encoded values instead.
- Do avoid enter multiple words for S1 in a single request as space (' ') will appear as '+' instead.