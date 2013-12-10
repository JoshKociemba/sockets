/**
 * Author: Joshua Kociemba (kociembj), Joshua.Kociemba@oregonstate.edu
 * Created: 2013-12-7
 * Filename: calculate.c
 *
 * Description: Computes perfect numbers by testing all numbers from the
 * starting point given by the user, subject to constraints given. All of
 * the communication is done through manage, and the ranges given by the user
 * are passed through report to manage, and then to compute.
 **/

 /*Include comments pulled from McGrath's example files*/
 
#define _GNU_SOURCE 1

#include <sys/types.h>	/* basic system data types */
#include <sys/socket.h>	/* basic socket definitions */
#include <sys/time.h>	/* timeval{} for select() */
#include <time.h>	/* timespec{} for pselect() */
#include <netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>	/* inet(3) functions */
#include <errno.h>
#include <fcntl.h>	/* for nonblocking */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>     /* for bzero */
#include <sys/stat.h>	/* for S_xxx file mode constants */
#include <sys/uio.h>	/* for iovec{} and readv/writev */
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <stdbool.h>

#define LISTENQ 1024
#define MAXLINE 4096
#define MAXSOCKADDR 128
#define BUFFSIZE 4096

#define SERV_PORT 9879
#define SERV_PORT_STR "9879"

unsigned int MIN_NUMS;
unsigned int MAX_NUMS;
int COUNT;

/**
 * Connects to server by checking for the localhost
 * and setting that as the host name
 **/
static int server_connect(void);

/**
 * Sends string output to the server connected to the
 * server_fd and returns the server_fd used
 **/
static int server_send(const char *output);

/**
 * Receives data from the server using read
 * and the servers file descriptor server_fd
 * it then sets the max and min values for the
 * calculations
 **/
void server_recv(int server_fd);

/**
 * Uses uname to find the address of the servername
 * and then uses strdup to copy the string at that address
 * into a usable string name
 **/
static char *whois_localhost(void);

/**
 * Calls is_perfect in a for loop that runs between the constraints given
 * it continues calling is_perfect marking a count for all of the perfect
 * numbers found
 **/
void calc_perfect(void);

/**
 * Calculates perfect numbers by first moding by two to make sure
 * that the number is prime, and then continue to calculate for perfect
 * numbers by modding in a for loop and summing up the mods that equal zero
 * it then compares the sum of all of the modded values and checks if it equals
 * the value being tested
 **/
bool is_perfect(unsigned int num);

/**
 * MAIN FUNCTION
 **/
int main(int argc, char **argv) {
	char output[100];

	while(true) {
		int server_fd = server_connect();
		server_recv(server_fd);
		close(server_fd);
		printf("received min: %u max: %u\n", MIN_NUMS, MAX_NUMS);
		calc_perfect();
		server_fd = server_send(output);
		sprintf(output, "%d", COUNT);
		printf("%s\n", output);
	}

	return 0;
}

static int server_connect(void) {
	struct hostent *server_name = gethostbyname(whois_localhost());
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_address;

	if (server_name == NULL) {
		perror("Host not found.");
		exit(-1);
	}
	
	if (socketfd == -1) {
		perror("Socket wasn't opened.");
		exit(-1);
	}

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	memcpy(&server_address.sin_addr.s_addr, server_name->h_addr, server_name->h_length);
	server_address.sin_port = htons(SERV_PORT);

	if (connect(socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
		perror("Couldn't connect to server.");
		exit(-1);
	}

	return socketfd;
}

static int server_send(const char *output) {
	int server_fd = server_connect();
	
	if (write(server_fd, output, strlen(output)) == -1) {
		perror("Couldn't write to server.");
		exit(-1);
	}
		
	return server_fd;
}

void server_recv(int server_fd) {
	char buffer[BUFFSIZE] = {};

	if (read(server_fd, buffer, BUFFSIZE) == -1) {
		perror("Couldn't read data from server");
		exit(-1);
	}

	sscanf(buffer, "%u %u", &MIN_NUMS, &MAX_NUMS);
	
	if (MIN_NUMS > MAX_NUMS) {
		unsigned int temp;
		temp = MIN_NUMS;
		MIN_NUMS = MAX_NUMS;
		MAX_NUMS = temp;
	}

}

static char *whois_localhost(void) {
	struct utsname serv_name;
	if (uname(&serv_name) == -1) {
		perror("Host not found.");
		exit(-1);
	}

	return strdup(serv_name.nodename);
}

void calc_perfect(void) {
	unsigned int i;

	for (i = MIN_NUMS; i < MAX_NUMS; i++) {
		if (is_perfect(i)) {
			printf("%u is perfect\n", i);
			COUNT++;		
		}
	}
}

bool is_perfect(unsigned int num) {
	unsigned int i, j, sum = 1;
	
	if (num % 2 != 0) {
		return false;
	}

	for (i = 2; i < num; i++) {
		if ((num % i) == 0) {
			sum += i;
		}
	}

	if (sum == num) {
		return true;
	} else {
		return false;
	}
}
