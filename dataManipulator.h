#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 150
#define TRUE 1
#define FALSE 0
#define DATA_SIZE 3
#define NUM_RECORDS 100
#define MAX_STRING_SIZE 100

typedef struct book{
    int id;
    char *title;
    char *author;
    struct book* next;
} BookRecord;

typedef struct primaryIndex{
    int id; 
    long byteOffset;
    struct primaryIndex* next;
}PrimaryIdx;

typedef struct secondaryIndex{
    char author[BUFFER_SIZE];
    int id; 
}SecondaryIdx;


int createFiles(FILE** dataFile, FILE** idxPrimary, FILE** idxSecondary);
char *readString(char **element);
void readBuffer(char *buffer, int bufferSize);
char* typeofOperations(char* operations);
void executeOperations (char** operations, char** datas, int size);
void removeData(char* data);
void searchData(char* data);
char* extractAuthor(const char* data);
int extractID(char* data);
BookRecord extractData (char* data);
void removeRecordsByIds(int* ids);
void removeRecordsByAuthor(const char* data);