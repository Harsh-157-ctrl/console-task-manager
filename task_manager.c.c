#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define MAX_TASKS 100
#define FILENAME "LIST.txt"

typedef struct {
    char title[100];
    char description[300];
    int timer_duration;
    int has_timer;
    int completed;
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

void clearScreen() {
    system("cls");
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void saveTasksToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        setColor(12);
        printf("Error: Could not save tasks to file!\n");
        setColor(7);
        return;
    }

    fprintf(file, "=======================================\n");
    fprintf(file, "         TASK MANAGER - TASK LIST\n");
    fprintf(file, "=======================================\n");
    fprintf(file, "Total Tasks: %d\n\n", task_count);

    if (task_count == 0) {
        fprintf(file, "No tasks found.\n");
    } else {
        for (int i = 0; i < task_count; i++) {
            fprintf(file, "TASK #%d:\n", i + 1);
            fprintf(file, "--------\n");
            fprintf(file, "Title: %s\n", tasks[i].title);
            fprintf(file, "Description: %s\n", tasks[i].description);

            if (tasks[i].has_timer) {
                if (tasks[i].timer_duration >= 60) {
                    int hours = tasks[i].timer_duration / 60;
                    int minutes = tasks[i].timer_duration % 60;
                    fprintf(file, "Timer: %d minutes (%d hours %d minutes)\n", 
                            tasks[i].timer_duration, hours, minutes);
                } else {
                    fprintf(file, "Timer: %d minutes\n", tasks[i].timer_duration);
                }
            } else {
                fprintf(file, "Timer: Not set\n");
            }

            fprintf(file, "Status: %s\n", tasks[i].completed ? "COMPLETED" : "PENDING");
            fprintf(file, "\n");
        }
    }

    fprintf(file, "=======================================\n");
    fprintf(file, "End of Task List\n");
    fprintf(file, "=======================================\n");

    fclose(file);
    setColor(10);
    printf("Tasks saved to %s in readable format!\n", FILENAME);
    setColor(7);
}

void loadTasksFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        setColor(14);
        printf("No existing task file found. Starting fresh!\n");
        setColor(7);
        task_count = 0;
        return;
    }

    char line[500];
    task_count = 0;
    int current_task = -1;

    while (fgets(line, sizeof(line), file) && task_count < MAX_TASKS) {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "TASK #", 6) == 0) {
            current_task = task_count;
            task_count++;
            tasks[current_task].has_timer = 0;
            tasks[current_task].timer_duration = 0;
            tasks[current_task].completed = 0;
            strcpy(tasks[current_task].title, "");
            strcpy(tasks[current_task].description, "");
        } else if (strncmp(line, "Title: ", 7) == 0 && current_task >= 0) {
            strcpy(tasks[current_task].title, line + 7);
        } else if (strncmp(line, "Description: ", 13) == 0 && current_task >= 0) {
            strcpy(tasks[current_task].description, line + 13);
        } else if (strncmp(line, "Timer: ", 7) == 0 && current_task >= 0) {
            if (strcmp(line + 7, "Not set") != 0) {
                tasks[current_task].has_timer = 1;
                sscanf(line + 7, "%d", &tasks[current_task].timer_duration);
            }
        } else if (strncmp(line, "Status: ", 8) == 0 && current_task >= 0) {
            if (strcmp(line + 8, "COMPLETED") == 0) {
                tasks[current_task].completed = 1;
            }
        }
    }

    fclose(file);
    setColor(10);
    printf("Loaded %d tasks from %s!\n", task_count, FILENAME);
    setColor(7);
}

void backupTasks() {
    char backupName[50];
    sprintf(backupName, "BACKUP_LIST.txt");

    FILE *original = fopen(FILENAME, "r");
    FILE *backup = fopen(backupName, "w");

    if (original == NULL || backup == NULL) {
        setColor(12);
        printf("Error creating backup!\n");
        setColor(7);
        if (original) fclose(original);
        if (backup) fclose(backup);
        return;
    }

    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), original)) {
        fputs(buffer, backup);
    }

    fclose(original);
    fclose(backup);

    setColor(10);
    printf("Backup created as %s!\n", backupName);
    setColor(7);
}

void playAlert() {
    for (int i = 0; i < 3; i++) {
        MessageBeep(MB_ICONEXCLAMATION);
        Sleep(200);
    }
}

