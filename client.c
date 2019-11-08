#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#define Fifo1 "fifo1"
#define Fifo2 "fifo2"
#define Perms 0666
char fname[256];
int main()
{
int readfd,writefd,fd;
ssize_t n;
char buff[512];
printf("Trying to connect to server..\n");
writefd=open(Fifo1,O_WRONLY,0);
readfd=open(Fifo2,O_RDONLY,0);
printf("Connected..\n");
printf("Enter the filename to request from server:");
scanf("%s",fname);
write(writefd,fname,strlen(fname));
printf("Waiting for server to reply..\n");
while((n=read(readfd,buff,512))>0)
write(1,buff,n);
close(readfd);
close(writefd);
return 0;
}

/*

bmsce@bmsce-Precision-T1700:~/Documents$ gcc client.c -o client.o
bmsce@bmsce-Precision-T1700:~/Documents$ ./client.o
Trying to connect to server..
Connected..
Enter the filename to request from server:hello.txt
Waiting for server to reply..
Hello wassup XD

*/
