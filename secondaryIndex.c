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
int* searchByAuthor(const char* author) {
    FILE* idxSecondary;
    int* id = NULL;
    int count = 0;
    SecondaryIdx data;

    if (openFile(&idxSecondary, "secondaryIdx.bin", "rb")) {
        while (fread(&data, sizeof(SecondaryIdx), 1, idxSecondary)) {
            if (strcmp(data.author, author) == 0) {
                count++;
                int* tmp = realloc(id, (count + 1) * sizeof(int));
                if (tmp == NULL) {
                    fclose(idxSecondary);
                    free(id);
                    return NULL;
                }
                id = tmp;
                id[count - 1] = data.id;
            }
        }

        fclose(idxSecondary);
    }

    if (count == 0) {
        // Não foram encontrados IDs correspondentes
        free(id);
        return NULL;
    }

    // Adicionar valor -1 manualmente ao final do array de IDs
    int* tmp = realloc(id, (count + 1) * sizeof(int));
    if (tmp == NULL) {
        // Gerenciar o erro de realocação de memória
        free(id);
        return NULL;
    }
    id = tmp;
    id[count] = -1;

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
