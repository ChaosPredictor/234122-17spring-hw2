#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "visitor_room.h"


Result init_room(ChallengeRoom *room, char *name, int num_challenges) {
	printf("in init: %s   %d\n", name, num_challenges);
	return OK;
}
