#include <stdio.h>
#include "Student.h"
#include <string.h>
#include <stdlib.h>

int compare(Student *s1, Student *s2);

void insert( Student** root, Student* node );
Student* search( Student* root, char* first, char* last );
Student* delete( Student** root, char* first, char* last );
void printInOrder( Student *root );
void printStudent( Student *s );

int main(int argc, char **argv)
{
	Student *houses[5] = {NULL, NULL, NULL, NULL};

	char command[1024] = "";
	while (strcmp(command, "quit") != 0)
	{
		//printf();
	}


	return 0;
}

void insert( Student** root, Student* node )
{
    if (*root == NULL)
    {
        *root = node;
		return;
    }
	int difference = compare(node, *root);
	if ( difference < 0) 
	{
		// left
		insert(&(*root)->left, node);
	} 
	else 
	{
		// right
		insert(&(*root)->right, node);
	}
}

int compare(Student *s1, Student *s2)
{
	int lastDiff = strcmp(s1->last, s2->last);
	if (lastDiff != 0)
		return lastDiff;
	else
		return strcmp(s1->first, s2->first);
}

void printStudent( Student *s )
{
	char name[1024];
	sprintf(name, "%s %s", s->first, s->last);

	printf("%-25s Points: %d\tYear: %d\tHouse: %s\n", name, s->points, s->year, 
			HOUSE_NAMES[s->house]);
}

void printInOrder( Student *root )
{
	if (root != NULL) {
		printInOrder( root->left );
		printStudent(root);
		printInOrder( root->right );
	}
} 
