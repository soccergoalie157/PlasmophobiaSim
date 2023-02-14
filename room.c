#include <stdio.h>
#include "defs.h"

/*
  Function:  initRoomList
  Purpose:   initialize the variables in a RoomListType structure
       in:   the RoomListType structure that needs to be initialized
   return:   No return value
*/
void initRoomList(RoomListType *list) {
	list->head = NULL;
	list->size = 0;
}

/*
  Function:  initRoom
  Purpose:   initialize the variables in the RoomType structure
   in/out:   the RoomType structure that needs to be initialized
       in:   name of the room
   return:   No return value
*/
void initRoom(RoomType **room, char *name) {
	strcpy((*room)->name, name);
	(*room)->connected = (RoomListType *) calloc(1, sizeof(RoomListType));
	initRoomList((*room)->connected);
	(*room)->leftBehind = (EvidenceListType *) calloc(1, sizeof(EvidenceListType));
	initEvidenceList((*room)->leftBehind);
	(*room)->inRoom = (HunterArrayType *) calloc(1, sizeof(HunterArrayType));
	initHunterArray((*room)->inRoom);
	(*room)->isHere = NULL;
	sem_init(&(*room)->mutex, 0, 1);
}

/*
  Function:  appendRoom
  Purpose:   add a RoomNodeType to the RoomListType
       in:   the RoomListType which is added to
       in:   the RoomNodeType to be added
   return:   No return value
*/
void appendRoom(RoomListType *list, RoomNodeType *room) {
	RoomNodeType* curr = list->head;
	room->next = NULL;
	if (curr == NULL) {
		list->head = room;
	}
	else {
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = room;
	}
	list->size += 1;
}

/*
  Function:  connectRooms
  Purpose:   connect two RoomTypes together
       in:   a RoomType which is going to be connected to
       in:   a RoomType which is going to be connected to
   return:   No return value
*/
void connectRooms(RoomType *room1, RoomType *room2) {
	RoomNodeType* room1_node = calloc(1, sizeof(RoomNodeType));
	room1_node->room = room1;
	RoomNodeType* room2_node = calloc(1, sizeof(RoomNodeType));
	room2_node->room = room2;
	appendRoom(room1->connected, room2_node);
	appendRoom(room2->connected, room1_node);
}	

/*
  Function:  cleanupRoomData
  Purpose:   Free the space allocated by the data in the given RoomListType
       in:   the RoomListType whose data will be freed
   return:   No return value
*/
void cleanupRoomData(RoomListType *list) {
	RoomNodeType* curr = list->head;
	while (curr != NULL) {
		cleanupRoomList(curr->room->connected);
		free(curr->room->connected);
		free(curr->room->inRoom);
		cleanupEvidenceData(curr->room->leftBehind);
		cleanupEvidenceList(curr->room->leftBehind);
		free(curr->room->leftBehind);
		free(curr->room);
		curr = curr->next;
	}
}

/*
  Function:  cleanupRoomList
  Purpose:   Free the space allocated by the given RoomListType
       in:   the RoomListType that will be freed
   return:   No return value
*/
void cleanupRoomList(RoomListType *list) {
	RoomNodeType* curr = list->head;
	RoomNodeType* prev = NULL;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
}
