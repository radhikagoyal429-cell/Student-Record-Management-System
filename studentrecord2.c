#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

struct Student {
    int roll;
    char name[50];
    char branch[30];
    float marks;
};

/* Safe input function */
int readLine(char str[], int size) {
    if (fgets(str, size, stdin) == NULL)
        return 0;

    size_t len = strlen(str);

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        printf("Input too long! Please enter again.\n");
        return -1;
    }

    return 1;
}

/* Integer input validation */
int readInt(char message[], int *value) {
    char input[100], *end;
    long num;

    while (1) {
        printf("%s", message);

        int result = readLine(input, sizeof(input));
        if (result == 0) return 0;
        if (result == -1) continue;

        errno = 0;
        num = strtol(input, &end, 10);

        if (end == input || *end != '\0' ||
            errno == ERANGE || num < 1 || num > 2147483647) {
            printf("Invalid input! Enter a positive number.\n");
            continue;
        }

        *value = (int)num;
        return 1;
    }
}

/* Marks input validation */
int readMarks(float *marks) {
    char input[100], *end;
    float num;

    while (1) {
        printf("Enter Marks (0-100): ");

        int result = readLine(input, sizeof(input));
        if (result == 0) return 0;
        if (result == -1) continue;

        errno = 0;
        num = strtof(input, &end);

        if (end == input || *end != '\0' ||
            errno == ERANGE || !isfinite(num) ||
            num < 0 || num > 100) {
            printf("Invalid marks! Enter between 0 and 100.\n");
            continue;
        }

        *marks = num;
        return 1;
    }
}

/* Name and branch input */
int readText(char message[], char str[], int size) {
    while (1) {
        printf("%s", message);

        int result = readLine(str, size);

        if (result == 0) return 0;
        if (result == -1) continue;

        if (strlen(str) == 0) {
            printf("Input cannot be empty!\n");
            continue;
        }

        return 1;
    }
}

/* Add Student */
void addStudent() {
    struct Student s, temp;

    FILE *fp = fopen("students.dat", "ab+");

    if (fp == NULL) {
        printf("File opening failed!\n");
        return;
    }

    if (!readInt("Enter Roll No: ", &s.roll)) {
        fclose(fp);
        return;
    }

    rewind(fp);

    while (fread(&temp, sizeof(temp), 1, fp) == 1) {
        if (temp.roll == s.roll) {
            printf("Duplicate Roll No! Record Rejected.\n");
            fclose(fp);
            return;
        }
    }

    if (ferror(fp)) {
        printf("File reading error!\n");
        fclose(fp);
        return;
    }

    if (!readText("Enter Name: ", s.name, sizeof(s.name)) ||
        !readText("Enter Branch: ", s.branch, sizeof(s.branch)) ||
        !readMarks(&s.marks)) {
        fclose(fp);
        return;
    }

    if (fseek(fp, 0, SEEK_END) != 0 ||
        fwrite(&s, sizeof(s), 1, fp) != 1) {
        printf("Record could not be saved!\n");
    } else {
        printf("Record Saved Successfully!\n");
    }

    fclose(fp);
}

/* Display Students */
void displayStudents() {
    struct Student s;

    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("No Records Found!\n");
        return;
    }

    printf("\n--- All Students ---\n");

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("Roll: %d | Name: %s | Branch: %s | Marks: %.2f\n",
               s.roll, s.name, s.branch, s.marks);
    }

    if (ferror(fp))
        printf("Error reading student records!\n");

    fclose(fp);
}

/* Search Student */
void searchStudent() {
    int r, found = 0;
    struct Student s;

    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("No Records Found!\n");
        return;
    }

    if (!readInt("Enter Roll No to Search: ", &r)) {
        fclose(fp);
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == r) {
            printf("Found! Roll: %d | Name: %s | Branch: %s | Marks: %.2f\n",
                   s.roll, s.name, s.branch, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student Not Found!\n");

    fclose(fp);
}

/* Update Student */
void updateStudent() {
    int r, found = 0;
    struct Student s;

    FILE *fp = fopen("students.dat", "rb+");

    if (fp == NULL) {
        printf("No Records Found!\n");
        return;
    }

    if (!readInt("Enter Roll No to Update: ", &r)) {
        fclose(fp);
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == r) {

            if (!readText("Enter New Name: ", s.name, sizeof(s.name)) ||
                !readMarks(&s.marks)) {
                fclose(fp);
                return;
            }

            if (fseek(fp, -(long)sizeof(s), SEEK_CUR) != 0 ||
                fwrite(&s, sizeof(s), 1, fp) != 1) {
                printf("Update failed!\n");
            } else {
                printf("Record Updated!\n");
            }

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student Not Found!\n");

    fclose(fp);
}

/* Delete Student */
void deleteStudent() {
    int r, found = 0;
    struct Student s;

    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("No Records Found!\n");
        return;
    }

    if (!readInt("Enter Roll No to Delete: ", &r)) {
        fclose(fp);
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");

    if (temp == NULL) {
        printf("Temporary file could not be created!\n");
        fclose(fp);
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll != r) {
            if (fwrite(&s, sizeof(s), 1, temp) != 1) {
                printf("Writing error!\n");
                fclose(fp);
                fclose(temp);
                remove("temp.dat");
                return;
            }
        } else {
            found = 1;
        }
    }

    if (ferror(fp)) {
        printf("Reading error!\n");
        fclose(fp);
        fclose(temp);
        remove("temp.dat");
        return;
    }

    fclose(fp);

    if (fclose(temp) != 0) {
        printf("Temporary file closing error!\n");
        remove("temp.dat");
        return;
    }

    if (!found) {
        printf("Student Not Found!\n");
        remove("temp.dat");
        return;
    }

    remove("students.bak");

    if (rename("students.dat", "students.bak") != 0) {
        printf("Could not create backup!\n");
        remove("temp.dat");
        return;
    }

    if (rename("temp.dat", "students.dat") != 0) {
        printf("Delete operation failed! Restoring original file.\n");
        rename("students.bak", "students.dat");
        return;
    }

    remove("students.bak");
    printf("Record Deleted!\n");
}

/* Main Function */
int main() {
    int ch;

    while (1) {
        printf("\n=== STUDENT RECORD SYSTEM ===\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");

        if (!readInt("Enter Choice: ", &ch)) {
            printf("Program closed.\n");
            break;
        }

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                printf("Thank You!\n");
                return 0;
            default:
                printf("Invalid Choice! Try Again.\n");
        }
    }

    return 0;
}
