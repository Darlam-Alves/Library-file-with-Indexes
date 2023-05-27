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

        // Verifica se o ID já existe na lista de índices primários
        PrimaryIdx* current = head;
        if (checkDuplicateID(head, book.id)){
            fclose(dataFile);
            return;
        }

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

void searchRegister(long byteOffset) {
    FILE* dataFile;
    if (openFile(&dataFile, "dataFile.bin", "rb")) {
        fseek(dataFile, byteOffset, SEEK_SET);

        char marker[3];
        fread(marker, sizeof(char), 2, dataFile);
        marker[2] = '\0'; 

        if (strcmp(marker, "|*") == 0) {
            //printf("Registro removido.\n");
            fclose(dataFile);
            return;
        }

        int intValue;
        fread(&intValue, sizeof(int), 1, dataFile);

        char title[100];
        char author[100];

        fscanf(dataFile, "%[^|]|%[^#]#", title, author);

        printf("%d - %s - %s\n", intValue, title, author);

        fclose(dataFile);
    }
}

void removeRegister(long byteOffset) {
    FILE* dataFile;
    if (openFile(&dataFile, "dataFile.bin", "rb+")) {
        fseek(dataFile, byteOffset, SEEK_SET);
        
        char marker[] = "|*";
        fwrite(marker, sizeof(char), sizeof(marker), dataFile);
        
        fclose(dataFile);
    }
}

int checkDuplicateID(PrimaryIdx* head, int newID) {
    PrimaryIdx* current = head;
    while (current != NULL) {
        if (current->id == newID) {
            //printf("ID already exists. Record not inserted.\n");
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}