#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "challenge_system.h"

#define NAME_LENG 51 //Max leng 50 + 1 of end of string

typedef char* Name;

Result nameRead(Name name, FILE* inputFile);
Result numberRead(int* number, FILE* inputFile);
Result challengeRead(Challenge* challenge, FILE* inputFile);
Result challengeRoomRead(ChallengeRoom* challengeRoom, FILE* inputFile);
Challenge* findChallengeById(ChallengeRoomSystem *sys, int id);
ChallengeRoom* findRoomByName(ChallengeRoomSystem *sys, char* name);
Visitor* find_visitor_by_name(char* visitor_name);
//int isVisitorNowInRoom(ChallengeRoomSystem *sys, int visitor_id);
//Challenge* findChallengeInRoom(ChallengeRoomSystem *sys, char *room_name, Level level);




Result create_system(char *init_file, ChallengeRoomSystem **sys) {

	Result result=OK;

	FILE *file = fopen(init_file, "r");
	if (file == NULL) {
		return NULL_PARAMETER;
	}

	*sys = malloc(sizeof(sys));
	if (*sys == NULL) {
		fclose(file);
		return MEMORY_PROBLEM;
	}

  char tempName[NAME_LENG] = "";
	nameRead(tempName, file);
	//TODO - check return
	(*sys)->name = malloc(strlen(tempName));
	if ((*sys)->name == NULL) {
		free(*sys);		
		fclose(file);
		return MEMORY_PROBLEM;
	}
	//snprintf((*sys)->name, strlen(tempName)+1, "%s", tempName);
	strcpy((*sys)->name, tempName);


	// READ challenges
  int tempNumber = 0;
	numberRead(&tempNumber, file); //number of Challenges
	//TODO - check return
	(*sys)->numberOfChallenges = tempNumber;
	(*sys)->challenges = malloc(sizeof(Challenge) * (*sys)->numberOfChallenges);
	if ((*sys)->challenges == NULL) {
		free((*sys)->name);		
		free(*sys);
		fclose(file);				
		return MEMORY_PROBLEM;
	}

	int id, level;
	for(int i = 0; i < (*sys)->numberOfChallenges; i++) {
		if ( nameRead(tempName, file) == OK && \
				fscanf(file, " %d %d\n", &id, &level)) {
			result = init_challenge(&((*sys)->challenges[i]), id, tempName, level);
			if ( result == OK ) 
				continue;
				//TODO - what to do if only part succeed
		}
		free((*sys)->challenges);
		free((*sys)->name);
		free(*sys);
		fclose(file);
		return MEMORY_PROBLEM;	
	}


	// READ Challenges Rooms
	numberRead(&tempNumber, file); //number of Challenge Rooms
	//TODO - check return
	//printf("\n%d\n", tempNumber);
	(*sys)->numberOfChallengeRooms = tempNumber;	
	(*sys)->challengeRooms = \
			malloc(sizeof(ChallengeRoom) * (*sys)->numberOfChallengeRooms);
	if ((*sys)->challengeRooms == NULL) {
		for(int i = 0; i < (*sys)->numberOfChallenges; i++ ) {
			reset_challenge(&((*sys)->challenges[i]));
		}
		free((*sys)->challenges);
		free((*sys)->name);
		free(*sys);
		fclose(file);				
		return MEMORY_PROBLEM;
	}
	
	int numberOfChallenges;
	for(int i = 0; i < (*sys)->numberOfChallengeRooms; i++) {
		if ( nameRead(tempName, file) == OK && fscanf(file, " %d", &numberOfChallenges)) {
				printf("Room Name: %s & chall: %d\n", tempName, numberOfChallenges);
				result = init_room(&((*sys)->challengeRooms[i]), tempName, numberOfChallenges);
				for(int j = 0; j < numberOfChallenges; j++) {
					fscanf(file, " %d", &tempNumber);
					Challenge* challenge = findChallengeById(*sys, tempNumber);
					init_challenge_activity(&((*sys)->challengeRooms[i].challenges[j]), challenge);
					//printf("  challenge name: %s\n", chall->name);
				}	

		} 
	}
	
	(*sys)->lastTime = 0;	
	(*sys)->visitor_head = NULL;
	return OK;
}



