#include <stdio.h>
#include "defs.h"

/*
  Function:  initHunterArray
  Purpose:   initialize the variables in a HunterArrayType structure
       in:   the HunterArrayType structure that needs to be initialized
   return:   No return value
*/
void initHunterArray(HunterArrayType *arr) {
	arr->size = 0;
}

/*
  Function:  addHunter
  Purpose:   add a HunterType to the HunterArrayType
       in:   the HunterType to be added
       in:   the HunterArrayType which is added to
   return:   No return value
*/
void addHunter(HunterType *h, HunterArrayType *arr) {
	int i = 0;
	while (i < arr->size) {
		if (arr->hunters[i] == NULL) {
			break;
		}
		i++;
	}
	arr->hunters[i] = h;
	arr->size += 1;
}

/*
  Function:  removeHunter
  Purpose:   remove a HunterType from a HunterArrayType
       in:   the HunterType to be removed
       in:   the HunterArrayType which is removed from
   return:   No return value
*/
void removeHunter(HunterType *h, HunterArrayType *arr) {
	int i = 0;
	while (i < arr->size) {
		if (strcmp(arr->hunters[i]->name, h->name) == 0) {
			break;
		}
		i++;
	}
	for (int x = i; x < arr->size; x++) {
		arr->hunters[x] = arr->hunters[x+1];
	}
	arr->size -= 1;
	arr->hunters[arr->size] = NULL;
}

/*
  Function:  initHunter
  Purpose:   allocate space for the HunterType structure, initialize the variables in the HunterType structure
       in:   EvidenceClassType of the hunter
       in:   name of the hunter
   in/out:   the HunterType structure that needs to be initialized
       in:   the BuildingType which the hunter is added to
   return:   No return value
*/
void initHunter(EvidenceClassType type, char *name, HunterType** h, BuildingType *b) {
	*h = (HunterType *) calloc(1, sizeof(HunterType));
	(*h)->room = b->rooms.head->room;
	addHunter((*h), (*h)->room->inRoom);
	(*h)->evidenceType = type;
	(*h)->evidence = (EvidenceListType *) calloc(1, sizeof(EvidenceListType));
	initEvidenceList((*h)->evidence);
	strcpy((*h)->name, name);
	(*h)->fear = 0;
	(*h)->timer = BOREDOM_MAX;
	(*h)->numOfTypes = 0;
	addHunter((*h), &b->hunters);
}

/*
  Function:  getHunterName
  Purpose:   set the hunter name by user input
       in:   the name that will be used for the name of the hunter
   return:   No return value
*/
void getHunterName(char *name) {
	fgets(name, MAX_STR, stdin);
	for (int i = 0; i < MAX_STR; i++) {
		if (name[i] == '\n') {
			name[i] = '\0';
			break;
		}
	}
}

/*
  Function:  roomNextHunter
  Purpose:   get next room for hunter
       in:   HunterType used to get next room
   return:   Return RoomType pointer
*/
RoomType* roomNextHunter(HunterType *h) {
	RoomNodeType* curr = h->room->connected->head;
	int i = randInt(0, h->room->connected->size);
	while (i > 0) {
		curr = curr->next;
		i--;
	}
	return curr->room;
}

/*
  Function:  hunterMove
  Purpose:   move the hunter into the specified room
       in:   HunterType which is going to move
       in:   RoomType which is the next room
   return:   No return value
*/
void hunterMove(HunterType *h, RoomType *room) {
	removeHunter(h, h->room->inRoom);
	h->room = room;
	addHunter(h, h->room->inRoom);
	printf("%s has moved to %s\n", h->name, h->room->name);
	h->timer -= 1;
}

/*
  Function:  checkEvidence
  Purpose:   check for duplicate evidence
       in:   HunterType whose evidence list is used to check for evidence
       in:   EvidenceType which is used as the comparison
   return:   returns 1 if no evidence matches, 0 if there is a match
*/
int checkEvidence(HunterType *h, EvidenceType *e) {
	EvidenceNodeType* curr = h->evidence->head;
	if (curr == NULL) {
		return 0;
	}
	while (curr != NULL) {
		if (curr->data->evidenceType == e->evidenceType && curr->data->value == e->value) {
			return 0;
		}
	}
	return 1;
}

