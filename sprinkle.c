#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "rabbit.h"
#include "sprinkle.h"

#define log(fmt, ...) { printf("[%s] " fmt "\n", whoami ? whoami : "N/A", ##__VA_ARGS__); }

char* whoami = NULL;

void sprinkle(Rabbit** rabbits, int numberOfRabbits) {
    int fd[numberOfRabbits][2];

    for (int i = 0; i < numberOfRabbits; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    whoami = "parent";

    for (int i = 0; i < numberOfRabbits; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            close(fd[i][0]);
            for (int j = 0; j < numberOfRabbits; j++) {
                if (j != i) {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }

            whoami = "child";
            srand(getpid());
            int claimed = rand() % 20 + 1;
            log("for poem \"%s\" I collected %d eggs", rabbits[i]->poem, claimed);

            write(fd[i][1], &claimed, sizeof(claimed));
            close(fd[i][1]);
            exit(0);
        }

        close(fd[i][1]);
    }

    // Parent reads results from each child via its pipe
    for (int i = 0; i < numberOfRabbits; i++) {
        int claimed;
        if (read(fd[i][0], &claimed, sizeof(claimed)) == sizeof(claimed)) {
            rabbits[i]->numberOfEggs += claimed;
            log("added %d eggs to %s's count", claimed, rabbits[i]->name);
        }
        close(fd[i][0]);
    }

    // Wait for all children to exit
    for (int i = 0; i < numberOfRabbits; i++) {
        wait(NULL);
    }

    log("All children have exited, sprinkling is done!");
}