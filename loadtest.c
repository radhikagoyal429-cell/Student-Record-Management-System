#include <stdio.h>
#include <string.h>

struct Student {
    int roll;
    char name[50];
    char branch[30];
    float marks;
};

int main() {
    FILE *fp = fopen("students.dat", "ab");

    if (fp == NULL) {
        printf("File opening failed!\n");
        return 1;
    }

    struct Student s;

    for (int i = 1; i <= 5000; i++) {
        s.roll = 1000 + i;

        sprintf(s.name, "Student%d", i);
        strcpy(s.branch, "Computer Science");
        s.marks = 70 + (i % 30);

        fwrite(&s, sizeof(s), 1, fp);
    }

    fclose(fp);

    printf("5000 records added successfully!\n");
    return 0;
}