void displayTask(int index) {
    if (index >= 0 && index < task_count) {
        if (tasks[index].completed) {
            setColor(10);
            printf("[%d] [COMPLETED] %s\n", index + 1, tasks[index].title);
        } else {
            setColor(15);
            printf("[%d] %s\n", index + 1, tasks[index].title);
        }

        setColor(7);
        printf("    Description: %s\n", tasks[index].description);

        if (tasks[index].has_timer) {
            setColor(14);
            if (tasks[index].timer_duration >= 60) {
                int hours = tasks[index].timer_duration / 60;
                int minutes = tasks[index].timer_duration % 60;
                printf("    Timer: %d minutes (%d hours %d minutes)\n", 
                        tasks[index].timer_duration, hours, minutes);
            } else {
                printf("    Timer: %d minutes\n", tasks[index].timer_duration);
            }
        } else {
            setColor(8);
            printf("    Timer: Not set\n");
        }
        setColor(7);
    }
}

void addTask() {
    if (task_count >= MAX_TASKS) {
        setColor(12);
        printf("Task list full! Maximum %d tasks allowed.\n", MAX_TASKS);
        setColor(7);
        return;
    }

    getchar();

    setColor(11);
    printf("Enter task title: ");
    setColor(15);
    fgets(tasks[task_count].title, sizeof(tasks[task_count].title), stdin);
    tasks[task_count].title[strcspn(tasks[task_count].title, "\n")] = 0;

    setColor(11);
    printf("Enter task description: ");
    setColor(15);
    fgets(tasks[task_count].description, sizeof(tasks[task_count].description), stdin);
    tasks[task_count].description[strcspn(tasks[task_count].description, "\n")] = 0;

    char timer_choice;
    setColor(14);
    printf("Set a timer for this task? (y/n): ");
    setColor(15);
    scanf(" %c", &timer_choice);

    if (timer_choice == 'y' || timer_choice == 'Y') {
        setColor(14);
        printf("Enter timer duration (in minutes): ");
        setColor(15);
        scanf("%d", &tasks[task_count].timer_duration);

        if (tasks[task_count].timer_duration <= 0) {
            setColor(12);
            printf("Invalid duration! Setting to 1 minute.\n");
            tasks[task_count].timer_duration = 1;
        }

        tasks[task_count].has_timer = 1;
        setColor(10);
        printf("Timer set for %d minutes.\n", tasks[task_count].timer_duration);
    } else {
        tasks[task_count].has_timer = 0;
        tasks[task_count].timer_duration = 0;
    }

    tasks[task_count].completed = 0;
    task_count++;

    saveTasksToFile();

    setColor(10);
    printf("Task added successfully and saved to file!\n");
    setColor(7);
}

void startTimer(int index) {
    if (index < 0 || index >= task_count) {
        setColor(12);
        printf("Invalid task number!\n");
        setColor(7);
        return;
    }

    if (!tasks[index].has_timer) {
        setColor(12);
        printf("No timer set for this task!\n");
        setColor(7);
        return;
    }

    if (tasks[index].completed) {
        setColor(14);
        printf("This task is already completed!\n");
        setColor(7);
        return;
    }

    clearScreen();
    setColor(11);
    printf("=======================================\n");
    printf("         TIMER STARTED\n");
    printf("=======================================\n");
    printf("Task: %s\n", tasks[index].title);
    printf("Duration: %d minutes\n", tasks[index].timer_duration);
    printf("Press any key to stop the timer early...\n");
    printf("=======================================\n\n");
    setColor(7);

    int totalSeconds = tasks[index].timer_duration * 60;
    int timeLeft = totalSeconds;

    while (timeLeft > 0) {
        if (_kbhit()) {
            char key = _getch();
            setColor(14);
            printf("\n\nTimer stopped by user!\n");
            int elapsed_minutes = (totalSeconds - timeLeft) / 60;
            int elapsed_seconds = (totalSeconds - timeLeft) % 60;
            printf("Time elapsed: %d minutes %d seconds\n", elapsed_minutes, elapsed_seconds);
            setColor(7);
            return;
        }

        int minutes = timeLeft / 60;
        int seconds = timeLeft % 60;

        if (timeLeft <= 60) {
            setColor(12);
        } else if (timeLeft <= 300) {
            setColor(14);
        } else {
            setColor(10);
        }

        printf("\rTime remaining: %02d:%02d ", minutes, seconds);

        int progress = ((totalSeconds - timeLeft) * 20) / totalSeconds;
        printf("[");
        for (int i = 0; i < 20; i++) {
            if (i < progress) {
                printf("=");
            } else {
                printf(" ");
            }
        }
        printf("] %.1f%%", ((float)(totalSeconds - timeLeft) / totalSeconds) * 100);

        fflush(stdout);
        Sleep(1000);
        timeLeft--;

        if (timeLeft > 0 && timeLeft <= 300 && timeLeft % 60 == 0) {
            MessageBeep(MB_ICONINFORMATION);
        }
    }

    clearScreen();
    setColor(12);
    printf("=======================================\n");
    printf("         TIME'S UP!\n");
    printf("=======================================\n");
    setColor(10);
    printf("Task completed: %s\n", tasks[index].title);
    printf("Duration: %d minutes\n", tasks[index].timer_duration);
    setColor(12);
    printf("ALERT! Timer finished!\n");
    printf("=======================================\n");
    setColor(7);

    playAlert();

    char complete_choice;
    setColor(11);
    printf("\nMark this task as completed? (y/n): ");
    setColor(15);
    scanf(" %c", &complete_choice);

    if (complete_choice == 'y' || complete_choice == 'Y') {
        tasks[index].completed = 1;
        saveTasksToFile();
        setColor(10);
        printf("Task marked as completed and saved!\n");
        setColor(7);
    }
}

