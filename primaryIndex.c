#include "dataFile.h"

void createNode(PrimaryIdx** head, int id, long byteOffset) {
    PrimaryIdx* newNode = (PrimaryIdx*) malloc(sizeof(PrimaryIdx));
    newNode->id = id;
    newNode->byteOffset = byteOffset;
    newNode->next = NULL;

    if (*head == NULL || id < (*head)->id) {
        newNode->next = *head;
        *head = newNode;
    } else {
        PrimaryIdx* current = *head;
        while (current->next != NULL && id > current->next->id) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

void freePrimaryIndex(PrimaryIdx** head) {
    PrimaryIdx* current = *head;
    PrimaryIdx* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;  
}

void insertPrimaryIndex(PrimaryIdx* head) {
    PrimaryIdx* current = head;
    
    if (current == NULL)
        return;
    
    FILE* idxPrimary = fopen("primaryIdx.bin", "wb");
    
    if (idxPrimary != NULL) {
        while (current != NULL) {
            fseek(idxPrimary, 0, SEEK_END);
            fwrite(&current->id, sizeof(int), 1, idxPrimary);
            fwrite(&current->byteOffset, sizeof(long), 1, idxPrimary);
            current = current->next;
        }
        fclose(idxPrimary);
    }
}

long searchByID(int id) {
    FILE* file = fopen("primaryIdx.bin", "rb");
    
    if (file == NULL)
        return -1;
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long numRecords = fileSize / (sizeof(int) + sizeof(long));
    
    long start = 0;
    long end = numRecords - 1;
    long byteOffset = -1;
    
    while (start <= end) {
        long mid = (start + end) / 2;
        
        fseek(file, mid * (sizeof(int) + sizeof(long)), SEEK_SET);
        
        int recordId;
        fread(&recordId, sizeof(int), 1, file);
        if (recordId == id) {
            fread(&byteOffset, sizeof(long), 1, file);
            break;
        }
        else if (recordId < id) {
            start = mid + 1;
        }
        else {
            end = mid - 1;
        }
    }
    
    fclose(file);
    
    return byteOffset;
}
