#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "challenge_system.h"

#define ASSERT(test_number, test_condition)  \
   if (!(test_condition)) {printf("\nTEST %s FAILED", test_number); } \
   else printf("\nTEST %s OK", test_number);

#define MAX_NAME_LENG 51 //Max leng 50 + 1 of end of string


int main(int argc, char **argv)
{
	ChallengeRoomSystem *sys=NULL;
	Result r=OK;

	r=create_system("test_2.txt", &sys);

	ASSERT("1.0a\n" , r==NULL_PARAMETER)	

	r=create_system("test_1.txt", &sys);
	
	//print system name
	printf("system name:%s leng:%lu\n", sys->name, strlen(sys->name));	

	// print system challenges
	for(int i = 0; i < sys->numberOfChallenges; i++) {
		printf("challenge id:%d name:%s level:%u\n", sys->challenges[i].id, sys->challenges[i].name, sys->challenges[i].level);	
	}

	// print system challenge rooms
	for(int i = 0; i < sys->numberOfChallengeRooms; i++) {
		ChallengeRoom* room = &(sys->challengeRooms[i]);
		int num_of_challenges = room->num_of_challenges;
		printf("room name:%s nmuber of challenges in the room:%d\n", room->name, num_of_challenges);
		for(int j = 0; j < room->num_of_challenges; j++) {
			Challenge* challenge = room->challenges[j].challenge;
			printf("	challenge name:%s chall level:%u \n", challenge->name, challenge->level);
		}		
	}


	ASSERT("1.0b\n" , r==OK)
	
	r = init_challenge(NULL, 2, NULL, 1);
  ASSERT("2.0a - init_challenge" , r==NULL_PARAMETER)	
	r = init_challenge(NULL, 2, "name", 1);
  ASSERT("2.0b - init_challenge" , r==NULL_PARAMETER)	

	Challenge *challenge = malloc(sizeof(Challenge));
	//TODO - free
	if (challenge != NULL) {
		r = init_challenge(challenge, 2, NULL, 1);
		ASSERT("2.0c - init_challenge" , r==NULL_PARAMETER)
		r = init_challenge(challenge, 2, "name", 1);
		ASSERT("2.0d - init_challenge" , r==OK)
	}

	r = reset_challenge(NULL);
	ASSERT("2.1a - reset_challenge" , r==NULL_PARAMETER)
	r = reset_challenge(challenge);
	ASSERT("2.1b - reset_challenge" , r==OK)

	r = init_room(NULL, NULL, 1);
  ASSERT("2.2a - init_room" , r==NULL_PARAMETER)	
	r = init_room(NULL, "name", 1);
  ASSERT("2.2b - init_room" , r==NULL_PARAMETER)	

	ChallengeRoom *room = malloc(sizeof(ChallengeRoom));
	//TODO - free	
	if (challenge != NULL) {
		r = init_room(room, NULL, 1);
		ASSERT("2.2c - init_room" , r==NULL_PARAMETER)
		r = init_room(room, "name", 1);
		ASSERT("2.2d - init_room" , r==OK)
	}
		

	r = reset_room(NULL);
	ASSERT("2.3a - reset_room" , r==NULL_PARAMETER)
	r = reset_room(room);
	ASSERT("2.3b - reset_room" , r==OK)

	//TODO init_challenge_activity tests
/*
	r = init_challenge_activity(NULL, 2, NULL, 1);
  ASSERT("2.4a - init_challenge" , r==NULL_PARAMETER)	
	r = init_challenge_activity(NULL, 2, "name", 1);
  ASSERT("2.4b - init_challenge" , r==NULL_PARAMETER)	

	if (challenge != NULL) {
		r = init_challenge_activity(challenge, 2, NULL, 1);
		ASSERT("2.4c - init_challenge" , r==NULL_PARAMETER)
		r = init_challenge_activity(challenge, 2, "name", 1);
		ASSERT("2.4d - init_challenge" , r==OK)
	}
*/

	r=visitor_arrive(sys, "room_2", "visitor_1", 201, Medium, -5);
	ASSERT("2.6a - visitor_arrive" , r==ILLEGAL_TIME)
	r=visitor_arrive(NULL, "room_2", "visitor_1", 201, Medium, 0);
	ASSERT("2.6b - visitor_arrive" , r==NULL_PARAMETER)
	r=visitor_arrive(sys, NULL, "visitor_1", 201, Medium, 5);
	ASSERT("2.6c - visitor_arrive" , r==ILLEGAL_PARAMETER)
	r=visitor_arrive(sys, "room_2", NULL, 201, Medium, 5);
	ASSERT("2.6d - visitor_arrive" , r==ILLEGAL_PARAMETER)
	r=visitor_arrive(sys, "room_1", "visitor_1", 201, Medium, 5);
	ASSERT("2.6e - visitor_arrive" , r==NO_AVAILABLE_CHALLENGES)
	r=visitor_arrive(sys, "room_1", "visitor_1", 201, Hard, 5);
	//printf("\n\n%u\n\n",r);
	ASSERT("2.6x - visitor_arrive" , r==OK)

	
	r=init_visitor(NULL, "name", 3);
	ASSERT("2.7a - init_visitor" , r==NULL_PARAMETER)
	Visitor *visitor = malloc(sizeof(*visitor));
	r=init_visitor(visitor, NULL, 3);
	ASSERT("2.7b - init_visitor" , r==NULL_PARAMETER)
	r=init_visitor(visitor, "name", 3);
	ASSERT("2.7c - init_visitor" , r==OK)

	r=reset_visitor(NULL);
	ASSERT("2.8a - reset_visitor" , r==NULL_PARAMETER)
	r=reset_visitor(visitor);
	ASSERT("2.8b - reset_visitor" , r==OK)


  char* room_name = malloc(MAX_NAME_LENG);
	r=system_room_of_visitor(NULL, "name", &room_name);
	ASSERT("2.9a - system_room_of_visitor" , r==NULL_PARAMETER)
	r=system_room_of_visitor(sys, NULL, &room_name);
	ASSERT("2.9b - system_room_of_visitor" , r==ILLEGAL_PARAMETER)
	r=system_room_of_visitor(sys, "name", NULL);
	ASSERT("2.9c - system_room_of_visitor" , r==ILLEGAL_PARAMETER)

	int place;
	r=num_of_free_places_for_level(NULL, 1, &place);
	ASSERT("2.10a - num_of_free_places_for_level" , r==NULL_PARAMETER)
	r=num_of_free_places_for_level(&(sys->challengeRooms[3]), Easy, &place);
	ASSERT("2.10b - num_of_free_places_for_level" , r==OK && place == 1)
	r=num_of_free_places_for_level(&(sys->challengeRooms[3]), Medium, &place);
	ASSERT("2.10c - num_of_free_places_for_level" , r==OK && place == 1)
	r=num_of_free_places_for_level(&(sys->challengeRooms[3]), Hard, &place);
	//printf("\n\n%d\n\n",place);
	ASSERT("2.10d - num_of_free_places_for_level" , r==OK && place == 2)
	r=num_of_free_places_for_level(&(sys->challengeRooms[3]), All_Levels, &place);
	ASSERT("2.10a - num_of_free_places_for_level" , r==OK && place == 4)
	//TODO - check if challenge was taken

	r=init_visitor(visitor, "name", 3);
	ASSERT("2.11 - init_visitor" , r==OK)

	r=visitor_enter_room(NULL, visitor, Hard, 5);
	ASSERT("2.11a - visitor_enter_room" , r==NULL_PARAMETER)
	r=visitor_enter_room(&(sys->challengeRooms[3]), NULL, Hard, 5);
	ASSERT("2.11b - visitor_enter_room" , r==NULL_PARAMETER)
	struct SChallengeActivity *challenge2 = malloc(sizeof(struct SChallengeActivity*));
	visitor->current_challenge = challenge2;
	r=visitor_enter_room(&(sys->challengeRooms[3]), visitor, Hard, 5);
	ASSERT("2.11c - visitor_enter_room" , r==ALREADY_IN_ROOM)
	visitor->current_challenge = NULL;
	r=visitor_enter_room(&(sys->challengeRooms[1]), visitor, Medium, 5);
	ASSERT("2.11d - visitor_enter_room" , r==NO_AVAILABLE_CHALLENGES)
	r=visitor_enter_room(&(sys->challengeRooms[3]), visitor, Easy, 15);
	ASSERT("2.11e - visitor_enter_room" , r==OK)



	//char* room_name = malloc(sizeof(char) * MAX_NAME_LENG );
	char *room_name2 = malloc(sizeof(char*));	
	r=room_of_visitor(NULL, &room_name2);
	ASSERT("2.12a - room_of_visitor" , r==NULL_PARAMETER)
	r=room_of_visitor(visitor, NULL);
	ASSERT("2.12b - room_of_visitor" , r==NULL_PARAMETER)
	//work because step 2.11e
	r=room_of_visitor(visitor, &room_name2);
	ASSERT("2.12c - room_of_visitor" , r==OK && strcmp(room_name2, "room_4")==0)
	//printf("\n\n%s\n\n", room_name2);

	r=visitor_quit_room(NULL, 17);
	ASSERT("2.13a - visitor_quit_room" , r==NULL_PARAMETER)
	r=visitor_quit_room(visitor, 17);
	ASSERT("2.13b - visitor_quit_room" , r==OK)

	Visitor *visitor0 = malloc(sizeof(*visitor0));
	r=visitor_quit_room(visitor0, 17);
	ASSERT("2.13c - visitor_quit_room" , r==NOT_IN_ROOM)

	//r=visitor_enter_room(&(sys->challengeRooms[3]), visitor, Easy, 15);
	//ASSERT("2.11e - visitor_enter_room" , r==OK)

	printf("\n\n\nfull tests\n\n\n");
	r=num_of_free_places_for_level(&(sys->challengeRooms[3]), Hard, &place);
	ASSERT("3.1a - num_of_free_places_for_level" , r==OK && place == 2)
	Visitor *visitor1 = malloc(sizeof(*visitor1));
	r=visitor_enter_room(&(sys->challengeRooms[3]), visitor1, Hard, 25);
	ASSERT("3.1b - visitor_enter_room" , r==OK)
	r=num_of_free_places_for_level(&(sys->challengeRooms[3]), Hard, &place);
	ASSERT("3.1c - num_of_free_places_for_level" , r==OK && place == 1)
	Visitor *visitor2 = malloc(sizeof(*visitor2));
	r=visitor_enter_room(&(sys->challengeRooms[3]), visitor2, Hard, 30);
	ASSERT("3.1d - visitor_enter_room" , r==OK)
	r=num_of_free_places_for_level(&(sys->challengeRooms[3]), Hard, &place);
	ASSERT("3.1e - num_of_free_places_for_level" , r==OK && place == 0)
	Visitor *visitor3 = malloc(sizeof(*visitor3));
	r=visitor_enter_room(&(sys->challengeRooms[3]), visitor3, Hard, 35);
	ASSERT("3.1f - visitor_enter_room" , r==NO_AVAILABLE_CHALLENGES)
	r=num_of_free_places_for_level(&(sys->challengeRooms[3]), Hard, &place);
	ASSERT("3.1g - num_of_free_places_for_level" , r==OK && place == 0)




	free(visitor);

	//TODO - if challenges was taken

/*
   r=visitor_arrive(sys, "room_1", "visitor_2", 202, Easy, 8);

   r=visitor_quit(sys, 203, 10);
   ASSERT("1.1" , r==NOT_IN_ROOM)

   r=visitor_quit(sys, 201, 9);
   ASSERT("1.2" , r==OK)

   int time;
   r=best_time_of_system_challenge(sys, "challenge_2", &time);
   ASSERT("1.3" , time==4)

   r=change_system_room_name(sys, "room_1", "room_111");

   r=visitor_arrive(sys, "room_1", "visitor_3", 203, Easy, 8);
   ASSERT("1.4" , r==ILLEGAL_PARAMETER)

   r=visitor_arrive(sys, "room_111", "visitor_3", 203, Easy, 8);
   ASSERT("1.5" , r==ILLEGAL_TIME)

   r=visitor_arrive(sys, "room_111", "visitor_3", 203, Easy, 15);
   ASSERT("1.6" , r==OK)

   r=visitor_arrive(sys, "room_111", "visitor_4", 204, Easy, 16);
   ASSERT("1.7" , r==NO_AVAILABLE_CHALLENGES)

   r=change_challenge_name(sys, 11, "challenge_1111");

   r=best_time_of_system_challenge(sys, "challenge_1111", &time);

   ASSERT("1.8" , time==0)

   char *namep=NULL;
   r=most_popular_challenge(sys, &namep);
   ASSERT("1.9" , namep!=NULL && strcmp(namep, "challenge_2")==0)

   free(namep);

   char *room=NULL;
   r=system_room_of_visitor(sys, "visitor_4", &room);
   ASSERT("1.10" , r==NOT_IN_ROOM)
   free(room);

   r=system_room_of_visitor(sys, "visitor_3", &room);
   ASSERT("1.11" , r==OK && room!=NULL && strcmp(room, "room_111")==0)
   free(room);

   r=all_visitors_quit(sys, 17);

   r=best_time_of_system_challenge(sys, "challenge_1111", &time);
   ASSERT("1.12" , time==9)

   r=best_time_of_system_challenge(sys, "challenge_4", &time);
   ASSERT("1.13" , time==2)

   char *most_popular_challenge=NULL, *challenge_best_time=NULL;
   r=destroy_system(sys, 18, &most_popular_challenge, &challenge_best_time);
   ASSERT("1.14" , most_popular_challenge!=NULL && strcmp(most_popular_challenge, "challenge_1111")==0)
   ASSERT("1.15" , challenge_best_time!=NULL && strcmp(challenge_best_time, "challenge_4")==0)

   free(most_popular_challenge);

   free(challenge_best_time);
*/


   return 0;
}

