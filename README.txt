Author: Bibek Singh Kahlon
Student ID: 101220551

Program Description:
This program simulates the game Plasmophobia to some extent. There is a ghost that moves around and there are hunters who are trying to collect enough evidence to determine what type of ghost is in the building. The user is prompted for hunter names and then all the actions will slowly print out along with the winner at the end.
Files: building.c, room.c, main.c, ghost.c, defs.h, hunter.c, evidence.c, Makefile

Instructions for compilation and launch:
First switch to the directory the files are in using cd and extract the files from the zip. Then type in make all to compile the files and create the executable main in the terminal. Then type in ./main to run the executable. Type make clean if you want to remove all the executable files

Problems in program:

1. Sometimes when running valgrind, leaks show but it says x allocs, x frees meaning it frees all the allocs but still shows leaks. Other times it might not do this either. Running the program with a smaller max fear like 10 and a max boredom of 9 works but it still gives read of uninitialized values because of line 81 in room.c. I don't know what is causing extra memory leakage because I am freeing everything.

2. Running multiple hunter threads causes a deadlock, and even then each hunter does not print to the screen. I can't seem to fund why because I took Prof Connor Hillen's code sample in the announcements and tried to modifyy it to match the requirements of the threads but it just didn't work. I have just run the first hunter in the hunter array.

3. The ghost can get stuck in a room and never come out, but if running the ghost thread and hunter threads alone, the ghost will leave evidence and move rooms properly and the hunter will move rooms

