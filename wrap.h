#ifndef _WRAP_H
#define _WRAP_H
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

void perr_exit(const char *s); 

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr); 
void Bind(int fd, const struct sockaddr *sa, socklen_t salen); 
void Connect(int fd, const struct sockaddr *sa, socklen_t salen); 
void Listen(int fd, int backlog);

int Socket(int family, int type, int protocol); 

ssize_t Read(int fd, void *ptr, size_t nbytes); 
ssize_t Write(int fd, const void *ptr, size_t nbytes); 


ssize_t Readn(int fd, void *vptr, size_t n); 
ssize_t Writen(int fd, const void *vptr, size_t n); 


#endif
