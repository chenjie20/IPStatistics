#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <sys/time.h>

const int MAX_IP_PART_NUM = 4;
const int REPEATED_TIMES = 5;
const int MAX_IP_PART_VALUE = 255;
const int MAX_MEMORY_BLOCK_LEN = 256;
const int MAX_BLOCK_ADDRESS_LEN = 256;
const int MAX_BLOCK_ELEMENT_LEN = 256;


const int MAX_RECORD_LEN = 5000000;
const int AVERAGE_REPEATED_TIMES = 100;
const int DEFAULT_VALUE_OF_K = 10;

/* allocating all memory blocks for the first layer */
int ** allocate_memory_blocks();

/* destorying all memory blocks in the first layer */
void destory_memory_blocks(int **memory_blocks);

/* randomly generating the IP addresses saved in a simulated log file, which is represented by a two dimensional array */
void generate_ip_addresses(int **ip_addresses, int max_record_len, int max_ip_part_num);

/* print ip addresses */
void print_ip_addresses(int **ip_addresses, int max_record_len, int max_ip_part_num);

/* performing the task of IP usage statistics by the traversal of all IP addresses */
void peform_statistics_ip_addresses(int 
**ip_addresses, int **memory_blocks_in_first_layer);

/* add 1 to a particular element in a sparse matrix  */
void add_1_to_a_element_in_sparse_matrix(int **memory_blocks_in_first_layer, int a, int b, int c, int d);

/* printing */
void print_all_results(int **memory_blocks_in_first_layer);

/* obtaining the count of an ip address by searching the sparse matrix */
int obtain_count_of_ip_in_sparse_matrix(int **memory_blocks_in_first_layer, int a, int b, int c, int d);

/* randomly generating ip address */
void generate_ip_addresses_by_settings(int max_record_len, int max_ip_part_num);

/* read all ip addresses from a log file */
void read_ip_addresses_from_log_file(int **ip_addresses, int max_record_len, int max_ip_part_num);

/* destorying the memory block in the second layer */
void destory_memory_blocks_in_secoond_layer(int **memory_blocks_in_first_layer);

/* adjust a heap of size n */
void adjust_min_heap(int *heap, int largest_ip_address[][MAX_IP_PART_NUM], int n, int i);

/* sorting */
void sorting_min_heap(int **memory_blocks_in_first_layer, int *heap, int first_k, int largest_ip_address[][MAX_IP_PART_NUM]);

/* the ip address of postition i is replaced by that of position j*/
void swap_ip_addresses(int largest_ip_address[][MAX_IP_PART_NUM], int i, int j);

/* adjust a heap of size n */
void adjust_new_min_heap(int *heap, int* largest_ip_address, int n, int i);

char log_file_name[] = ".\\1\\ip_log0.txt";

clock_t  start, stop;

/* main function */
int main(int argc, char *argv[])
{    
    int **ip_addresses_in_log = NULL;
    int **memory_blocks_in_first_layer = NULL; /* the first address of a spare matrix*/
    int *min_heap = NULL;    
    int largest_ip_address[DEFAULT_VALUE_OF_K+1][MAX_IP_PART_NUM];
    int first_k = DEFAULT_VALUE_OF_K;    
    char letter;

    // scanf("%c", &letter);
    // log_file_name[10] = letter;
    // printf("%s. \n", log_file_name); 

    start = clock(); 

    ip_addresses_in_log = (int **)malloc(MAX_RECORD_LEN * sizeof(int *));
    for (int i = 0; i < MAX_RECORD_LEN; ++i)
	{
		ip_addresses_in_log[i] = (int *)malloc(MAX_IP_PART_NUM * sizeof(int));
	}

    generate_ip_addresses_by_settings(MAX_RECORD_LEN, MAX_IP_PART_NUM);
    read_ip_addresses_from_log_file(ip_addresses_in_log, MAX_RECORD_LEN, MAX_IP_PART_NUM);

    /* randomly generating the IP addresses saved in a simulated log file, which is represented by a two dimensional array */
    // generate_ip_addresses(ip_addresses_in_log, MAX_RECORD_LEN, MAX_IP_PART_NUM);
    
    /* printing all ip addresses */
    //print_ip_addresses(ip_addresses_in_log, MAX_RECORD_LEN, MAX_IP_PART_NUM);

    /* allocating all memory blocks for the first layer */
    memory_blocks_in_first_layer = allocate_memory_blocks();
    
    /* performing the task of IP usage statistics by the traversal of all IP addresses */
    peform_statistics_ip_addresses(ip_addresses_in_log, memory_blocks_in_first_layer);

    /* testing a single ip address in the sparse matrix */
    //int count = obtain_count_of_ip_in_sparse_matrix(memory_blocks_in_first_layer, ip_addresses_in_log[0][0], ip_addresses_in_log[0][1], ip_addresses_in_log[0][2], ip_addresses_in_log[0][3]);
    //printf("%d.%d.%d.%d: %d.\n", ip_addresses_in_log[0][0], ip_addresses_in_log[0][1], ip_addresses_in_log[0][2], ip_addresses_in_log[0][3], count);     
                
    /* printing all results of IP usage statistics */
    //print_all_results(memory_blocks_in_first_layer);

    int memory_len = sizeof(int)*(first_k+1);
    min_heap = (int *)malloc(memory_len);
    memset(min_heap, 0, memory_len);
    sorting_min_heap(memory_blocks_in_first_layer, min_heap, first_k, largest_ip_address);
    free(min_heap);

    /* destorying all memory blocks in the second layer */
    destory_memory_blocks_in_secoond_layer(memory_blocks_in_first_layer);

    /* destorying all memory blocks in the first layer */
    destory_memory_blocks(memory_blocks_in_first_layer);

    for (int i = 0; i < MAX_RECORD_LEN; ++i)
	{
        free(ip_addresses_in_log[i]);
    }
    free(ip_addresses_in_log);

    stop = clock(); 
    double duration = ((double)(stop - start)) / CLK_TCK;
	printf("\nComputational cost: %.02f seconds.\n", duration);

    // getchar();
    // getchar();

    return 0;
}

