#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "visitor_room.h"

Result isVisitorNowInRoom(Visitor* visitor_id);
Challenge* findChallengeInRoom(ChallengeRoom* room, Level level);

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


Result init_visitor(Visitor *visitor, char *name, int id) {
	if( visitor == NULL || name == NULL ) {
		return NULL_PARAMETER;
	}
	visitor->visitor_name = malloc(strlen(name));
	if (visitor->visitor_name == NULL) {
		return MEMORY_PROBLEM;
	}
	strcpy(visitor->visitor_name, name);

	visitor->room_name = NULL;
	visitor->current_challenge = NULL;
	
	//TODO - add visitor to list;	
	return OK;
}

Result reset_visitor(Visitor *visitor) {
	if( visitor == NULL ) {
		return NULL_PARAMETER;
	}
	free(visitor->visitor_name);
	return OK;
}

Result init_challenge_activity(ChallengeActivity *activity, Challenge *challenge) {
	if ( activity == NULL || challenge == NULL) {
		return NULL_PARAMETER;
	}
	activity->challenge = challenge;
	return OK;
}

Result reset_challenge_activity(ChallengeActivity *activity) {
	if ( activity == NULL) {
		return NULL_PARAMETER;
	}
	free(activity->challenge);
	free(activity->visitor);
	return OK;
}

Result visitor_enter_room(ChallengeRoom *room, Visitor *visitor, Level level, int start_time) {
	if ( isVisitorNowInRoom(visitor) != OK ) {
		return ALREADY_IN_ROOM;
	}
	printf("\nvisitor arrived \n\n");
	findChallengeInRoom(room, level);

	return OK;
}


Result isVisitorNowInRoom(Visitor* visitor) {
	//TODO - check if visitor exist;	
	return OK;
}

Challenge* findChallengeInRoom(ChallengeRoom *room, Level level) {
	//TODO - find challenge in room;
	return NULL;
}




