#include "dataFile.h"

void insertSecondaryIndex (SecondaryIdx data){
    FILE* idxSecondary;
    if (openFile(&idxSecondary, "secondaryIdx.bin", "rb+")){
        fseek(idxSecondary, 0, SEEK_END);
        fwrite(data.author, sizeof(data), strlen(data.author), idxSecondary);
        fwrite(&data.id, sizeof(data), 1, idxSecondary);
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

int* searchByAuthor(const char* author){
    FILE* idxSecondary;
    int* id = NULL;
    int count = 0; 
    SecondaryIdx data;
    
    if (openFile(&idxSecondary, "secondaryIdx.bin", "rb")){
        while (fread(&data, sizeof(SecondaryIdx), 1, idxSecondary)){
            if (strcmp(data.author, author) == 0) {
                count++;
                int* tmp = realloc(id, count * sizeof(int));
                if (tmp == NULL) {
                    fclose(idxSecondary);
                    return NULL;
                }
                id = tmp;
                id[count-1] = data.id; 
            }
        }        fclose(idxSecondary);
    }
    
    if (count == 0) {
        printf("não encontrado");
        free(id);
        return NULL;
    }   
    return id;
}
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
