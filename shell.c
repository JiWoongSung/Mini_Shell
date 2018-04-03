#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER 1000

char *arg;

char *tokptr[4];
char *one[100];
char *two[100];
char string[100];
char *back[100];
char *temp;
char history[20][100];
char *a[100];
char b[100];
char *str;
char buf[100];
char p_buf[100];
char s_buf[100];
char t_buf[100];
char redirec[100];
char cwd[255]; // 현재 버퍼를 저장 
char *ptr;  //파싱할때 사용 
char *ptr2;  //파싱할때 사용 
int count= 0;
int num=0;
int n,j;  //a의 인덱스
int pid;
char *save;
char tok[] = " \t\n";
int p[2];

int fatal(char *s)
{
  perror(s);
  exit(1);
}

int join(char *com1[], char *com2[]);
int str_len(char * str);
void ls();
void cd(char * str);
char * iscd(char *str);
void history_f(int num);
void sort();
char* blank(char *s);
void redirection(char *a[],char *file);
void redirection2(char *a[],char *file);
void redirection3(char *a[],char *file);
int redirectInput(char *a[],char *file);
char * parse_cd(char *str);
void single_pipe(char * p_buf);
void multi_join(char *com1[], char *com2[],char *pipe_buffer);//pipe


int main(void){
    while(1){
            getcwd(cwd, 255);
            printf("%s minishell$ ",cwd);
            fgets(buf,100,stdin);
       
            back:

            strcpy(p_buf,blank(buf));
            strncpy(history[num],p_buf,str_len(p_buf)); // history에 저장

            n = 0;
            str = strtok_r(buf, tok, &save);
            while (str != NULL)
            {
                  a[n++] = str;
                  str = strtok_r(NULL, tok, &save);
            }

            a[n] = (char *)0;

            printf("프로세스 아이디 %d \n", getpid());


            if(num==20){
                  sort(p_buf);
                  num=19;
            }

            char* p;


            if(strstr(p_buf,";")!=NULL){  //만약 ;가 있으면 
   
                    ptr = strtok_r(p_buf, ";",&tokptr[0]);
                    while(ptr!=NULL)  
                    {   

                            bzero(s_buf,100);
                            strcpy(s_buf,blank(ptr));
                            strcpy(t_buf,blank(ptr));
                
                            for(int j=0; j<n; j++){
                                    a[j] = (char *)0;
                            }
                            n = 0;
                            str = strtok_r(s_buf, tok, &save);
                            while (str != NULL)
                            {
                                  a[n++] = str;
                                  str = strtok_r(NULL, tok, &save);
                            }

                            a[n] = (char *)0;
                        
                            if(strcmp("cd",a[0])==0){
                                   cd(ptr);
                            }
                            else if(strcmp("history",blank(ptr))==0){
                                    history_f(num);
                            }
                            else if(strcmp("!!",blank(ptr))==0){  //이전 커맨드 재실행 
                                      if(num>=1){
                                                 memset(buf,0,strlen(buf));
                                                 strncpy(buf,history[num-1],strlen(history[num-1]));
                        
                                      goto back;
                                      }
                                      else{
                                          continue;
                                      }
                            }
                            else if(strcmp("!2",blank(ptr))==0){  //2번째 커맨드 재실행 
                                  if(num>=2){
                                            bzero(buf,strlen(buf));
                                            strncpy(buf,history[1],strlen(history[1]));
                                            goto back;
                                  }     
                                  else{
                                      continue;
                                  }

                            }
                            else if(strstr(blank(ptr),"<")!=NULL){
                                   for(int q=0; q<n; q++){
                                            a[q]=(char *)0;
                                   }

                                  ptr=strtok(t_buf, "<");
                                  ptr = strtok(NULL, " ");

                                  int j=0;
                                  temp=strtok(t_buf," ");
                                  while(temp!=NULL){

                                        a[j]=temp;

                                        j++;
                                        temp=strtok(NULL, " ");
                                  }


                                  redirectInput(a,ptr);
                                   printf("<연산 성공!!\n");

                            }  
                            else if(strstr(blank(ptr),">>")!=NULL){

                                    for(int q=0; q<n; q++){
                                            a[q]=(char *)0;
                                    }
                                    strcat(redirec,blank(ptr));

                                    ptr2= strtok(redirec," ");
                                    for(int u=0; u<n-1; u++){      
                                            ptr2=strtok(NULL," ");
                                    }


                                    ptr=strtok(t_buf, ">");
                                    ptr = strtok(NULL, " ");

                                    int j=0;

                                    temp=strtok(t_buf," ");

                                    while(temp!=NULL){
                                          a[j]=temp;
                                          j++;
                                          temp=strtok(NULL, " ");
                                    }
                                    printf("a값들'%s''%s''%s''%s'\n",a[0],a[1],a[2],a[3] );

                                    while(ptr!=NULL){

                                        printf("ptr '%s'\n",ptr );
                                        ptr=strtok(NULL, ">");
                                    }

                                    for(int q=0; q<n; q++){    
                                        printf("a[%d] '%s'\n",q,a[q]);
                                    }
           
                                    printf("ptr2 '%s'\n",ptr2);
                                    printf("a값입니다. '%s'\n",a[0]);

                                    redirection2(a,ptr2);
                              } 
                              else if(strstr(blank(ptr),">!")!=NULL){
                                      printf("첫번째 '%s'\n",blank(ptr) );
                                      for(int q=0; q<n; q++){
                                            a[q]=(char *)0;
                                      }

                                      bzero(redirec,100);
                                      strcat(redirec,blank(ptr));

                                      ptr2= strtok(redirec," ");
          
                                      for(int u=0; u<n-1; u++){ 
                                            ptr2=strtok(NULL," ");
                                      }

                                      ptr=strtok(t_buf, ">");
                                      ptr = strtok(NULL, " ");

                                      int j=0;
                                      temp=strtok(t_buf," ");
                                      while(temp!=NULL){
                                            a[j]=temp;
                                            j++;
                                            temp=strtok(NULL, " ");
                                      }

                                      printf("ptr2 %s\n",ptr2);
                                      redirection3(a,ptr2);

                              }
                              else if(strstr(blank(ptr),">")!=NULL&&strstr(blank(ptr),">>")==NULL){

                                    for(int q=0; q<n; q++){
                                        a[q]=(char *)0;
                                    }
                                    printf("blank(ptr): '%s'\n",blank(ptr) );
                                    ptr2=strtok(t_buf, ">");                                  
                                    ptr2= strtok(NULL, " ");
                                    int j=0;
                                     printf("ptr2값 '%s'\n",ptr2);
                                    temp=strtok(t_buf,">");
                                    temp=strtok(temp," ");
                                    while(temp!=NULL){

                                          a[j]=temp;
                                          j++;
                                          temp=strtok(NULL, " ");
                                    }

                                    printf("a값들'%s''%s''%s''%s'\n",a[0],a[1],a[2],a[3] );
                                    printf("ptr2값 '%s'\n",ptr2);
                                    redirection(a,ptr2);
                              }      
                              else if(strstr(blank(ptr),"|")!=NULL){  //멀티 파이프  

                                      if((pid = fork())<0){
                                           printf("fork에러\n");
                                      }

                                      else if(pid != 0){
                                          pid = wait(NULL);
                                      }            
                                      else{
                                          single_pipe(blank(ptr));
                                      }

                              }
                              else if(strstr(t_buf,"&")!=0){
        
                                      printf("backgoround\n");
                                      int t;

                                      ptr = strtok(t_buf, "&");

                                      back[0]=ptr;
                                      
                                      printf("back'%s'\n",back[0] );
                                      
                                      if((pid = fork())<0){
                                          printf("fork에러\n");
                                      }
                                      else if(pid != 0){
                                          printf("pid%d\n",getpid());

                                      }            
                                      else{
                                          execvp(back[0], back);
                                      }
                                      
      
                              }
                              else{   
                                  if((pid = fork())<0){
                                      perror("fork에러\n");
                                  }
                                  else if(pid != 0){
                                      pid = wait(NULL);
                                  } 

                                  else{

                                      printf("생성된 아이디%d\n", getpid());
                                      if(execvp(a[0], a)>0){
                                              printf("생성된 아이디%d\n", getpid());
                                              
                                      }

                                      else{
                                              printf("command not found\n");
                                              exit(1);
                                      }

                                  }
                              }

                        ptr = strtok_r(NULL, ";",&tokptr[0]);
                    }   

            }
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

            else if(strcmp(p_buf,"")==0){
                  printf("command not found\n");
                  continue;
            }

            else{ // ; 없을때
       
                  if(strcmp("cd",a[0])==0){
                          cd(p_buf);
                  }

                  else if(strcmp("history",p_buf)==0){
                          history_f(num);
                  }

                  else if(strstr(p_buf,"<")!=NULL){
                          for(int q=0; q<n; q++){
                          a[q]=(char *)0;
                          }

                          ptr=strtok(p_buf, "<");
                          ptr = strtok(NULL, " ");

                          int j=0;
                          temp=strtok(p_buf," ");

                          while(temp!=NULL){
                                  a[j]=temp;
                                  j++;
                                  temp=strtok(NULL, " ");
                          } 

                          redirectInput(a,ptr);
                          printf("<연산 수행\n");

                  }  

                  else if(strstr(p_buf,">>")!=NULL){

                          printf("첫번째 '%s'\n",p_buf );
                          for(int q=0; q<n; q++){
                                a[q]=(char *)0;
                          }
                          strcat(redirec,p_buf);

                          ptr2= strtok(redirec," ");
                          for(int u=0; u<n-1; u++){
                                ptr2=strtok(NULL," ");
                          }
                          ptr=strtok(p_buf, ">");
                          ptr = strtok(NULL, " ");

                          int j=0;
                          temp=strtok(p_buf," ");
                          while(temp!=NULL){
                                a[j]=temp;
                                j++;
                                temp=strtok(NULL, " ");
                          }


                          while(ptr!=NULL){

                                printf("ptr '%s'\n",ptr );
                                ptr=strtok(NULL, ">");
                          }

                          for(int q=0; q<n; q++){
                                printf("a[%d] '%s'\n",q,a[q]);
                          }
           
                          printf("ptr2 %s\n",ptr2);
                          printf("a값입니다.'%s'\n",a[0]);
                          redirection2(a,ptr2);
                  }

                  else if(strstr(p_buf,">!")!=NULL){
                          printf("첫번째 '%s'\n",p_buf );
                          for(int q=0; q<n; q++){
                                a[q]=(char *)0;
                          }
                          strcat(redirec,p_buf);

                          ptr2= strtok(redirec," ");
          
                          for(int u=0; u<n-1; u++){
                                ptr2=strtok(NULL," ");
                          }

                          ptr=strtok(p_buf, ">");
                          ptr = strtok(NULL, " ");

                          int j=0;
                          temp=strtok(p_buf," ");
                          while(temp!=NULL){
                                a[j]=temp;
                                j++;
                                temp=strtok(NULL, " ");
                          }

                          printf("ptr2 '%s'\n",ptr2);
                          printf("a값입니다.'%s'\n",a[0]);
                           redirection3(a,ptr2);
                  }


                  else if(strstr(p_buf,">")!=NULL&&strstr(p_buf,">>")==NULL){

                        for(int q=0; q<n; q++){
                            a[q]=(char *)0;
                        }

                        ptr=strtok(p_buf, ">");
                        ptr = strtok(NULL, " ");

                        int j=0;
                        temp=strtok(p_buf," ");
                        while(temp!=NULL){

                              a[j]=temp;

                              j++;
                              temp=strtok(NULL, " ");
                        }

                        printf("a값들'%s''%s''%s''%s'\n",a[0],a[1],a[2],a[3] );
                        printf("ptr값 '%s'\n",ptr);
                        redirection(a,ptr);
                  }

        

                  else if(strcmp("!!",p_buf)==0){  //이전 커맨드 재실행 
                        if(num>=1){
                                  bzero(buf,strlen(buf));
                                  strcat(buf,history[num-1]);
                        
                                goto back;
                        }
                        else{
                            continue;
                        }

                  }
                  else if(strcmp("!2",p_buf)==0){  //2번째 커맨드 재실행 
                        if(num>=2){
                                  bzero(buf,strlen(buf));
                                  strcat(buf,history[1]);
                        goto back;
                        }
           

                        else{
                            continue;
                        }
            
                  }


                  else if(strstr(p_buf,"|")!=NULL){  //멀티 파이프 

                          if((pid = fork())<0){
                                printf("fork에러\n");
                          }
                          else if(pid != 0){
                               pid = wait(NULL);
                          }            
                          else{
                                single_pipe(p_buf);
                          }
                          
                  }   

                  else if(strstr(p_buf,"&")!=0){
        
                          printf("backgoround\n");
                          int t;

                          ptr = strtok(p_buf, "&");

                          back[0]=ptr;

                          printf("back'%s'\n",back[0] );
          
                          if((pid = fork())<0){
                                printf("fork에러\n");
                          }
                          else if(pid != 0){
                                printf("pid%d\n",getpid());
                                continue;
                          }            
                          else{
                                execvp(back[0], back);
                          }
      
                  }

                  else{   

                          if((pid = fork())<0){
                                printf("fork에러\n");
                          }
                          else if(pid != 0){
                                pid = wait(NULL);
                                printf("프로세스 종료: %d\n",pid );
                        
                          } 

                          else{
                              printf("생성된 아이디%d\n", getpid());
                              if(execvp(a[0], a)>0){
                              printf("생성된 아이디%d\n", getpid());
                              exit(0);
                              }

                              else{
                              perror("command not found\n");
                              exit(1);
                              }
                          }
                  }

            }   
     num++;
    
    }//while끝

    return 0;
}

