# Menu-Driven 2D Graphics Editor in C

> A syllabus-compliant mini-project for **Advanced C Programming with Generative AI**

---

## Problem Statement

Design and implement a menu-driven 2D Graphics Editor in C using a 2D character array as the drawing canvas.

- The canvas is initially filled with `_` (underscore) characters.
- All graphical objects are drawn using `*` (asterisk) characters.
- Objects can be added, deleted, modified, listed, and the canvas can be saved/loaded from a file.

---

## Syllabus Concepts Used

| Concept | Where Applied |
|---------|--------------|
| 2D Arrays | `char canvas[ROWS][COLS]` as the drawing canvas |
| Structures | `struct GraphicObject` stores shape details |
| Array of Structures | `struct GraphicObject *objects` dynamic array |
| Passing arrays to functions | `canvas[ROWS][COLS]` passed to all draw functions |
| Passing structures to functions | `struct GraphicObject objects[]` passed to list/delete/modify |
| Pointers | `**objects` for realloc, `*count`, `*capacity`, `*nextId` |
| Dynamic Memory Allocation | `calloc` (initial), `realloc` (expand), `free` (exit) |
| Strings | `char type[MAX_TYPE_LENGTH]` in struct; `strcpy` used |
| User-defined string logic | `compareString()` replaces `strcmp` |
| File handling | `fopen`, `fprintf`, `fgets`, `fclose` for save/load |
| Error checking | `feof`, `ferror`, `NULL` checks on `fopen` |
| Functions & prototypes | All functions declared before `main` |
| Local & global scope | All variables local; macros global |
| Call by value / reference | `safeInputInteger` returns by value; `addObject` uses `**` |
| Basic math | `abs()`, `round()`, `cos()`, `sin()` for drawing |
| Input validation | `safeInputInteger()` loops on bad input |

---

## Features

1. **Draw objects** — Line, Rectangle, Circle, Triangle
2. **Add** an object interactively with coordinate prompts
3. **Delete** an object by ID (soft delete — sets `active = 0`)
4. **Modify** an object's parameters and auto-redraw
5. **List** all active objects in a table
6. **Display** the canvas with column/row rulers
7. **Clear** the canvas
8. **Save** canvas to `canvas.txt`
9. **Load** canvas from `canvas.txt`

---

## File Structure

```
2d-graphics-editor/
├── main.c       ← All source code (~370 lines)
├── Makefile     ← Build system
└── README.md    ← This file
```

---

## Compile Command

```bash
gcc -Wall -Wextra -std=c11 -o graphics_editor main.c -lm
```

Or using Make:

```bash
make
```

---

## Run Command

```bash
make run
# or
./graphics_editor
```

On Windows:

```powershell
.\graphics_editor.exe
```

---

## Menu Explanation

```
+----------------------------------+
|   2D Graphics Editor in C        |
+----------------------------------+
|  1. Display Canvas               |  → Print canvas with rulers
|  2. Add Object                   |  → Choose shape, enter coordinates
|  3. Delete Object                |  → Soft-delete by object ID
|  4. Modify Object                |  → Update coordinates, canvas redraws
|  5. List Objects                 |  → Show all active objects + params
|  6. Clear Canvas                 |  → Reset all cells to '_'
|  7. Save Canvas to File          |  → Write canvas.txt
|  8. Load Canvas from File        |  → Read canvas.txt into canvas
|  9. Exit                         |  → Free memory and quit
+----------------------------------+
```

---

## Coordinate System

| Term | Meaning |
|------|---------|
| `x` (col) | Horizontal: 0 = left, 59 = right |
| `y` (row) | Vertical:   0 = top,  24 = bottom |

---

## Example Usage

```
Choice: 2
  Shape: 1.Line  2.Rectangle  3.Circle  4.Triangle  0.Cancel
  Choice: 3
  cx centre col (0-59): 30
  cy centre row (0-24): 12
  radius    (1-12): 8
  Object ID=1 (Circle) added successfully.

Choice: 1
   012345678901234567890123456789012345678901234567890123456789
   +------------------------------------------------------------+
 4 |____________________________*****___________________________|
 5 |__________________________**_____**_________________________|
...
```

---

## Dynamic Memory Details

- Initial allocation: `calloc(20, sizeof(struct GraphicObject))`
- When full: `realloc(*objects, capacity * 2 * sizeof(...))`
- On exit (option 9): `free(objects)`

---

## GitHub Submission Notes

- **Repository**: `https://github.com/VyomVadodariya/2d-graphics-editor`
- **Branch**: `main`
- **Commit message**: `Add syllabus-based 2D graphics editor in C`
- **Files**: `main.c`, `Makefile`, `README.md`
- No external libraries. Compiles with `gcc -lm` on any platform.
- Tested with GCC on Windows (MinGW-w64).

---

## Author

**Vyom Vadodariya** — Advanced C Programming Mini-Project
