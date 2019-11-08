#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#define Fifo1 "fifo1"
#define Fifo2 "fifo2"
#define Perms 0666

char fname[256];

int main(){
	int readfd,writefd,fd;
	ssize_t n;
	char buff[512];
	
	if(mkfifo(Fifo1,Perms)<0){
		printf("Can\'t create fifo files\n");
	}
	
	if(mkfifo(Fifo2,Perms)<0){
		printf("Can\'t create fifo files\n");
	}
	
	printf("Waiting for connection request..\n");
	
	readfd=open(Fifo1,O_RDONLY,0);
	writefd=open(Fifo2,O_WRONLY,0);
	
	printf("Connection Estabilished..\n");
	read(readfd,fname,255);
	printf("Client has requested file %s\n",fname);
	
	if((fd=open(fname,O_RDWR))<0){
		strcpy(buff,"File does not exit..\n");
		write(writefd,buff,strlen(buff));
	}
	else{
		while((n=read(fd,buff,512))>0){
		write(writefd,buff,n);}
	}
	
	close(readfd); unlink(Fifo1);
	close(writefd); unlink(Fifo1);
}

/*

bmsce@bmsce-Precision-T1700:~/Documents$ gcc server.c -o server.o
bmsce@bmsce-Precision-T1700:~/Documents$ ./server.o
Waiting for connection request..
Connection Estabilished..
Client has requested file hello.txt

*/
