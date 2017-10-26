#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"support.h"

/* hello() - print some output so we know the shared object loaded.
 *
 * The function signature takes an untyped parameter, and returns an untyped
 * parameter. In this way, the signature *could* support any behavior, by
 * passing in a struct and returning a struct. */
void *hello(void *input)
{
	printf("hello from a .so\n");
	return NULL;
}

void *ucase(struct team_t *team1)
{
	struct team_t *new_team = malloc(sizeof(struct team_t));
	new_team->name1 = uppercase(team1->name1);
	new_team->name2 = uppercase(team1->name2);
	new_team->name3 = uppercase(team1->name3);
	new_team->email1 = uppercase(team1->email1);
	new_team->email2 = uppercase(team1->email2);
	new_team->email3 = uppercase(team1->email3);
	printf("Student 1 : %s\n", new_team->name1);
	printf("Email 1   : %s\n", new_team->email1);
	printf("Student 2 : %s\n", new_team->name2);
	printf("Email 2   : %s\n", new_team->email2);
	printf("Student 3 : %s\n", new_team->name3);
	printf("Email 3   : %s\n", new_team->email3);
	free(new_team);
	printf("\n");
}