#include <pthread.h>
#include <stdio.h>
#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3
int available[NUM_RESOURCES]={10,12,14};
int maximum[NUM_CUSTOMERS][NUM_RESOURCES]={{4,5,2},{5,7,3},{6,3,8},{5,9,7},{10,5,4}};
int allocation[NUM_CUSTOMERS][NUM_RESOURCES]={{1,1,0},{2,0,1},{0,0,2},{0,0,0},{0,0,0}};
int need[NUM_CUSTOMERS][NUM_RESOURCES]={{3,4,2},{3,7,2},{6,3,6},{5,9,7},{10,5,4}};
pthread_mutex_t mutex;
pthread_cond_t cond_var;

//Release resources
void release(int  id)
{
	available[0]+=maximum[id][0];
	available[1]+=maximum[id][1];
	available[2]+=maximum[id][2];
}
//Apply for resources
void request(int id,int input[NUM_RESOURCES])
{
	int i;
	//available resources>needed resources?
	if(input[0]>need[id][0]||input[1]>need[id][1]||input[2]>need[id][2])
		printf("what you input is over the need for %d!\n",id+1);
	//needed resources<available resources?
	else if(input[0]>available[0]||input[1]>available[1]||input[2]>available[2])
		printf("what you need is over the available!\n");
	//If the user use up all available resources and don't finish his job, then deadlock
	else if(need[id][0]>available[0]&&((available[0]-input[0])<need[(id+1)%NUM_CUSTOMERS][0]||(available[0]-input[0])<need[(id+2)%NUM_CUSTOMERS][0]||(available[0]-input[0])<need[(id+3)%NUM_CUSTOMERS][0]||(available[0]-input[0])<need[(id+4)%NUM_CUSTOMERS][0])&&need[id][1]>available[1]&&((available[1]-input[1])<need[(id+1)%NUM_CUSTOMERS][1]||(available[1]-input[1])<need[(id+2)%NUM_CUSTOMERS][1]||(available[1]-input[1])<need[(id+3)%NUM_CUSTOMERS][1]||(available[1]-input[1])<need[(id+4)%NUM_CUSTOMERS][1])&&need[id][2]>available[2]&&((available[2]-input[2])<need[(id+1)%NUM_CUSTOMERS][2]||(available[2]-input[2])<need[(id+2)%NUM_CUSTOMERS][2]||(available[2]-input[2])<need[(id+3)%NUM_CUSTOMERS][2]||(available[2]-input[2])<need[(id+4)%NUM_CUSTOMERS][2]))
	{
		
	printf("this input will make the system come into a dead_lock!\n");
	}
	//Allocate resources
	else 
	{
		for(i=0;i<3;i++)
			{
				need[id][i]=need[id][i]-input[i];
				allocation[id][i]+=input[i];
				available[i]-=input[i];
			}
		//If finish, release resources
		if(need[id][0]==0&&need[id][1]==0&&need[id][2]==0)
		{
		release(id);
		need[id][0]=need[id][1]=need[id][2]=-1;
		}
	}
}

void run(int id)
{
	int i=0;
	int input[3];
printf("available is %d %d %d\n",available[0],available[1],available[2]);
printf("   maximum    allocation   need\n");
for(i=0;i<NUM_CUSTOMERS;i++)
{
	printf("%d:%2d %2d %2d ",i+1,maximum[i][0],maximum[i][1],maximum[i][2]);
	printf("   %2d %2d %2d ",allocation[i][0],allocation[i][1],allocation[i][2]);
	printf("   %2d %2d %2d \n",need[i][0],need[i][1],need[i][2]);
}
	pthread_mutex_lock(&mutex);


{
	printf("please input %d's need(3 resources):",id+1);
	scanf("%d %d %d",&input[0],&input[1],&input[2]);
	request(id,input);
	
}
	pthread_mutex_unlock(&mutex);

sleep(rand()%3);
}

void inite(void *args)
{
	srand((int)time(NULL));
	int id=rand()%NUM_CUSTOMERS;
	while(1)
	{
		int id=rand()%NUM_CUSTOMERS;
		run(id);
	}
	return;
}


void main()
{
	int rc;
	pthread_t thread;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond_var,NULL);
	rc=pthread_create(&thread,NULL,inite,(void *)(1));
	if(rc)
	{
		printf("ERROR;RETURN CODE IS %d\n",rc);
		return ;
	}
		pthread_join(thread,NULL);
	
	return ;
}
