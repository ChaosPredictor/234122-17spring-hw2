#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge.h"

Result init_challenge(Challenge *challenge, int id, char *name, Level level) {
	if ( name == NULL || challenge == NULL) {
		return NULL_PARAMETER;
	} 
	challenge->name = malloc(sizeof(char) * (strlen(name) + 1));
	if ( challenge->name == NULL) {
		return MEMORY_PROBLEM;
	} 
	strcpy(challenge->name, name);
	challenge->id = id;
	challenge->level = level;
	challenge->best_time = 0;
	challenge->num_visits = 0;
	return OK;
}

Result reset_challenge(Challenge *challenge) {
	if ( challenge == NULL) {
		return NULL_PARAMETER;
	}
	free(challenge->name);
	return OK;
}

Result change_name(Challenge *challenge, char *name) {
	if ( name == NULL || challenge == NULL) {
		return NULL_PARAMETER;
	} 
	challenge->name = realloc(challenge->name, sizeof(char) * (strlen(name) + 1));
	if ( challenge->name == NULL) {
		return MEMORY_PROBLEM;
	} 
	strcpy(challenge->name, name);
	return OK;

}

Result set_best_time_of_challenge(Challenge *challenge, int time) {
	if ( challenge == NULL) {
		return NULL_PARAMETER;
	}
  if ( time < 0 || time < challenge->best_time ) {
		return ILLEGAL_PARAMETER;
	}
	challenge->best_time = time;	
	return OK;
}

Result best_time_of_challenge(Challenge *challenge, int *time) {
	return OK;
}
