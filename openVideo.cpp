#include<iostream>
#include<opencv2/opencv.hpp>
#include <jsoncpp/json/json.h>


using namespace std;
using namespace Json;

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
		
		
		char c=waitKey(200);
		if(c>0)
		{
			break;
		}
	}
	return 0;
}
