/*
生产者消费者问题概念模型
线程是一个死循环,再里面不断执行语句
while(1){
  p() 消费一个buffer空间
  v()空出一个buffer空间
}
生产者
void producer(){                  void condume(){
p(empty)                                    p(ful)
  生产                                          消费
  v(full)                                       v(empty)
}                                                       }
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define M 1
int buffer=0;
sem_t empty_sem;//同步信号量,阻止生产者放产品
sem_t full_sem;//同步信号量,组织消费者消费
pthread_mutex_t mutex;//互斥信号量,一次只能有一个线程访问缓冲

//作为一个线程,打印一条语句

//生产者方法
void *product(){
//死循环,使进程不断进行
  while (1) {
    sem_wait(&empty_sem);//相当于p操作
    pthread_mutex_lock(&mutex);
    printf("生产了一个东西\n" );
    buffer=1;
    pthread_mutex_unlock(&mutex);
    sem_post(&full_sem);//相当于V操作
  }
}

void *consumer(){
  while (1) {
    sem_wait(&full_sem);//p操作
    pthread_mutex_lock(&mutex);
    printf("消费了一个东西\n");
    buffer=0;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty_sem);//v操作
  }
}


int main(void) {
  pthread_t id1;//创建线程
  pthread_t id2;//创建线程
  int ret1;
  int ret2;
  int ini1=sem_init(&empty_sem,0,1);
  int ini2=sem_init(&full_sem,0,0);
  if (ini1!=0&&ini2!=0) {
    printf("sem init failed\n");
    exit(1);
  }
  int ini3=pthread_mutex_init(&mutex,NULL);
  if(ini3!=0){
    printf("mutex init failed\n");
    exit(1);
  }
  ret1=pthread_create(&id1,NULL,product,NULL);
  if(ret1!=0)     printf("failed\n");
  ret2=pthread_create(&id2,NULL,consumer,NULL);
if(ret2!=0)     printf("failed\n");
pthread_join(id1,NULL);
pthread_join(id2,NULL);
  return 0;
}
