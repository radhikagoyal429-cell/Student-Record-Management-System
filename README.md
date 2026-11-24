# Student Record Management System

![C CI](https://github.com/radhikagoyal429-cell/Student-Record-Management-System/actions/workflows/ci.yml/badge.svg)

A simple and efficient Student Record Management System written in C. Manage student records easily - add, view, search , update and delete students with marks.

## Features
- Add new student record (Roll No, Name,Branch, Marks)
- View all student records
- Search student by Roll No
- Update student Name and Marks
- Delete student with backup safety 
- Input Validation 
- Simple menu-driven interface
- File handling for data persistence

## Tech Stack
- **Language:** C
- **Build:** GCC
- **CI/CD:** GitHub Actions
- **Containerization:** Docker

## How to Run Locally

### Using GCC
```bash
gcc -Wall -o newcode newcode.c -lm
./newcode

## Load Test - 5000 Records
```bash
gcc loadtest.c -o loadtest
./loadtest