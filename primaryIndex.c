#include "dataFile.h"

void createNode(PrimaryIdx** head, int id, long byteOffset) {
    PrimaryIdx* newNode = (PrimaryIdx*)malloc(sizeof(PrimaryIdx));
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

void insertPrimaryIndex(PrimaryIdx** head) {
    if (*head == NULL) 
        return; 
    
    FILE* idxPrimary;
    if (openFile(&idxPrimary, "primaryIdx.bin", "wb")) {
        while ((*head) != NULL) {
            printf("%d\n", (*head)->id);
            fseek(idxPrimary, 0, SEEK_END);
            fwrite(&(*head)->id, sizeof(int), 1, idxPrimary);
            fwrite(&(*head)->byteOffset, sizeof(long), 1, idxPrimary);
            (*head) = (*head)->next;
        }
        fclose(idxPrimary);
    }
}

/* void traversal(PrimaryIdx* head){
    PrimaryIdx* aux = head;
    while (aux != NULL){
        printf("%d\n", aux->id);
        aux = aux->next;
    }
} */


/* void insertPrimaryIndex (int id, long byteOffSet){
    FILE* idxPrimary; 
    if (openFile(&idxPrimary, "primaryIdx.bin", "rb+")){
        fseek(idxPrimary, 0, SEEK_END);
        fwrite(&id, sizeof(int), 1, idxPrimary);
        fwrite(&byteOffSet, sizeof(long), 1, idxPrimary);
        fclose(idxPrimary);
    }
} */