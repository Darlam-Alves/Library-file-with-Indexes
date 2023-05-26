#include "dataFile.h"

void insertSecondaryIndex (SecondaryIdx data){
    FILE* idxSecondary;
    if (openFile(&idxSecondary, "secondaryIdx.bin", "rb+")){
        fseek(idxSecondary, 0, SEEK_END);
        fwrite(data.author, sizeof(char), strlen(data.author), idxSecondary);
        fwrite(&data.id, sizeof(int), 1, idxSecondary);
        fclose(idxSecondary);
    }
}

/* void searchByAuthor(const char* author) {
    FILE* idxSecondary;
    if (openFile(&idxSecondary, "secondaryIdx.bin", "rb+")) {
        int id;
        char currentAuthor[BUFFER_SIZE];
        while (fread(&id, sizeof(int), 1, idxSecondary) == 1) {
            fread(currentAuthor, sizeof(char), BUFFER_SIZE, idxSecondary);

            if (strcmp(author, currentAuthor) == 0) {
                printf("Author: %s, ID: %d\n", currentAuthor, id);
            }
        }
        fclose(idxSecondary);
    }
} */

void readAllData() {
    FILE* idxSecondary;
    if (openFile(&idxSecondary, "secondaryIdx.bin", "rb")) {
        SecondaryIdx data;
        while (!feof(idxSecondary)){
            fread(&(data.author), sizeof(char), sizeof(data.author), idxSecondary); 
            fread(&(data.id), sizeof(int), 1, idxSecondary);
            printf("%s\n", data.author);
            printf("%d\n", data.id);
        }
        fclose(idxSecondary);
    }
}
