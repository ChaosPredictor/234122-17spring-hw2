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
void find_challenge_by_id(ChallengeRoomSystem *sys,int idNumber);

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
	(*sys)->challenges = malloc(sizeof(Challenge) * tempNumber);
	if ((*sys)->challenges == NULL) {
		free((*sys)->name);		
		free(*sys);
		fclose(file);				
		return MEMORY_PROBLEM;
	}
	int id, level;
	for(int i = 0; i < tempNumber; i++) {
		if ( nameRead(tempName, file) == OK && fscanf(file, " %d %d\n", &id, &level)) {
			result = init_challenge(&((*sys)->challenges[i]), id, tempName, level);
			if ( result == OK ) 
				continue;
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
	(*sys)->challengeRooms = malloc(sizeof(ChallengeRoom) * tempNumber);
	if ((*sys)->challenges == NULL) {
		
		free((*sys)->challenges);
		free((*sys)->name);
		free(*sys);
		fclose(file);				
		return MEMORY_PROBLEM;
	}
	/*ChallengeRoom *tempChallengeRoom = malloc(sizeof(*tempChallengeRoom)); 
	if (!tempChallengeRoom) {
		free((*sys)->challengeRooms);
		free((*sys)->challenges);
		free((*sys)->name);
		free(*sys);
		fclose(file);				
		return MEMORY_PROBLEM;
	}
	tempChallengeRoom->name = tempName;*/

	for(int i = 0; i < tempNumber; i++) {
//		printf("%d\n", i);
		(*sys)->challengeRooms[i].name = tempName;
		challengeRoomRead(&((*sys)->challengeRooms[i]), file);
		result = init_room((*sys)->challengeRooms, (*sys)->challengeRooms[i].name, (*sys)->challengeRooms[i].num_of_challenges);
		if(	result != OK ) {
			//free(tempChallengeRoom);
			free((*sys)->challengeRooms);
			free((*sys)->challenges);
			free((*sys)->name);
			free(*sys);
			fclose(file);				
			return result;			
		}
		(*sys)->challengeRooms->challenges = malloc(sizeof(ChallengeActivity) * (*sys)->challengeRooms[i].num_of_challenges);
		if(	(*sys)->challengeRooms->challenges == NULL ) {
			//free(tempChallengeRoom);
			free((*sys)->challengeRooms);
			free((*sys)->challenges);
			free((*sys)->name);
			free(*sys);
			fclose(file);				
			return result;			
		} 
		int tempNumber2 = 0;
		for(int j=0; j < (*sys)->challengeRooms[i].num_of_challenges; j++) {
			//printf("dsfsdf\n");
			numberRead(&tempNumber2, file);
			//init_challenge_activity(
			find_challenge_by_id(*sys, tempNumber2);
			//init_challenge_activity((*sys)->challengeRooms->challenges[j], find_challenge_by_id(tempNumber2));	
			//printf("  %d\n", tempNumber2);
		}
	}
	//free(tempChallengeRoom);


	
	
	//challenge = temp;		
	//sys->name = temp;
	//(*sys)->name = &challenge;

	//printf("%s\n", (*sys)->name);
//	fgets(line, 100, file);
//	printf("leng of %s:%d\n", line, (int)sizeof(line));

	//*sys->name = line;

//	while (fgets(line, 100, file)) {
//		printf("%s", line);
//	}


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

void find_challenge_by_id(ChallengeRoomSystem *sys,int idNumber) {
	//printf("temp %d\n", (*sys).numberOfChallenges);
	int number_of_challenges = (*sys).numberOfChallenges;
	for(int i = 0; i < number_of_challenges; i++) {
		//printf("			%d\n"), (*sys).challenges[i].id);
		//printf("			%d\n", i);
	}
	return;
}

