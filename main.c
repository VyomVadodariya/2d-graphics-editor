#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS                    25
#define COLS                    60
#define INITIAL_OBJECT_CAPACITY 20
#define MAX_TYPE_LENGTH         20

struct GraphicObject {
    int  id;
    char type[MAX_TYPE_LENGTH];
    int  x1, y1;
    int  x2, y2;
    int  x3, y3;
    int  radius;
    int  active;
};

void initializeCanvas(char canvas[ROWS][COLS]);
void displayCanvas(char canvas[ROWS][COLS]);
void clearCanvas(char canvas[ROWS][COLS]);
int  isValidPoint(int x, int y);
void plotPoint(char canvas[ROWS][COLS], int x, int y);
void drawLine(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2);
void drawRectangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2);
void drawCircle(char canvas[ROWS][COLS], int cx, int cy, int radius);
void drawTriangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3);
void redrawCanvas(char canvas[ROWS][COLS], struct GraphicObject objects[], int count);
void addObject(char canvas[ROWS][COLS], struct GraphicObject **objects, int *count, int *capacity, int *nextId);
void deleteObject(char canvas[ROWS][COLS], struct GraphicObject objects[], int count);
void modifyObject(char canvas[ROWS][COLS], struct GraphicObject objects[], int count);
void listObjects(struct GraphicObject objects[], int count);
void saveCanvasToFile(char canvas[ROWS][COLS]);
void loadCanvasFromFile(char canvas[ROWS][COLS]);
int  compareString(char str1[], char str2[]);
int  safeInputInteger(void);
void menu(void);

void initializeCanvas(char canvas[ROWS][COLS]) {
    int r, c;
    for (r = 0; r < ROWS; r++)
        for (c = 0; c < COLS; c++)
            canvas[r][c] = '_';
}

void displayCanvas(char canvas[ROWS][COLS]) {
    int r, c;
    printf("\n   ");
    for (c = 0; c < COLS; c++) printf("%d", c % 10);
    printf("\n   +");
    for (c = 0; c < COLS; c++) printf("-");
    printf("+\n");
    for (r = 0; r < ROWS; r++) {
        printf("%2d |", r);
        for (c = 0; c < COLS; c++) printf("%c", canvas[r][c]);
        printf("|\n");
    }
    printf("   +");
    for (c = 0; c < COLS; c++) printf("-");
    printf("+\n\n");
}

void clearCanvas(char canvas[ROWS][COLS]) {
    initializeCanvas(canvas);
    printf("  Canvas cleared.\n");
}

int isValidPoint(int x, int y) {
    return (x >= 0 && x < COLS && y >= 0 && y < ROWS);
}

void plotPoint(char canvas[ROWS][COLS], int x, int y) {
    if (isValidPoint(x, y))
        canvas[y][x] = '*';
}

void drawLine(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2) {
    int   dx    = abs(x2 - x1);
    int   dy    = abs(y2 - y1);
    int   steps = (dx > dy) ? dx : dy;
    float xInc, yInc, x, y;
    int   i;

    if (steps == 0) { plotPoint(canvas, x1, y1); return; }

    xInc = (float)(x2 - x1) / (float)steps;
    yInc = (float)(y2 - y1) / (float)steps;
    x    = (float)x1;
    y    = (float)y1;

    for (i = 0; i <= steps; i++) {
        plotPoint(canvas, (int)round(x), (int)round(y));
        x += xInc;
        y += yInc;
    }
}

void drawRectangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2) {
    drawLine(canvas, x1, y1, x2, y1);
    drawLine(canvas, x1, y2, x2, y2);
    drawLine(canvas, x1, y1, x1, y2);
    drawLine(canvas, x2, y1, x2, y2);
}

void drawCircle(char canvas[ROWS][COLS], int cx, int cy, int radius) {
    int   angle;
    float rad;
    for (angle = 0; angle < 360; angle++) {
        rad = (float)angle * 3.14159265f / 180.0f;
        plotPoint(canvas, cx + (int)round((float)radius * cos(rad)),
                          cy + (int)round((float)radius * sin(rad)));
    }
}

void drawTriangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(canvas, x1, y1, x2, y2);
    drawLine(canvas, x2, y2, x3, y3);
    drawLine(canvas, x3, y3, x1, y1);
}

void redrawCanvas(char canvas[ROWS][COLS], struct GraphicObject objects[], int count) {
    int i;
    initializeCanvas(canvas);
    for (i = 0; i < count; i++) {
        if (!objects[i].active) continue;
        if      (compareString(objects[i].type, "Line")      == 0)
            drawLine(canvas, objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
        else if (compareString(objects[i].type, "Rectangle") == 0)
            drawRectangle(canvas, objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
        else if (compareString(objects[i].type, "Circle")    == 0)
            drawCircle(canvas, objects[i].x1, objects[i].y1, objects[i].radius);
        else if (compareString(objects[i].type, "Triangle")  == 0)
            drawTriangle(canvas, objects[i].x1, objects[i].y1,
                                 objects[i].x2, objects[i].y2,
                                 objects[i].x3, objects[i].y3);
    }
}

void addObject(char canvas[ROWS][COLS], struct GraphicObject **objects,
               int *count, int *capacity, int *nextId) {
    int choice;
    struct GraphicObject  obj;
    struct GraphicObject *temp;

    if (*count >= *capacity) {
        *capacity *= 2;
        temp = (struct GraphicObject *)realloc(*objects,
               (size_t)*capacity * sizeof(struct GraphicObject));
        if (temp == NULL) { printf("  Memory reallocation failed.\n"); return; }
        *objects = temp;
    }

    printf("\n  Shape: 1.Line  2.Rectangle  3.Circle  4.Triangle  0.Cancel\n");
    printf("  Choice: ");
    choice = safeInputInteger();
    if (choice < 1 || choice > 4) { printf("  Cancelled.\n"); return; }

    memset(&obj, 0, sizeof(struct GraphicObject));
    obj.id     = *nextId;
    obj.active = 1;

    if (choice == 1) {
        strcpy(obj.type, "Line");
        printf("  x1 (col 0-%d): ", COLS - 1); obj.x1 = safeInputInteger();
        printf("  y1 (row 0-%d): ", ROWS - 1); obj.y1 = safeInputInteger();
        printf("  x2 (col 0-%d): ", COLS - 1); obj.x2 = safeInputInteger();
        printf("  y2 (row 0-%d): ", ROWS - 1); obj.y2 = safeInputInteger();
        drawLine(canvas, obj.x1, obj.y1, obj.x2, obj.y2);

    } else if (choice == 2) {
        strcpy(obj.type, "Rectangle");
        printf("  x1 top-left  col (0-%d): ", COLS - 1); obj.x1 = safeInputInteger();
        printf("  y1 top-left  row (0-%d): ", ROWS - 1); obj.y1 = safeInputInteger();
        printf("  x2 bot-right col (0-%d): ", COLS - 1); obj.x2 = safeInputInteger();
        printf("  y2 bot-right row (0-%d): ", ROWS - 1); obj.y2 = safeInputInteger();
        drawRectangle(canvas, obj.x1, obj.y1, obj.x2, obj.y2);

    } else if (choice == 3) {
        strcpy(obj.type, "Circle");
        printf("  cx centre col (0-%d): ", COLS - 1); obj.x1     = safeInputInteger();
        printf("  cy centre row (0-%d): ", ROWS - 1); obj.y1     = safeInputInteger();
        printf("  radius    (1-%d): ",     ROWS / 2); obj.radius = safeInputInteger();
        drawCircle(canvas, obj.x1, obj.y1, obj.radius);

    } else {
        strcpy(obj.type, "Triangle");
        printf("  Ax (col 0-%d): ", COLS - 1); obj.x1 = safeInputInteger();
        printf("  Ay (row 0-%d): ", ROWS - 1); obj.y1 = safeInputInteger();
        printf("  Bx (col 0-%d): ", COLS - 1); obj.x2 = safeInputInteger();
        printf("  By (row 0-%d): ", ROWS - 1); obj.y2 = safeInputInteger();
        printf("  Cx (col 0-%d): ", COLS - 1); obj.x3 = safeInputInteger();
        printf("  Cy (row 0-%d): ", ROWS - 1); obj.y3 = safeInputInteger();
        drawTriangle(canvas, obj.x1, obj.y1, obj.x2, obj.y2, obj.x3, obj.y3);
    }

    (*objects)[*count] = obj;
    (*count)++;
    (*nextId)++;
    printf("  Object ID=%d (%s) added successfully.\n", obj.id, obj.type);
}

void deleteObject(char canvas[ROWS][COLS], struct GraphicObject objects[], int count) {
    int id, i, found = 0;
    if (count == 0) { printf("  No objects to delete.\n"); return; }
    listObjects(objects, count);
    printf("  Enter object ID to delete (0 = cancel): ");
    id = safeInputInteger();
    if (id == 0) return;
    for (i = 0; i < count; i++) {
        if (objects[i].active && objects[i].id == id) {
            objects[i].active = 0;
            found = 1;
            break;
        }
    }
    if (!found) { printf("  Object ID %d not found.\n", id); return; }
    redrawCanvas(canvas, objects, count);
    printf("  Object ID=%d deleted.\n", id);
}

void modifyObject(char canvas[ROWS][COLS], struct GraphicObject objects[], int count) {
    int id, i, found = 0;
    if (count == 0) { printf("  No objects to modify.\n"); return; }
    listObjects(objects, count);
    printf("  Enter object ID to modify (0 = cancel): ");
    id = safeInputInteger();
    if (id == 0) return;
    for (i = 0; i < count; i++) {
        if (objects[i].active && objects[i].id == id) {
            found = 1;
            printf("  Modifying %s (ID=%d) - enter new parameters:\n", objects[i].type, id);
            if (compareString(objects[i].type, "Line") == 0 ||
                compareString(objects[i].type, "Rectangle") == 0) {
                printf("  x1: "); objects[i].x1 = safeInputInteger();
                printf("  y1: "); objects[i].y1 = safeInputInteger();
                printf("  x2: "); objects[i].x2 = safeInputInteger();
                printf("  y2: "); objects[i].y2 = safeInputInteger();
            } else if (compareString(objects[i].type, "Circle") == 0) {
                printf("  cx: ");     objects[i].x1     = safeInputInteger();
                printf("  cy: ");     objects[i].y1     = safeInputInteger();
                printf("  radius: "); objects[i].radius = safeInputInteger();
            } else {
                printf("  Ax: "); objects[i].x1 = safeInputInteger();
                printf("  Ay: "); objects[i].y1 = safeInputInteger();
                printf("  Bx: "); objects[i].x2 = safeInputInteger();
                printf("  By: "); objects[i].y2 = safeInputInteger();
                printf("  Cx: "); objects[i].x3 = safeInputInteger();
                printf("  Cy: "); objects[i].y3 = safeInputInteger();
            }
            break;
        }
    }
    if (!found) { printf("  Object ID %d not found.\n", id); return; }
    redrawCanvas(canvas, objects, count);
    printf("  Object ID=%d modified successfully.\n", id);
}

void listObjects(struct GraphicObject objects[], int count) {
    int i, found = 0;
    printf("\n  %-4s %-12s %-38s\n", "ID", "Type", "Parameters");
    printf("  --------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        if (!objects[i].active) continue;
        found = 1;
        printf("  %-4d %-12s ", objects[i].id, objects[i].type);
        if (compareString(objects[i].type, "Circle") == 0)
            printf("cx=%d cy=%d r=%d\n", objects[i].x1, objects[i].y1, objects[i].radius);
        else if (compareString(objects[i].type, "Triangle") == 0)
            printf("A(%d,%d) B(%d,%d) C(%d,%d)\n",
                   objects[i].x1, objects[i].y1,
                   objects[i].x2, objects[i].y2,
                   objects[i].x3, objects[i].y3);
        else
            printf("(%d,%d) -> (%d,%d)\n",
                   objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
    }
    if (!found) printf("  (no active objects)\n");
    printf("\n");
}

void saveCanvasToFile(char canvas[ROWS][COLS]) {
    int r, c;
    FILE *fp = fopen("canvas.txt", "w");
    if (fp == NULL) { printf("  Error: cannot open file for writing.\n"); return; }
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) fprintf(fp, "%c", canvas[r][c]);
        fprintf(fp, "\n");
    }
    if (ferror(fp)) printf("  Write error occurred.\n");
    else            printf("  Canvas saved to canvas.txt\n");
    fclose(fp);
}

void loadCanvasFromFile(char canvas[ROWS][COLS]) {
    char line[COLS + 5];
    int  r = 0, c;
    FILE *fp = fopen("canvas.txt", "r");
    if (fp == NULL) { printf("  Error: canvas.txt not found.\n"); return; }
    initializeCanvas(canvas);
    while (!feof(fp) && r < ROWS) {
        if (fgets(line, (int)sizeof(line), fp) == NULL) break;
        for (c = 0; c < COLS && line[c] != '\0' && line[c] != '\n'; c++)
            canvas[r][c] = line[c];
        r++;
    }
    if (ferror(fp)) printf("  Read error occurred.\n");
    else            printf("  Canvas loaded from canvas.txt\n");
    fclose(fp);
}

int compareString(char str1[], char str2[]) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return (int)str1[i] - (int)str2[i];
        i++;
    }
    return (int)str1[i] - (int)str2[i];
}