Result visitor_arrive(ChallengeRoomSystem *sys, char *room_name, char *visitor_name, int visitor_id, Level level, int start_time) {
	if( sys == NULL) {
		return NULL_PARAMETER;
	}
	if( sys->lastTime > start_time) {
		return ILLEGAL_TIME;
	}
	if( (room_name == NULL) || (visitor_name == NULL) ) {
		return ILLEGAL_PARAMETER;
	}
  char* room = malloc(NAME_LENG);	
	Result result = system_room_of_visitor(sys, visitor_name, &room);
	if( result == ALREADY_IN_ROOM) {
		return ALREADY_IN_ROOM;
	}
	//ChallengeRoom* room = findRoomByName(sys, room_name);
	Visitor* visitor = malloc(sizeof(visitor));
	//TODO - check malloc
	result = init_visitor(visitor, visitor_name, visitor_id);
	if( result != OK) {
		return result;
	}
	
	return OK;
	//return visitor_enter_room(room, visitor, level, start_time);
}


Result system_room_of_visitor(ChallengeRoomSystem *sys, char *visitor_name, char **room_name) {
	if (sys == NULL ) {
		return NULL_PARAMETER;
	}	
	if (visitor_name == NULL || room_name == NULL) {
		return ILLEGAL_PARAMETER;
	}
	Visitor* visitor = find_visitor_by_name(visitor_name);
	if ( visitor != NULL ) {
		return ALREADY_IN_ROOM;
	}
	room_of_visitor(visitor, room_name);
	return NOT_IN_ROOM;
}



Result nameRead(char* name, FILE* inputFile) {
	if (name == NULL) {
		return NULL_PARAMETER;
	}
	if (!fscanf(inputFile,"%s", name)) {
	//if (!fgets(name, NAME_LENG, inputFile)) {
		return ILLEGAL_PARAMETER;
	}
	
	return OK;
}

Result numberRead(int* number, FILE* inputFile) {
	if (number == NULL) {
		return NULL_PARAMETER;
	}

	if (!fscanf(inputFile, "%d", number)) {
		return ILLEGAL_PARAMETER;
	}
	
	return OK;
}

Result challengeRead(Challenge* challenge, FILE* inputFile) {
	if (challenge == NULL) {
		return NULL_PARAMETER;
	}

	//char* mystring;
  //mystring = (char*)malloc(50); // set this to any size
	if (!fscanf(inputFile, "%s %d %u\n", challenge->name, &(challenge->id), &(challenge->level))) {
		return ILLEGAL_PARAMETER;
	}
	//printf("%s   %d   %i\n", challenge->name, challenge->id, challenge->level);
	
	return OK;
}

Result challengeRoomRead(ChallengeRoom* challengeRoom, FILE* inputFile) {
	if (challengeRoom == NULL) {
		return NULL_PARAMETER;
	}

	//char* mystring;
  //mystring = (char*)malloc(50); // set this to any size
	if (!fscanf(inputFile, "%s %d\n", challengeRoom->name, &(challengeRoom->num_of_challenges) )) {
		return ILLEGAL_PARAMETER;
	}
 	
	return OK;
}

Challenge* findChallengeById(ChallengeRoomSystem *sys, int id) {
	//printf("temp %d\n", (*sys).numberOfChallenges);
	int number_of_challenges = (*sys).numberOfChallenges;
	for(int i = 0; i < number_of_challenges; i++) {
		if ( (*sys).challenges[i].id == id) {
			return &((*sys).challenges[i]);
			//printf("%d %s  ",i , (*sys).challenges[i].name);
		}
	}
	printf("\n");
	return NULL;
}


ChallengeRoom* findRoomByName(ChallengeRoomSystem *sys, char* name) {
	//printf("ROOM: %s\n", name);
	int number_of_challengeRooms = (*sys).numberOfChallengeRooms;
	for(int i = 0; i < number_of_challengeRooms; i++) {
		//printf("%d %s  \n", i , (*sys).challengeRooms[i].name);
		if ( strcmp((*sys).challengeRooms[i].name, name) == 0) {
			//printf("%d %s  \n", i , (*sys).challengeRooms[i].name);
			return &((*sys).challengeRooms[i]);
		}
	}
	//printf("\n");
	return NULL;
}


Visitor* find_visitor_by_name(char* visitor_name) {
	//TODO - find visitor by name	
	return NULL;
}


/*
int isVisitorNowInRoom(ChallengeRoomSystem *sys, int visitor_id) {
	//TODO - check if visitor in the list;
	return 0;
}

Challenge* findChallengeInRoom(ChallengeRoomSystem *sys, char *room_name, Level level) {
	ChallengeRoom* room = findRoomByName(sys, room_name);
	printf("room name: %s number: %d\n", room->name, room->num_of_challenges);
	
	//TODO - find if there is challenge in the room inask level; 	
	return NULL;
}
*/


