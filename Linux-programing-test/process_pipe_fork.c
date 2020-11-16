#include<unistd.h>
#include<memory.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
int pipe_fd[2];
pid_t pid;
char buf_r[100];
char* p_wbuf;
int r_num;
memset(buf_r,0,sizeof(buf_r));//数组中的数据清0；
if(pipe(pipe_fd)<0){
printf("pipe create error\n");
return -1;
}
if((pid=fork())==0){    //SUB-PROCESS
printf("\n");   
close(pipe_fd[1]);
sleep(2);
if((r_num=read(pipe_fd[0],buf_r,100))>0){
printf("%d numbers read from be pipe is %s\n",r_num,buf_r);
}
close(pipe_fd[0]);
exit(0);

}else if(pid>0){        //MAIN-PROCESS

close(pipe_fd[0]);
if(write(pipe_fd[1],"Hello",5)!=-1)
printf("parent write success!\n");
if(write(pipe_fd[1]," Pipe",5)!=-1)
printf("parent wirte2 succes!\n");
close(pipe_fd[1]);
sleep(3);
waitpid(pid,NULL,0);
exit(0);
}
}

/*
父进程fork的以后，子进程是会继承父进程的文件描述符的，
所以父进程和子进程都有一个pipe_fd[0]，
而且两个pipe_fd[0]指向的都是同一个pipe的读端
，父进程close的是父进程的pipe_fd[0]，子进程的pipe_fd[0]并没有关闭
*/