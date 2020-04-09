#include <stdio.h>
#include "Student.h"

int main(int argc, char **argv)
{
    
	return 0;
}

void insert( Student** root, Student* node )
{
    if (root == NULL)
    {
        *root = node;
        
    }
        
}

Student* search( Student* root, char* first, char* last );
Student* delete( Student** root, char* first, char* last );
