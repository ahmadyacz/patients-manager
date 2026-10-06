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

patient *head = NULL;

int load(FILE *data);
void place(patient *current, patient *new_patient);
void search(int id, patient *node);
void free_tree(patient *node);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s data.csv\n", argv[0]);
        return 1;
    }

    int len = strlen(argv[1]);
    if (len < 4 || strcmp(argv[1] + len - 4, ".csv") != 0) {
        printf("The app accepts only .csv files\n");
        return 2;
    }

    FILE *database = fopen(argv[1], "r");
    if (database == NULL) {
        printf("Couldn't open the data file\n");
        return 3;
    }

    if (load(database) != 0) {
        printf("Error: Couldn't load the data\n");
        fclose(database);
        return 4;
    }
    fclose(database);

    char id_buffer[20];
    printf("Id: ");
    if (fgets(id_buffer, sizeof(id_buffer), stdin) != NULL) {
        search(atoi(id_buffer), head);
    }

    free_tree(head);
    return 0;
}

int load(FILE *data)
{
    char line[124];
    bool skip = false;

    while (fgets(line, sizeof(line), data) != NULL) {
        if (!skip) {
            skip = true; // Skip header row
            continue;
        }

        patient *new_patient = malloc(sizeof(patient));
        if (new_patient == NULL) return 1;

        char *word = strtok(line, ",\r\n");
        int field = 0;

        while (word != NULL) {
            if (field == 0) {
                new_patient->id = atoi(word);
            } else if (field == 1) {
                strncpy(new_patient->name, word, sizeof(new_patient->name) - 1);
                new_patient->name[sizeof(new_patient->name) - 1] = '\0';
            } else if (field == 2) {
                new_patient->emergency_level = atoi(word);
            } else if (field == 3) {
                new_patient->room = atoi(word);
            }
            word = strtok(NULL, ",\r\n");
            field++;
        }

        new_patient->left = NULL;
        new_patient->right = NULL;

        if (head == NULL) {
            head = new_patient;
        } else {
            place(head, new_patient);
        }
    }
    return 0;
}

void place(patient *current, patient *new_patient)
{
    if (new_patient->id < current->id) {
        if (current->left == NULL) {
            current->left = new_patient;
        } else {
            place(current->left, new_patient);
        }
    } else if (new_patient->id > current->id) {
        if (current->right == NULL) {
            current->right = new_patient;
        } else {
            place(current->right, new_patient);
        }
    } else {
        // Duplicate ID found, clean up allocated memory
        free(new_patient);
    }
}

void search(int id, patient *node)
{
    if (node == NULL) {
        printf("Couldn't find the wanted person\n");
        return;
    }

    if (id == node->id) {
        printf("Name:%s Room:%d Emergency_level:%d\n", node->name, node->room, node->emergency_level);
    } else if (id < node->id) {
        search(id, node->left);
    } else {
        search(id, node->right);
    }
}

void free_tree(patient *node)
{
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}