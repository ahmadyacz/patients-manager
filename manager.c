#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct patient
{
    int id;
    char name[50];
    int emergency_level;
    int room;
    struct patient *left;
    struct patient *right;
} patient;

patient *head;

int load(FILE *data);
void place(patient *current, patient *new);
void search(int id, patient *node);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s data.csv\n", argv[0]);
        return 1;
    }
    if (!(argv[0][strlen(argv[0] - 1)] == 'v' && argv[0][strlen(argv[0] - 2)] == 's' && argv[0][strlen(argv[0] - 3)] == 'c')) {
        printf("The app accept only the csv files\n");
        return 2;
    }
    FILE *database = fopen(argv[1], "r");
    if (database == NULL) {
        printf("Couldn't open the data\n");
        return 3;
    }
    if (load(database) != 0) {
        printf("Error: Couldn't load the data\n");
        return 4;
    }
    char *id;
    printf("Id: ");
    fgets(id, sizeof(int), stdin);
    search(atoi(id), head);
    
}

int load(FILE *data)
{
    char line[124];
    bool skip = false;
    while (fgets(line, sizeof(line), data) != NULL) {
        if (!skip) {
            skip = true;
            continue;
        }
        patient *new = malloc(sizeof(patient));
        char *word = strtok(line, ",");
        int wichone = 0;
        while (word != NULL) {
            if (wichone == 0) {
                new->id = atoi(word);
            } else if (wichone == 1) {
                strcpy(new->name, word);
            } else if (wichone == 2) {
                new->emergency_level = atoi(word);
            } else if (wichone == 3) {
                new->room = atoi(word);
                wichone = 0;
            }
            word = strtok(NULL, ",");
            wichone++;
        }
        new->left = NULL;
        new->right = NULL;
        patient *cursor = head;
        if (cursor == NULL) {
            head = new;
            continue;
        }
        place(cursor, new);
    }
    return 0;
}

void place(patient *current, patient *new)
{
    if (new->id < current->id && current->left == NULL) {
        current->left = new;
    }
    else if (new->id > current->id && current->right == NULL) {
        current->right = new;
    }
    else if (new->id < current->id) {
        place(current->left, new);
    }
    else if (new->id > current->id) {
        place(current->right, new);
    }
    return;
}

void search(int id, patient *node)
{
    if (id == node->id) {
        printf("Name:%s Room:%i Emergency_level:%i\n", node->name, node->room, node->emergency_level);
    }
    else if (id < node->id && node->left != NULL) {
        search(id, node->left);
    }
    else if (id > node->id && node->right != NULL) {
        search(id, node->right);
    }
    else {
        printf("Could't find the wanted person\n");
    }
}