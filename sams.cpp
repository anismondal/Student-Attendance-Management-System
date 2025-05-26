/*
 * Created by Md Anis Mondal
 * BCA 2nd Semester
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <termios.h>    // For terminal control on macOS
#include <unistd.h>     // For STDIN_FILENO
#include <cstdlib>      // For system()

using namespace std;

#define MAX_STUDENTS 100
#define MAX_DAYS 31

// Console enhancement functions for macOS
void setConsoleColor(int color) {
    // ANSI color codes for terminal
    switch(color) {
        case 7:  cout << "\033[0m"; break;    // Reset/White
        case 10: cout << "\033[32m"; break;   // Green
        case 11: cout << "\033[36m"; break;   // Cyan
        case 12: cout << "\033[31m"; break;   // Red
        case 14: cout << "\033[33m"; break;   // Yellow
        default: cout << "\033[0m"; break;    // Default
    }
}

void clearScreen() {
    system("clear");
}

void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}

// Cross-platform getch() implementation for macOS
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void showWelcomeScreen() {
    clearScreen();
    setConsoleColor(11); // Light cyan
    cout << "\n\n";
    cout << "  +======================================================+\n";
    cout << "  |                                                      |\n";
    cout << "  |        STUDENT ATTENDANCE MANAGEMENT SYSTEM         |\n";
    cout << "  |                                                      |\n";
    cout << "  |              Created by Md Anis Mondal              |\n";
    cout << "  |                  BCA 2nd Semester                   |\n";
    cout << "  |                                                      |\n";
    cout << "  +======================================================+\n";
    setConsoleColor(7); // Reset to white
    cout << "\n\n              Press any key to continue...";
    getch();
}

void pauseScreen() {
    setConsoleColor(14); // Yellow
    cout << "\n\nPress any key to continue...";
    setConsoleColor(7);
    getch();
}

class Student {
private:
    int rollNumber;
    string name;
    bool attendance[MAX_DAYS] = {false};
    string remarks;
    
public:
    Student() : rollNumber(0), name(""), remarks("") {}
    
    // Getters
    int getRollNumber() const { return rollNumber; }
    string getName() const { return name; }
    bool getAttendance(int day) const { return attendance[day]; }
    string getRemarks() const { return remarks; }
    
    // Setters
    void setRollNumber(int roll) { rollNumber = roll; }
    void setName(const string& studentName) { name = studentName; }
    void setAttendance(int day, bool present) { 
        if (day >= 0 && day < MAX_DAYS) {
            attendance[day] = present;
        }
    }
    void setRemarks(const string& studentRemarks) { remarks = studentRemarks; }
    
    // Calculate attendance percentage for the month
    double getAttendancePercentage(int totalDays) const {
        if (totalDays <= 0) return 0;
        
        int presentDays = 0;
        for (int i = 0; i < totalDays; i++) {
            if (attendance[i]) presentDays++;
        }
        
        return (static_cast<double>(presentDays) / totalDays) * 100.0;
    }
};

class AttendanceSystem {
private:
    Student students[MAX_STUDENTS];
    int studentCount;
    int currentMonth;
    int daysInMonth;
    
    bool isValidName(const string& name) {
        for (char c : name) {
            if (!isalpha(c) && c != ' ') {
                return false;
            }
        }
        return !name.empty();
    }
    
public:
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    AttendanceSystem() : studentCount(0), currentMonth(5), daysInMonth(31) {}
    
    // Enhanced display with colors and better formatting
    void displayStudents() {
        clearScreen();
        if (studentCount == 0) {
            setConsoleColor(12); // Red
            cout << "\n  X No students registered.\n";
            setConsoleColor(7);
            pauseScreen();
            return;
        }
        
        setConsoleColor(11); // Light cyan
        cout << "\n+==================================================================+\n";
        cout << "|                          STUDENT LIST                           |\n";
        cout << "+==================================================================+\n";
        setConsoleColor(7);
        
        cout << "| Roll No |      Name      | Attendance % |    Remarks    |\n";
        cout << "+---------+----------------+--------------+---------------+\n";
        
        for (int i = 0; i < studentCount; i++) {
            double attendancePercentage = students[i].getAttendancePercentage(daysInMonth);
            
            // Color code based on attendance percentage
            if (attendancePercentage >= 85) setConsoleColor(10); // Green
            else if (attendancePercentage >= 75) setConsoleColor(14); // Yellow
            else setConsoleColor(12); // Red
            
            printf("| %7d | %-14s |    %6.2f%%    | %-13s |\n", 
                   students[i].getRollNumber(), 
                   students[i].getName().substr(0, 14).c_str(),
                   attendancePercentage,
                   students[i].getRemarks().substr(0, 13).c_str());
        }
        
        setConsoleColor(7);
        cout << "+=================================================================+\n";
        pauseScreen();
    }
    
    // Get valid roll number input (unique)
    int getRollNumber() {
        string input;
        int rollNumber;
        
        while (true) {
            cout << "Enter student roll number: ";
            getline(cin, input);
            
            try {
                rollNumber = stoi(input);
                
                if (rollNumber <= 0) {
                    cout << "Invalid roll number. Please enter a positive number.\n";
                    continue;
                }
                
                // Check for duplicate roll number
                bool duplicate = false;
                for (int i = 0; i < studentCount; i++) {
                    if (students[i].getRollNumber() == rollNumber) {
                        duplicate = true;
                        break;
                    }
                }
                
                if (!duplicate) {
                    return rollNumber; // Return valid, non-duplicate roll number
                } else {
                    cout << "Roll number already exists. Please try again.\n";
                }
            } catch (exception&) {
                cout << "Invalid roll number. Please enter numbers only.\n";
            }
        }
    }
    
    // Add a new student
    void addStudent() {
        clearScreen();
        setConsoleColor(11);
        cout << "\n+======================================+\n";
        cout << "|            ADD NEW STUDENT           |\n";
        cout << "+======================================+\n";
        setConsoleColor(7);
        
        if (studentCount < MAX_STUDENTS) {
            int rollNumber = getRollNumber();
            string name;
            
            while (true) {
                cout << "\nEnter student name: ";
                getline(cin, name);
                
                if (isValidName(name)) {
                    break;
                } else {
                    setConsoleColor(12);
                    cout << "X Invalid name. Please enter letters only.\n";
                    setConsoleColor(7);
                }
            }
            
            students[studentCount].setRollNumber(rollNumber);
            students[studentCount].setName(name);
            students[studentCount].setRemarks("");
            
            setConsoleColor(10);
            cout << "\n* Student added successfully!\n";
            setConsoleColor(7);
            studentCount++;
        } else {
            setConsoleColor(12);
            cout << "\nX Maximum number of students reached.\n";
            setConsoleColor(7);
        }
        pauseScreen();
    }
    
    // Mark attendance for a student on a specific day
    void markAttendance() {
        if (studentCount == 0) {
            cout << "No students to mark attendance for.\n";
            return;
        }
        
        int rollNumber;
        cout << "Enter student roll number: ";
        cin >> rollNumber;
        clearInputBuffer();
        
        int day;
        cout << "Enter day (1-" << daysInMonth << "): ";
        cin >> day;
        clearInputBuffer();
        
        if (day < 1 || day > daysInMonth) {
            cout << "Invalid day. Please enter a day between 1 and " << daysInMonth << ".\n";
            return;
        }
        
        int status;
        cout << "Mark student as (1: Present, 0: Absent): ";
        cin >> status;
        clearInputBuffer();
        
        if (status != 0 && status != 1) {
            cout << "Invalid input. Please enter 0 for absent or 1 for present.\n";
            return;
        }
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getRollNumber() == rollNumber) {
                students[i].setAttendance(day - 1, status == 1);
                cout << "Attendance marked successfully for " << students[i].getName() 
                         << " on day " << day << " as " 
                         << (status == 1 ? "Present" : "Absent") << ".\n";
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Student with roll number " << rollNumber << " not found.\n";
        }
    }
    
    // View attendance for a specific student
    void viewStudentAttendance() {
        if (studentCount == 0) {
            cout << "No students registered.\n";
            return;
        }
        
        int rollNumber;
        cout << "Enter student roll number: ";
        cin >> rollNumber;
        clearInputBuffer();
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getRollNumber() == rollNumber) {
                cout << "\nAttendance record for " << students[i].getName() << ":\n";
                cout << "----------------------------\n";
                cout << "Day | Status\n";
                cout << "----------------------------\n";
                
                for (int day = 0; day < daysInMonth; day++) {
                    cout << day + 1 << " | " 
                            << (students[i].getAttendance(day) ? "Present" : "Absent") << "\n";
                }
                
                double attendancePercentage = students[i].getAttendancePercentage(daysInMonth);
                cout << "----------------------------\n";
                cout << "Attendance Percentage: " << attendancePercentage << "%\n";
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Student with roll number " << rollNumber << " not found.\n";
        }
    }
    
    // View attendance by day
    void viewDayAttendance() {
        if (studentCount == 0) {
            cout << "No students registered.\n";
            return;
        }
        
        int day;
        cout << "Enter day (1-" << daysInMonth << "): ";
        cin >> day;
        clearInputBuffer();
        
        if (day < 1 || day > daysInMonth) {
            cout << "Invalid day. Please enter a day between 1 and " << daysInMonth << ".\n";
            return;
        }
        
        cout << "\nAttendance for Day " << day << ":\n";
        cout << "----------------------------\n";
        cout << "Roll Number | Name | Status\n";
        cout << "----------------------------\n";
        
        for (int i = 0; i < studentCount; i++) {
            cout << students[i].getRollNumber() << " | " 
                    << students[i].getName() << " | " 
                    << (students[i].getAttendance(day - 1) ? "Present" : "Absent") << "\n";
        }
        cout << "----------------------------\n";
        
        // Calculate attendance statistics for the day
        int presentCount = 0;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getAttendance(day - 1)) {
                presentCount++;
            }
        }
        
        double presentPercentage = (static_cast<double>(presentCount) / studentCount) * 100.0;
        double absentPercentage = 100.0 - presentPercentage;
        
        cout << "Present: " << presentCount << " (" << presentPercentage << "%)\n";
        cout << "Absent: " << (studentCount - presentCount) << " (" << absentPercentage << "%)\n";
    }
    
    // Calculate average attendance for all students
    void calculateAverageAttendance() {
        if (studentCount == 0) {
            cout << "No students to calculate average attendance.\n";
            return;
        }
        
        double totalAttendancePercentage = 0;
        for (int i = 0; i < studentCount; i++) {
            totalAttendancePercentage += students[i].getAttendancePercentage(daysInMonth);
        }
        
        double averageAttendance = totalAttendancePercentage / studentCount;
        cout << "Average Attendance Percentage: " << averageAttendance << "%\n";
    }
    
    // Find students with highest and lowest attendance
    void findHighestLowestAttendance() {
        if (studentCount == 0) {
            cout << "No students to evaluate.\n";
            return;
        }
        
        int highestStudentIndex = 0;
        int lowestStudentIndex = 0;
        double highestAttendance = students[0].getAttendancePercentage(daysInMonth);
        double lowestAttendance = students[0].getAttendancePercentage(daysInMonth);
        
        for (int i = 1; i < studentCount; i++) {
            double attendance = students[i].getAttendancePercentage(daysInMonth);
            if (attendance > highestAttendance) {
                highestAttendance = attendance;
                highestStudentIndex = i;
            }
            if (attendance < lowestAttendance) {
                lowestAttendance = attendance;
                lowestStudentIndex = i;
            }
        }
        
        cout << "Highest Attendance: " << highestAttendance << "% (Student: " 
                << students[highestStudentIndex].getName() << ", Roll Number: " 
                << students[highestStudentIndex].getRollNumber() << ")\n";
        cout << "Lowest Attendance: " << lowestAttendance << "% (Student: " 
                << students[lowestStudentIndex].getName() << ", Roll Number: " 
                << students[lowestStudentIndex].getRollNumber() << ")\n";
    }
    
    // Update student name
    void updateStudentName() {
        if (studentCount == 0) {
            cout << "No students to update.\n";
            return;
        }
        
        int rollNumber;
        cout << "Enter the roll number of the student to update: ";
        cin >> rollNumber;
        clearInputBuffer();
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getRollNumber() == rollNumber) {
                string newName;
                while (true) {
                    cout << "Enter new name for the student: ";
                    getline(cin, newName);
                    
                    if (isValidName(newName)) {
                        students[i].setName(newName);
                        cout << "Student name updated successfully.\n";
                        found = true;
                        break;
                    } else {
                        cout << "Invalid name. Please enter letters only.\n";
                    }
                }
                break;
            }
        }
        
        if (!found) {
            cout << "Student with roll number " << rollNumber << " not found.\n";
        }
    }
    
    // Update student roll number
    void updateStudentRollNumber() {
        if (studentCount == 0) {
            cout << "No students to update.\n";
            return;
        }
        
        int oldRollNumber;
        cout << "Enter the current roll number of the student: ";
        cin >> oldRollNumber;
        clearInputBuffer();
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getRollNumber() == oldRollNumber) {
                int newRollNumber;
                cout << "Enter new roll number for the student: ";
                cin >> newRollNumber;
                clearInputBuffer();
                
                // Check for duplicate roll number
                bool duplicate = false;
                for (int j = 0; j < studentCount; j++) {
                    if (j != i && students[j].getRollNumber() == newRollNumber) {
                        duplicate = true;
                        break;
                    }
                }
                
                if (!duplicate) {
                    students[i].setRollNumber(newRollNumber);
                    cout << "Student roll number updated successfully.\n";
                } else {
                    cout << "Roll number already exists. Please try again.\n";
                }
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Student with roll number " << oldRollNumber << " not found.\n";
        }
    }
    
    // Update student remarks
    void updateStudentRemarks() {
        if (studentCount == 0) {
            cout << "No students to update.\n";
            return;
        }
        
        int rollNumber;
        cout << "Enter the roll number of the student to update remarks: ";
        cin >> rollNumber;
        clearInputBuffer();
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getRollNumber() == rollNumber) {
                cout << "Enter remarks for the student (1: Poor, 2: Average, 3: Good, 4: Excellent): ";
                int choice;
                cin >> choice;
                clearInputBuffer();
                
                switch (choice) {
                    case 1:
                        students[i].setRemarks("Poor");
                        break;
                    case 2:
                        students[i].setRemarks("Average");
                        break;
                    case 3:
                        students[i].setRemarks("Good");
                        break;
                    case 4:
                        students[i].setRemarks("Excellent");
                        break;
                    default:
                        cout << "Invalid choice. Remarks not updated.\n";
                        return;
                }
                cout << "Student remarks updated successfully.\n";
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Student with roll number " << rollNumber << " not found.\n";
        }
    }
    
    // Delete a student
    void deleteStudent() {
        if (studentCount == 0) {
            cout << "No students to delete.\n";
            return;
        }
        
        int rollNumber;
        cout << "Enter the roll number of the student to delete: ";
        cin >> rollNumber;
        clearInputBuffer();
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getRollNumber() == rollNumber) {
                // Shift students to the left to fill the gap
                for (int j = i; j < studentCount - 1; j++) {
                    students[j] = students[j + 1];
                }
                studentCount--;
                cout << "Student with roll number " << rollNumber << " deleted successfully.\n";
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Student with roll number " << rollNumber << " not found.\n";
        }
    }
    
    // Search student by roll number
    void searchStudentByRollNumber() {
        if (studentCount == 0) {
            cout << "No students to search.\n";
            return;
        }
        
        int rollNumber;
        cout << "Enter the roll number of the student to search: ";
        cin >> rollNumber;
        clearInputBuffer();
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getRollNumber() == rollNumber) {
                double attendancePercentage = students[i].getAttendancePercentage(daysInMonth);
                
                cout << "Student found:\n";
                cout << "Roll Number: " << students[i].getRollNumber() << "\n";
                cout << "Name: " << students[i].getName() << "\n";
                cout << "Attendance Percentage: " << attendancePercentage << "%\n";
                cout << "Remarks: " << students[i].getRemarks() << "\n";
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Student with roll number " << rollNumber << " not found.\n";
        }
    }
    
    // Sort students by attendance percentage
    void sortStudentsByAttendance() {
        if (studentCount == 0) {
            cout << "No students to sort.\n";
            return;
        }
        
        // Simple bubble sort
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - i - 1; j++) {
                double attendance1 = students[j].getAttendancePercentage(daysInMonth);
                double attendance2 = students[j + 1].getAttendancePercentage(daysInMonth);
                
                if (attendance1 < attendance2) {
                    Student temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        
        cout << "Students sorted by attendance percentage.\n";
        displayStudents(); // Display sorted students
    }
    
    // Sort students by name
    void sortStudentsByName() {
        if (studentCount == 0) {
            cout << "No students to sort.\n";
            return;
        }
        
        // Simple bubble sort
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - i - 1; j++) {
                if (students[j].getName() > students[j + 1].getName()) {
                    Student temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        
        cout << "Students sorted by name.\n";
        displayStudents(); // Display sorted students
    }
    
    // Sort students by roll number
    void sortStudentsByRollNumber() {
        if (studentCount == 0) {
            cout << "No students to sort.\n";
            return;
        }
        
        // Simple bubble sort
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - i - 1; j++) {
                if (students[j].getRollNumber() > students[j + 1].getRollNumber()) {
                    Student temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        
        cout << "Students sorted by roll number.\n";
        displayStudents(); // Display sorted students
    }
    
    // Display total number of students
    void displayTotalNumberOfStudents() {
        cout << "Total number of students: " << studentCount << "\n";
    }
    
    // Display students with attendance above a certain threshold
    void displayStudentsAboveThreshold() {
        if (studentCount == 0) {
            cout << "No students to display.\n";
            return;
        }
        
        double threshold;
        cout << "Enter the attendance percentage threshold: ";
        cin >> threshold;
        clearInputBuffer();
        
        cout << "\nStudents with attendance percentage above " << threshold << "%:\n";
        cout << "----------------------------\n";
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            double attendancePercentage = students[i].getAttendancePercentage(daysInMonth);
            if (attendancePercentage > threshold) {
                cout << "Roll Number: " << students[i].getRollNumber() 
                        << ", Name: " << students[i].getName() 
                        << ", Attendance: " << attendancePercentage << "%\n";
                found = true;
            }
        }
        
        if (!found) {
            cout << "No students found with attendance above " << threshold << "%.\n";
        }
        
        cout << "----------------------------\n";
    }
    
    // Display students with attendance below a certain threshold
    void displayStudentsBelowThreshold() {
        if (studentCount == 0) {
            cout << "No students to display.\n";
            return;
        }
        
        double threshold;
        cout << "Enter the attendance percentage threshold: ";
        cin >> threshold;
        clearInputBuffer();
        
        cout << "\nStudents with attendance percentage below " << threshold << "%:\n";
        cout << "----------------------------\n";
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            double attendancePercentage = students[i].getAttendancePercentage(daysInMonth);
            if (attendancePercentage < threshold) {
                cout << "Roll Number: " << students[i].getRollNumber() 
                        << ", Name: " << students[i].getName() 
                        << ", Attendance: " << attendancePercentage << "%\n";
                found = true;
            }
        }
        
        if (!found) {
            cout << "No students found with attendance below " << threshold << "%.\n";
        }
        
        cout << "----------------------------\n";
    }
    
    // Display students with attendance in a specific range
    void displayStudentsInAttendanceRange() {
        if (studentCount == 0) {
            cout << "No students to display.\n";
            return;
        }
        
        double minAttendance, maxAttendance;
        cout << "Enter the minimum attendance percentage: ";
        cin >> minAttendance;
        cout << "Enter the maximum attendance percentage: ";
        cin >> maxAttendance;
        clearInputBuffer();
        
        cout << "\nStudents with attendance percentage between " 
                << minAttendance << "% and " << maxAttendance << "%:\n";
        cout << "----------------------------\n";
        
        bool found = false;
        for (int i = 0; i < studentCount; i++) {
            double attendancePercentage = students[i].getAttendancePercentage(daysInMonth);
            if (attendancePercentage >= minAttendance && attendancePercentage <= maxAttendance) {
                cout << "Roll Number: " << students[i].getRollNumber() 
                        << ", Name: " << students[i].getName() 
                        << ", Attendance: " << attendancePercentage << "%\n";
                found = true;
            }
        }
        
        if (!found) {
            cout << "No students found with attendance between " 
                    << minAttendance << "% and " << maxAttendance << "%.\n";
        }
        
        cout << "----------------------------\n";
    }
    
    // Update month and days in month
    void setMonth() {
        int month;
        cout << "Enter month number (1-12): ";
        cin >> month;
        clearInputBuffer();
        
        if (month < 1 || month > 12) {
            cout << "Invalid month number. Please enter a number between 1 and 12.\n";
            return;
        }
        
        currentMonth = month;
        
        // Set days in month based on month number
        switch (month) {
            case 2:
                daysInMonth = 28; // Simplified, not accounting for leap years
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                daysInMonth = 30;
                break;
            default:
                daysInMonth = 31;
        }
        
        cout << "Month set to " << month << " with " << daysInMonth << " days.\n";
    }
    
    // Save data to file
    void saveToFile() {
        ofstream file("students.dat", std::ios::binary);
        if (!file) {
            cout << "Error opening file for writing.\n";
            return;
        }
        
        file.write(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        file.write(reinterpret_cast<char*>(&currentMonth), sizeof(currentMonth));
        file.write(reinterpret_cast<char*>(&daysInMonth), sizeof(daysInMonth));
        file.write(reinterpret_cast<char*>(students), sizeof(Student) * studentCount);
        
        file.close();
        cout << "Student data saved to file.\n";
    }
    
    // Load data from file
    void loadFromFile() {
        ifstream file("students.dat", std::ios::binary);
        if (!file) {
            cout << "No saved data found or error opening file.\n";
            return;
        }
        
        file.read(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        file.read(reinterpret_cast<char*>(&currentMonth), sizeof(currentMonth));
        file.read(reinterpret_cast<char*>(&daysInMonth), sizeof(daysInMonth));
        file.read(reinterpret_cast<char*>(students), sizeof(Student) * studentCount);
        
        file.close();
        cout << "Student data loaded from file.\n";
    }
    
    // Display additional information about the project
    void displayAdditionalInfo() {
        cout << "\n";
        cout << "Project Team Contributions\n";
        cout << "----------------------------\n";
        cout << "Md Anis Mondal: Coding and Report\n";
        cout << "Md Reag Rahaman: PowerPoint Presentation\n";
        cout << "Harsh: PowerPoint Presentation\n";
        cout << "----------------------------\n";
        cout << "If you encounter any issues or bugs, kindly report them to Md Anis Mondal (BCA)\n";
    }
};

// Enhanced menu display
void displayMenu() {
    clearScreen();
    setConsoleColor(11); // Light cyan
    cout << "\n+==============================================================+\n";
    cout << "|           STUDENT ATTENDANCE MANAGEMENT SYSTEM              |\n";
    cout << "+==============================================================+\n";
    setConsoleColor(14); // Yellow
    cout << "|  1. Add Student                    13. Sort by Attendance    |\n";
    cout << "|  2. Display Students               14. Sort by Name          |\n";
    cout << "|  3. Mark Attendance                15. Sort by Roll Number   |\n";
    cout << "|  4. View Student Attendance        16. Display Total Students|\n";
    cout << "|  5. View Day Attendance            17. Students Above %      |\n";
    cout << "|  6. Calculate Average Attendance   18. Students Below %      |\n";
    cout << "|  7. Highest/Lowest Attendance      19. Students in Range     |\n";
    cout << "|  8. Update Student Name            20. Set Month             |\n";
    cout << "|  9. Update Roll Number             21. Save to File          |\n";
    cout << "| 10. Update Remarks                 22. Additional Info       |\n";
    cout << "| 11. Delete Student                 23. Exit                  |\n";
    cout << "| 12. Search Student                                           |\n";
    setConsoleColor(11);
    cout << "+==============================================================+\n";
    setConsoleColor(7);
    cout << "\nEnter your choice (1-23): ";
}

int main() {
    showWelcomeScreen();
    
    AttendanceSystem system;
    system.loadFromFile();
    
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;
        system.clearInputBuffer();
        
        switch (choice) {
            case 1: 
                system.addStudent(); 
                break;
            case 2: 
                system.displayStudents(); 
                break;
            case 3:
                system.markAttendance();
                pauseScreen();
                break;
            case 4:
                system.viewStudentAttendance();
                pauseScreen();
                break;
            case 5:
                system.viewDayAttendance();
                pauseScreen();
                break;
            case 6:
                system.calculateAverageAttendance();
                pauseScreen();
                break;
            case 7:
                system.findHighestLowestAttendance();
                pauseScreen();
                break;
            case 8:
                system.updateStudentName();
                pauseScreen();
                break;
            case 9:
                system.updateStudentRollNumber();
                pauseScreen();
                break;
            case 10:
                system.updateStudentRemarks();
                pauseScreen();
                break;
            case 11:
                system.deleteStudent();
                pauseScreen();
                break;
            case 12:
                system.searchStudentByRollNumber();
                pauseScreen();
                break;
            case 13:
                system.sortStudentsByAttendance();
                break;
            case 14:
                system.sortStudentsByName();
                break;
            case 15:
                system.sortStudentsByRollNumber();
                break;
            case 16:
                system.displayTotalNumberOfStudents();
                pauseScreen();
                break;
            case 17:
                system.displayStudentsAboveThreshold();
                pauseScreen();
                break;
            case 18:
                system.displayStudentsBelowThreshold();
                pauseScreen();
                break;
            case 19:
                system.displayStudentsInAttendanceRange();
                pauseScreen();
                break;
            case 20:
                system.setMonth();
                pauseScreen();
                break;
            case 21:
                system.saveToFile();
                pauseScreen();
                break;
            case 22:
                system.displayAdditionalInfo();
                pauseScreen();
                break;
            case 23:
                clearScreen();
                setConsoleColor(10);
                cout << "\n\n  Thank you for using SAMS!\n";
                cout << "     Data has been saved automatically.\n";
                cout << "     Have a great day! 👋\n\n";
                setConsoleColor(7);
                system.saveToFile();
                return 0;
            default:
                setConsoleColor(12);
                cout << "\n❌ Invalid choice. Please try again.\n";
                setConsoleColor(7);
                pauseScreen();
                break;
        }
    }
}
