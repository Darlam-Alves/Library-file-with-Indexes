#include "dataFile.h"

int main(){
    char **data;
    int totalData = 0;
   
    data = malloc(sizeof(char *) * NUM_RECORDS);
    readString(&data[totalData]);
    while (strcmp(data[totalData], "EXIT") != 0){
        totalData++;
        readString(&data[totalData]);
    }

    char **operations = malloc(sizeof(char *) * totalData);
    for (int i = 0; i < totalData; i++) 
        operations[i] = typeofOperations(data[i]);
    executeOperations(operations, data, totalData);
   
    for (int i = 0; i < totalData; i++)
        free(data[i]);
    free(data);

     for (int i = 0; i < totalData; i++)
        free(operations[i]);
    free(operations);
}

void readBuffer(char *buffer, int bufferSize){
    scanf(" %[^(\r|\n)]", buffer);
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

void executeOperations (char** operations, char** data, int size){
    FILE* dataFile; 
    FILE* idxPrimary; 
    FILE* idxSecondary;

    createFiles(&dataFile, &idxPrimary, &idxSecondary);

    for (int i = 0; i < size; i++){
        if(strcmp(operations[i], "ADD") == 0){
            BookRecord book = extractData(data[i]);
            insertDataFile(book);
            free (book.title);
            free (book.author);
        }
        if(strcmp(operations[i], "REMOVE") == 0){
            removeData(data[i]);
        }
        if(strcmp(operations[i], "SEARCH") == 0){
            searchData(data[i]);
        }
        if(strcmp(operations[i], "EXIT") == 0)
            exit(0);
    }

    fclose(dataFile);
    fclose(idxPrimary);
    fclose(idxSecondary);
}
void removeData(char* data){
   if (strstr(data, "autor=") != NULL){
        char* author = extractAuthor(data);
        free(author);
   } else if (strstr(data, "id=") != NULL){
        int id = extractID(data);
   }
}
void searchData(char* data){
   if (strstr(data, "autor=") != NULL){
        int* ids;
        char* author = extractAuthor(data);
        ids = searchByAuthor(author); 
        if (ids == NULL) {
            printf("Nao encontrado");
        } else {
            // Do something with the IDs here
            for (int i = 0; ids[i] != '\0'; i++) {
                //printf("Matching ID: %d\n", ids[i]);
            }
            free(ids);
        }
        free(author);
   } else if (strstr(data, "id=") != NULL){
       int id = extractID(data);
       long byteOffSet = searchByID(id);
       //printf ("id = %d", id);
       printf("%ld", byteOffSet);
       //pesquisa no arquivo de dados

   }
}
char* extractAuthor(char* data){
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
    char buffer[BUFFER_SIZE], aux[BUFFER_SIZE];
    char *token;
    char **datas = malloc(sizeof(char *) * DATA_SIZE);
    int i = 0;
    char *result = malloc(sizeof(char) * BUFFER_SIZE);
    strcpy(buffer, data);
    token = strtok(buffer, "'");
    while (token != NULL){
        strcpy(aux, strtok(NULL, "'"));
        datas[i] = malloc(sizeof(char) * (strlen(aux) + 1));
        token = strtok(NULL, "'");
        strcpy(datas[i], aux);
        i++;
    }
    book.id = atoi(datas[0]);
    book.title = malloc(sizeof(char) * (strlen(datas[1]) + 1));
    strcpy(book.title, datas[1]);
    book.author = malloc(sizeof(char) * (strlen(datas[2]) + 1));
    strcpy(book.author, datas[2]);

    for (int j = 0; j < DATA_SIZE; j++)
        free(datas[j]);
    free(datas);
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

