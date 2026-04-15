# TaskFlow-Pro-CLI
# TaskFlow Pro CLI

A command-line task manager written in C. Lets you add, view, and delete tasks with deadlines — stored persistently in a binary file.

---

## Features

- Add tasks with title, description, priority, and deadline
- View all tasks sorted by deadline (earliest first)
- Filter to show only pending tasks
- Auto-detects overdue tasks and highlights them in red
- Color-coded status: green (done), yellow (pending), red (overdue)
- Tasks persist between runs using binary file storage (`tasks.dat`)

---

## How to Compile and Run

**Requirements:** GCC (any version supporting C99 or later)

```bash
gcc main.c -o taskflow
./taskflow
```

---

## Usage

```
==== TASKFLOW PRO CLI ====
1. Add Task
2. View All Tasks
3. View Pending Tasks
4. Delete Task
5. Exit
```

- Tasks are stored in `tasks.dat` in the same directory as the binary
- Each task gets a unique auto-incremented ID
- Deadline format when adding: `DD MM YYYY` (e.g., `25 12 2026`)

---

## Project Structure

```
TaskFlow-Pro-CLI/
└── main.c      # All source code in a single file
```

---

## Known Limitations

- Max 100 tasks loaded in memory at once
- Title limited to 50 characters, description to 100 characters
- No task editing — delete and re-add to update
- No mark-as-complete feature in current version

---

## Language

C (standard C99) — no external libraries required
