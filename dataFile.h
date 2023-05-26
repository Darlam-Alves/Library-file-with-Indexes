#include "dataManipulator.h"

#define TRUE 1
#define FALSE 0

void readAllData();
void insertDataFile(BookRecord book);
int openFile(FILE **arq, const char* fileName, char *type);
void insertPrimaryIndex (PrimaryIdx** head);
void insertSecondaryIndex(SecondaryIdx data);
void removeDatas (BookRecord book);
void searchDatas (BookRecord book);
void removeByAuthor(char* author);
void removeByID(int id); 
void searchByID(int id);
void createNode(PrimaryIdx** head, int id, long byteOffset);
void freePrimaryIndex(PrimaryIdx** head);
void searchByAuthor(const char* author);
void writeData(FILE* dataFile, BookRecord book);