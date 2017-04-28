#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "visitor_room.h"


Result init_room(ChallengeRoom *room, char *name, int num_challenges) {
	//printf("in init: %s   %d\n", name, num_challenges);

	if ( name == NULL || room == NULL) {
		return NULL_PARAMETER;
	} 

	room->name = malloc(sizeof(char) * (strlen(name) + 1));
	if ( room->name == NULL) {
		return MEMORY_PROBLEM;
	} 
	strcpy(room->name, name);
	room->num_of_challenges = num_challenges;
	
	room->challenges = malloc(sizeof(ChallengeActivity) * num_challenges);
	if ( room->challenges == NULL) {
		free(room->name);
		return MEMORY_PROBLEM;
	} 
	return OK;
}


Result reset_room(ChallengeRoom *room) {
	if ( room == NULL) {
		return NULL_PARAMETER;
	}
	free(room->challenges);
	free(room->name);
	return OK;
}
