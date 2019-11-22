#include<bits/stdc++.h>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#define bucketsize 512
using namespace std;

void bktinput(int a,int b)
{
	if(a>bucketsize)
		cout<<"\n\t\tBucket Overflow";
	else
	{
		usleep(500);
		while(a>b)
			{
			cout<<"\n\t\t"<<b<<"bytes outputed";
			a=a-b;
			usleep(500);
			}
		if(a>0)
			cout<<"\n\t\tLast"<<a<<"bytes sent\t";
			cout<<"\n\t\tBucket output: successful";
	}
}

int main()
{
	int op,pktsize;
	cout<<"enter output rate:";
	cin>>op;
	for (int i=1;i<=5;i++)
	{
		usleep(rand()%1000);
		pktsize=rand()%1000;
		cout<<"\nPacket no "<<i<<"\tPacket size= "<<pktsize;
		bktinput(pktsize,op);
	}
	return 0;
}


/*
enter output rate:200

Packet no 1	Packet size= 886
		Bucket Overflow
Packet no 2	Packet size= 915
		Bucket Overflow
Packet no 3	Packet size= 335
		200bytes outputed
		Last135bytes sent	
		Bucket output: successful
Packet no 4	Packet size= 492
		200bytes outputed
		200bytes outputed
		Last92bytes sent	
		Bucket output: successful
Packet no 5	Packet size= 421
		200bytes outputed
		200bytes outputed
		Last21bytes sent	
		Bucket output: successful

*/