%module network

%inline%{
extern int TcpOpen(int port);
extern int client(char *ip, int port);
extern int close(int pid);
%}
