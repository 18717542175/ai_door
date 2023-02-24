/***********************************************************
	时间：2022/7/5
	作者：Hangover
	内容：智能门禁
***********************************************************/
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
using namespace cv;

int flage = 0;
int confd;
int left1;
int top;
int width;
int height;

/************************************************
函数功能：子线程   负责人脸检测
入口参数：视频帧对象 
*************************************************/
void *getFace(void *arg)
{
	// set APPID/AK/SK
	std::string app_id = "26598320";
	std::string api_key = "TWiMquourIxHEEb8Uhqmw5m2";
	std::string secret_key = "iW7IpMTCl5ZYuh49Q7tiafKklRjeXbXn";

	aip::Face client(app_id, api_key, secret_key);
	
	Mat fm = *(Mat*)arg;
	
	//dis face and exchange image
	Value result;
	vector <unsigned char>buf;
	imencode(".jpg",fm,buf);

	string image = base64_encode((char *)buf.data(), buf.size());

	// calling dis face
	result = client.detect(image, "BASE64", aip::null);

	//print number of face
	cout << result["result"]["face_num"] << endl;
	
	left1 = result["result"]["face_list"][0]["location"]["left"].asInt();
	top = result["result"]["face_list"][0]["location"]["top"].asInt();
	width = result["result"]["face_list"][0]["location"]["width"].asInt();
	height = result["result"]["face_list"][0]["location"]["height"].asInt();

	
}

/************************************************
函数功能：子线程   负责人脸识别
入口参数：视频帧对象 
*************************************************/
void *recFace(void *arg)
{
	// set APPID/AK/SK
	std::string app_id = "26598320";
	std::string api_key = "TWiMquourIxHEEb8Uhqmw5m2";
	std::string secret_key = "iW7IpMTCl5ZYuh49Q7tiafKklRjeXbXn";
	aip::Face client(app_id, api_key, secret_key);
	Json::Value root;
	string people[3] = {"dis_face_lg","dis_face_whm","dis_face_ltc"};
	Mat fm = *(Mat*)arg;
	
	Value result;
	vector <unsigned char>buf;
	imencode(".jpg",fm,buf);

	string image = base64_encode((char *)buf.data(), buf.size());

	// calling dis face
	result = client.search(image, "BASE64","dis_face_lg", aip::null);
	
	float score = result["result"]["user_list"][0]["score"].asInt();
	
	cout<<"score:"<<score<<endl;
	if(score > 70)
	{
		flage = 1;
	}else{
		flage = 0;
	}
	
}


/************************************************
函数功能：子线程   负责发送数据到客户端
入口参数：无 
*************************************************/
void *sendData(void *arg)
{
	send(confd,"1",100,0);
	return 0;
}
void *sendData2(void *arg)
{
	send(confd,"0",100,0);
	return 0;
}

/************************************************
函数功能：入口函数
入口参数：无
*************************************************/
int main()
{
	VideoCapture video;
	pthread_t getFace_pthread;
	pthread_t recFace_pthread;
	pthread_t sData;
	pthread_t cData;
	
	/***********************open video*************************************/
	if(false == video.open(-1))
	{
		cout<<"open error"<<endl;
		return -1;
	}
	cout<<"open ok"<<endl;
		
	/********************************Server_init************************/
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
    confd = accept(sockfd,NULL,NULL);
    if(confd < 0)
    {
            printf("accept error\r\n");
            return -1;
    }
	cout<<"accept ok\r\n"<<endl;
	
	
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
		//cout<<"read ok"<<endl;
		
		//create pthread
		pthread_create(&getFace_pthread,NULL,getFace,(void*)&frame);
		pthread_create(&recFace_pthread,NULL,recFace,(void*)&frame);
		
		//cout<<"flage:"<<"liugang"<<endl;
		rectangle(frame,Point(left1,top),Point(left1+width,top+height),CV_RGB(255,0,0));
		imshow("text",frame);
		
		if(flage == 1)
		{
			
			pthread_create(&sData,NULL,sendData,NULL);
			//flage = 0;
		}
		if(flage == 0)
		{
			pthread_create(&cData,NULL,sendData2,NULL);
		}
		
		char c=waitKey(200);
		if(c>0)
		{
			break;
		}
	}
	return 0;
}
