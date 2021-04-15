/**************************************
udp_ser.c: the source file of the server in udp transmission
**************************************/
#include "headsock.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Only one arguments (port number) is required. \n");
	}
	int port = strtol(argv[1], NULL, 10);
	printf("connecting to port %d \n", port);

	int sockfd;
	struct sockaddr_in my_addr, addr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{ //create socket
		printf("error in socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{ //bind socket
		printf("error in binding");
		exit(1);
	}

	printf("start receiving\n");

	while (1)
	{
		char recvs[MAXSIZE];
		int n = 0, slen;

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
				continue;
			}

			char *http = strstr(recvs, "HTTP/1.1");

			if (!http)
			{
				printf("No HTTP/1.1 requests received. \n");
				continue;
			}

			char response[] = "HTTP/1.1 200 OK\r\n\r\n<html><head><title>CA2 S2</title></head><body>EE-4210: Continuous assessment</body></html>";

			if ((n = sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&addr, slen)) == -1)
			{
				printf("send error!"); 
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
}