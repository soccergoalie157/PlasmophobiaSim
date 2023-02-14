#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99
#define STANDARD_DATA       0
#define GHOSTLY_DATA        1

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;

// Room Types
typedef struct {
	char name[MAX_STR];
	struct RoomList *connected;
	struct EvidenceList *leftBehind;
	struct HunterArray *inRoom;
	struct Ghost *isHere;
	sem_t mutex;
} RoomType;

typedef struct RoomNode {
	RoomType *room;
	struct RoomNode* next;
} RoomNodeType;

typedef struct RoomList {
	 RoomNodeType *head;
	 int size;
} RoomListType;


// Evidence Types
typedef struct Evidence{
	EvidenceClassType evidenceType;
	float value;
	int isGhostly;
} EvidenceType;

typedef struct EvidenceNode {
	EvidenceType *data;
	struct EvidenceNode* next;
} EvidenceNodeType;

typedef struct EvidenceList {
	EvidenceNodeType *head;
} EvidenceListType;

// Hunter Type
typedef struct Hunter {
	RoomType *room;
	EvidenceClassType evidenceType;
	EvidenceListType *evidence;
	char name[MAX_STR];
	int fear;
	int timer;
	int typesCollected[4];
	int numOfTypes;
} HunterType;

typedef struct HunterArray {
	HunterType *hunters[MAX_HUNTERS];
	int size;
} HunterArrayType;

// Building Type
typedef struct Building {
	struct Ghost *ghost;
	HunterArrayType hunters;
	RoomListType rooms;
} BuildingType;

// Ghost Type
typedef struct Ghost{
	GhostClassType ghostType;
	RoomType *room;
	int boredom;
} GhostType;

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters
void printWinner(HunterType **, GhostType *);

void populateRooms(BuildingType *);   // Populates the building with sample data for rooms

void initRoomList(RoomListType *);
void initRoom(RoomType **, char *);
void appendRoom(RoomListType *, RoomNodeType *);
void connectRooms(RoomType *, RoomType *);
void printRooms(RoomListType *);
void cleanupRoomData(RoomListType *);
void cleanupRoomList(RoomListType *);

void initEvidenceList(EvidenceListType *);
void initEvidence(EvidenceClassType, float, int, EvidenceType *);
void appendEvidence(EvidenceListType *, EvidenceType *);
void removeEvidence(EvidenceListType *, EvidenceType *);
void addStandardEvidence(HunterType *);
int checkEvidence(HunterType *, EvidenceType *);
void printEvidenceType(EvidenceType *);
void cleanupEvidenceData(EvidenceListType *);
void cleanupEvidenceList(EvidenceListType *);

void initGhost(GhostClassType, GhostType **, BuildingType *); 
void ghostMove(GhostType *, RoomType *);
RoomType* roomNextGhost(GhostType *);
void leaveEvidence(GhostType *);
int typeOfResult(EvidenceClassType, float, GhostType *);
void ghostAction(GhostType *, int);
void *ghostThread(void *);
void printGhostType(HunterType *);

void initHunterArray(HunterArrayType *);
void addHunter(HunterType *, HunterArrayType *);
void removeHunter(HunterType *, HunterArrayType *); 
void initHunter(EvidenceClassType, char *, HunterType**, BuildingType *);
void getHunterName(char *);
RoomType* roomNextHunter(HunterType *);
void hunterMove(HunterType *, RoomType *);
void shareEvidence(HunterType *);
void collectEvidence(HunterType *);
void hunterAction(HunterType *, int choice);
void typesCollected(HunterType *);
void numOfTypesCollected(HunterType *);
void *hunterThread(void *);
void cleanupHunterArray(HunterArrayType *);

void initBuilding(BuildingType *);
void cleanupBuilding(BuildingType *);
