#include "defs.h"

int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    // You may change this code; this is for demonstration purposes
    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);
    
    // Declare GhostType and initialize
    GhostType *ghost;
    initGhost((GhostClassType)randInt(0, 4), &ghost, &building);
    
    // Declare HunterType array of pointers
    HunterType *hunters[MAX_HUNTERS];
    
    // Set names for hunters
    char name[MAX_STR];
    printf("Enter the name for the first hunter: ");
    getHunterName(name);
    initHunter((EvidenceClassType)0, name, &hunters[0], &building);
    printf("Enter the name for the second hunter: ");
    getHunterName(name);
    initHunter((EvidenceClassType)1, name, &hunters[1], &building);
    printf("Enter the name for the third hunter: ");
    getHunterName(name);
    initHunter((EvidenceClassType)2, name, &hunters[2], &building);
    printf("Enter the name for the fourth hunter: ");
    getHunterName(name);
    initHunter((EvidenceClassType)3, name, &hunters[3], &building);
    
    // Threads
    pthread_t g;
    pthread_t h[MAX_HUNTERS];
    // Print where ghost is
    printf("GHOST is in %s\n", ghost->room->name);
    // Loop for threads
    while (1) {
    	// Run ghost thread only if boredom is greater than 0 
    	if (ghost->boredom > 0) {
    		pthread_create(&g, NULL, ghostThread, ghost);
    		pthread_join(g, NULL);
    	}
    	// Run hunter thread if boredom > 0, fear < 100 and numOfTypes != 3
    	if (hunters[0]->timer > 0 && hunters[0]->fear < MAX_FEAR && hunters[0]->numOfTypes != 3) {
    		pthread_create(&h[0], NULL, hunterThread, hunters[0]);
    		pthread_join(h[0], NULL);
    	} 
    	// Break if hunter thread is done
    	else {
    		break;
    	}
    }
    // Print and cleanup building
    printWinner(hunters, ghost);
    cleanupBuilding(&building);
    return 0;
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}


void printWinner(HunterType **hunters, GhostType *ghost) {
	printf("\nHUNTERS with fear greater than or equal to 100:\n");
	int numOver = 0;
	int hunterOver;
	for (int i = 0; i < MAX_HUNTERS; i++) {
		if (hunters[i]->fear >= 100) {
			numOver++;
			printf("%s\n", hunters[i]->name);
		} else {
			hunterOver = i;
		}
	}
	if (numOver == 0) {
		printf("No HUNTERS with fear >= 100!\n");
	}
	if (numOver < 4) {
		printf("HUNTERS win! GHOST is of type ");
		printGhostType(hunters[hunterOver]);
	} else {
		printf("The GHOST has won!");
	}
}


void *ghostThread(void *g) {
	// Convert void parameter to ghost pointer
	GhostType *ghost = (GhostType*) g;
	int i = 0;
	// See if current room is available
	if (sem_trywait(&(ghost->room->mutex)) == 0) {
		if (ghost->room->inRoom->size > 0) {
			i = randInt(0, 2);
			ghost->boredom = BOREDOM_MAX;
		}
		else {
			i = randInt(0, 3);
			ghost->boredom -= 1;
		}
		// If ghost wants to move
		if (i == 2) {
			// See if next room is available
			RoomType *nextRoom = roomNextGhost(ghost);
			if (sem_trywait(&(nextRoom->mutex)) != 0) {
				sem_post(&(ghost->room->mutex));
			} else {
				ghostMove(ghost, nextRoom);
				printf("GHOST has moved to %s\n", ghost->room->name);
				sem_post(&(ghost->room->mutex));
				sem_post(&(nextRoom->mutex));
			}
		}
		else {
			ghostAction(ghost, i);
			sem_post(&(ghost->room->mutex));
		}
	}
	usleep(USLEEP_TIME);
}

void *hunterThread(void *h) {
	// Convert void parameter to hunter pointer
	HunterType *hunter = (HunterType*) h;
	int i = randInt(0, 3);
	// See if current room is available
	if (sem_trywait(&(hunter->room->mutex)) == 0) {
		if (hunter->room->isHere != NULL) {
			hunter->fear += FEAR_RATE;
		}
		// If hunter wants to move
		if (i == 1) {
			// See if next room is available
			RoomType *nextRoom = roomNextHunter(hunter);
			if (sem_trywait(&(nextRoom->mutex)) != 0) {
				sem_post(&(hunter->room->mutex));
			} else {
				hunterMove(hunter, nextRoom);
				sem_post(&(hunter->room->mutex));
				sem_post(&(nextRoom->mutex));
			}
		}
		else {
			hunterAction(hunter, i);
			sem_post(&(hunter->room->mutex));
		}
	}
	usleep(USLEEP_TIME);
}
