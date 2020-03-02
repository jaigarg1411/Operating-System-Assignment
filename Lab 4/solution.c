#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80


int find(char* input,int length,char* args[])
{
	int answer = 0;
	int l = -1,i;
	for(i=0;i<=length;i++)
	{
		if(input[i] != ' ' && input[i])
			continue;

		if(l != i-1)
		{
			args[answer] = (char*)malloc(i-l);
			if(!args[answer])
			{
				printf("osh: Memory allocation failed!\n");
				return 1;
			}

			int index = l+1,j=0;
			for(index=l+1,j=0;j<=i-l-1;j++,index++)
				args[answer][j] = input[index];
			args[answer][i-l] = '\0';
			
			answer++;
			args[answer] = NULL;
		}
		l = i;
	}
	return answer;
}



int main(void)
{
	char *args[MAX_LINE/2 + 1];
	int should_run = 1;


	char past[10][MAX_LINE+1];
	int count = 0;

	while (should_run)
	{
		printf("osh>");
		fflush(stdout);

		char input[MAX_LINE+1];

		char temp[1000];
		char in;
		int length = 0;
		while((in = getchar()) != '\n')
		{
			temp[length] = in;
			length++;
		}
		temp[length] = '\0';

		if(length > MAX_LINE)
		{
			printf("osh: Command length exceeded\n");
			continue;
		}

		int temp_index = 0;
		for(temp_index=0;temp_index<length;temp_index++)
			input[temp_index] = temp[temp_index];

		input[length] = '\0';
		

		if (strcmp(input,"!!") == 0)
		{
			if(count == 0)
			{
				printf("osh: No commands in history\n");
				continue;
			}

			int l = strlen(past[count%10]);
			int i;
			for(i=0;i<l;i++)
				input[i] = past[count%10][i];

			input[i] = '\0';
			length = l;
		}


		int arg_num;
		arg_num = find(input,length,args);

		if(!arg_num)
			continue;

		if(strcmp(args[0],"!") == 0)
		{
			int t = atoi(args[1]);

			if(t <= 0 || t+9<count || t > count)
			{
				printf("osh: No such command exist in history!\n");
				continue;
			}


			memcpy(input, past[t%10],MAX_LINE+1);
			length = strlen(input);
			arg_num = find(input,length,args);
		}


		if (strcmp(args[0],"exit") == 0)
		{
			should_run = 0;
			continue;
		}

		if (strcmp(args[0],"history") == 0)
		{
			if (count == 0)
			{
				printf("osh: No history\n");
				continue;
			}

			int i,j=10;
			for (i=count;i>0&&j>0;i--,j--) // printing maximum of last 10 commands in history
			{
				printf("%4d\t%s\n",i,past[i%10]);
			}
			count++;
			memcpy(past[count% 10],input,MAX_LINE + 1);
			continue;
		}

		count++;
		memcpy(past[count% 10],input,MAX_LINE + 1);

		int background = 0;
		if (strcmp(args[arg_num-1],"&") == 0)
		{
			background = 1;
			args[arg_num] = NULL;
			arg_num--;
		}

		pid_t pid = fork();
		if (pid < 0)
		{
			printf("osh: Fork failed\n");
			return 1;
		}

		int status;

		if (pid == 0)
		{
			status = execvp(args[0],args);
			if (status == -1)
			{
				printf("osh: Failed to execute the command\n");
			}
			return 0;
		}

		else
		{
			if(background)
				printf("pid #%d running in background %s\n",pid,input);
			
			else
				wait(&status);
		}

	}
	return 0;
}