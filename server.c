
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct message {
  char source[50];
  char target[50];
  char msg[200]; // message body
};

void terminate(int sig) {
  printf("Exiting....\n");
  fflush(stdout);
  exit(0);
}

int main() {
  int server;
  int target;
  int dummyfd;
  struct message req;
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, terminate);
  server = open("serverFIFO", O_RDONLY);
  dummyfd = open("serverFIFO", O_WRONLY);

  // Error handling
  if (server == -1 || dummyfd == -1) {
    perror("Error opening serverFIFO");
    return 1;
  }

  while (1) {
    // TODO:
    // read requests from serverFIFO
    if (read(server, &req, sizeof(req)) == -1) {
      perror("Error reading from serverFIFO");
      break;
    }

    printf("Received a request from %s to send the message %s to %s.\n",
           req.source, req.msg, req.target);

    // TODO:
    // open target FIFO and write the whole message struct to the target FIFO
    // close target FIFO after writing the message
    if ((target = open(req.target, O_WRONLY)) == -1) {
      perror("Error opening target");
      continue;
    }
    write(target, &req, sizeof(req));
    close(target);
  }
  close(server);
  close(dummyfd);
  return 0;
}
