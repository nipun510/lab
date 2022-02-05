#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


int spawn(char *program, char ** args)
{
  pid_t childId = fork();
  if (childId == -1) {
    fprintf(stderr, "fork failed\n");
    return -1;
  }

  if (childId > 0) {
    return childId;
  }

  execvp(program, args);
  return 0;
}

void cleanChild(int sigNum) {
  int stat;
  pid_t childId = wait(&stat);
  if (WEXITSTATUS(stat)) {
    printf("command pid %d failed with exit code %d\n", childId, WEXITSTATUS(stat));
  } else {
    printf("command pid %d succeded\n", childId);
  }
}

int main() {
  struct sigaction sigact;
  memset(&sigact, 0, sizeof(sigact));
  sigact.sa_handler = &cleanChild;
  sigaction(SIGCHLD, &sigact, NULL);
  char *program = "ps";
  char *args[] = {"ps", "-elf", NULL}; 
  int processId = spawn(program, args);
  sleep(10);
  return 0;
}
