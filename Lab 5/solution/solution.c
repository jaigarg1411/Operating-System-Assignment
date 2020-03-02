/* Name:- Jai Garg
	Entry no.:- 2017CSB1081
	Date:- 10/10/2019*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sudoku[9][9];
int result[11];
int temp_index = 2;

typedef struct
{
    int row;
    int column;
} parameters;


void* column_runner(void* params)
{
	int col_no=0,row_no=0;

	for(col_no=0;col_no<9;col_no++)
	{
		int check[9] = {0};

		for(row_no=0;row_no<9;row_no++)
		{
			if(check[sudoku[row_no][col_no] - 1] == 0)
				check[sudoku[row_no][col_no] - 1] = 1;

			else
			{
				result[1] = 0;
				pthread_exit(0);
			}
		}
	}

	result[1] = 1;
	pthread_exit(0);
}

void* row_runner(void* params)
{
	int col_no=0,row_no=0;

	for(row_no=0;row_no<9;row_no++)
	{
		int check[9] = {0};

		for(col_no=0;col_no<9;col_no++)
		{
			if(check[sudoku[row_no][col_no] - 1] == 0)
				check[sudoku[row_no][col_no] - 1] = 1;

			else
			{
				result[0] = 0;
				pthread_exit(0);
			}
		}
	}

	result[0] = 1;
	pthread_exit(0);
}

void* subgrid_runner(void* params)
{
	parameters *temp = (parameters*)params;
    int row_no = temp->row;
    int col_no = temp->column;

    int check[9] = {0};

    int i,j;
    for(i=row_no;i<row_no+3;i++)
    {
    	for(j=col_no;j<col_no+3;j++)
    	{
    		if(check[sudoku[i][j] - 1] == 0)
    			check[sudoku[i][j] - 1] = 1;

    		else
    		{
    			result[temp_index] = 0;
    			temp_index++;
    			pthread_exit(0);
    		}
    	}
    }

    result[temp_index] = 1;
    temp_index++;
    pthread_exit(0);
}



int main()
{
	printf("Enter sudoku board!\n");
	int i,j;
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			scanf("%d",&sudoku[i][j]);
		}
	}

	for(i=0;i<11;i++)
		result[i] = 0;

	
	pthread_t check_row,check_col;
	pthread_t subgrid1,subgrid2,subgrid3,subgrid4,subgrid5,subgrid6,subgrid7,subgrid8,subgrid9;


	parameters *arr[11];

	for(i=0;i<11;i++)
	{
		arr[i] = (parameters*)malloc(sizeof(parameters));
	}

	arr[0]->row = 0;
	arr[0]->column = 0;
	
	arr[1]->row = 0;
	arr[1]->column = 0;
	
	arr[2]->row = 0;
	arr[2]->column = 0;

	arr[3]->row = 0;
	arr[3]->column = 3;

	arr[4]->row = 0;
	arr[4]->column = 6;

	arr[5]->row = 3;
	arr[5]->column = 0;

	arr[6]->row = 3;
	arr[6]->column = 3;

	arr[7]->row = 3;
	arr[7]->column = 6;

	arr[8]->row = 6;
	arr[8]->column = 0;

	arr[9]->row = 6;
	arr[9]->column = 3;

	arr[10]->row = 6;
	arr[10]->column = 6;


	pthread_create(&check_row,NULL,row_runner,(void*)arr[0]);
	pthread_create(&check_col,NULL,column_runner,(void*)arr[1]);
	pthread_create(&subgrid1,NULL,subgrid_runner,(void*)arr[2]);
	pthread_create(&subgrid2,NULL,subgrid_runner,(void*)arr[3]);
	pthread_create(&subgrid3,NULL,subgrid_runner,(void*)arr[4]);
	pthread_create(&subgrid4,NULL,subgrid_runner,(void*)arr[5]);
	pthread_create(&subgrid5,NULL,subgrid_runner,(void*)arr[6]);
	pthread_create(&subgrid6,NULL,subgrid_runner,(void*)arr[7]);
	pthread_create(&subgrid7,NULL,subgrid_runner,(void*)arr[8]);
	pthread_create(&subgrid8,NULL,subgrid_runner,(void*)arr[9]);
	pthread_create(&subgrid9,NULL,subgrid_runner,(void*)arr[10]);

	pthread_join(check_row,NULL);
	pthread_join(check_col,NULL);
	pthread_join(subgrid1,NULL);
	pthread_join(subgrid2,NULL);
	pthread_join(subgrid3,NULL);
	pthread_join(subgrid4,NULL);
	pthread_join(subgrid5,NULL);
	pthread_join(subgrid6,NULL);
	pthread_join(subgrid7,NULL);
	pthread_join(subgrid8,NULL);
	pthread_join(subgrid9,NULL);


	int temp = 1;

	for(i=0;i<11;i++)
	{
		if(!result[i])
		{
			temp = 0;
			break;
		}
	}

	if(temp)
		printf("Sudoku solved!\n");

	else
		printf("Sudoku unsolved!\n");


	return 0;
}