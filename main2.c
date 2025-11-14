#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50 //number of students

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

int* isLate(Entry *e, char *deadline)
{
    int *late_arr = malloc(3 * sizeof(int));
    late_arr[0] = 0; // 0 = not late, 1 = late
    late_arr[1] = 0; // duration
    late_arr[2] = 0; // type

    char tempDeadline[200], tempTime[200];
    strcpy(tempDeadline, deadline);
    strcpy(tempTime, e->timestamp);

    int d, m, y, h, mi;
    int ed, em, ey, eh, emi;

    sscanf(tempDeadline, "%d/%d/%d %d:%d", &d, &m, &y, &h, &mi);
    sscanf(tempTime, "%d/%d/%d %d:%d", &ed, &em, &ey, &eh, &emi);

    if (ey > y) { late_arr[0] = 1; late_arr[1] = ey - y; late_arr[2] = 5; }
    else if (ey < y) return late_arr;

    else if (em > m) { late_arr[0] = 1; late_arr[1] = em - m; late_arr[2] = 4; }
    else if (em < m) return late_arr;

    else if (ed > d) { late_arr[0] = 1; late_arr[1] = ed - d; late_arr[2] = 3; }
    else if (ed < d) return late_arr;

    else if (eh > h) { late_arr[0] = 1; late_arr[1] = eh - h; late_arr[2] = 2; }
    else if (eh < h) return late_arr;

    else if (emi > mi) { late_arr[0] = 1; late_arr[1] = emi - mi; late_arr[2] = 1; }

    return late_arr;
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
    if (deadlineSet) {new->flag = isLate(new, currentDeadline)[0];}

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
        subStack[t]->flag = isLate(subStack[t], time)[0];

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

void calcPenalty(int id, int totalMarks)
{
    Entry *e;

    int found = 0;
    for (int t = 0; t < top; t++)
    {
        if (subStack[t]->id == id)
        {
            e = subStack[t];
            found = 1;
            break;
        }
    }

    if (found == 0) {
        printf("No submission found for ID %d.\n", id);
        return;
    }

    int *late_info = isLate(e, currentDeadline);
    if (late_info[0] == 1) {

        int duration = late_info[1];
        int type = late_info[2];
        float penalty = 0.0;

        switch(type) {
            case 1: // minutes
                printf("\n\tLate by: %d minutes\n" , late_info[1]);
                penalty = duration * 0.1f;
                break;
            case 2: // hours
                printf("\n\tLate by: %d hours\n" , late_info[1]);
                penalty = duration * 0.5f;
                break;
            case 3: // days
                printf("\n\tLate by: %d days\n" , late_info[1]);
                penalty = duration * 1.0f;
                break;
            case 4: // months
                printf("\n\tLate by: %d months\n" , late_info[1]);
                penalty = totalMarks;
                break;
            case 5: // years
                printf("\n\tLate by: %d years\n" , late_info[1]);
                penalty = totalMarks;
                break;
        }

        printf("\tPenalty for submission ID %d is %.2f marks.\n\n", e->id, penalty);
    } else {
        printf("\tNo penalty for submission ID %d.\n\n", e->id);
    }
    free(late_info);
}

int main()
{
    int ch, stdid, totalMarks;

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
        printf("1. UPLOAD\n2. SET DEADLINE\n3. CHECK STATUS\n4. DISPLAY LATE SUBMISSIONS\n5. UPDATE THE DATA FILE AND END TASKS\n6. CLEAR THE FILE AND END\n7. CACULATE PENALTY\n");
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

            case 7:
                printf("Enter the id: ");
                scanf("%d" , &stdid);
                getchar();
                printf("Enter total marks for this assignment: ");
                scanf("%d" , &totalMarks);
                getchar();
                calcPenalty(stdid, totalMarks);
                break;
        }

    } while (ch != 5 && ch != 6);

    return 0;
}
