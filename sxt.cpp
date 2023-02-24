#include<iostream>
#include<opencv2/opencv.hpp>
#include "face.h"
#include <jsoncpp/json/json.h>
#include <pthread.h>
#include <string>
#include <list>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


using namespace std;
using namespace Json;
using namespace aip;
using namespace cv;



int main()
{

	VideoCapture video;
	if(false == video.open(-1))
	{
		cout<<"open error"<<endl;
		return -1;
	}
	cout<<"open ok"<<endl;
	
	/********************************Server************************/
	int sockfd = socket(PF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
	{
                printf("error\n");
                return -1;
    }
    printf("ok!\n");
    struct sockaddr_in myuser;
    myuser.sin_family = PF_INET;
    myuser.sin_port = htons(8888);
    myuser.sin_addr.s_addr = inet_addr("0.0.0.0");
	int ret = bind(sockfd,(struct sockaddr*)&myuser,sizeof(myuser));
    if(ret<0)
    {
            printf("bind error\r\n");
            return -1;
    }
    printf("bind ok\r\n");
        //jianting
    ret = listen(sockfd,1);
    if(ret<0)
    {
            printf("listen error\r\n");
            return -1;
    }
    printf("listen ok");
    //set connection
    int confd = accept(sockfd,NULL,NULL);
    if(confd < 0)
    {
            printf("accept error\r\n");
            return -1;
    }
    printf("accept ok\r\n");
	
	/******************************cong  face**************************/

	while(1)
	{
		Mat frame;
		namedWindow("text");
		if(false == video.read(frame))
		{
			cout<<"read error"<<endl;
			continue;
		}
		cout<<"read ok"<<endl;
		

		imshow("text",frame);
		
		char c=waitKey(200);
		if(c>0)
		{
			break;
		}
	}
	return 0;
}
