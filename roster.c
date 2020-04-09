#include <stdio.h>
#include "Student.h"
#include <string.h>
#include <stdlib.h>

/* Helper functions */
int compare(Student *s1, Student *s2);

/* Tree functions */
void insert( Student** root, Student* node );
Student* search( Student* root, char* first, char* last );
Student* delete( Student** root, char* first, char* last );

/* Display functions */
void printStudent( Student *s );
void printInOrder( Student *root );
void printPostOrder( Student *root );
void printPreOrder( Student *root );

/* Command functions */
int add(char *commandLine, Student *houses[]);

int main(int argc, char **argv)
{
	Student *houses[5] = {NULL, NULL, NULL, NULL, NULL};

	char *line = NULL;
	size_t lineSize = 0;
	char command[1024] = "";
	while (strcmp(command, "quit") != 0)
	{
		// Get whole command line to work with.
		printf("Enter command: ");
		getline(&line, &lineSize, stdin);

		// Get first word from command
		sscanf(line, "%s", command);

	
		// Command: inorder
		if (strcmp(command, "inorder") == 0) 
		{
			// Print each house
			for (int i = 0; i < HOUSES; ++i) 
			{
				printf("%s House\n\n\n", HOUSE_NAMES[i]);
				printInOrder(houses[i]);
			}
			
			// Print deceased students
			printf("Deceased\n\n\n");
			printInOrder(houses[HOUSES]);
		}	
		// Command: preorder
		else if (strcmp(command, "preorder") == 0) 
		{
			// Print each house
			for (int i = 0; i < HOUSES; ++i) 
			{
				printf("%s House\n\n\n", HOUSE_NAMES[i]);
				printPreOrder(houses[i]);
			}
			
			// Print deceased students
			printf("Deceased\n\n\n");
			printPreOrder(houses[HOUSES]);
		}
		// Command: postorder
		else if (strcmp(command, "postorder") == 0) 
		{
			// Print each house
			for (int i = 0; i < HOUSES; ++i) 
			{
				printf("%s House\n\n\n", HOUSE_NAMES[i]);
				printPostOrder(houses[i]);
			}
			
			// Print deceased students
			printf("Deceased\n\n\n");
			printPostOrder(houses[HOUSES]);
		}
		// Command: add
		else if (strcmp(command, "add") == 0) 
		{
			add(line, houses);
		} 
		else if (strcmp(command, "quit") != 0) 
		{
			// Catch all other commands, not including quit.
			printf("Unknown command: %s\n", command);
		}
	}
	free(line);



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
	if (root != NULL) 
	{
		printInOrder( root->left );
		printStudent( root );
		printInOrder( root->right );
	}
} 

void printPostOrder( Student *root )
{
	if (root != NULL) 
	{
		printPostOrder( root->left );
		printPostOrder( root->right );
		printStudent( root );
	}
}

void printPreOrder( Student *root )
{
	if (root != NULL) 
	{
		printStudent( root );
		printPostOrder( root->left );
		printPostOrder( root->right );
	}
}

int add(char *commandLine, Student *houses[])
{
	char first[1024];
	char last[1024];
	int points;
	int year;
	char house[1024];
	
	// Check that formating is right
	if (sscanf(commandLine, "add %s %s %d %d %s", first, last, &points, &year, house) == 5)
	{ 
		// Invalid year
		if (year > 7 || year < 1)
		{ 
			//TODO Correct error handling.
			printf("Invalid year: %d\n", year);			
			return 1;
		}

		// Get corresponding house number, leave it at -1 if none match.
		int houseNumber = -1;
		for (int i = 0; i < HOUSES; ++i)
			if (strcmp(HOUSE_NAMES[i], house) == 0)
				houseNumber = i;

		// Invalid house name
		if (houseNumber == -1)
		{ 
			//TODO Correct error handling.
			printf("Invalid house name: %s\n", house);			
			return 1;
		}
	
		Student *s = malloc(sizeof(Student));
		s->first = strdup(first);
		s->last = strdup(last);
		s->points = points;
		s->year = year;
		s->house = houseNumber;	
		s->left = NULL;
		s->right = NULL;

		insert(&houses[houseNumber], s);

		printf("Added %s %s to the roster.\n", s->first, s->last);
	}	
	return 0;
}
