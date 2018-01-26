#include <stdio.h>
#include <stdlib.h>
#define MAX_BURST 10
#define PRIORITY_LEVELS 5
#define MAX_ARRIVAL 8
#define MAX_PROCESSES 10
#define MAX_TIME 200
#define preemptive 1
#define nonpreemptive 0
#define quantum 3

/* Define a PCB (Process Control Block) */
struct process{
	int pid;
	int burst;
	int priority;
	int arrival;
};
typedef struct process Process;

/* Process initialization */
Process process_init(int,int,int,int);
void sort_readyQ(Process [],int);

/* Scheduling Algorithm Prototypes */
void fcfs(int [],Process [],int);
void sjf(int [],int p,Process [],int);
void hpf(int [],int p,Process [],int);
void rr(int [],Process [],int);

/* Display Gantt Chart of processes */
void gannt_chart(int []);

int main(){
	int i;
	int totalburst=0;
	int pid;
	int burst;
	int priority;
	int arrival;
	int pGannt[MAX_TIME];
	int n; // number of processes
	/* Define the ready queue */
	Process readyQ[MAX_PROCESSES]; //Array is used to implement the ready queue

	/* Initialize a ready queue */
	srand(time(0));
	n = (rand() % MAX_PROCESSES) + 1;
	pid = 1;
	while (pid<=n){
		burst = (rand() % MAX_BURST) + 1;
		priority = (rand() % PRIORITY_LEVELS);
		arrival = (rand()% MAX_ARRIVAL);
		readyQ[pid-1] = process_init(pid,burst,priority,arrival);
		totalburst += readyQ[pid-1].burst;
		pid +=1;
	}
	sort_readyQ(readyQ,n); //sort by arrival time
	/* Display Processes */
	printf("Processes in the Ready Queue: \n");
	for(i=0;i<n;i++){
		printf("P[%d]: arrival time = %d, burst time = %d, priority = %d\n",readyQ[i].pid,readyQ[i].arrival,readyQ[i].burst,readyQ[i].priority);
	}
	printf("-----------------------------------------------------------\n");
	/* Start scheduling algorithms */
	printf("\nFirst Come First Served Scheduling Gannt Chart:\n");
	fcfs(pGannt,readyQ,n);
	gannt_chart(pGannt);
	printf("\nNon-Preemptive Shortest Job First Scheduling Gannt Chart:\n");
	sjf(pGannt,nonpreemptive,readyQ,n);
	gannt_chart(pGannt);
	printf("\nPreemptive Shortest Job First Scheduling Gannt Chart:\n");
	sjf(pGannt,preemptive,readyQ,n);
	gannt_chart(pGannt);
	printf("\nNon-Preemptive Priority Scheduling Gannt Chart:\n");
	hpf(pGannt,nonpreemptive,readyQ,n);
	gannt_chart(pGannt);
	printf("\nPreemptive Priority Scheduling Gannt Chart:\n");
	hpf(pGannt,preemptive,readyQ,n);
	gannt_chart(pGannt);
	printf("\nRound Robin Scheduling Gannt Chart:\n");
	rr(pGannt,readyQ,n);
	gannt_chart(pGannt);

	return 0;
}

Process process_init(int pid, int burst, int priority, int arrival){

	Process proc;
	proc.pid = pid;
	proc.burst = burst;
	proc.priority = priority;
	proc.arrival = arrival;
	return proc;
}

void sort_readyQ(Process readyQ[],int n){
	Process tempP;
	int i,j;
	for(i=0;i<n-1;i++){
		for(j=i+1;j<n;j++){
			if(readyQ[i].arrival > readyQ[j].arrival){
				tempP = readyQ[i];
				readyQ[i] = readyQ[j];
				readyQ[j] = tempP;
			}
		}
	}
}

