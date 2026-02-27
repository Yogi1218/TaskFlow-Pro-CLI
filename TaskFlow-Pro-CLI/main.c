//
//  main.c
//  TaskFlow-Pro-CLI
//
//  Created by Yogi Patel on 27/02/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

struct Task {
    int id;
    char title[50];
    char description[100];
    int priority;
    int completed;
    int day, month, year;
    int cday, cmonth, cyear;
};

void addTask();
void viewTasks();
void deleteTask();
int generateID();
int isOverdue(struct Task t);
void sortTasks(struct Task tasks[], int count);

int main() {
    int choice;

    do {
        printf(CYAN "\n==== TASKFLOW PRO CLI ====\n" RESET);
        printf("1. Add Task\n");
        printf("2. View All Tasks\n");
        printf("3. View Pending Tasks\n");
        printf("4. Delete Task\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: viewTasks(1); break;
            case 4: deleteTask(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while(choice != 5);

    return 0;
}

int generateID() {
    FILE *fp = fopen("tasks.dat", "rb");
    struct Task t;
    int lastID = 0;

    if (fp == NULL) return 1;

    while (fread(&t, sizeof(struct Task), 1, fp))
        lastID = t.id;

    fclose(fp);
    return lastID + 1;
}

void addTask() {
    FILE *fp;
    struct Task t;
    time_t now;
    struct tm *current;

    t.id = generateID();
    t.completed = 0;

    time(&now);
    current = localtime(&now);

    t.cday = current->tm_mday;
    t.cmonth = current->tm_mon + 1;
    t.cyear = current->tm_year + 1900;

    printf("\nEnter Title: ");
    getchar();
    fgets(t.title, sizeof(t.title), stdin);
    t.title[strcspn(t.title, "\n")] = 0;

    printf("Enter Description: ");
    fgets(t.description, sizeof(t.description), stdin);
    t.description[strcspn(t.description, "\n")] = 0;

    printf("Enter Priority (1=High,2=Medium,3=Low): ");
    scanf("%d", &t.priority);

    printf("Enter Deadline (DD MM YYYY): ");
    scanf("%d %d %d", &t.day, &t.month, &t.year);

    fp = fopen("tasks.dat", "ab");
    fwrite(&t, sizeof(struct Task), 1, fp);
    fclose(fp);

    printf(GREEN "\nTask Added Successfully!\n" RESET);
}

int isOverdue(struct Task t) {
    time_t now;
    struct tm *current;
    time(&now);
    current = localtime(&now);

    if (t.completed) return 0;

    if (t.year < current->tm_year + 1900) return 1;
    if (t.year == current->tm_year + 1900 && t.month < current->tm_mon + 1) return 1;
    if (t.year == current->tm_year + 1900 && t.month == current->tm_mon + 1 && t.day < current->tm_mday)
        return 1;

    return 0;
}

void sortTasks(struct Task tasks[], int count) {
    struct Task temp;
    for (int i = 0; i < count-1; i++) {
        for (int j = i+1; j < count; j++) {
            if (tasks[i].year > tasks[j].year ||
               (tasks[i].year == tasks[j].year && tasks[i].month > tasks[j].month) ||
               (tasks[i].year == tasks[j].year && tasks[i].month == tasks[j].month && tasks[i].day > tasks[j].day)) {
                temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }
        }
    }
}

void viewTasks(int pendingOnly) {
    FILE *fp = fopen("tasks.dat", "rb");
    struct Task tasks[100];
    int count = 0;

    if (fp == NULL) {
        printf("No tasks found.\n");
        return;
    }

    while (fread(&tasks[count], sizeof(struct Task), 1, fp))
        count++;

    fclose(fp);

    sortTasks(tasks, count);

    for (int i = 0; i < count; i++) {

        if (pendingOnly && tasks[i].completed)
            continue;

        printf("\nID: %d\n", tasks[i].id);
        printf("Title: %s\n", tasks[i].title);
        printf("Deadline: %02d/%02d/%d\n",
               tasks[i].day, tasks[i].month, tasks[i].year);

        if (tasks[i].completed)
            printf(GREEN "Status: Completed\n" RESET);
        else if (isOverdue(tasks[i]))
            printf(RED "Status: OVERDUE\n" RESET);
        else
            printf(YELLOW "Status: Pending\n" RESET);

        printf("----------------------\n");
    }
}

void deleteTask() {
    FILE *fp = fopen("tasks.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Task t;
    int id, found = 0;

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fread(&t, sizeof(struct Task), 1, fp)) {
        if (t.id != id)
            fwrite(&t, sizeof(struct Task), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove("tasks.dat");
    rename("temp.dat", "tasks.dat");

    if (found)
        printf(GREEN "Task deleted successfully!\n" RESET);
    else
        printf("Task not found.\n");
}
