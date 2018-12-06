/*
	»ù´¡Á·Ï°¡ª¡ª16×ª8_3¡ª¡ªsscanf()°æ±¾ 
*/
#include <stdio.h>
#include <string.h>

int main (void)
{
	int n;
	scanf("%d",&n);
	getchar();
	char arr_16[n][100001];
	
	int i,j;
	int arr_len[n];
	
	for(i = 0;i<n;i++)
	{
		gets(arr_16[i]);
		arr_len[i] = strlen(arr_16[i]);
	}
	
	for(i = 0;i<n;i++)
	{
		int x = 0;
		j = arr_len[i] % 6;
		if(j != 0)
		{
			switch(j)
			{
				case 1:
					sscanf(&arr_16[i][0],"%1x",&x);
					printf("%o",x);
					break;
				case 2:
					sscanf(&arr_16[i][0],"%2x",&x);
					printf("%o",x);
					break;
				case 3:
					sscanf(&arr_16[i][0],"%3x",&x);
					printf("%o",x);
					break;
				case 4:
					sscanf(&arr_16[i][0],"%4x",&x);
					printf("%o",x);
					break;
				case 5:
					sscanf(&arr_16[i][0],"%5x",&x);
					printf("%o",x);
					break;
			}
		}
		
		for(j ;j<arr_len[i];j += 6)
		{	
			sscanf(&arr_16[i][j],"%6x",&x);
			if(j != 0)
				printf("%08o",x);
			else
				printf("%8o",x);
		}
		
		printf("\n");
	}
	
	return 0;
}
