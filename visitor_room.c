#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "visitor_room.h"

typedef struct visitorNode {
	Visitor visitor;
	struct visitorNode* next;
} Node;


Result isVisitorNowInRoom(Visitor* visitor_id);
Challenge* findChallengeInRoom(ChallengeRoom* room, Level level);


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

Result num_of_free_places_for_level(ChallengeRoom *room, Level level, int *places) {
	if ( room == NULL ) {
		return NULL_PARAMETER;
	}
	*places = 0;
	int num_of_challenges = room->num_of_challenges;
	for(int i = 0; i < num_of_challenges; i++) {
		//printf("level: %u, saved: %u\n", level,room->challenges[i].challenge->level);
		if ( room->challenges[i].visitor == NULL && ( level == All_Levels || level == room->challenges[i].challenge->level) ) {
			(*places)++;
		}
	}
	return OK;
}

Result room_of_visitor(Visitor *visitor, char **room_name) {
	if ( visitor == NULL || room_name == NULL )	{
		return NULL_PARAMETER;
	}
	//printf("\n\nroom name: %s\n", *(visitor->room_name));
	//room_name = malloc(
	//TODO find room for visitor;
	*room_name = malloc( strlen(*(visitor->room_name)) + 1 );
	if (*room_name == NULL ) {
		return MEMORY_PROBLEM;
	}	
//TODO - check malloc
	strcpy(*room_name, *(visitor->room_name));
	return OK;
}

Result visitor_enter_room(ChallengeRoom *room, Visitor *visitor, Level level, int start_time) {
	if( room == NULL || visitor == NULL ) {
		return NULL_PARAMETER;
	}
	if( visitor->current_challenge != NULL ) {
		return ALREADY_IN_ROOM;
	}
	int places = 0;
	num_of_free_places_for_level(room, level, &places);
	if( places == 0 ) {
		return NO_AVAILABLE_CHALLENGES;
	}
	int index_of_challenge;	
	int num_of_challenges = room->num_of_challenges;
	for(int i = 0; i < num_of_challenges; i++) {
		//printf("level: %u, saved: %u\n", level,room->challenges[i].challenge->level);
		if ( room->challenges[i].visitor == NULL && ( level == All_Levels || level == room->challenges[i].challenge->level) ) {			index_of_challenge = i;
			break;
		}
	}
	room->challenges[index_of_challenge].start_time = start_time;
	room->challenges[index_of_challenge].visitor = visitor;
	visitor->room_name = malloc(sizeof(char**));
	visitor->room_name = &(room->name);
	visitor->current_challenge = malloc(sizeof(struct SChallengeActivity));
	visitor->current_challenge = &(room->challenges[index_of_challenge]);
	return OK;
}

/*
Result visitor_enter_room(ChallengeRoom *room, Visitor *visitor, Level level, int start_time) {
	if ( isVisitorNowInRoom(visitor) != OK ) {
		return ALREADY_IN_ROOM;
	}
	printf("\nvisitor arrived \n\n");
	findChallengeInRoom(room, level);

	return OK;
}*/



Result isVisitorNowInRoom(Visitor* visitor) {
	//TODO - check if visitor exist;	
	return OK;
}

Challenge* findChallengeInRoom(ChallengeRoom *room, Level level) {
	//TODO - find challenge in room;
	return NULL;
}




