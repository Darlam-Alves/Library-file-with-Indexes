#include "dataFile.h"
#define DELIMITER_TITLE "|"
#define DELIMITER_AUTHOR "#"

int openFile(FILE **arq, const char* fileName, char *type){
    *arq = fopen(fileName, type);
    if (*arq == NULL){
        printf("Error opening file.\n");
        exit(1);
        return FALSE;
    } else {
        return TRUE; 
    }
}

void insertDataFile(BookRecord book) {
    static PrimaryIdx* head = NULL;
    SecondaryIdx data; 
    data.id = book.id;
    strcpy(data.author, book.author);
 
    FILE* dataFile;
    if (openFile(&dataFile, "dataFile.bin", "rb+")) {
        fseek(dataFile, 0, SEEK_END);
        createNode(&head, book.id, ftell(dataFile)); 
        writeData(dataFile, book);
        insertPrimaryIndex(head);
        //printf("registro inserido\n");
        insertSecondaryIndex(data);
        fclose(dataFile);
    }
}

void writeData(FILE* dataFile, BookRecord book){
    fwrite(&book.id, sizeof(int), 1, dataFile);
    fwrite(book.title, sizeof(char), strlen(book.title), dataFile);
    fwrite(DELIMITER_TITLE, sizeof(char), strlen(DELIMITER_TITLE), dataFile);
    fwrite(book.author, sizeof(char), strlen(book.author), dataFile);
    fwrite(DELIMITER_AUTHOR, sizeof(char), strlen(DELIMITER_AUTHOR), dataFile);
}

