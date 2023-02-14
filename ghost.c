#include <stdio.h>
#include "defs.h"

/*
  Function:  initGhost
  Purpose:   allocate space for the GhostType structure, initialize the variables in the GhostType structure
       in:   GhostClassType for the ghost
   in/out:   the GhostType structure that needs to be initialized
       in:   BuildingType that the ghost is added to
   return:   No return value
*/
void initGhost(GhostClassType type, GhostType **ghost, BuildingType *b) {
	*ghost = (GhostType *) calloc(1, sizeof(GhostType));
	(*ghost)->ghostType = type;
	int i = randInt(1, b->rooms.size);
	RoomNodeType* curr = b->rooms.head;
	while (i > 0) {
		curr = curr->next;
		i--;
	}
	(*ghost)->room = curr->room;
	(*ghost)->boredom = BOREDOM_MAX;
	curr->room->isHere = (*ghost);
	b->ghost = (*ghost);
}
/*
  Function:  roomNextGhost
  Purpose:   get next room for ghost
       in:   GhostType used to get next room
   return:   Return RoomType pointer
*/
RoomType* roomNextGhost(GhostType *ghost) {
	RoomNodeType* curr = ghost->room->connected->head;
	int i = randInt(0, ghost->room->connected->size);
	while (i > 0) {
		curr = curr->next;
		i--;
	}
	return curr->room;
}

/*
  Function:  ghostMove
  Purpose:   move the ghost into the specified room
       in:   GhostType which is going to move
       in:   RoomType which is the next room
   return:   No return value
*/
void ghostMove(GhostType *ghost, RoomType *room) {
	ghost->room->isHere = NULL;
	ghost->room = room;
	ghost->room->isHere = ghost;
}

/*
  Function:  leaveEvidence
  Purpose:   leave evidence in the room the ghost is in
       in:   GhostType which is going to leave evidence
   return:   No return value
*/
void leaveEvidence(GhostType *ghost) {
	int i;
	switch (ghost->ghostType) {
		case POLTERGEIST:
			do {
				i = randInt(0, 4);
			} while (i != 3);
			break;
		case BANSHEE:
			do {
				i = randInt(0, 4);
			} while (i != 2);
			break;
		case BULLIES:
			do {
				i = randInt(0, 4);
			} while (i != 1);
			break;
		case PHANTOM:
			do {
				i = randInt(0, 4);
			} while (i != 0);
			break;
	}
	EvidenceClassType type = (EvidenceClassType)i;
	float value;
	switch (type) {
		case EMF:
			value = randFloat(4.70, 5.00);
			break;
		case TEMPERATURE:
			value = randFloat(-10.00, 1.00);
			break;
		case FINGERPRINTS:
			value = 1.00;
			break;
		case SOUND:
			value = randFloat(65.00, 75.00);
			break;
	}
	EvidenceType *e;
	initEvidence(type, value, typeOfResult(type, value, ghost), e);
	appendEvidence(ghost->room->leftBehind, e);
}
	
/*
  Function:  typeOfResult
  Purpose:   figure out whether an evidence type is ghostly or standard
       in:   EvidenceClassType of evidence
       in:   float value of the evidence
       in:   GhostType used to print where evidence is left
   return:   returns 1 if ghostly evidence, 0 if standard
*/
int typeOfResult(EvidenceClassType type, float value, GhostType *ghost) {
	switch (type) {
		case EMF:
			if (value >= 0.00 && value <= 4.90) {
				type = STANDARD_DATA;
			} else {
				type = GHOSTLY_DATA;
			}
			break;
		case TEMPERATURE:
			if (value >= 0.00 && value <= 27.00) {
				type = STANDARD_DATA;
			} else {
				type = GHOSTLY_DATA;
			}
			break;
		case FINGERPRINTS:
			if (value == 0.00) {
				type = STANDARD_DATA;
			} else {
				type = GHOSTLY_DATA;
			}
			break;
		case SOUND:
			if (value >= 40.00 && value <= 70.00) {
				type = STANDARD_DATA;
			} else {
				type = GHOSTLY_DATA;
			}
			break;
	}
	if (type == GHOSTLY_DATA) {
		printf("GHOST left ghostly evidence in %s\n", ghost->room->name);
		return GHOSTLY_DATA;
	} else {
		printf("GHOST left standard evidence in %s\n", ghost->room->name);
		return STANDARD_DATA;
	}
}

/*
  Function:  ghostAction
  Purpose:   have the GhostType perform an action
       in:   GhostType which will perform action
       in:   choice that ghost takes depending on situation
   return:   No return value
*/
void ghostAction(GhostType *ghost, int choice) {
	switch (choice) {
		case 0:
			printf("GHOST is staying in %s\n", ghost->room->name);
			break;
		case 1:
			leaveEvidence(ghost);
			break;
		case 2:
			RoomType *room = roomNextGhost(ghost);
			ghostMove(ghost, room);
			printf("GHOST has moved to %s\n", ghost->room->name);
			break;
	}
}

/*
  Function:  printGhostType
  Purpose:   print type of ghost depending on HunterType evidence
       in:   HunterType from which evidence list is acquired
   return:   No return value
*/
void printGhostType(HunterType *h) {
	if (h->typesCollected[0] == 1 && h->typesCollected[1] == 1 && h->typesCollected[2] == 1) {
		printf("POLTERGEIST\n");
	} else if (h->typesCollected[0] == 1 && h->typesCollected[1] == 1 && h->typesCollected[3] == 1) {
		printf("BANSHEE\n");
	} else if (h->typesCollected[0] == 1 && h->typesCollected[2] == 1 && h->typesCollected[3] == 1) {
		printf("BULLIES\n");
	} else if (h->typesCollected[1] == 1 && h->typesCollected[2] == 1 && h->typesCollected[3] == 1) {
		printf("PHANTOM\n");
	} else {
		printf("\n NEVERMIND, NOBODY HAS WON, NOT ENOUGH EVIDENCE\n");
	}
}