int str_len(char * str){   //입력한 문자열 만큼 파싱하기 위해 
    int count=0;
     while((*str)!='\0'){
        count++;
        str++;
     }

     return count;
}


void cd(char * str){
      int n,j,i=0;
      char *c;
      char *d;
      c=malloc(strlen(str));
      d=malloc(strlen(str));
      
      strcpy(d,blank(str));

     for(i=0; i<strlen(d)-2; i++){
        c[i]=d[i+2];
      }
      n=chdir(blank(c));
      if(strcmp(d,"cd")==0 || strcmp(d,"cd ~")==0){

        chdir(getenv("HOME"));
      }



     else if(n!=0&&strcmp(d,"cd")!=0){
        printf("그런 디렉토리는 없습니다.\n");
        printf("c는'%s'\n",c);
      }

}

char * iscd(char *str){
    char *c;
    c=malloc(strlen(str));

    strncpy(c,blank(str),2);
    return c;
}

char * parse_cd(char *str){
    char* p;
    bzero(b,100);
    strcat(b,blank(str));
    p = strtok(b," ");
    printf("p값%s\n",p );

    return p;             
}


void history_f(int num){
    for(int i=0; i<=num; i++){
        printf("%d  %s\n",i+1,history[i] );

    }
    printf("num: %d\n",num+1 );
}

