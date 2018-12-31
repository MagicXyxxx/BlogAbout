/*
	cdecl.c---C语言声明解析器修改版
	author:Magic激流
	data:2018/12/31 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define MAXTOKENS 100
#define MAXTOKENLEN 64 

enum type_tag {
	IDENTIFIER,QUALIFIER,TYPE
};

struct token{
	char type;
	char string[MAXTOKENLEN];
};

int top = -1;
int argc = 0;
struct token stack[MAXTOKENS];

struct token thistoken;

#define pop stack[top--]
#define push(s) stack[++top] = s
#define STRCMP(a,R,b) (strcmp(a,b) R 0) 

enum type_tag classify_string();
void gettoken();
void read_to_first_identifier();
void deal_with_arrays();
void deal_with_function_args();
void deal_with_pointers();
void deal_with_declarator();
void check_type_or_id();
void deal_with_parameter();

void check_type_or_id(char *s)
{
	char temp[MAXTOKENLEN] = {'\0'};
	
	struct token temp_struct_one = thistoken;
	
	gettoken();
	struct token temp_struct = thistoken;
	
	gettoken();
	struct token temp_struct3 = thistoken;
			
	if(thistoken.type==IDENTIFIER)
	{
		strcat(temp,temp_struct_one.string);
		strcat(temp," called ");
		strcat(temp,temp_struct.string);
		strcpy(s,temp);
		thistoken = temp_struct3;
		strcpy(temp_struct_one.string,temp);
	}
	else
	{
		thistoken = temp_struct; 
		for(int i = strlen(temp_struct3.string)-1;i>=0;i--)
		{
			ungetc(temp_struct3.string[i],stdin);
		}
	}
	
	if(thistoken.type>=0 && thistoken.type<=2)
	{
		for(int i = strlen(thistoken.string)-1;i>=0;i--)
		{
			ungetc(thistoken.string[i],stdin);
		}
	}
	thistoken = temp_struct_one;
}

enum type_tag classify_string()
{
	char *s = thistoken.string;
	if(STRCMP(s,==,"const"))
	{
		strcpy(s,"read-only");
		return QUALIFIER;
	}
	if(STRCMP(s,==,"volatile"))
		return QUALIFIER;
	if(STRCMP(s,==,"void"))
		return TYPE;
	if(STRCMP(s,==,"char"))
		return TYPE;
	if(STRCMP(s,==,"singed"))
		return TYPE;
	if(STRCMP(s,==,"unsinged"))
		return TYPE;
	if(STRCMP(s,==,"short"))
		return TYPE;
	if(STRCMP(s,==,"int"))
		return TYPE;
	if(STRCMP(s,==,"long"))
		return TYPE;
	if(STRCMP(s,==,"float"))
		return TYPE;
	if(STRCMP(s,==,"double"))
		return TYPE;
	if(STRCMP(s,==,"struct"))
	{
		check_type_or_id(s);
		return TYPE;
	}
	if(STRCMP(s,==,"union"))
	{
		check_type_or_id(s);
		return TYPE;
	}
	if(STRCMP(s,==,"enum"))
	{
		check_type_or_id(s);
		return TYPE;
	}
	return IDENTIFIER;
}

void gettoken()
{
	char *p = thistoken.string;
	
	while((*p = getchar()) == ' ');
	
	if(isalnum(*p))
	{
		while(isalnum(*++p = getchar()));
		ungetc(*p,stdin);
		*p = '\0';
		thistoken.type = classify_string();
		return ;
	}
	
	if(*p == '*')
	{
		strcpy(thistoken.string,"pointer to");
		thistoken.type = '*';
		return ;
	}
	thistoken.string[1] = '\0';
	thistoken.type = *p;
	return ;
}

void read_to_first_identifier()
{
	gettoken();
	while(thistoken.type != IDENTIFIER)
	{
		push(thistoken);
		gettoken();
	}
	printf("%s is ",thistoken.string);
	gettoken();
}

void deal_with_arrays()
{
	while(thistoken.type == '[')
	{
		printf("array ");
		gettoken();
		if(isdigit(thistoken.string[0]))
		{
			printf("0..%d ",atoi(thistoken.string)-1);
			gettoken();
		}
		gettoken();
		printf("of ");
	}
}

void deal_with_function_args()
{
	char str[MAXTOKENLEN] = {'\0'};
	char para[MAXTOKENLEN] = {'\0'}; 
	bool flag_no_para = true;
	bool para_is_one = true;

	strcat(str,"function");

	gettoken();
	 
	if(thistoken.type != ')')
	{
		//有参数
		strcat(str," whose parameter");
		flag_no_para = false;
	}	
	while(thistoken.type != ')')
	{
		if(thistoken.string[0] == ',')
		{
			if(para_is_one == true)
			{
				strcat(str,"s are");
				para_is_one = false;
			}
				
			strcat(para," and");
		}
		else
		{
			strcat(para," ");
			strcat(para,thistoken.string);
		}	
		gettoken();
	}
	if(para_is_one == true && flag_no_para== false)
	{
		strcat(str," is");
	}
	strcat(str,para);
	gettoken();
	if(flag_no_para == true)
	{
		strcat(str," returning ");
	}
	else
	{
		strcat(str,",it returns ");
	}
	printf("%s",str); 
}

void deal_with_pointers()
{
	while(stack[top].type == '*')
	{
		printf("%s ",pop.string);
	}
}

void deal_with_declarator()
{
	switch(thistoken.type)
	{
		case '[':
			deal_with_arrays();
			break;
		case '(':
			deal_with_function_args();
			break;
	}
	
	deal_with_pointers();
	
	while(top >= 0)
	{
		if(stack[top].type == '(')
		{
			pop;
			gettoken();
			deal_with_declarator();
		}
		else
		{
			printf("%s ",pop.string);
		}
	}
}

int main (void)
{
	read_to_first_identifier(); 
	deal_with_declarator(); 
	
	return 0;	
}
