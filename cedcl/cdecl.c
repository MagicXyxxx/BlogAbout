/*
	cdelc.c---声明解析
	摘录与《C专家编程》p75,无删改 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
struct token stack[MAXTOKENS];
struct token thistoken;

#define pop stack[top--]
#define push(s) stack[++top] = s

enum type_tag classify_string()
{
	char *s = thistoken.string;
	if(!strcmp(s,"const"))
	{
		strcpy(s,"read-only");
		return QUALIFIER;
	}
	if(!strcmp(s,"volatile"))
		return QUALIFIER;
	if(!strcmp(s,"void"))
		return TYPE;
	if(!strcmp(s,"char"))
		return TYPE;
	if(!strcmp(s,"singed"))
		return TYPE;
	if(!strcmp(s,"unsinged"))
		return TYPE;
	if(!strcmp(s,"short"))
		return TYPE;
	if(!strcmp(s,"int"))
		return TYPE;
	if(!strcmp(s,"long"))
		return TYPE;
	if(!strcmp(s,"float"))
		return TYPE;
	if(!strcmp(s,"double"))
		return TYPE;
	if(!strcmp(s,"struct"))
		return TYPE;
	if(!strcmp(s,"union"))
		return TYPE;
	if(!strcmp(s,"enum"))
		return TYPE;
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
	while(thistoken.type != '(')
	{
		gettoken();
	}
	gettoken();
	printf("function returning ");
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