void sort(char* s){
    printf("솔트완료\n");
    char *temp;

    for(int i=0; i<19; i++){
       temp= history[i+1];
       bzero(history[i],100);
       strcpy(history[i],temp);
    }

    bzero(history[19],100);
    strcpy(history[19],s);   
}


char* r_blank(char* s) {
  char t[100];
  char *end;
  strcpy(t, s); 
  end = t + strlen(t) - 1;
  while (end != s && isspace(*end))
    --end;
  *(end + 1) = '\0';
  s = t;

  return s;
}


char* blank(char *s) {
  char* begin;
  begin = s;

  while (*begin != '\0') {
    if (isspace(*begin))
      begin++;
    else {
      s = begin;
      break;
    }
  }

  return r_blank(s);
}


void redirection(char *a[],char *file){

   char buf[100];
   int p[2],pid, i;
   int n,fd;

   if(pipe(p)== -1){
      perror("pipe call");
      exit(1);
   }

   pid = fork();

   if(pid<0){
      perror("fork call");
      exit(1);
   }

   else if(pid ==0){
      dup2(p[1],1);
      close(p[0]);
      close(p[1]);
      execvp(a[0], a);
      exit(1);
   }
   else{
    
      close(p[1]);

     fd = creat(file, 0644);
     
                  
   bzero(buf,100);
   while((n= read(p[0], buf, sizeof(buf)))!=0){
       write(fd,buf,n);
   }
   printf("redirection완료\n");
    wait(NULL);
     
   }
}


