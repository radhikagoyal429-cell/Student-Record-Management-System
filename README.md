# Student Record Management System

![C CI](https://github.com/radhikagoyal429-cell/Student-Record-Management-System/actions/workflows/ci.yml/badge.svg)

A simple and efficient Student Record Management System developed using C. It allows users to manage student records through a menu-driven interface.

## 🚀 Live Application

**Live Demo:** https://student-record-management-system-1-o6kc.onrender.com/

## ✨ Features

- Add new student records (Roll Number, Name, Branch, Marks).
- View all student records.
- Search students by Roll Number.
- Update student Name and Marks.
- Delete student records with backup safety.
- Input validation.
- Duplicate Roll Number validation.
- File handling for data persistence.
- Simple menu-driven interface.

## 🛠️ Tech Stack

- **Programming Language:** C
- **Compiler:** GCC
- **CI:** GitHub Actions
- **Containerization:** Docker
- **Cloud Hosting:** Render

## 💻 How to Run Locally

### Requirements

- GCC Compiler
- Git

### Compile and Run

```bash
gcc -Wall -o newcode newcode.c -lm
./newcode
```

## 🧪 Load Testing

The project includes a load testing program to generate 5000 student records.

```bash
gcc loadtest.c -o loadtest
./loadtest
```

## 🔄 CI/CD

GitHub Actions is configured to:

- Compile the C application.
- Execute automated test scripts.
- Run load testing.

Render Auto-Deploy is enabled for cloud deployment.

## ☁️ Cloud Deployment

The application is deployed using Render.

**Live URL:** https://student-record-management-system-1-o6kc.onrender.com/

## 📁 Project Structure

- `newcode.c` – Main application source code.
- `loadtest.c` – Load testing program.
- `tests/run_tests.sh` – Test execution script.
- `.github/workflows/ci.yml` – GitHub Actions workflow.
- `Dockerfile` – Docker configuration.

## 🔮 Future Scope

- Database integration.
- User authentication.
- Admin dashboard.
- Improved graphical user interface.
- Advanced reporting features.

## 👩‍💻 Author

Radhika Goyal

## Conclusion

The Student Record Management System provides a simple and efficient way to manage student records using C programming, file handling, Docker, and cloud deployment.
