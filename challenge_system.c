#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"

Result create_system(char *init_file, ChallengeRoomSystem **sys) {
	printf("abcdef\n");

	*sys = malloc(sizeof(sys));

	if (*sys == NULL)
		return MEMORY_PROBLEM;

	FILE *file = fopen("test_1.txt", "r");
	if (file == NULL)
		return ILLEGAL_PARAMETER;

  char line[100];
	fgets(line, 100, file);
	printf("leng of %s:%d\n", line, (int)sizeof(line));

	//*sys->name = line;

	while (fgets(line, 100, file)) {
		printf("%s", line); 
	}


	//printf("Retrieved line of length %zu :\n", read);
	//printf("%s", line);
/*
	while ((read = getline(&line, &len, fp)) != -1) {
		printf("%s", line);
	}




*/
    //fclose(file);
    //if (line)
    //   free(line);

	return OK;
}
