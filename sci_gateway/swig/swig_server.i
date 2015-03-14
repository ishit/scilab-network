%module server

%inline %{
extern void *get_in_addr(struct sockaddr *sa)
extern int TcpOpen(int port);
%}