/* randomly generating ip address */
void generate_ip_addresses_by_settings(int max_record_len, int max_ip_part_num)
{
    FILE *fp = NULL;
    int *ip_addresses = NULL;
   
    if (!access(log_file_name,0) )
    {
        /* the log file is existed. */
        return;
    }

    /* individual IP addresses */
    int individual_ip_num = max_record_len / AVERAGE_REPEATED_TIMES;
    int total_memory_len = individual_ip_num * max_ip_part_num * sizeof(int);
    ip_addresses = (int *)malloc(total_memory_len);

    srand((unsigned)time(NULL));
    for (int i = 0; i < individual_ip_num; ++i)
    {
        for (int j = 0; j < max_ip_part_num; ++j)
        {
            ip_addresses[i * max_ip_part_num + j] = rand() % MAX_IP_PART_VALUE;
        }
    } 

    /* saved in a log file */
    fp = fopen(log_file_name, "w");
    if (!fp)
    {        
        printf("fopen error: %s. \n", log_file_name); 
        return;
    }

    srand((unsigned)time(NULL));
    for (int i = 0; i < max_record_len; ++i)
    {
        int index = i;
        if(i >= individual_ip_num)
        {
            index = rand() % individual_ip_num;
        }        
        for (int j = 0; j < max_ip_part_num; ++j)
        {
            fprintf(fp, "%d ", ip_addresses[index * max_ip_part_num + j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    free(ip_addresses);
}

/* read all ip addresses from a log file */
void read_ip_addresses_from_log_file(int **ip_addresses, int max_record_len, int max_ip_part_num)
{
    FILE *fp = NULL;

    fp = fopen(log_file_name, "r");
    if (!fp)
    {        
        printf("fopen error: %s. \n", log_file_name); 
        return;
    }
    for (int i = 0; i < max_record_len; i++)
    {
        for (int j = 0; j < max_ip_part_num; j++)
        {
            fscanf(fp, "%d ", &ip_addresses[i][j]);
        }
    }

    fclose(fp);
}

/* randomly generating the IP addresses saved in a simulated log file, which is represented by a two dimensional array */
void generate_ip_addresses(int **ip_addresses, int max_record_len, int max_ip_part_num)
{    
    srand((unsigned)time(NULL));
    for (int i = 0; i < max_record_len / REPEATED_TIMES; i++)
    {
        for (int j = 0; j < max_ip_part_num; j++)
        {
            ip_addresses[i][j] = rand() % MAX_IP_PART_VALUE;                         
        }
    }
    for (int i = max_record_len / REPEATED_TIMES; i < max_record_len; i++)
    {
        int index = rand() % (max_record_len / REPEATED_TIMES - 1);
        for (int j = 0; j < max_ip_part_num; j++)
        {
            ip_addresses[i][j] = ip_addresses[index][j];
        }
    }
}

/* allocating all memory blocks for the first layer */
int ** allocate_memory_blocks()
{    
    int **memory_blocks = NULL;

    int memory_len_on_first_layer = MAX_MEMORY_BLOCK_LEN * MAX_MEMORY_BLOCK_LEN * MAX_BLOCK_ADDRESS_LEN;
    int total_len = memory_len_on_first_layer * sizeof(int**);
    memory_blocks = (int**)malloc(total_len);
    memset(memory_blocks, 0, total_len); 

    return memory_blocks;
}

/* destorying all memory blocks in the first layer */
void destory_memory_blocks(int **memory_blocks)
{    
    if(NULL != memory_blocks)
    {
        free(memory_blocks);
        memory_blocks = NULL;
    }
}

/* printing al ip addresses */
void print_ip_addresses(int **ip_addresses, int max_record_len, int max_ip_part_num)
{
    for (int i = 0; i < max_record_len; i++)
    {
        for (int j = 0; j < max_ip_part_num; j++)
        {            
            if (max_ip_part_num - 1 == j)
            {
                 printf("%d\n", ip_addresses[i][j]);        
            }
            else
            {
                 printf("%d.", ip_addresses[i][j]);        
            }               
        }
    }

}

/* performing the task of IP usage statistics by the traversal of all IP addresses */
void peform_statistics_ip_addresses(int **ip_addresses, int **memory_blocks_in_first_layer)
{     
    int a = 0, b = 0, c = 0, d = 0;

    /* add 1 to a particular element in a sparse matrix */
    for (int i = 0; i < MAX_RECORD_LEN; i++)
    {
        a = ip_addresses[i][0];
        b = ip_addresses[i][1];
        c = ip_addresses[i][2];
        d = ip_addresses[i][3];
        
        add_1_to_a_element_in_sparse_matrix(memory_blocks_in_first_layer, a, b, c, d);       
    }
}

/* add 1 to a particular element in a sparse matrix  */
void add_1_to_a_element_in_sparse_matrix(int **memory_blocks_in_first_layer, int a, int b, int c, int d)
{
    int total_len = 0, pos = 0;
    int *ip_element = NULL;

    pos = (a * MAX_MEMORY_BLOCK_LEN * MAX_BLOCK_ADDRESS_LEN + b * MAX_BLOCK_ADDRESS_LEN) + c;
    if(NULL == memory_blocks_in_first_layer[pos])
    {
        /* Allocating the memory block for the second layer according to its needs*/
        total_len = MAX_BLOCK_ELEMENT_LEN * sizeof(int);
        ip_element  = (int*)malloc(total_len);   
        memset(ip_element, 0, total_len);
        memory_blocks_in_first_layer[pos] = ip_element;           
    }
    else
    {
        ip_element = memory_blocks_in_first_layer[pos];
    }
    *(ip_element + d) += 1;
}

/* printing */
void print_all_results(int **memory_blocks_in_first_layer)
{
    int *ip_element = NULL;
    int pos = 0;

    for (int i = 0; i < MAX_MEMORY_BLOCK_LEN; i++)
    {
        for (int j = 0; j < MAX_MEMORY_BLOCK_LEN; j++)
        {
            for (int k = 0; k < MAX_BLOCK_ADDRESS_LEN; k++)
            {
                pos = (i * MAX_MEMORY_BLOCK_LEN * MAX_BLOCK_ADDRESS_LEN + j * MAX_BLOCK_ADDRESS_LEN) + k;
                if(NULL !=  memory_blocks_in_first_layer[pos])
                {
                    ip_element = memory_blocks_in_first_layer[pos];
                    for(int l = 0; l < MAX_BLOCK_ELEMENT_LEN; l++)
                    {
                        int count = *(ip_element + l);
                        if (count > 0)
                        {
                            printf("%d.%d.%d.%d: %d.\n", i, j, k, l, count);
                        }
                    }    
                }                
            }
        }
    }
    ip_element = NULL;
}

/* destorying the memory block in the second layer */
void destory_memory_blocks_in_secoond_layer(int **memory_blocks_in_first_layer)
{
    int *ip_element = NULL;
    int pos = 0;

    for (int i = 0; i < MAX_MEMORY_BLOCK_LEN; i++)
    {
        for (int j = 0; j < MAX_MEMORY_BLOCK_LEN; j++)
        {
            for (int k = 0; k < MAX_BLOCK_ADDRESS_LEN; k++)
            {
                pos = (i * MAX_MEMORY_BLOCK_LEN * MAX_BLOCK_ADDRESS_LEN + j * MAX_BLOCK_ADDRESS_LEN) + k;
                if(NULL !=  memory_blocks_in_first_layer[pos])
                {                    
                    free(memory_blocks_in_first_layer[pos]);          
                }                
            }
        }
    }
    ip_element = NULL;
}

/* obtaining the count of an ip address by searching the sparse matrix */
int obtain_count_of_ip_in_sparse_matrix(int **memory_blocks_in_first_layer, int a, int b, int c, int d)
{
    int count = 0;

    if(a > MAX_IP_PART_VALUE || b > MAX_IP_PART_VALUE || c > MAX_IP_PART_VALUE || d > MAX_IP_PART_VALUE || a < 0 || b < 0 || c < 0|| d < 0)
    {
        printf("exceptions in parameters.\n");
        
        return 0;
    }

    int pos = (a * MAX_MEMORY_BLOCK_LEN * MAX_BLOCK_ADDRESS_LEN + b * MAX_BLOCK_ADDRESS_LEN) + c;
    if(NULL !=  memory_blocks_in_first_layer[pos])
    {
        int *ip_element = memory_blocks_in_first_layer[pos];
        count = *(ip_element + d);      
    } 

    return count;
}

/* sorting */
void sorting_min_heap(int **memory_blocks_in_first_layer, int *heap, int first_k, int largest_ip_address[][MAX_IP_PART_NUM])
{    
    int *ip_element = NULL;
    int pos = 0, index = 1;

    for (int i = 0; i < MAX_MEMORY_BLOCK_LEN; ++i)
    {
        for (int j = 0; j < MAX_MEMORY_BLOCK_LEN; ++j)
        {
            for (int k = 0; k < MAX_BLOCK_ADDRESS_LEN; ++k)
            {
                pos = (i * MAX_MEMORY_BLOCK_LEN * MAX_BLOCK_ADDRESS_LEN + j * MAX_BLOCK_ADDRESS_LEN) + k;
                if(NULL !=  memory_blocks_in_first_layer[pos])
                {
                    ip_element = memory_blocks_in_first_layer[pos];
                    for(int l = 0; l < MAX_BLOCK_ELEMENT_LEN; ++l)
                    {
                        int count = *(ip_element + l);
                        if (count > 0)
                        {
                            if (index <= first_k)
                            {
                                /* init a heap of size k */
                                heap[index++] = count;  
                                largest_ip_address[index-1][0] = i;
                                largest_ip_address[index-1][1] = j;
                                largest_ip_address[index-1][2] = k;
                                largest_ip_address[index-1][3] = l;

                                //printf("%d ", count);
                                if((index - 1) == first_k)
                                {       
                                    /* printf("\n");
                                    for (int m = 1; m <= first_k; ++m) 
                                    {
                                        printf("%d ", heap[m]);		
                                    }              
                                    printf("\n");  */         
                                    for (int m = first_k/2; m > 0; --m) 
                                    {
                                        adjust_min_heap(heap, largest_ip_address, first_k, m);
                                    }
                                    /* for (int m = 1; m <= first_k; ++m) 
                                    {
                                        printf("%d ", heap[m]);		
                                    } */ 
                                }                           
                            }
                            else
                            {
                                /* adjust a heap of size k */
                                //printf("%d %d ", count, heap[1]);
                                if (count > heap[1])
                                {
                                    heap[1] = count;
                                    largest_ip_address[1][0] = i;
                                    largest_ip_address[1][1] = j;
                                    largest_ip_address[1][2] = k;
                                    largest_ip_address[1][3] = l;
                                    adjust_min_heap(heap, largest_ip_address, first_k, 1);
                                }

                            }                            
                        }
                    }                           
                }                
            }
        }
    }
    ip_element = NULL;
    printf("\nThe largest %d numbers are: \n", first_k);
	for (int i = 1; i <= first_k; ++i) 
    {        
		printf("%d.%d.%d.%d: %d.\n", largest_ip_address[i][0], largest_ip_address[i][1], largest_ip_address[i][2], largest_ip_address[i][3], heap[i]);	
        //printf("%d \n", heap[i]);	
	}
}

/* adjust a heap of size n */
void adjust_min_heap(int *heap, int largest_ip_address[][MAX_IP_PART_NUM], int n, int i)
{
    int index = 0;

	heap[0] = heap[i];
    swap_ip_addresses(largest_ip_address, 0, i);
	i <<= 1;
	while (i <= n)
    {
		if ((i < n) && (heap[i+1] < heap[i])) 
        {
			i++;
		}
		if (heap[i] >= heap[0])
        {
			break;
		}
        index = i>>1;
		heap[index] = heap[i];
        swap_ip_addresses(largest_ip_address, index, i);
		i <<= 1;
	}
    index = i>>1;
	heap[index] = heap[0];
    swap_ip_addresses(largest_ip_address, index, 0);
}

/* the ip address of postition i is replaced by that of position j*/
void swap_ip_addresses(int largest_ip_address[][MAX_IP_PART_NUM], int i, int j)
{
    largest_ip_address[i][0] = largest_ip_address[j][0];
    largest_ip_address[i][1] = largest_ip_address[j][1];
    largest_ip_address[i][2] = largest_ip_address[j][2];
    largest_ip_address[i][3] = largest_ip_address[j][3];
}