/*
	Name:- Jai Garg
	Entry No.:- 2017CSB1081
	Date:- 07/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int page_size = 256; //2^8 bytes
int page_table_entries = 256;
int tlb_entries = 16;
int frame_size = 256;
int no_frames = 256;

int tlb_index = 0; //FIFO
int temp = 0;
int page_faults = 0;
int no_entered_addresses = 0;
int tlb_hits = 0;

void get_physical(int logical,int Page_table[page_table_entries],int tlb[tlb_entries][2],char physical_memory[no_frames][frame_size])
{
    int mask = 0xFF;
    int offset,page_no;

    page_no = (logical >> 8) & mask;
    offset = logical & mask;

    bool tlb_hit = false;
    no_entered_addresses++;


    int i,frame_no;
    for(i=0;i<tlb_entries;i++)
    {
        if(tlb[i][0] == page_no)
        {
            frame_no = tlb[i][1];
            tlb_hit = true;
            tlb_hits++;
            break;
        }
    }

    if(!tlb_hit)
    {
        if(Page_table[page_no] != -1)
        {
            frame_no = Page_table[page_no];
        }

        else
        {

        	char to_read[frame_size];

			memset(to_read,0,sizeof(to_read));

			FILE* fd = fopen("BACKING_STORE.bin","rb");
			if(!fd)
		    {
		        printf("Error opening the BACKING STORE.....Exiting program!\n");
		        exit(0);
		    }

		    if (fseek(fd, page_no * frame_size, SEEK_SET) != 0) //fseek unsuccessful
			{
				printf("Error..... Page no. not found in backing store!\n");
			}

			if (fread(to_read, sizeof(char), frame_size, fd) == 0) //fread unsuccessful
			{
				printf("Error..... Unable to read from backing store!\n");
			}

			fclose(fd);
			
			int q;
			for(q=0;q<frame_size;q++)
			{
				physical_memory[temp][q] = to_read[q];
			}

			temp++;



            frame_no = temp-1;
            Page_table[page_no] = frame_no;
            page_faults++;
        }

        tlb[tlb_index][0] = page_no;
        tlb[tlb_index][1] = frame_no;

        tlb_index++;
        tlb_index %= tlb_entries;

        int index = (frame_no*frame_size)+offset;
		int value = physical_memory[frame_no][offset];

		FILE* fd = fopen("final_output.txt", "a");
		if(!fd)
	    {
	        printf("Error opening the output file.....Exiting program!\n");
	        exit(0);
	    }

		fprintf(fd, "Virtual address: %d\tPhysical address: %d\tValue: %d\n",logical, index, value);
		fclose(fd);
	}
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Too few arguments entered.....Exiting program!\n");
        exit(0);
    }

    if(argc > 2)
    {
        printf("Too many arguments entered.....Exiting program!\n");
        exit(0);
    }


    int Page_table[page_table_entries];
    
    int i;
    
    for(i=0;i<page_table_entries;i++)
        Page_table[i] = -1;

    int tlb[tlb_entries][2]; //page no. and frame no.

    for(i=0;i<tlb_entries;i++)
    {
        tlb[i][0] = -1;
        tlb[i][1] = -1;
    }
    fclose(fopen("final_output.txt", "w"));

    FILE* fd = fopen(argv[1],"r");

    if(!fd)
    {
        printf("Error opening the input file.....Exiting program!\n");
        exit(0);
    }

    char physical_memory[no_frames][frame_size];

    int logical;

    while(fscanf(fd,"%d",&logical) == 1)
    	get_physical(logical,Page_table,tlb,physical_memory);

    fclose(fd);
    
    double page_fault_rate = (double)(page_faults)/(double)(no_entered_addresses) * 100;
    double tlb_hit_rate = (double)(tlb_hits)/(double)(no_entered_addresses) * 100;

    fd = fopen("final_output.txt", "a");
	fprintf(fd, "\nPage fault rate: %lf %%\nTLB hit rate: %lf %%\n",page_fault_rate, tlb_hit_rate);
	fclose(fd);
}
