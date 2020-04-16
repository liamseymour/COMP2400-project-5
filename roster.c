#include <stdio.h>
#include "Student.h"
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1024

/* Helper functions */
int compare(Student *s1, Student *s2);
int compareNames(char *first1, char *last1, char *first2, char *last2);
int parseHouseName(char *name, Student **houses);
Student *popLeftMost(Student **root);
Student *findRightMost(Student *root);

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
int add(char *first, char* last, int points, int year, int house, Student *houses[]);
int load(char *filepath, Student *houses[]);

int main(int argc, char **argv)
{
	Student *houses[5] = {NULL, NULL, NULL, NULL, NULL};

	char *line = NULL;
	size_t lineSize = 0;
	char command[MAX_LINE] = "";
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
			printf("Inorder print-out of roster:\n");
			// Print each house
			for (int i = 0; i < HOUSES; ++i) 
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
				printInOrder(houses[i]);
			}
			
			// Print deceased students
			printf("\nDeceased\n\n");
			printInOrder(houses[HOUSES]);
		}	

		// Command: preorder
		else if (strcmp(command, "preorder") == 0) 
		{
			printf("Preorder print-out of roster:\n");
			// Print each house
			for (int i = 0; i < HOUSES; ++i) 
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
				printPreOrder(houses[i]);
			}
			
			// Print deceased students
			printf("\nDeceased\n\n");
			printPreOrder(houses[HOUSES]);
		}
		
		// Command: postorder
		else if (strcmp(command, "postorder") == 0) 
		{
			printf("Postorder print-out of roster:\n");
			// Print each house
			for (int i = 0; i < HOUSES; ++i) 
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
				printPostOrder(houses[i]);
			}
			
			// Print deceased students
			printf("\nDeceased\n\n");
			printPostOrder(houses[HOUSES]);
		}
		
		// Command: add
		else if (strcmp(command, "add") == 0) 
		{
			char first[MAX_LINE];
			char last[MAX_LINE];
			int points;
			int year;
			char house[MAX_LINE];

			sscanf(line, "add %s %s %d %d %s", first, last, &points, &year, house);

			int houseNumber = parseHouseName(house, houses);

			// Invalid house name
			if (houseNumber == -1)
				printf("Invalid house name: %s\n", house);			

			// Invalid year
			else if (year > 7 || year < 1)
				printf("Invalid year: %d\n", year);			

			// Make sure that name is not in the roster already
			else if (search(houses[houseNumber], first, last) != NULL)
				printf("Add failed. Student named %s %s already present in roster.\n", 
						first, last);
			else 
			{ 
				add(first, last, points, year, houseNumber, houses);
				printf("Added %s %s to roster.\n", first, last);
			}
		} 

		// Command: kill
		else if (strcmp(command, "kill") == 0)
		{
			char first[MAX_LINE];
			char last[MAX_LINE];
			char houseName[MAX_LINE];

			sscanf(line, "kill %s %s %s", first, last, houseName);

			int houseNumber = parseHouseName(houseName, houses);

			// Invalid house name
			if (houseNumber == -1)
				printf("Invalid house name: %s\n", houseName);			
		
			else
			{
				Student *removed = delete(&houses[houseNumber], first, last);
				if (removed == NULL)
					printf("Kill failed. %s %s was not found in %s House\n", first, last, 
							houseName);
				else
				{
					removed->left = NULL;
					removed->right = NULL;
					insert(&houses[HOUSES], removed);
					printf("Moved %s %s to list of deceased students.\n", first, last);
				}
			}

		}

		// Command: load
		else if (strcmp(command, "load") == 0)
		{
			char filepath[2024];
			sscanf(line, "load %s", filepath);
			int error = load(filepath, houses); 
			if (error != 0)
				printf("Load failed.\n");
				// TODO: Fix error codes
			else
				printf("Successfully loaded data from file %s.\n", filepath);
		}

		// Non-existent command
		else if (strcmp(command, "quit") != 0) 
		{
			// Catch all other commands, not including quit.
			printf("Unknown command: %s\n", command);
			
			// Fix command remaining unchanged when user hits enter with no command.
			command[0] = '\0';
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

Student* search( Student* root, char* first, char* last )
{
	if (root == NULL)
		return NULL;
	
	int difference = compareNames(root->first, root->last, first, last);
	
	if ( difference == 0 )
		return root;	

	else if ( difference > 0 )
		return search(root->right, first, last);
	
	else	
		return search(root->left, first, last);
}

/* Delete a student with first name last name, and return that student, or NULL if not 
 * found. */
Student* delete( Student** root, char* first, char* last )
{
	// Student is not in the tree.
	if (root == NULL)
		return NULL;

	// The difference between root's name and the student's name we are searching for.
	int difference = compareNames((*root)->first, (*root)->last, first, last);

	// Student found
	if (difference == 0) 
	{
		// To return.
		Student *found = *root; 

		// Student has no children, so we simply set to NULL.
		if ((*root)->left == NULL && (*root)->right == NULL)
		{
			*root = NULL;
		}

		// Student has both children. Here we move the leftmost child of the
		// right subtree to the position of the deleted node. We move the rest of the
		// right sub tree to the rightmost of the leftmost node we found (or itself if it
		// has no right children). 
		else if ((*root)->left != NULL && (*root)->right != NULL)
		{
			// First get reference to right subtree
			Student *rightTree = (*root)->right;

			// Get the leftmost of the right subtree and disconnect it from it's parent.
			// This will replace root.
			Student *replacement = popLeftMost(&rightTree);
			replacement->left = (*root)->left;
			*root = replacement;

			// Move the right tree onto the rightmost node of replacement.
			findRightMost(replacement)->right = rightTree;
		}
		
		// Student has one child, so it takes students place.
		else 
		{
			// Disambiguate which child is not NULL.
			Student *child = (*root)->right;
			if ((*root)->left != NULL)
				child = (*root)->left;

			*root = child;
		}

		return found;
	}

	// Go right.
	else if (difference > 0)
		return delete(&(*root)->right, first, last);
	// Go left.
	else 
		return delete(&(*root)->left, first, last);
}

/* Returns the leftmost node of root (or root if it has not left child).
 * Disconnects the leftmost node from its parent. Returns NULL if root is NULL. */
Student *popLeftMost(Student **root)
{
	if ((*root)->left == NULL) {
		Student *found = *root;
		*root = NULL;
		return found;
	}

	if ((*root)->left->left == NULL)
	{
		Student *found = (*root)->left;
		(*root)->left = NULL;
		return found;
	}
	else
		return popLeftMost(&(*root)->left);
}

/* Return the rightmost node of root (or root if it has no right child).
 * Returns NULL if root is NULL. */
Student *findRightMost(Student *root)
{
	if (root == NULL)
		return NULL;

	if (root->right == NULL)
		return root;

	else
		return findRightMost(root->right);
}

int compareNames(char *first1, char *last1, char *first2, char *last2)
{
	int lastDiff = strcmp(last1, last2);
	if (lastDiff != 0)
		return lastDiff;
	else
		return strcmp(first1, first2);
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
	char name[MAX_LINE];
	sprintf(name, "%s %s", s->first, s->last);

	printf("%-24s Points: %d\tYear: %d\tHouse: %s\n", name, s->points, s->year, 
			HOUSE_NAMES[s->house]);
}

/* Parses house name into the cooresponding integer representation. If the house name is 
 * invalid, returns -1. */
int parseHouseName(char *name, Student **houses)
{
	// Get corresponding house number, leave it at -1 if none match.
	int houseNumber = -1;
	for (int i = 0; i < HOUSES; ++i)
		if (strcmp(HOUSE_NAMES[i], name) == 0)
			houseNumber = i;

	return houseNumber;
}

/* Prints an in-order traversal of the Student tree root */
void printInOrder( Student *root )
{
	if (root != NULL) 
	{
		printInOrder( root->left );
		printStudent( root );
		printInOrder( root->right );
	}
} 

/* Prints an post-order traversal of the Student tree root */
void printPostOrder( Student *root )
{
	if (root != NULL) 
	{
		printPostOrder( root->left );
		printPostOrder( root->right );
		printStudent( root );
	}
}

/* Prints an pre-order traversal of the Student tree root */
void printPreOrder( Student *root )
{
	if (root != NULL) 
	{
		printStudent( root );
		printPreOrder( root->left );
		printPreOrder( root->right );
	}
}

/* Creates and adds the given student to the roster of the given house. Does NOT check
 * for valid data or if student is already present in house.  */
int add(char *first, char* last, int points, int year, int house, Student *houses[])
{
	Student *s = malloc(sizeof(Student));
	s->first = strdup(first);
	s->last = strdup(last);
	s->points = points;
	s->year = year;
	s->house = house;
	s->left = NULL;
	s->right = NULL;
	
	if (house < HOUSES + 1)
		insert(&houses[house], s);
	 	
	return 0;
}


/* Load in students from filepath. Return error code, or 0 on success. */
int load(char *filepath, Student *houses[])
{
	FILE* in = fopen(filepath, "r");
	if (in == NULL)
		return 1;
		// TODO: Fix error codes.
	
	char first[MAX_LINE];
	char last[MAX_LINE];
	int points;
	int year;
	char house[MAX_LINE];

	char line[MAX_LINE] = "";
	while (fgets(line, MAX_LINE, in) != NULL) {
		sscanf(line, "%s %s %d %d %s", first, last, &points, &year, house);

		// Get corresponding house number, leave it at -1 if none match.
		int houseNumber = -1;
		for (int i = 0; i < HOUSES; ++i)
			if (strcmp(HOUSE_NAMES[i], house) == 0)
				houseNumber = i;

		// Invalid house name
		if (houseNumber == -1) 
		{
			printf("Invalid house name: %s\n", house);			
			return 2;
			// TODO: Fix error codes.
		}
		// Invalid year
		else if (year > 7 || year < 1)
		{
			printf("Invalid year: %d\n", year);			
			return 3;
			// TODO: Fix error codes.
		}
		// Make sure that name is not in the roster already
		else if (search(houses[houseNumber], first, last) != NULL) 
		{
			printf("Add failed. Student named %s %s already present in roster.\n", 
					first, last);
			return 4;
			// TODO: Fix error codes.
		}
		else 
			add(first, last, points, year, houseNumber, houses);
	}

	return 0;
}

