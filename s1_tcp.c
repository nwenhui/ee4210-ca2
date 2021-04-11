/**********************************
tcp_ser.c: the source file of the server in tcp transmission
***********************************/

#include "headsock.h"

#define BACKLOG 5

void str_ser(int sockfd); // transmitting and receiving function
char* getDate();

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Only one arguments (port number) is required. \n");
	}
	int port = strtol(argv[1], NULL, 10);
	printf("connecting to port %d \n", port);

	int sockfd, con_fd, ret;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;

	pid_t pid;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("error in socket!");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port); //port number
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);
	// ret = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));                //bind socket
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		printf("error in binding");
		exit(1);
	}

	// ret = listen(sockfd, BACKLOG);                              //listen
	if (listen(sockfd, BACKLOG) < 0)
	{
		printf("error in listening");
		exit(1);
	}

	printf("receiving start\n");

	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);
		con_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); //accept the packet
		if (con_fd < 0)
		{
			printf("error in accept\n");
			exit(1);
		}

		if ((pid = fork()) == 0)
		{ // creat acception process
			close(sockfd);
			str_ser(con_fd); //receive packet and response
			close(con_fd);
			exit(0);
		}
		else
			close(con_fd); //parent process
	}
	close(sockfd);
	exit(0);
}

void str_ser(int sockfd)
{
	char recvs[MAXSIZE];
	int n = 0;

	if ((n = recv(sockfd, &recvs, MAXSIZE, 0)) == -1)
	{ //receive the packet
		printf("receiving error!\n");
		return;
	}

	recvs[n] = '\0';
	// printf("the received string:\n%s\n", recvs);

	// printf("hello");
	// printf("Current local time and date: %s", getDate());

	if (n < 14) // GET / HTTP/1.1 <- length = 14 (Base HTTP Request request line)
	{
		printf("No HTTP/1.1 requests received. \n");
		return;
	}

	char* http = strstr(recvs, "HTTP/1.1");

	if (!http) 
	{
		printf("No HTTP/1.1 requests received. \n");
		return;
	}

	char* date = getDate();

	if (recvs[5] == ' ')
	{
		printf("default \n");
		char response[] = "HTTP/1.1 200 OK\r\nDate: ";
		for (int i = 0; i < strlen(date); i++) {
			int len = strlen(response);
			response[len] = date[i];
			response[len+1] = '\0';
		}
		char response_end[] = " SGT\r\nContent-Type: text/html\r\n\r\n<html><head><title>CA2 S1</title></head><body><center><form action=\"\" method=\"GET\"><br><input type=\"text\" name=\"input\" value=\"\"><br><input type=\"submit\" value=\"Enter\"></form></center></body></html>";
		for (int i = 0; i < strlen(response_end); i++) {
			int len = strlen(response);
			response[len] = response_end[i];
			response[len+1] = '\0';
		}
	
		// printf("RESPONSE: \n%s", response);
		if ((n = send(sockfd, &response, strlen(response), 0)) == -1)
		{
			printf("send error!"); //send the ack
			exit(1);
		}
		else
		{
			printf("sent response successfully :)\n");
		}
	}
	else if (recvs[5] == '?')
	{
		printf("data received: %s\n", recvs);
		char response[] = "HTTP/1.1 200 OK\r\nDate: ";
		for (int i = 0; i < strlen(date); i++) {
			int len = strlen(response);
			response[len] = date[i];
			response[len+1] = '\0';
		}
		char response_end[] = " SGT\r\nContent-Type: text/html\r\n\r\n";
		for (int i = 0; i < strlen(response_end); i++) {
			int len = strlen(response);
			response[len] = response_end[i];
			response[len+1] = '\0';
		}
		char* ptr = strstr(recvs, " HTTP/1.1\r\n");
		int stop = ptr - recvs;
		for (int i = 12; i < stop; i++) {
			int len = strlen(response);
			response[len] = recvs[i];
			response[len+1] = '\0';
		}
		// printf("\nresponse: %s\n", response);
		if ((n = send(sockfd, &response, strlen(response), 0)) == -1)
		{
			printf("send error!"); //send the ack
			exit(1);
		}
		else
		{
			printf("sent response successfully :)\n");
		}
	}
	else
	{
		printf("No HTTP/1.1 requests received. \n");
	}
}

char* getDate()
{
	time_t rawtime;
	struct tm *timeptr;
	time(&rawtime);
	timeptr = localtime(&rawtime);
	static const char wday_name[][4] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	static const char mon_name[][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	static char result[80];
	sprintf(result, "%.3s,%3d %.3s %d %.2d:%.2d:%.2d",
			wday_name[timeptr->tm_wday],
			timeptr->tm_mday,
			mon_name[timeptr->tm_mon],
			1900 + timeptr->tm_year,
			timeptr->tm_hour,
			timeptr->tm_min,
			timeptr->tm_sec);
	return result;
}