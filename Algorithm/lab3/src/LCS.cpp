#include <iostream>
#include <string>

int LCS(char* s1, char* s2, int **b)
{
	int i,j,l1,l2,len;
	l1 = strlen(s1);
	l2 = strlen(s2);
 
	//allocate the caculate array
	int **c = new int*[l1+1]; 
	for(i = 0; i < l1+1; i++)
		c[i] = new int[l2+1];
 
    //init the array
	for(i = 0; i < l1+1; i++)
		c[i][0]=0;       
	for(j = 0; j < l2+1; j++)
		c[0][j]=0;       
    
    //use LCS algorithm
	for(i = 1; i < l1+1; i++)
	{
		for(j = 1; j < l2+1; j++)
		{
			if(s1[i-1]==s2[j-1])
			{
				c[i][j]=c[i-1][j-1]+1;
				b[i][j]=0;                  //get from (i-1,j-1)          
			}
			else if(c[i-1][j]>c[i][j-1])
			{
				c[i][j]=c[i-1][j];
				b[i][j]=1;                  //get from (i-1,j)
			}
			else
			{
				c[i][j]=c[i][j-1];
				b[i][j]=-1;                 //get from (i,j-1)
			}
		}
	}
	len=c[l1][l2];
	return len;
}

void printLCS(int **b, char *s1, int i, int j)
{
	if(i==0 || j==0)                        //finish print
		return;                         
	if(b[i][j]==0)                          //print this letter && goto(i-1,j-1)
	{
		printLCS(b, s1, i-1, j-1);
		printf("%c",s1[i-1]); 
	}
	else if(b[i][j]==1)                     //goto (i-1,j)
		printLCS(b, s1, i-1, j);
	else                                    //goto (i,j-1)
		printLCS(b, s1, i, j-1);
}
 
int main()
{
	char s1[100],s2[100];
	int i,l1,l2,len;

    //get input
	printf("Input the first string:\n");
	std::cin>>s1;
	printf("Input the second string:\n");
	std::cin>>s2;

    //allocate the direction array
	l1 = strlen(s1);
	l2 = strlen(s2);
	int **b = new int*[l1+1];
	for(i= 0; i < l1+1; i++)
		b[i] = new int[l2+1];

    //get LCS result
	len=LCS(s1,s2,b);
	printf("The LCS length is:%d\n",len);
	printf("The LCS is:");
	printLCS(b,s1,l1,l2);
	printf("\n");
	return 0;
}