/*
  Function:  shareEvidence
  Purpose:   share evidence with another hunter
       in:   HunterType who will get evidence from another hunter
   return:   No return value
*/
void shareEvidence(HunterType *h) {
	HunterType *other = h;
	if (h->room->inRoom->size > 1) {
		do {
			int i = randInt(0, h->room->inRoom->size);
			other = h->room->inRoom->hunters[i];
		} while (strcmp(h->name, other->name) == 0);
		
		EvidenceNodeType* curr = other->evidence->head;
		while (curr != NULL) {
			if (curr->data->isGhostly == GHOSTLY_DATA && checkEvidence(h, curr->data) == 1) {
				EvidenceType *ev;
				initEvidence(curr->data->evidenceType, curr->data->value, curr->data->isGhostly, ev);
				appendEvidence(h->evidence, ev);
				printf("%s has collected ", h->name);
				printEvidenceType(ev);
				printf(" evidence from %s\n", other->name);
				break;
			}
			curr = curr->next;
		}
	}
}

/*
  Function:  collectEvidence
  Purpose:   collect evidence from room
       in:   HunterType who will collect evidence
   return:   No return value
*/
void collectEvidence(HunterType *h) {
	EvidenceNodeType* curr = h->room->leftBehind->head;
	while (curr != NULL) {
		if (curr->data->evidenceType == h->evidenceType && curr->data->isGhostly == GHOSTLY_DATA) {
			appendEvidence(h->evidence, curr->data);
			removeEvidence(h->room->leftBehind, curr->data);
			h->timer = BOREDOM_MAX;
			h->typesCollected[h->evidenceType] = 1;
			printf("%s has collected ", h->name);
			printEvidenceType(curr->data);
			break;
		}
		curr = curr->next;
	}
}

/*
  Function:  addStandardEvidence
  Purpose:   add randomized standar evidence to HunterTyoe evidence
       in:   HunterType who will add the standard evidence
   return:   No return value
*/
void addStandardEvidence(HunterType *h) {
	EvidenceType *e = (EvidenceType *) calloc(1, sizeof(EvidenceType));
	float value;
	switch (h->evidenceType) {
		case EMF:
			value = randFloat(0, 4.90);
			break;
		case TEMPERATURE:
			value = randFloat(0, 27.00);
			break;
		case FINGERPRINTS:
			value = 0.00;
			break;
		case SOUND:
			value = randFloat(40.00, 70.00);
			break;
	}
	initEvidence(h->evidenceType, value, STANDARD_DATA, e);
	appendEvidence(h->evidence, e);
	printf("%s has generated and collected standard evidence\n", h->name);
}

/*
  Function:  hunterAction
  Purpose:   have the HunterType perform an action
       in:   HunterType which will perform action
       in:   choice that hunter takes depending on situation
   return:   No return value
*/
void hunterAction(HunterType *h, int choice) {
	switch (choice) {
		case 0:
			if (h->room->leftBehind->head = NULL) {
				addStandardEvidence(h);
			} else {
				collectEvidence(h);
			}
			break;
		case 1:
			RoomType *room = roomNextHunter(h);
			hunterMove(h, room);
			break;
		case 2:
			shareEvidence(h);
			typesCollected(h);
			break;
	}
}

/*
  Function:  numOfTypesCollected
  Purpose:   compute number of evidence types HunterType has collected
       in:   HunterType which has the evidence
   return:   No return value
*/
void numOfTypesCollected(HunterType *h) {
	for (int i = 0; i < 4; i++) {
		if (h->typesCollected[i] == 1) {
			h->numOfTypes += 1;
		}
	}
}

/*
  Function:  typesCollected
  Purpose:   set types collected in HunterType typesCollected to 1
       in:   HunterType which has the evidence
   return:   No return value
*/
void typesCollected(HunterType *h) {
	EvidenceNodeType* curr = h->evidence->head;
	while (curr != NULL) {
		if (curr->data->evidenceType != h->evidenceType) {
			h->typesCollected[curr->data->evidenceType] = 1;
		}
		curr = curr->next;
	}
	numOfTypesCollected(h);
}

/*
  Function:  cleanupHunterArray
  Purpose:   Free the space allocated by the given HunterArrayType
       in:   the HunterArrayType whose data will be freed
   return:   No return value
*/
void cleanupHunterArray(HunterArrayType *arr) {
	for (int i = 0; i < arr->size; i++) {
		cleanupEvidenceData(arr->hunters[i]->evidence);
		cleanupEvidenceList(arr->hunters[i]->evidence);
		free(arr->hunters[i]->evidence);
		free(arr->hunters[i]);
	}
}
	
