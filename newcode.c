#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

#define DATA_FILE   "students.dat"
#define TEMP_FILE   "temp.dat"
#define BACKUP_FILE "students.bak"

struct Student {
    int roll;
    char name[50];
    char branch[30];
    float marks;
};

/* ---------- Input helpers ---------- */

/* Reads one non-empty line. Asks again if it is empty or too long.
   Returns 1 on success, 0 if input has ended (Ctrl+D / Ctrl+Z). */
int readText(const char *msg, char *str, int size) {
    while (1) {
        printf("%s", msg);
        if (fgets(str, size, stdin) == NULL)
            return 0;

        size_t len = strlen(str);
        if (len == 0 || str[len - 1] != '\n') {      /* line did not fit */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("Input too long! Please enter again.\n");
            continue;
        }
        str[len - 1] = '\0';

        if (str[0] == '\0') {
            printf("Input cannot be empty!\n");
            continue;
        }
        return 1;
    }
}

/* Reads a positive whole number. */
int readInt(const char *msg, int *value) {
    char input[100], *end;

    while (readText(msg, input, sizeof(input))) {
        errno = 0;
        long num = strtol(input, &end, 10);
        if (*end == '\0' && errno != ERANGE && num >= 1 && num <= INT_MAX) {
            *value = (int)num;
            return 1;
        }
        printf("Invalid input! Enter a positive number.\n");
    }
    return 0;
}

/* Reads marks between 0 and 100. */
int readMarks(float *marks) {
    char input[100], *end;

    while (readText("Enter Marks (0-100): ", input, sizeof(input))) {
        errno = 0;
        float num = strtof(input, &end);
        if (*end == '\0' && errno != ERANGE && isfinite(num) &&
            num >= 0 && num <= 100) {
            *marks = num;
            return 1;
        }
        printf("Invalid marks! Enter between 0 and 100.\n");
    }
    return 0;
}

/* ---------- File helpers ---------- */

FILE *openRecords(const char *mode) {
    FILE *fp = fopen(DATA_FILE, mode);
    if (fp == NULL)
        printf("No Records Found!\n");
    return fp;
}

/* Looks for a roll number from the start of the file.
   Returns 1 and fills *s if found (file position is just after that record). */
int findStudent(FILE *fp, int roll, struct Student *s) {
    rewind(fp);
    while (fread(s, sizeof(*s), 1, fp) == 1)
        if (s->roll == roll)
            return 1;
    return 0;
}

void printStudent(const char *prefix, const struct Student *s) {
    printf("%sRoll: %d | Name: %s | Branch: %s | Marks: %.2f\n",
           prefix, s->roll, s->name, s->branch, s->marks);
}

/* ---------- Menu operations ---------- */

void addStudent() {
    struct Student s, old;
    FILE *fp = fopen(DATA_FILE, "ab+");

    if (fp == NULL) {
        printf("File opening failed!\n");
        return;
    }

    if (!readInt("Enter Roll No: ", &s.roll)) {
        fclose(fp);
        return;
    }

    if (findStudent(fp, s.roll, &old)) {
        printf("Duplicate Roll No! Record Rejected.\n");
    } else if (ferror(fp)) {
        printf("File reading error!\n");
    } else if (readText("Enter Name: ", s.name, sizeof(s.name)) &&
               readText("Enter Branch: ", s.branch, sizeof(s.branch)) &&
               readMarks(&s.marks)) {
        if (fseek(fp, 0, SEEK_END) != 0 || fwrite(&s, sizeof(s), 1, fp) != 1)
            printf("Record could not be saved!\n");
        else
            printf("Record Saved Successfully!\n");
    }

    fclose(fp);
}

void displayStudents() {
    struct Student s;
    FILE *fp = openRecords("rb");
    if (fp == NULL)
        return;

    printf("\n--- All Students ---\n");
    while (fread(&s, sizeof(s), 1, fp) == 1)
        printStudent("", &s);

    if (ferror(fp))
        printf("Error reading student records!\n");

    fclose(fp);
}

void searchStudent() {
    int roll;
    struct Student s;
    FILE *fp = openRecords("rb");
    if (fp == NULL)
        return;

    if (readInt("Enter Roll No to Search: ", &roll)) {
        if (findStudent(fp, roll, &s))
            printStudent("Found! ", &s);
        else
            printf("Student Not Found!\n");
    }

    fclose(fp);
}

void updateStudent() {
    int roll;
    struct Student s;
    FILE *fp = openRecords("rb+");
    if (fp == NULL)
        return;

    if (readInt("Enter Roll No to Update: ", &roll)) {
        if (!findStudent(fp, roll, &s)) {
            printf("Student Not Found!\n");
        } else if (readText("Enter New Name: ", s.name, sizeof(s.name)) &&
                   readMarks(&s.marks)) {
            /* go back one record and overwrite it */
            if (fseek(fp, -(long)sizeof(s), SEEK_CUR) != 0 ||
                fwrite(&s, sizeof(s), 1, fp) != 1)
                printf("Update failed!\n");
            else
                printf("Record Updated!\n");
        }
    }

    fclose(fp);
}

/* Copies every record except the deleted one into temp.dat,
   then swaps files. The original is restored if anything fails. */
void deleteStudent() {
    int roll, found = 0, ok = 1;
    struct Student s;
    FILE *fp = openRecords("rb");
    if (fp == NULL)
        return;

    if (!readInt("Enter Roll No to Delete: ", &roll)) {
        fclose(fp);
        return;
    }

    FILE *tmp = fopen(TEMP_FILE, "wb");
    if (tmp == NULL) {
        printf("Temporary file could not be created!\n");
        fclose(fp);
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == roll)
            found = 1;
        else if (fwrite(&s, sizeof(s), 1, tmp) != 1) {
            ok = 0;
            break;
        }
    }
    if (ferror(fp))
        ok = 0;
    fclose(fp);
    if (fclose(tmp) != 0)
        ok = 0;

    if (!ok) {
        printf("Delete failed! Original data is safe.\n");
        remove(TEMP_FILE);
    } else if (!found) {
        printf("Student Not Found!\n");
        remove(TEMP_FILE);
    } else {
        remove(BACKUP_FILE);
        if (rename(DATA_FILE, BACKUP_FILE) != 0) {
            printf("Could not create backup!\n");
            remove(TEMP_FILE);
        } else if (rename(TEMP_FILE, DATA_FILE) != 0) {
            printf("Delete operation failed! Restoring original file.\n");
            rename(BACKUP_FILE, DATA_FILE);
        } else {
            remove(BACKUP_FILE);
            printf("Record Deleted!\n");
        }
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n=== STUDENT RECORD SYSTEM ===\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");

        if (!readInt("Enter Choice: ", &choice)) {
            printf("Program closed.\n");
            return 0;
        }

        switch (choice) {
            case 1: addStudent();      break;
            case 2: displayStudents(); break;
            case 3: searchStudent();   break;
            case 4: updateStudent();   break;
            case 5: deleteStudent();   break;
            case 6: printf("Thank You!\n"); return 0;
            default: printf("Invalid Choice! Try Again.\n");
        }
    }
}