/**************************************
udp_ser.c: the source file of the server in udp transmission
**************************************/
#include "headsock.h"

void str_ser1(int sockfd); // transmitting and receiving function
char* getDate();

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Only one arguments (port number) is required. \n");
	}
	int port = strtol(argv[1], NULL, 10);
	printf("connecting to port %d \n", port);

	int sockfd;
	struct sockaddr_in my_addr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{ //create socket
		printf("error in socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{ //bind socket
		printf("error in binding");
		exit(1);
	}

	printf("start receiving\n");

	while (1)
	{
		str_ser1(sockfd); // send and receive
	}
	close(sockfd);
	exit(0);
}

void str_ser1(int sockfd)
{
	char recvs[MAXSIZE];
	int n = 0, slen;
	struct sockaddr_in addr;

	pid_t pid;

	slen = sizeof(addr);

	if ((n = recvfrom(sockfd, recvs, MAXSIZE, 0, (struct sockaddr *)&addr, &slen)) == -1)
	{ //receive the packet
		printf("error receiving");
		exit(1);
	}
	recvs[n] = '\0';
	printf("the received string is :\n%s", recvs);

	if ((pid = fork()) == 0) 
	{
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

		char response[] = "HTTP/1.1 200 OK\r\nDate: ";
		for (int i = 0; i < strlen(date); i++) {
			int len = strlen(response);
			response[len] = date[i];
			response[len+1] = '\0';
		}
		char response_end[] = " SGT\r\nContent-Type: text/html\r\n\r\nEE-4210: Continuous assessment";
		for (int i = 0; i < strlen(response_end); i++) {
			int len = strlen(response);
			response[len] = response_end[i];
			response[len+1] = '\0';
		}
		printf("RESPONSE: %s\n", response);
		if ((n = sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&addr, slen)) == -1)
		{
			printf("send error!"); //send the ack
			exit(1);
		}
		else
		{
			printf("sent response successfully :)\n");
		}
		close(sockfd);
		exit(0);
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