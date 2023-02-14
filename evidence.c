#include <stdio.h>
#include "defs.h"

/*
  Function:  initEvidenceList
  Purpose:   initialize the variables in a EvidenceListType structure
       in:   the EvidenceListType structure that needs to be initialized
   return:   No return value
*/
void initEvidenceList(EvidenceListType *list) {
	list->head = NULL;
}

/*
  Function:  initEvidence
  Purpose:   initialize the variables in the EvidenceType structure
       in:   EvidenceClassType of evidence
       in:   float value of evidence
       in:   type of evidence as int, 1 if ghostly, 0 if standard
   in/out:   the EvidenceType structure that needs to be initialized
   return:   No return value
*/
void initEvidence(EvidenceClassType type, float value, int isGhostly, EvidenceType *e) {
	e = (EvidenceType *) calloc(1, sizeof(EvidenceType));
	e->evidenceType = type;
	e->value = value;
	e->isGhostly = isGhostly;
}

/*
  Function:  appendEvidence
  Purpose:   add a EvidenceType to the EvidenceListType
       in:   the EvidenceListType which is added to
       in:   the EvidenceType to be added
   return:   No return value
*/
void appendEvidence(EvidenceListType *list, EvidenceType *e) {
	EvidenceNodeType* new = (EvidenceNodeType *) calloc(1, sizeof(EvidenceNodeType));
	new->data = e;
	new->next = NULL;
	EvidenceNodeType* curr = list->head;
	if (curr == NULL) {
		list->head = new;
	}
	else {
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = new;
	}
}

/*
  Function:  removeEvidence
  Purpose:   remove a EvidenceType from a EvidenceListType
       in:   the EvidenceType to be removed
       in:   the EvidenceListType which is removed from
   return:   No return value
*/
void removeEvidence(EvidenceListType *list, EvidenceType *e) {
	EvidenceNodeType* curr = list->head;
	EvidenceNodeType* prev = NULL;
	while (curr->next != NULL) {
		if (curr->data == e) {
			break;
		}
		prev = curr;
		curr = curr->next;
	}
	if (curr == list->head) {
		list->head = curr->next;
	} else if (curr->next = NULL) {
		prev->next = NULL;
	} else {
		prev->next = curr->next;
	}
}

/*
  Function:  printEvidenceType
  Purpose:   print type of evidence given
       in:   the EvidenceType used to print type
   return:   No return value
*/
void printEvidenceType(EvidenceType *e) {
	switch (e->evidenceType) {
		case EMF:
			printf("EMF");
			break;
		case TEMPERATURE:
			printf("TEMPERATURE");
			break;
		case FINGERPRINTS:
			printf("FINGERPRINTS");
			break;
		case SOUND:
			printf("SOUND");
			break;
	}
}

/*
  Function:  cleanupEvidenceData
  Purpose:   Free the space allocated by the data in the given EvidenceListType
       in:   the EvidenceListType whose data will be freed
   return:   No return value
*/
void cleanupEvidenceData(EvidenceListType *list) {
	EvidenceNodeType* curr = list->head;
	while (curr != NULL) {
		free(curr->data);
		curr = curr->next;
	}
}

/*
  Function:  cleanupEvidenceList
  Purpose:   Free the space allocated by the given EvidenceListType
       in:   the EvidenceListType that will be freed
   return:   No return value
*/
void cleanupEvidenceList(EvidenceListType *list) {
	EvidenceNodeType* curr = list->head;
	EvidenceNodeType* prev = NULL;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
}

