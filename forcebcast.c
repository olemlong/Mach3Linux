/* forcebcast.so — enables SO_BROADCAST on all UDP sockets (for the ESS plugin under Wine) */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>

int socket(int domain, int type, int protocol)
{
    static int (*real_socket)(int, int, int);
    if (!real_socket) real_socket = dlsym(RTLD_NEXT, "socket");
    int fd = real_socket(domain, type, protocol);
    if (fd >= 0 && domain == AF_INET && (type & 0xf) == SOCK_DGRAM) {
        int one = 1;
        setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &one, sizeof one);
    }
    return fd;
}
