#include <wiringPi.h>
#include <threads.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define CAMS_NO 3
#define SOCK_NAME "./cam.socket"
#define FILENAME_LEN 12

struct cam_args_t {
  char cam[FILENAME_LEN];
  int socket;
};

struct motor_args_t {
  int socket;
};

// Function declaration
void cam(struct cam_args_t*);
void motor(struct motor_args_t*);

int main(void) {
  thrd_t cams[CAMS_NO];

  // Creating socket for Inter-Process Communication with childs
  if((socket_d = socket(PF_UNIX, SOCK_STREAM, 0) == -1) {
    printf("Unable to create socket");
    abort();
  }
  // Binding wiht file
  struct sockaddr_un* addr;
  addr->sun_family = AF_UNIX;
  addr->sun_path = SOCK_NAME;
  bind(socket_d, addr, sizeof(sockaddr_un));

  // Creating thread to manage every camera
  printf("Creating threads for cams\n");
  for(int i = 0; i < CAMS_NO; i++) {

    // Filling up args struct for every camera
    cam_args_t* args;
    sprintf(&args->cam, "/dev/video%i", i);
    args->socket = socket_d;

    // Creating thread and checking for error
    if(thrd_create(cams[i], cam, args) != thrd_success) {
      printf("Unable to create camera thread: %i", i);
      abort();
    }
  }

  // Arguments for servo function
  motor_args_t* args;
  args->socket = socket_d

  // Create thread for servo controlling
  if(thrd_create(motor, motor, args != thrd_success) {
    printf("Unable to create motor thread");
    abort();
  }

  // Deleting socket
  unlink(SOCK_NAME);
}