void redirection2(char *a[],char *file){

   char buf[100];
   int p[2],pid, i;
   int n,fd;

   if(pipe(p)== -1){
      perror("pipe call");
      exit(1);
   }

   pid = fork();

   if(pid<0){
      perror("fork call");
      exit(1);
   }

   else if(pid ==0){
      dup2(p[1],1);
      close(p[0]);
      close(p[1]);
      execvp(a[0], a);
      exit(1);
   }
   else{
    
      close(p[1]);

      printf("file'%s'\n",file );
     fd = open(file, O_RDWR | O_CREAT |O_APPEND , 0644);
     
                  
   bzero(buf,100);
   while((n= read(p[0], buf, sizeof(buf)))!=0){
       write(fd,buf,n);
   }
   printf("redirection2완료\n");
    wait(NULL);
     
   }
}

void redirection3(char *a[],char *file){

   char buf[100];
   int p[2],pid, i;
   int n,fd;

   if(pipe(p)== -1){
      perror("pipe call");
      exit(1);
   }

   pid = fork();

   if(pid<0){
      perror("fork call");
      exit(1);
   }

   else if(pid ==0){
      dup2(p[1],1);
      close(p[0]);
      close(p[1]);
      execvp(a[0], a);
      exit(1);
   }
   else{
    
      close(p[1]);

     fd = open(file, O_RDWR|O_TRUNC , 0644);
      if(fd<0){
        perror("No file");
      }
      else{
        printf("redirection3완료\n");
      }
     
                  
   bzero(buf,100);
   while((n= read(p[0], buf, sizeof(buf)))!=0){
       write(fd,buf,n);
   }
    wait(NULL);
     
   }
}