void markTaskCompleted() {
    if (task_count == 0) {
        setColor(14);
        printf("No tasks available!\n");
        setColor(7);
        return;
    }

    for (int i = 0; i < task_count; i++) {
        displayTask(i);
        printf("\n");
    }

    int choice;
    setColor(11);
    printf("Enter task number to mark as completed (0 to cancel): ");
    setColor(15);
    scanf("%d", &choice);

    if (choice > 0 && choice <= task_count) {
        if (tasks[choice - 1].completed) {
            setColor(14);
            printf("Task is already completed!\n");
        } else {
            tasks[choice - 1].completed = 1;
            saveTasksToFile();
            setColor(10);
            printf("Task marked as completed and saved!\n");
        }
    } else if (choice != 0) {
        setColor(12);
        printf("Invalid task number!\n");
    }
    setColor(7);
}

void viewTasks() {
    if (task_count == 0) {
        setColor(14);
        printf("No tasks to display.\n");
        setColor(7);
        return;
    }

    while (1) {
        clearScreen();
        setColor(11);
        printf("========= TASK LIST =========\n\n");

        for (int i = 0; i < task_count; i++) {
            displayTask(i);
            printf("\n");
        }

        setColor(11);
        printf("\nOptions:\n");
        printf("[1] Start Timer for a Task\n");
        printf("[2] Mark Task as Completed\n");
        printf("[0] Return to Main Menu\n");
        printf("Enter your choice: ");
        setColor(15);
        int choice, task_index;
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
            case 1:
                setColor(11);
                printf("Enter task number to start timer: ");
                setColor(15);
                scanf("%d", &task_index);
                startTimer(task_index - 1);
                break;
            case 2:
                markTaskCompleted();
                break;
            default:
                setColor(12);
                printf("Invalid choice! Try again.\n");
                Sleep(1000);
                break;
        }
    }
    setColor(7);
}

int main() {
    loadTasksFromFile();

    while (1) {
        clearScreen();
        setColor(11);
        printf("=======================================\n");
        printf("        TASK MANAGER - MAIN MENU\n");
        printf("=======================================\n");
        setColor(15);
        printf("[1] Add New Task\n");
        printf("[2] View Tasks\n");
        printf("[3] Backup Tasks\n");
        printf("[4] Save Tasks to File\n");
        printf("[0] Exit\n");
        setColor(11);
        printf("Enter your choice: ");
        setColor(15);

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                backupTasks();
                break;
            case 4:
                saveTasksToFile();
                break;
            case 0:
                setColor(10);
                printf("Exiting... Goodbye!\n");
                setColor(7);
                exit(0);
            default:
                setColor(12);
                printf("Invalid choice! Try again.\n");
                Sleep(1000);
                break;
        }

        setColor(7);
        printf("\nPress any key to return to the main menu...");
        _getch();
    }

    return 0;
}
