#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 60 //number of students

typedef struct submission{
    int flag;
    char timestamp[200];
    int id;
}Entry;

Entry *subStack[MAX];
int top = 0;
char currentDeadline[200] = "";
int deadlineSet = 0;

Entry * newEntry(int id, char *time)
{
    Entry *new;
    new = (Entry*)malloc(sizeof(Entry));
    new->flag = 0;
    new->id = id;
    strcpy(new->timestamp , time);

    return new;
}

int isLate(Entry *e, char *deadline)
{
    char tempDeadline[200], tempTime[200];
    strcpy(tempDeadline, deadline);
    strcpy(tempTime, e->timestamp);

    // DEADLINE SPLIT
    char *temp = strtok(tempDeadline, "/");
    int d = atoi(temp);
    temp = strtok(NULL, "/");
    int m = atoi(temp);
    temp = strtok(NULL, " ");
    int y = atoi(temp);
    temp = strtok(NULL, ":");
    int h = atoi(temp);
    temp = strtok(NULL, " ");
    int mi = atoi(temp);

    // ENTRY TIME SPLIT
    temp = strtok(tempTime, "/");
    int ed = atoi(temp);
    temp = strtok(NULL, "/");
    int em = atoi(temp);
    temp = strtok(NULL, " ");
    int ey = atoi(temp);
    temp = strtok(NULL, ":");
    int eh = atoi(temp);
    temp = strtok(NULL, " ");
    int emi = atoi(temp);

    // Compare numerically 
    if (ey > y) return 1;
    if (ey < y) return 0;
    if (em > m) return 1;
    if (em < m) return 0;
    if (ed > d) return 1;
    if (ed < d) return 0;
    if (eh > h) return 1;
    if (eh < h) return 0;
    if (emi > mi) return 1;

    return 0;
}

void uploadSubmission(int id)
{
    if (top == MAX) {
        printf("Maximum submission amount already met!!\n");
        return;
    }

    char time[200];
    printf("Enter the time of submission (dd/mm/yy hh:mm): ");
    fgets(time, 200, stdin);
    time[strcspn(time, "\n")] = '\0'; // Remove newline

    Entry *new = newEntry(id , time);
    if (deadlineSet) {new->flag = isLate(new, currentDeadline);}

    subStack[top++] = new;

    printf("Submission logged!\n");
}

void getDeadline()
{
    char time[200];
    printf("Enter the deadline time (dd/mm/yy hh:mm): ");
    fgets(time, 200, stdin);
    time[strcspn(time, "\n")] = '\0'; // Remove newline
    strcpy(currentDeadline, time);
    deadlineSet = 1;

    for (int t = 0; t < top; t++)
        subStack[t]->flag = isLate(subStack[t], time);

    printf("All entries modified according to new deadline\n");
}

void displayLate()
{
    printf("\tID\tTIME OF SUBMISSION\n");
    for (int t = 0; t < top; t++)
    {
        if (subStack[t]->flag == 1)
            printf("\t%d\t%s\n" , subStack[t]->id , subStack[t]->timestamp);
    }
}

void getStatus(int id)
{
    int found = 0;
    for (int t = 0; t < top; t++)
    {
        if (subStack[t]->id == id)
        {
            found = 1;
            if (subStack[t]->flag == 1)
                printf("\t%d\tLATE\n" , subStack[t]->id);
            else
                printf("\t%d\tSUBMITTED IN TIME\n" , subStack[t]->id);
            break;
        }
    }

    if (!found)
        printf("\t%d\tNOT SUBMITTED\n" , id);
}

void endTasks()
{
    FILE *fptr = fopen("subs.txt", "w");
    if (fptr == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    // Write all current entries back into the file
    for (int i = 0; i < top; i++) {
        fprintf(fptr, "%03d,%s\n", subStack[i]->id, subStack[i]->timestamp);
    }

    fclose(fptr);

    // Free all allocated memory
    for (int i = 0; i < top; i++) {
        free(subStack[i]);
        subStack[i] = NULL;
    }

    top = 0; // Reset stack counter
    printf("All tasks finalized. Memory freed and subs.txt updated.\n");
}

void clearFile()
{
    FILE *fptr = fopen("subs.txt", "w");
    if (fptr == NULL) {
        printf("Error opening file for clearing!\n");
        return;
    }
    fclose(fptr);

    // Free all allocated memory
    for (int i = 0; i < top; i++) {
        free(subStack[i]);
        subStack[i] = NULL;
    }

    top = 0; // Reset stack counter
    printf("Memory freed and subs.txt cleared.\n");
}

int main()
{
    int ch, stdid;

    printf("=====SUBMISSION TRACKER=====\n");
    printf("WELCOME TO THE SUBMISSION TRACKER PROGRAM\n\n");
    printf("TIME FORMAT (24 HOUR FORMAT): dd/mm/yy hh:mm\n");
    printf("REMEMBER TO SET DEADLINE, ELSE ALL SUBMISSIONS WILL BE MARKED AS EARLY\n\n");
    FILE *fptr;
    char line[200];
    int id;
    char time[200];

    fptr = fopen("subs.txt", "r");
    if (fptr == NULL) {printf("Error in opening the file!! PROGRAM ABORTED.\n"); exit(0);}
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        id = atoi(strtok(line, ","));
        strcpy(time, strtok(NULL, "\n"));
        Entry *new = newEntry(id, time);
        subStack[top++] = new;
        printf("Loaded entry: ID %d %s\n", id, time);
    }
    fclose(fptr);
    
    do
    {
        printf("-----MENU-----\n");
        printf("1. UPLOAD\n2. SET DEADLINE\n3. CHECK STATUS\n4. DISPLAY LATE SUBMISSIONS\n5. UPDATE THE DATA FILE AND END TASKS\n6. CLEAR THE FILE AND END\n");
        printf("Enter your choice: ");
        scanf("%d" , &ch);
        getchar();

        switch (ch)
        {
            case 1:
                printf("Enter the id: ");
                scanf("%d" , &stdid);
                getchar();
                uploadSubmission(stdid);
                break;

            case 2:
                getDeadline();
                break;

            case 3:
                printf("Enter the id: ");
                scanf("%d" , &stdid);
                getchar();
                getStatus(stdid);
                break;

            case 4:
                displayLate();
                break;

            case 5:
                endTasks();
                printf("Exiting program. Goodbye!\n");
                break;

            case 6:
                clearFile();
                printf("Exiting program. Goodbye!\n");
                break;
        }

    } while (ch != 5 && ch != 6);

    return 0;
}
