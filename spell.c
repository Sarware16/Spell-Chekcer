#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spell.h"

#define min(x, y) (x > y) ? y : x
#define MAX_LINELEN 2000
#define MAX_WORD_LEN 100


/* You can write helper functions here */

int BinarySearch(char *dict[], char *word, int n){
    int left = 0;
    int right = n - 1;
    
    while(left <= right){
        
        int mid = left + (right -left)/ 2;
        int comparison = strcasecmp(word,dict[mid]);
        
        
        if(comparison == 0){
            return mid;
        }
        
        else if(comparison>0){
            left = mid + 1;
        }
        
        else{
            right = mid -1;
        }
        
        
    }
    
    return -1;
    
    
}

int cmpstr(const void* string1_void, const void* string2_void)
{
    return strcmp(*(char**)string1_void,*(char**)string2_void);
}

int min_dist(char *dictionary[], char* word, int dictsize){
    int result = 100;
    
    for(int i = 0; i < dictsize; i++){
        int temp = edit_distance(dictionary[i],word,0);
        if(temp<=result){
            result = temp;
        }
    }
    
    return result;
}



int edit_distance(char * first_string, char * second_string, int print_table){
	
	int m, n;

	m = strlen(first_string);
	n = strlen(second_string);

	int c[m+1][n+1];
	
	memset(c, 0, sizeof(c));

	for(int i =0; i<m+1; i++){
	    c[i][0] = i;
	}

	for(int j=0; j<n+1; j++){
	    c[0][j] = j;
	}
	
	for(int i = 1; i<m+1; i++){
	    for(int j = 1; j<n+1; j++){
			if (first_string[i - 1] == second_string[j - 1]){
				c[i][j] = c[i - 1][j - 1];
			}
			else{
				c[i][j] = min(min((c[i - 1][j] + 1), (c[i][j - 1] + 1)), (c[i - 1][j - 1] + 1));
			}
	    }
	}

    if(print_table==1){
        printf("\n");
        for (int i = -1; i < m + 1; i++)
        {
            for (int j = -1; j < n + 1; j++)
            {
        
                if((i==-1)&&(j==-1)){
                    printf("%5c|",'\0');
                }
                else if((i==0)&&(j==-1)){
                    printf("%5c|",'\0');
                }
                else if((i==-1)&&(j==0)){
                    printf("%5c|", '\0');
                }
                else if((i==-1)&&(j>0)){
                    printf("%4c|", second_string[j-1]);
                }
                else if((i>0)&&(j==-1)){
                    printf("%4c|",first_string[i-1]);
                }
                else printf("%4d|", c[i][j]);
            }
            printf("\n");
            
            for(int i=1; i<=(n+2)*5; i++){
                printf("-");
            }
            printf("\n");
        }
        
        printf("\n");   
    }
    
    
    return c[m][n];
}
	  



void spell_check(char * testname, char * dictname, int printOn){
	
	printf("Enter output file name: ");
	char outputfilename[MAX_WORD_LEN];
	scanf("%s", outputfilename);
	
	
	int textsize = 0;
	int i = 0;
	
	FILE *fptr  = fopen(testname, "r");
	
	if(fptr == NULL)
    {
      printf("Error!");   
      exit(1);             
    }
   
    char buff[MAX_LINELEN];
    
    fgets(buff, MAX_LINELEN, fptr);
	fclose(fptr);
	
	char buff2[MAX_LINELEN];
	strcpy(buff2, buff);
	
	char delim[] = " ,?.";

	char *ptr = strtok(buff, delim);

	while(ptr != NULL)
	{
		textsize++;
		ptr = strtok(NULL, delim);
	}
	

	ptr = strtok(buff2, delim);
	char *textwords[textsize];

	while(ptr != NULL)
	{
		textwords[i++] = ptr;
		ptr = strtok(NULL, delim);
		
	}

	
	

    char line[MAX_WORD_LEN + 1];
    int dictsize = 0;

    FILE *dict_fptr = fopen(dictname, "r");

    if(dict_fptr == NULL)
    {
        printf("Error!");
        return;
    }

    fgets(line, MAX_WORD_LEN + 1, dict_fptr);

    dictsize = atoi(line);

    char *dictionary[dictsize];

    for (int i = 0; i < dictsize; i++)
    {
        dictionary[i] = (char *)malloc(sizeof(char) * (MAX_WORD_LEN + 1));
    }

    for (int i = 0; (i < dictsize) && (fgets(dictionary[i], MAX_WORD_LEN + 1, dict_fptr) != NULL); i++)
    {
    }

    fclose(dict_fptr);

    for (int i = 0; (i < dictsize); i++)
    {
        dictionary[i][strlen(dictionary[i])-1] = '\0';
    }
    
    qsort((void *)dictionary, dictsize, sizeof(char*), cmpstr);
    
    


	
	for(int i= 0; i<textsize; i++){
        int choice;
        char correction[MAX_WORD_LEN];
        int min_indexes[100];
        int count = 0;
	    int search_result = BinarySearch(dictionary, textwords[i], dictsize);
	    
	    
	    if(search_result==-1){
	       int mindistance = min_dist(dictionary, textwords[i], dictsize);
	       
	       for(int j=0; j<dictsize; j++){
	           int temp = edit_distance(dictionary[j], textwords[i], 0);
	           if(temp==mindistance){
	               min_indexes[count++] = j;
	           }
	       }
	       
	       printf("\n-->|%s|", textwords[i]);
	       printf("\n-1: Type Correction");
	       printf("\n0: Leave it as is");
	       printf("\nMinimum distance: %d", mindistance);
	       printf("\nWords that give minimum distance: ");
	     
	       for(int z=1; z<=count; z++){
	           printf("\n%d: %s", z, dictionary[min_indexes[z]]);
	       }
	       
	       
	       printf("\nEnter your choice(from above options): ");
	       scanf("%d", &choice);
	       
	       if(choice == -1){
	           printf("\nType correction: ");
	           scanf("%s", correction);
	           strcpy(textwords[i], correction);

	       }
	       else if(choice == 0){
	           
	       }
	       else if(choice<count){
	           strcpy(textwords[i],dictionary[min_indexes[choice]]);
	       }            
	    }
	    
	    else{
	        printf("\n-->|%s|\nOK!",textwords[i]);
	    }
	    
	    
	    
	    
	}
    

    // open the file for writing
    FILE *fp = fopen(outputfilename, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", outputfilename);
        exit(1);
    }

    for (int i = 0; i < textsize; i++)
    {
        fprintf(fp, "%s ", textwords[i]);
    }

    fclose(fp);
    
    
    
    for (int i = 0; i < dictsize; i++)
    {
        free(dictionary[i]);
    }
	
}