int safeInputInteger(void) {
    int val;
    while (scanf("%d", &val) != 1) {
        while (getchar() != '\n');
        printf("  Invalid input. Enter a number: ");
    }
    return val;
}

void menu(void) {
    printf("\n+----------------------------------+\n");
    printf("|   2D Graphics Editor in C        |\n");
    printf("+----------------------------------+\n");
    printf("|  1. Display Canvas               |\n");
    printf("|  2. Add Object                   |\n");
    printf("|  3. Delete Object                |\n");
    printf("|  4. Modify Object                |\n");
    printf("|  5. List Objects                 |\n");
    printf("|  6. Clear Canvas                 |\n");
    printf("|  7. Save Canvas to File          |\n");
    printf("|  8. Load Canvas from File        |\n");
    printf("|  9. Exit                         |\n");
    printf("+----------------------------------+\n");
    printf("  Choice: ");
}

int main(void) {
    char canvas[ROWS][COLS];
    struct GraphicObject *objects;
    int count    = 0;
    int capacity = INITIAL_OBJECT_CAPACITY;
    int nextId   = 1;
    int choice;

    objects = (struct GraphicObject *)calloc((size_t)capacity,
               sizeof(struct GraphicObject));
    if (objects == NULL) {
        printf("  Fatal: memory allocation failed.\n");
        return 1;
    }

    initializeCanvas(canvas);
    printf("\n  Welcome to the 2D Graphics Editor!\n");
    printf("  Canvas: %d rows x %d cols  |  '_' = empty  '*' = drawn\n", ROWS, COLS);

    while (1) {
        menu();
        choice = safeInputInteger();
        switch (choice) {
            case 1: displayCanvas(canvas);                                        break;
            case 2: addObject(canvas, &objects, &count, &capacity, &nextId);      break;
            case 3: deleteObject(canvas, objects, count);                          break;
            case 4: modifyObject(canvas, objects, count);                          break;
            case 5: listObjects(objects, count);                                   break;
            case 6: clearCanvas(canvas);                                           break;
            case 7: saveCanvasToFile(canvas);                                      break;
            case 8: loadCanvasFromFile(canvas);                                    break;
            case 9:
                free(objects);
                printf("\n  Goodbye!\n\n");
                return 0;
            default:
                printf("  Invalid choice. Please select 1-9.\n");
        }
    }
}
