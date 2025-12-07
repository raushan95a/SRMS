#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// Student structure
struct Student {
    int id;
    string name;
    int age;
    string course;
    string email;
    float gpa;
    int marks;
};

// Function prototypes
void displayMenu();
void addStudent();
void viewAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
vector<Student> loadStudents();
void saveStudents(const vector<Student>& students);
int generateNewId();
void clearScreen();
string trim(const string& str);

const string FILENAME = "students.txt";

int main() {
    int choice;
    
    cout << "========================================\n";
    cout << "  STUDENT RECORD MANAGEMENT SYSTEM\n";
    cout << "========================================\n\n";
    
    while (true) {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                cout << "\nThank you for using SRMS!\n";
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
        clearScreen();
    }
    
    return 0;
}

void displayMenu() {
    cout << "\n========== MAIN MENU ==========\n";
    cout << "1. Add New Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Exit\n";
    cout << "===============================\n";
}

void addStudent() {
    Student student;
    
    cout << "\n--- Add New Student ---\n";
    cout << "Do you want to use a custom ID? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 'y' || choice == 'Y') {
        cout << "Enter Student ID: ";
        cin >> student.id;
        cin.ignore();
    } else {
        student.id = generateNewId();
        cout << "Student ID (Auto-generated): " << student.id << "\n";
    }
    
    cout << "Enter Name: ";
    getline(cin, student.name);
    
    cout << "Enter Age: ";
    cin >> student.age;
    cin.ignore();
    
    cout << "Enter Course: ";
    getline(cin, student.course);
    
    cout << "Enter Email: ";
    getline(cin, student.email);
    
    cout << "Enter GPA: ";
    cin >> student.gpa;
    cin.ignore();
    
    cout << "Enter Marks (0-100): ";
    cin >> student.marks;
    cin.ignore();
    
    // Save to file
    ofstream file(FILENAME, ios::app);
    if (file.is_open()) {
        file << student.id << "|" << student.name << "|" << student.age << "|" 
             << student.course << "|" << student.email << "|" << student.gpa << "|" << student.marks << "\n";
        file.close();
        cout << "\n✓ Student added successfully!\n";
    } else {
        cout << "\n✗ Error: Unable to open file!\n";
    }
}

void viewAllStudents() {
    vector<Student> students = loadStudents();
    
    cout << "\n========== ALL STUDENTS ==========\n";
    
    if (students.empty()) {
        cout << "No students found in the system.\n";
        return;
    }
    
    cout << "Total Students: " << students.size() << "\n\n";
    cout << "ID\tName\t\t\tAge\tCourse\t\t\tEmail\t\t\tGPA\tMarks\n";
    cout << "---------------------------------------------------------------------------------\n";
    
    for (const auto& student : students) {
        cout << student.id << "\t" 
             << student.name << "\t\t" 
             << student.age << "\t" 
             << student.course << "\t\t" 
             << student.email << "\t\t" 
             << student.gpa << "\t" 
             << student.marks << "\n";
    }
}

void searchStudent() {
    vector<Student> students = loadStudents();
    
    cout << "\n--- Search Student ---\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) {
        int searchId;
        cout << "Enter Student ID: ";
        cin >> searchId;
        cin.ignore();
        
        bool found = false;
        for (const auto& student : students) {
            if (student.id == searchId) {
                cout << "\n✓ Student Found!\n";
                cout << "ID: " << student.id << "\n";
                cout << "Name: " << student.name << "\n";
                cout << "Age: " << student.age << "\n";
                cout << "Course: " << student.course << "\n";
                cout << "Email: " << student.email << "\n";
                cout << "GPA: " << student.gpa << "\n";
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "\n✗ Student with ID " << searchId << " not found.\n";
        }
    } else if (choice == 2) {
        string searchName;
        cout << "Enter Student Name: ";
        getline(cin, searchName);
        
        bool found = false;
        for (const auto& student : students) {
            if (student.name.find(searchName) != string::npos) {
                cout << "\n✓ Student Found!\n";
                cout << "ID: " << student.id << "\n";
                cout << "Name: " << student.name << "\n";
                cout << "Age: " << student.age << "\n";
                cout << "Course: " << student.course << "\n";
                cout << "Email: " << student.email << "\n";
                cout << "GPA: " << student.gpa << "\n";
                cout << "Marks: " << student.marks << "\n";
                cout << "---\n";
                found = true;
            }
        }
        
        if (!found) {
            cout << "\n✗ No student found with name containing '" << searchName << "'.\n";
        }
    } else {
        cout << "\nInvalid choice!\n";
    }
}

