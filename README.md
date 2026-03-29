# C CLI Task Manager

A command-line task management application written in C for Windows. This tool allows users to create, track, and complete tasks, featuring a built-in visual countdown timer for productivity tracking.

## Features

* **Task Tracking:** Add tasks with titles and descriptions.
* **Productivity Timer:** Set custom timers for individual tasks with a live, color-coded visual progress bar.
* **Persistent Storage:** Tasks are automatically saved to and loaded from a local `LIST.txt` file.
* **Backup System:** Create manual backups of your task list (`BACKUP_LIST.txt`) at the press of a button.
* **Color-Coded UI:** Uses Windows console text attributes to provide clear, color-coded feedback for different statuses and alerts.

## Prerequisites

Because this project utilizes `<windows.h>` and `<conio.h>` for console coloring and keyboard inputs, it requires a **Windows environment** to compile and run.

* A C Compiler (like GCC via MinGW, or MSVC)
* Windows Operating System

## Compilation and Usage

Open your terminal or command prompt and compile the code using your preferred C compiler. For example, using GCC:

```bash
gcc task_manager.c -o task_manager