int redirectInput(char *a[],char *file){

    char buf[100];
    int p[2],pid, i;
    int n,fd;
    int status;


    switch(fork()){
      case -1:
          fatal("1st fork call in join");
      case 0:
        break;
    default:
        wait(&status);
        return status;
    }


    if(pipe(p)== -1){
      perror("pipe call");
      exit(1);
    }

    pid = fork();

    if(pid<0){
      perror("fork call");
      exit(1);
    }

    else if(pid ==0){
    
      close(p[0]);
      

      bzero(buf,100);
    //  fd = open(file, O_RDWR);

      //dup2();

      /*
      while((n=read(fd, buf, sizeof(buf)))>0){
      write(1,buf,n);
      }*/
    //  printf("d가 뭘까요 @%s@ \n",buf );
      exit(1);
    }
    else{

      fd = open(file, O_RDWR);
     
      dup2(fd,0);
      close(p[0]);
      close(p[1]);

     // printf("a[0]의 값'%s'\n",a[0] );
      execvp(a[0], a);
      wait(NULL);
      return status;
      

    }
    return status;
}

void single_pipe(char* p_buf){

    char pipe_buffer[100];

    memset(pipe_buffer, '\0', 100);
    strcat(pipe_buffer,p_buf);

    char aa[10][BUFFER];  // 파이프로 구분하여 나눔 ,aa[0]에 bb에 들어갈 내용을 파싱 
    char bb[10][BUFFER];  // 첫번째 문자열 공백삭제
    char cc[10][BUFFER];  // 두번째 문자열 공백삭제  
    char dd[10][BUFFER]; 
    
    int l,f,i,j,k,d=0;


    for(f=0; f<10; f++){
       bzero(aa[f],100);
    }
    for(l=0; l<10; l++){
       bzero(bb[l],100);
    }
    for(l=0; l<10; l++){
       bzero(cc[l],100);
    }
    for(l=0; l<10; l++){
       bzero(dd[l],100);
    }



    for(f=0; f<10; f++){
       one[f]=(char *)0;
    }
    one[k+1]=(char *)0;

    for(l=0; l<10; l++){
      two[l]=(char *)0;
    }

    i=0;

    ptr = strtok(p_buf, "|");
    while(ptr!=NULL)  
    {   
      strcpy(aa[i],ptr);
      ptr = strtok(NULL, "|");
      i++;
    }

    k=0;

    ptr = strtok(blank(aa[0])," ");
    while(ptr!=NULL)  
    {   
        strcpy(bb[k],ptr);
        k++;
        ptr = strtok(NULL, " ");
    }

    j=0; 

    ptr = strtok(blank(aa[1])," ");
    while(ptr!=NULL)  
    {   
        strcpy(cc[j],ptr);
        j++;
        ptr = strtok(NULL, " ");

    }
                          
    if(aa[2]!=NULL){
        d=0;

        ptr = strtok(blank(aa[2])," ");
        while(ptr!=NULL)  
        {   
            strcpy(dd[d],ptr);
            d++;
            ptr = strtok(NULL, " ");

        }
    } 

    for(int f=0; f<k; f++){
      one[f]=bb[f];
    }
           

    for(int l=0; l<j; l++){
      two[l]=cc[l];
    }
            
    if(strcmp(dd[0],"")==0){ //싱글 
      join(one,two);
    }
    else{ //멀티
      multi_join(one,two,pipe_buffer);

    }
}
int join(char *com1[], char *com2[])//pipe
{
  int status;

  if (pipe(p) == -1)
      perror("pipe call in join");

      switch(fork()){
        case -1:
          perror("2nd fork call in join");
        case 0:
            dup2(p[1],1); //표준 출력이 파이프로 가게한다. 
            close(p[0]);
            close(p[1]);
            execvp(com1[0],com1);
            
        default:
            dup2(p[0],0);  // 표준입력이 파이프로부터 오게한다. 
            close(p[1]);
            close(p[0]);
            execvp(com2[0], com2);
            
        return status;
      }
}
void multi_join(char *com1[], char *com2[],char *pipe_buffer)//pipe
{ 
          
  if (pipe(p) == -1)
      perror("pipe call in join");

      switch(fork()){
        case -1:
          perror("2nd fork call in join");
        case 0:
            dup2(p[1],1); //표준 출력이 파이프로 가게한다. 
            close(p[0]);
            close(p[1]);
            execvp(com1[0],com1);
            
        default:
            dup2(p[0],0);  // 표준입력이 파이프로부터 오게한다. 
            close(p[1]);
            close(p[0]);
            strtok(pipe_buffer, "|");
            arg = strtok(NULL, "");
            single_pipe(arg);
            
      }
            
}



