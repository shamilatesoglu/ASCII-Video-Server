#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "string_utils.h"
#include "video.h"

void
start_serving(int port, char *video_file_path);

void
error(char *msg);

int
read_in(int socket, char *buf, int len);

int
open_listener_socket();

void
bind_to_port(int socket, int port);

int
write_out(int socket, char *s);

void
handle_sigterm(int sig);

static int listener_d;

// sserver –p port –s streams –ch1 videofile [–ch2 videofile] [–ch3 videofile]
int
main(int argc, char *argv[])
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_sigterm;
    sigaction(SIGTERM, &action, NULL);

    int port = 443;
    char *video_file_path;

    // Parse command line params
    for (int i = 0; i < argc; i++)
    {
        if (EQUALS(argv[i], "-p"))
        {
            port = atoi(argv[i + 1]);
        }
        else if (EQUALS(argv[i], "-ch1"))
        {
            video_file_path = argv[i + 1];
        }
    }

    start_serving(port, video_file_path);
    return 0;
}

void
start_serving(int port, char *video_file_path)
{
    // Read video file.
    char video_buffer[MAX_VIDEO_BYTES];
    read_file(video_file_path, video_buffer);

    char *video_map[MAX_FRAME_COUNT];
    parse_frames(video_buffer, video_map);


    // Open socket.
    listener_d = open_listener_socket();

    bind_to_port(listener_d, port);

    if (listen(listener_d, 10) == -1)
    {
        error("Can't listen");
    }

    struct sockaddr_storage client_addr;

    unsigned int address_size = sizeof(client_addr);

    puts("Waiting for connection...");

    char buffer[255];

    while (1)
    {
        int connect_d = accept(listener_d, (struct sockaddr *) &client_addr, &address_size);
        if (connect_d == -1)
        {
            error("Can't open client socket");
        }
        if (!fork())
        {
            read_in(connect_d, buffer, sizeof(buffer));
            printf("Received: %s\n", buffer);
            if (count_substring(buffer, "FORMAT"))
            {
                char response[255];
                sprintf(response, "%dx%d %d", VIDEO_WIDTH, VIDEO_HEIGHT, get_number_of_frames(video_buffer));
                write_out(connect_d, response);
            }
            else if (count_substring(buffer, "TIME "))
            {
                int frame_i;
                if (sscanf(buffer, "TIME %d", &frame_i) == 1)
                {
                    int time_to_display;
                    get_time_to_display(video_map, frame_i, &time_to_display);
                    char response[10];
                    sprintf(response, "%d", time_to_display);
                    write_out(connect_d, response);
                }
                else
                {
                    write_out(connect_d, "Unrecognized command.");
                }
            }
            else if (count_substring(buffer, "GET"))
            {
                int frame_i;
                if (sscanf(buffer, "GET %d", &frame_i) == 1)
                {
                    char frame[(VIDEO_WIDTH + 2) * VIDEO_HEIGHT + 1];
                    framecast(video_map, frame_i, frame);
                    write_out(connect_d, frame);
                } else {
                    write_out(connect_d, "Unrecognized command.");
                }
            }
            close(connect_d);
            exit(0);
        }
        else
        {
            close(connect_d);
        }
    }
}

void
error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int
read_in(int socket, char *buf, int len)
{
    char *s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    while ((c > 0) && (s[c - 1] != '\n'))
    {
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
    }

    if (c < 0)
        return c; // there is no error
    else if (c == 0)
        buf[0] = '\0';
    else
        s[c - 1] = '\0'; // replace the "/r" character with a "\0"

    return len - slen;
}

int
open_listener_socket()
{
    // open an internet streaming socket
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1)
    {
        perror("Can't open socket.");
        printf("errno: %d - %s\n", errno, strerror(errno));
    }
    return s;
}

void
bind_to_port(int socket, int port)
{
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t) htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int)) == -1)
    {
        perror("Can't set the reuse option on the socket.");
    }
    int c = bind(socket, (struct sockaddr *) &name, sizeof(name));
    if (c == -1)
    {
        perror("Can't bind to socket.");
    }
}

int
write_out(int socket, char *s)
{
    int result = send(socket, s, strlen(s), 0);
    if (result == -1)
    {
        fprintf(stderr, "%s: %s\n", "Error talking to the client", strerror(errno));
    }
    return result;
}

void
handle_sigterm(int sig)
{
    if (listener_d)
    {
        close(listener_d);
    }
    fprintf(stderr, "Shutting down server.\n");
    exit(0);
}