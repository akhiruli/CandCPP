#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t readCountAccess;
sem_t writeAccess;
int readcount = 0;

void reader(void *);
void writer(void *);

int main(){
   int i;
   pthread_t reader_thread[100], writer_thread[100];
   sem_init(&readCountAccess, 0, 1);
   sem_init(&writeAccess, 0, 1);
   
   for(i=0; i < 100; i++)
      pthread_create(&reader_thread[i], NULL, reader, (void *)i);

   for(i=0; i < 100; i++)
      pthread_create(&writer_thread[i], NULL, writer, (void *)i);

   for(i=0; i < 100; i++)
      pthread_join(reader_thread[i], NULL);

   for(i=0; i < 100; i++)
      pthread_join(writer_thread[i], NULL);

   sem_destroy(&readCountAccess);
   sem_destroy(&writeAccess);

   return 0;
}

void writer(void *arg){
   int temp=(int)arg;
   sem_wait(&writeAccess);
   printf("writer %d is writing some data\n", temp);
   sem_post(&writeAccess);
}

void reader(void *arg){
   int temp=(int)arg;
   sem_wait(&readCountAccess);
   ++readcount;
   if (readcount == 1){
      sem_wait(&writeAccess);
   }

   sem_post(&readCountAccess);

   printf("reader %d is reading some data\n", temp);
   sem_wait(&readCountAccess);
   --readcount;
   if(readcount == 0){
      sem_post(&writeAccess);
   }
   sem_post(&readCountAccess);
}

// compile using the command "gcc readerWriter.c -lpthread"