void updateStudent() {
    vector<Student> students = loadStudents();
    
    cout << "\n--- Update Student ---\n";
    cout << "Enter Student ID to update: ";
    
    int updateId;
    cin >> updateId;
    cin.ignore();
    
    bool found = false;
    for (auto& student : students) {
        if (student.id == updateId) {
            found = true;
            
            cout << "\nCurrent Details:\n";
            cout << "Name: " << student.name << "\n";
            cout << "Age: " << student.age << "\n";
            cout << "Course: " << student.course << "\n";
            cout << "Email: " << student.email << "\n";
            cout << "GPA: " << student.gpa << "\n";
            cout << "Marks: " << student.marks << "\n";
            
            cout << "\nEnter New Details:\n";
            cout << "Name: ";
            getline(cin, student.name);
            
            cout << "Age: ";
            cin >> student.age;
            cin.ignore();
            
            cout << "Course: ";
            getline(cin, student.course);
            
            cout << "Email: ";
            getline(cin, student.email);
            
            cout << "GPA: ";
            cin >> student.gpa;
            cin.ignore();
            
            cout << "Marks (0-100): ";
            cin >> student.marks;
            cin.ignore();
            
            saveStudents(students);
            cout << "\n✓ Student updated successfully!\n";
            break;
        }
    }
    
    if (!found) {
        cout << "\n✗ Student with ID " << updateId << " not found.\n";
    }
}

void deleteStudent() {
    vector<Student> students = loadStudents();
    
    cout << "\n--- Delete Student ---\n";
    cout << "Enter Student ID to delete: ";
    
    int deleteId;
    cin >> deleteId;
    cin.ignore();
    
    auto it = remove_if(students.begin(), students.end(), 
                        [deleteId](const Student& s) { return s.id == deleteId; });
    
    if (it != students.end()) {
        students.erase(it, students.end());
        saveStudents(students);
        cout << "\n✓ Student deleted successfully!\n";
    } else {
        cout << "\n✗ Student with ID " << deleteId << " not found.\n";
    }
}

vector<Student> loadStudents() {
    vector<Student> students;
    ifstream file(FILENAME);
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Student student;
            stringstream ss(line);
            string temp;
            
            getline(ss, temp, '|');
            student.id = stoi(temp);
            
            getline(ss, student.name, '|');
            
            getline(ss, temp, '|');
            student.age = stoi(temp);
            
            getline(ss, student.course, '|');
            getline(ss, student.email, '|');
            
            getline(ss, temp, '|');
            student.gpa = stof(temp);
            
            getline(ss, temp, '|');
            if (!temp.empty()) {
                student.marks = stoi(temp);
            } else {
                student.marks = 0;
            }
            
            students.push_back(student);
        }
        file.close();
    }
    
    return students;
}

void saveStudents(const vector<Student>& students) {
    ofstream file(FILENAME);
    
    if (file.is_open()) {
        for (const auto& student : students) {
            file << student.id << "|" << student.name << "|" << student.age << "|" 
                 << student.course << "|" << student.email << "|" << student.gpa << "|" << student.marks << "\n";
        }
        file.close();
    }
}

int generateNewId() {
    vector<Student> students = loadStudents();
    
    if (students.empty()) {
        return 1001; // Starting ID
    }
    
    int maxId = 0;
    for (const auto& student : students) {
        if (student.id > maxId) {
            maxId = student.id;
        }
    }
    
    return maxId + 1;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
