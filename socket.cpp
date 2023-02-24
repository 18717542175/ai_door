#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


int main()
{
        int sockfd = socket(PF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
        {
                printf("error\n");
                return 0;
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
                return 0;

        }
        printf("bind ok\r\n");
        //jianting
        ret = listen(sockfd,1);
        if(ret<0)
        {
                printf("listen error\r\n");
                return 0;
        }
        printf("listen ok");
        //set connection
        int confd = accept(sockfd,NULL,NULL);
        if(confd < 0)
        {
                printf("accept error\r\n");
                return 0;
        }
        printf("accept ok\r\n");
        //create xiancheng
//      pthread_t video;
//      pthread_t detect;
        while(1)
        {
                char buf2[40];
                read(sockfd,buf2,sizeof(buf2)-1);
                printf("%s\n",buf2);
                char buf[100] = {0};
                fgets(buf,100,stdin);
                send(confd,buf,100,0);

        }
//      char str[] = "hello world";
//      write(sockfd,str,sizeof(str));
//      close(sockfd);
        return 0;
}
 