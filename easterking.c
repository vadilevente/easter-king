#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sprinkle.h"

#define FILE_NAME "sprinkle.dat"
#define PROMPT ""

char* rabbitToData(Rabbit* rabbit) {
    char* data = malloc(sizeof(char) * 256);
    strcpy(data, rabbit->name);
    strcat(data, "\n");
    strcat(data, rabbit->poem);
    strcat(data, "\n");
    char eggStr[12];
    sprintf(eggStr, "%d", rabbit->numberOfEggs);
    strcat(data, eggStr);
    strcat(data, "\n");
    return data;
}

char* rabbitToReadable(Rabbit* rabbit) {
    char* readable = malloc(sizeof(char) * 256);
    strcpy(readable, rabbit->name);
    strcat(readable, " | ");
    strcat(readable, rabbit->poem);
    strcat(readable, " | ");
    char eggStr[12];
    sprintf(eggStr, "%d", rabbit->numberOfEggs);
    strcat(readable, eggStr);
    strcat(readable, "\n");
    return readable;
}

int main() {
    int numberOfRabbits = 0;
    int maxRabbits = 2;
    Rabbit** rabbits = malloc(sizeof(Rabbit*) * maxRabbits);

    /* ── Read rabbits from file ── */
    {
        FILE* file = fopen(FILE_NAME, "r");
        if (file == NULL) {
            printf("Could not open %s\n", FILE_NAME);
            return 1;
        }
        char line[256];;
        while (fgets(line, sizeof(line), file)) {
            Rabbit* rabbit = malloc(sizeof(Rabbit));
            line[strlen(line) - 1] = '\0';
            rabbit->name = strdup(line);

            fgets(line, sizeof(line), file);
            line[strlen(line) - 1] = '\0';
            rabbit->poem = strdup(line);

            fgets(line, sizeof(line), file);
            line[strlen(line) - 1] = '\0';
            rabbit->numberOfEggs = atoi(line);

            if (numberOfRabbits >= maxRabbits) {
                maxRabbits *= 2;
                rabbits = realloc(rabbits, sizeof(Rabbit*) * maxRabbits);
            }
            rabbits[numberOfRabbits] = rabbit;
            numberOfRabbits++;
        }
        fclose(file);
    }

    /* ── Interactive menu ── */
    {
        printf("Commands: list | new | edit | delete | sprinkle | king | help | quit\n");
        char input[70];
        do {
            printf("%s\n", PROMPT);
            fgets(input, sizeof(input), stdin);
            input[strlen(input) - 1] = '\0';

            if (strcmp(input, "list") == 0) {
                for (int i = 0; i < numberOfRabbits; i++) {
                    char* out = rabbitToReadable(rabbits[i]);
                    printf("%d. %s", i + 1, out);
                    free(out);
                }
            }
            else if (strcmp(input, "new") == 0) {
                Rabbit* rabbit = malloc(sizeof(Rabbit));

                printf("name: ");
                fgets(input, sizeof(input), stdin);
                input[strlen(input) - 1] = '\0';
                rabbit->name = strdup(input);

                printf("poem: ");
                fgets(input, sizeof(input), stdin);
                input[strlen(input) - 1] = '\0';
                rabbit->poem = strdup(input);

                rabbit->numberOfEggs = 0;

                if (numberOfRabbits >= maxRabbits) {
                    maxRabbits *= 2;
                    rabbits = realloc(rabbits, sizeof(Rabbit*) * maxRabbits);
                }
                rabbits[numberOfRabbits] = rabbit;
                numberOfRabbits++;
                printf("Rabbit added successfully!\n");
            }
            else if (strcmp(input, "edit") == 0) {
                printf("row to edit (1, 2, ...): ");
                fgets(input, sizeof(input), stdin);
                input[strlen(input) - 1] = '\0';
                int index = atoi(input) - 1;

                if (index < 0 || index >= numberOfRabbits) {
                    printf("Invalid index.\n");
                    continue;
                }

                printf("field to edit (name, poem, eggs): ");
                fgets(input, sizeof(input), stdin);
                input[strlen(input) - 1] = '\0';

                if (strcmp(input, "name") == 0) {
                    printf("new name: ");
                    fgets(input, sizeof(input), stdin);
                    input[strlen(input) - 1] = '\0';
                    free(rabbits[index]->name);
                    rabbits[index]->name = strdup(input);
                }
                else if (strcmp(input, "poem") == 0) {
                    printf("new poem: ");
                    fgets(input, sizeof(input), stdin);
                    input[strlen(input) - 1] = '\0';
                    free(rabbits[index]->poem);
                    rabbits[index]->poem = strdup(input);
                }
                else if (strcmp(input, "eggs") == 0) {
                    printf("new egg count: ");
                    fgets(input, sizeof(input), stdin);
                    input[strlen(input) - 1] = '\0';
                    rabbits[index]->numberOfEggs = atoi(input);
                }
                else {
                    printf("Unknown field, edit cancelled.\n");
                }
            }
            else if (strcmp(input, "delete") == 0) {
                printf("row to delete (1, 2, ...): ");
                fgets(input, sizeof(input), stdin);
                input[strlen(input) - 1] = '\0';
                int index = atoi(input) - 1;

                if (index < 0 || index >= numberOfRabbits) {
                    printf("Invalid index.\n");
                    continue;
                }

                free(rabbits[index]->name);
                free(rabbits[index]->poem);
                free(rabbits[index]);

                for (int i = index; i < numberOfRabbits - 1; i++) {
                    rabbits[i] = rabbits[i + 1];
                }
                rabbits[numberOfRabbits - 1] = NULL;
                numberOfRabbits--;
            }
            else if (strcmp(input, "sprinkle") == 0) {
                sprinkle(rabbits, numberOfRabbits);
            }
            else if (strcmp(input, "king") == 0) {
                if (numberOfRabbits < 1) {
                    printf("No rabbits in the list.\n");
                } else {
                    int maxEggs = rabbits[0]->numberOfEggs;
                    int kingIndex = 0;
                    for (int i = 1; i < numberOfRabbits; i++) {
                        if (rabbits[i]->numberOfEggs > maxEggs) {
                            maxEggs = rabbits[i]->numberOfEggs;
                            kingIndex = i;
                        }
                    }
                    printf("All hail the Easter Sprinkling King, %s, owner of %d eggs!\n",
                           rabbits[kingIndex]->name, rabbits[kingIndex]->numberOfEggs);
                }
            }
            else if (strcmp(input, "help") == 0) {
                printf(
                    "list     - print all rabbits\n"
                    "new      - add a new rabbit\n"
                    "edit     - change a field of an existing rabbit\n"
                    "delete   - remove a rabbit from the list\n"
                    "sprinkle - rabbits go sprinkling and collect eggs\n"
                    "king     - announce the Sprinkling King\n"
                    "quit     - save and exit\n"
                );
            }
            else if (strcmp(input, "quit") != 0) {
                printf("Unknown command. Type 'help' for available commands.\n");
            }
        } while (strcmp(input, "quit") != 0);
    }

    /* ── Save rabbits to file ── */
    {
        FILE* file = fopen(FILE_NAME, "w");
        if (file == NULL) {
            printf("Could not open %s for writing\n", FILE_NAME);
            return 1;
        }
        for (int i = 0; i < numberOfRabbits; i++) {
            char* out = rabbitToData(rabbits[i]);
            fprintf(file, "%s", out);
            free(out);
        }
        fclose(file);
        printf("Saved successfully!\n");
    }

    /* ── Free memory ── */
    for (int i = 0; i < numberOfRabbits; i++) {
        free(rabbits[i]->name);
        free(rabbits[i]->poem);
        free(rabbits[i]);
    }
    free(rabbits);

    return 0;
}