/* Display Gannt chart */
void gannt_chart(int pGannt[]){
	int i=0;
	int j;
	int f = 0;
	int c=0;
	int pNow;
	printf("-------------------------------------------------------------\n");
	if(pGannt[0]!=-1){pNow = pGannt[0];}
	i = 0;
	while(pGannt[i]!=-1 && i<MAX_TIME){
		if(pGannt[i]==pNow){
			f = 0;
			c++;
			i++;
		}
		else{
			f=1;
		}
		if(f==1){
			printf("|");
			for(j=1;j<=((2*c-2)/2);j++){
				printf(" ");
			}
			printf("P%d",pNow);
			for(j=1;j<=((2*c-2)/2);j++){
				printf(" ");
			}
			c = 0;
			pNow = pGannt[i];
		}
	}
	printf("|");
	for(j=1;j<=((2*c-2)/2);j++){
		printf(" ");
	}
	printf("P%d",pNow);
	for(j=1;j<=((2*c-2)/2);j++){
		printf(" ");
	}
	printf("|\n");
	printf("-------------------------------------------------------------\n");
}
/* Scheduling Algorithm Implementations */
void fcfs(int pGannt[], Process readyQ[],int n){
	Process p[MAX_PROCESSES];
	int i;
	int j;
	int t;
	// clear time units (they will be loaded with pid of running processes)
	for(i=0;i<MAX_TIME;i++){
		pGannt[i]=-1;
	}
	//Copy ready queue in a temp queue
	for(i=0;i<n;i++){
		p[i]=readyQ[i];
	}
	i = 0;
	t = 0;
	while(i<n){
		if(t<p[i].arrival){
			pGannt[t]=0;
			t++;
		}
		else {
			for(j=0;j<p[i].burst;j++){
				pGannt[t+j]=p[i].pid;
			}
			t = t + j;
			i+=1;
		}
	}
}
void sjf(int pGannt[],int preemption, Process readyQ[],int n){
	Process p[MAX_PROCESSES],waiting[MAX_PROCESSES],Current_process;
	int i,j,counter=0,waiting_length=0,pos,t=0;
	// clear time units (they will be loaded with pid of running processes)
	for(i=0;i<MAX_TIME;i++){
		pGannt[i]=-1;
	}
	//Copy ready queue in a temp queue
	for(i=0;i<n;i++){
		p[i]=readyQ[i];
	}
	if(preemption == nonpreemptive)
	{
		i=0;
		counter=n;

		while(i<n || counter!=0)
		{
			if (t<p[i].arrival && i<n)
			{
				pGannt[t]=0;
				t++;
			}
			else
			{
				while (t>= p[i].arrival && i<n)
                {
                    waiting[waiting_length]=p[i];
                    waiting_length++;
                    i++;
                }

                //getting the minimum burst and its position
                Current_process=waiting[0];
                pos=0;
                for (j=1;j<waiting_length;j++)
                {
					if (Current_process.burst>waiting[j].burst)
                    {
                        Current_process=waiting[j];
                        pos=j;
                    }
                }

                //shifting the queue and deleting the current one
                for (j=pos;j<waiting_length-1;j++)
                {
                    waiting[j]=waiting[j+1];
                }
                waiting_length--;

                for(j=0;j<Current_process.burst;j++)
                {
					pGannt[t+j]=Current_process.pid;
                }
                t=t+Current_process.burst;
                counter--;

            }

        }
    }

	if(preemption == preemptive)
    {
        i=0;
        counter=n;
        Current_process.burst=0;
        while (i<n || counter!=0)
        {

            if (t < p[i].arrival && i<n && Current_process.burst ==0 && waiting_length==0 ) //idle
            {
                pGannt[t]=0;
                t++;
            }
            else
            {
                while (t>=p[i].arrival && i<n)
                {
                    waiting[waiting_length]=p[i];
                    i++;
                    waiting_length++;
                }

                //if 2 processes have the same burst, FCFS algorithm is used
                if (Current_process.burst!=0)
                {
                    for (j=waiting_length;j>pos;j--)
                        waiting[j]=waiting[j-1];
                    waiting[pos]=Current_process;
                    waiting_length++;
                }

                 //getting the minimum burst in Current_Process and its position
                Current_process=waiting[0];
                pos=0;
                for (j=1;j<waiting_length;j++)
                {
                    if (Current_process.burst>waiting[j].burst)
                    {
                        Current_process=waiting[j];
                        pos = j;
                    }
                }

                //shifting the waiting queue and deleting the current one
                for (j=pos;j<waiting_length-1;j++)
                {
                    waiting[j]=waiting[j+1];
                }
                waiting_length--;

                pGannt[t]=Current_process.pid;
                Current_process.burst--;
                t++;
                if (Current_process.burst==0) //process is done
                    counter--;

            }
        }



	}
}
void hpf(int pGannt[],int preemption,Process readyQ[],int n){
	Process p[MAX_PROCESSES],waiting [MAX_PROCESSES],Current_process;
	int i,pos=0,t=0,k,counter,waiting_length=0;
	// clear time units (they will be loaded with pid of running processes)
	for(i=0;i<MAX_TIME;i++){
		pGannt[i]=-1;
	}
	//Copy ready queue in a temp queue
	for(i=0;i<n;i++){
		p[i]=readyQ[i];
	}
	if(preemption == nonpreemptive)
    {
        i=0;
        counter=n;

        while (i<n || counter!=0)
        {

            if (t<p[i].arrival && i<n) //idle
            {
                pGannt[t]=0;
                t++;
            }

            else
            {
                while (t >= p[i].arrival && i<n)
                {
                    waiting[waiting_length]=p[i];
                    waiting_length++;
                    i++;
                }

                //the current process is the highest priority, considering that 1 is the highest
                Current_process=waiting[0];
                for (k=1;k<waiting_length;k++)
                {
                    if (Current_process.priority>waiting[k].priority)
                        Current_process=waiting[k];
                }

                for (k=0;k<Current_process.burst;k++)
                {
                    pGannt[t+k]=Current_process.pid;
                }
                counter--;
                t=t+Current_process.burst;

                //changing the implemented process's priority
                for(k=0;k<waiting_length;k++)
                {
                    if (Current_process.pid == waiting[k].pid)
                    {
                        waiting[k].priority = PRIORITY_LEVELS + 1;
                        break;
                    }

                }

            }
        }


	}
	if(preemption == preemptive)
    {
        i=0;
        counter=n;
        Current_process.burst=0;
        while (i<n || counter!=0)
        {

            if (t < p[i].arrival && i<n && Current_process.burst ==0 && waiting_length==0 ) //idle
            {
                pGannt[t]=0;
                t++;
            }
            else
            {
                while (t>=p[i].arrival && i<n)
                {
                    waiting[waiting_length]=p[i];
                    i++;
                    waiting_length++;
                }

                if (Current_process.burst!=0)
                {
                    for (k=waiting_length;k>pos;k--)
                        waiting[k]=waiting[k-1];
                    waiting[pos]=Current_process;
                    waiting_length++;
                }

                 //making the current process is the highest priority, considering that 1 is the highest
                Current_process=waiting[0];
                pos=0;
                for (k=1;k<waiting_length;k++)
                {
                    if (Current_process.priority>waiting[k].priority)
                    {
                        Current_process=waiting[k];
                        pos = k;
                    }
                }

                //shifting the waiting queue and deleting the current one
                for (k=pos;k<waiting_length-1;k++)
                {
                    waiting[k]=waiting[k+1];
                }
                waiting_length--;

                pGannt[t]=Current_process.pid;
                Current_process.burst--;
                t++;
                if (Current_process.burst==0)
                    counter--;

            }
        }

	}
}
void rr(int pGannt[],Process readyQ[],int n){
	Process p[MAX_PROCESSES],waiting[MAX_PROCESSES],Current_Process;
	int t=0,i,k,waiting_length=0;
	// clear time units (they will be loaded with pid of running processes)
	for(i=0;i<MAX_TIME;i++){
		pGannt[i]=-1;
	}
	//Copy ready queue in a temp queue
	for(i=0;i<n;i++){
		p[i]=readyQ[i];
	}

    //initialization
	i=0;
	Current_Process.burst =0;

	while (i<n || waiting_length!=0 || Current_Process.burst!=0) //no processes, no waiting and no running process
    {
        if (t< p[i].arrival && i<n && waiting_length==0 && Current_Process.burst ==0) //idle
        {
            pGannt[t]=0;
				t++;
        }
        else
        {
            while (t >= p[i].arrival && i<n)
            {
                waiting[waiting_length]=p[i];
                i++;
                waiting_length++;
            }

            if (Current_Process.burst !=0)
            {
                waiting[waiting_length]=Current_Process;
                waiting_length++;
            }

            Current_Process=waiting[0]; //always the first due to the shift

            //shifting
            for(k=0;k<waiting_length-1;k++)
            {
                waiting[k]=waiting[k+1];
            }
            waiting_length--;


            if (Current_Process.burst <= quantum )
            {
                for(k=0;k<Current_Process.burst;k++)
                {
                    pGannt[t+k]=Current_Process.pid;
                }
                t=t+Current_Process.burst;
                Current_Process.burst=0; //process is done

            }

            else
            {
                Current_Process.burst = Current_Process.burst-quantum;
                for(k=0;k<quantum;k++)
                {
                    pGannt[t+k]=Current_Process.pid;
                    }
                t=t+quantum;

            }

        }


    }

}
