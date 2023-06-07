#include "dataFile.h"

void readBuffer(char *buffer, int bufferSize){
    scanf(" %[^(\n|\r)]", buffer);
    buffer[strcspn(buffer, "\n")] = '\0';
}

char *readString(char **element){
    char buffer[BUFFER_SIZE];
    readBuffer(buffer, BUFFER_SIZE);
    *element = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(*element, buffer);
    return *element;
}

char* typeofOperations(char* operations) {
    char buffer[BUFFER_SIZE];
    strcpy(buffer, operations);
    char* token = strtok(buffer, " ");
    
    if (token != NULL) {
        char* result = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(result, token);
        return result;
    }
    
    return NULL;
}

void executeOperations(char** operations, char** data, int size) {
    FILE* dataFile;
    FILE* idxPrimary;
    FILE* idxSecondary;
    createFiles(&dataFile, &idxPrimary, &idxSecondary);

    for (int i = 0; i < size; i++) {
        printLine();
        if (strcmp(operations[i], "ADD") == 0) {
            BookRecord book = extractData(data[i]);
            insertDataFile(book);
            free(book.title);
            free(book.author);
        } else if (strcmp(operations[i], "REMOVE") == 0) {
            removeData(data[i]);
        } else if (strcmp(operations[i], "SEARCH") == 0) {
            searchData(data[i]);
        } 
    }

    fclose(dataFile);
    fclose(idxPrimary);
    fclose(idxSecondary);
}

void removeData(char* data){
   if (strstr(data, "autor=") != NULL){
      removeRecordsByAuthor(data);
   } else if (strstr(data, "id=") != NULL){
        int id = extractID(data);
        long byteOffSet = searchByID(id);
        if (byteOffSet == -1){
            printf ("Erro ao remover\n");
        } else  
            removeRegister(byteOffSet);
   }
}
void removeRecordsByAuthor(const char* data) {
    if (strstr(data, "autor=") != NULL) {
        int* ids;
        char* author = extractAuthor(data);
        ids = searchByAuthor(author);
        if (ids == NULL) {
            printf("Erro ao remover\n");
        } else {
            removeRecordsByIds(ids);
            free(ids);
        }
        free(author);
    }
}
void removeRecordsByIds(int* ids) {
    for (int i = 0; ids[i] != -1; i++) {
        long aux = searchByID(ids[i]);
        long byteOffset = checkRecordExistence(aux);
        if (byteOffset == -1) {
            printf("Erro ao remover\n");
            break;
        } else {
            removeRegister(byteOffset);
        }
    }
}

void searchData(char* data){
   if (strstr(data, "autor=") != NULL){
        int* ids;
        char* author = extractAuthor(data);
        long byteOffSet;
        long aux;
        ids = searchByAuthor(author); 
        for (int i = 0; ids[i] != -1; i++){
            aux = searchByID(ids[i]);
            byteOffSet = checkRecordExistence(aux);
        }
        if (byteOffSet == -1){
            ids = NULL;
        }
        if (ids == NULL) {
            printf("Não encontrado\n");
        } else {
            for (int i = 0; ids[i] != -1; i++) {
                int target = ids[i];
                byteOffSet = searchByID(target);
                
                if (byteOffSet == -1){
                    printf("Não encontrado\n");
                }
                    searchRegister(byteOffSet);
            }
            free(ids);
        }
        free(author);
   } else if (strstr(data, "id=") != NULL){
       int id = extractID(data);
       long byteOffSet = searchByID(id);
       if (byteOffSet != -1){
         searchRegister(byteOffSet);
       } else{
         printf("Não encontrado\n");

       }
   }
}
char* extractAuthor(const char* data){
    char buffer[BUFFER_SIZE];
    strcpy(buffer, data);
    strtok(buffer, "'");
    strcpy(buffer, strtok(NULL, "'"));

    char* author = malloc(strlen(buffer) + 1);
    if (author != NULL) 
        strcpy(author, buffer);
    return author;
}
int extractID(char* data){
    int id; 
    char buffer[BUFFER_SIZE];
    strcpy(buffer, data);
    strtok(buffer, "'");
    strcpy(buffer, strtok(NULL, "'"));

    id = atoi(buffer);
    return id;
}
BookRecord extractData(char* data){
    BookRecord book;
    char id[BUFFER_SIZE];
    char title[BUFFER_SIZE];
    char author[BUFFER_SIZE];
    sscanf (data, "ADD id = '%[^']' titulo = '%[^']' autor = '%[^']'", id, title, author);
    
    char **datas = malloc(sizeof(char *) * DATA_SIZE);
    book.id = atoi(id);
    book.title = malloc(sizeof(char) * (strlen(title) + 1));
    strcpy(book.title, title);
    book.author = malloc(sizeof(char) * (strlen(author) + 1));
    strcpy(book.author, author);
    return book; 
}

int createFiles(FILE** dataFile, FILE** idxPrimary, FILE** idxSecondary) {
    int flag = FALSE;
    if ((*dataFile = fopen("dataFile.bin", "wb")) != NULL) {
        if ((*idxPrimary = fopen("primaryIdx.bin", "wb")) != NULL) {
            if ((*idxSecondary = fopen("secondaryIdx.bin", "wb")) != NULL) {
                flag = TRUE;
            } else {
                fclose(*idxPrimary);
                fclose(*dataFile);
            }
        } else {
            fclose(*dataFile);
        }
    }
    return flag;
}

void printLine() {
    printf("----------------------------------------------------------\n");
}