#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge.h"

Result init_challenge(Challenge *challenge, int id, char *name, Level level) {
	challenge->name = malloc(sizeof(char) * (strlen(name) + 1));
	if ( challenge->name == NULL) {
		return MEMORY_PROBLEM;
	} 
	strcpy(challenge->name, name);
	challenge->id = id;
	challenge->level = level;
	challenge->best_time = 0;
	challenge->num_visits = 0
	return OK;
}
/* IMPLEMENT HERE ALL WHAT IS NEEDED */


Result reset_challenge(Challenge *challenge) {
	if ( challenge == NULL) {
		return NULL_PARAMETER;
	}
	free(challenge->name);
	return OK;
}
