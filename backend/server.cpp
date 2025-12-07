#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

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

const string FILENAME = "students.txt";
const int PORT = 8080;

// Function prototypes
vector<Student> loadStudents();
void saveStudents(const vector<Student>& students);
int generateNewId();
string handleRequest(const string& request);
string addStudentAPI(const string& data);
string getAllStudentsAPI();
string searchStudentAPI(int id);
string updateStudentAPI(const string& data);
string deleteStudentAPI(int id);
string createJSONResponse(const string& status, const string& message, const string& data = "");

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed\n";
        return 1;
    }
    
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }
    
    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    // Listen for connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cerr << "Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "========================================\n";
    cout << "  SRMS Backend Server Running\n";
    cout << "  Port: " << PORT << "\n";
    cout << "========================================\n\n";
    cout << "Waiting for connections...\n\n";
    
    // Accept and handle connections
    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed\n";
            continue;
        }
        
        char buffer[4096] = {0};
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        
        if (bytesReceived > 0) {
            string request(buffer);
            cout << "Received request:\n" << request.substr(0, 200) << "...\n\n";
            
            string response = handleRequest(request);
            
            // Send HTTP response
            string httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: application/json\r\n";
            httpResponse += "Access-Control-Allow-Origin: *\r\n";
            httpResponse += "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
            httpResponse += "Access-Control-Allow-Headers: Content-Type\r\n";
            httpResponse += "Content-Length: " + to_string(response.length()) + "\r\n";
            httpResponse += "\r\n";
            httpResponse += response;
            
            send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
            cout << "Response sent\n\n";
        }
        
        closesocket(clientSocket);
    }
    
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

string handleRequest(const string& request) {
    // Parse HTTP method and path
    istringstream stream(request);
    string method, path;
    stream >> method >> path;
    
    // Handle OPTIONS for CORS
    if (method == "OPTIONS") {
        return "{}";
    }
    
    // Extract request body
    string body = "";
    size_t bodyPos = request.find("\r\n\r\n");
    if (bodyPos != string::npos) {
        body = request.substr(bodyPos + 4);
    }
    
    // Route requests
    if (path == "/api/students" && method == "GET") {
        return getAllStudentsAPI();
    } else if (path == "/api/students" && method == "POST") {
        return addStudentAPI(body);
    } else if (path.find("/api/students/") == 0 && method == "GET") {
        int id = stoi(path.substr(14));
        return searchStudentAPI(id);
    } else if (path.find("/api/students/") == 0 && method == "PUT") {
        return updateStudentAPI(body);
    } else if (path.find("/api/students/") == 0 && method == "DELETE") {
        int id = stoi(path.substr(14));
        return deleteStudentAPI(id);
    }
    
    return createJSONResponse("error", "Invalid endpoint");
}

string addStudentAPI(const string& data) {
    // Parse JSON data (simple parsing)
    Student student;
    
    // Extract fields from JSON
    size_t pos;
    
    // Check if custom ID is provided
    pos = data.find("\"id\":");
    if (pos != string::npos) {
        size_t start = pos + 5;
        size_t end = data.find_first_of(",}", start);
        string idStr = data.substr(start, end - start);
        // Remove any whitespace or quotes
        idStr.erase(remove_if(idStr.begin(), idStr.end(), ::isspace), idStr.end());
        if (!idStr.empty() && idStr != "null" && idStr != "0") {
            student.id = stoi(idStr);
        } else {
            student.id = generateNewId();
        }
    } else {
        student.id = generateNewId();
    }
    
    pos = data.find("\"name\":\"");
    if (pos != string::npos) {
        size_t start = pos + 8;
        size_t end = data.find("\"", start);
        student.name = data.substr(start, end - start);
    }
    
    pos = data.find("\"age\":");
    if (pos != string::npos) {
        size_t start = pos + 6;
        size_t end = data.find(",", start);
        student.age = stoi(data.substr(start, end - start));
    }
    
    pos = data.find("\"course\":\"");
    if (pos != string::npos) {
        size_t start = pos + 10;
        size_t end = data.find("\"", start);
        student.course = data.substr(start, end - start);
    }
    
    pos = data.find("\"email\":\"");
    if (pos != string::npos) {
        size_t start = pos + 9;
        size_t end = data.find("\"", start);
        student.email = data.substr(start, end - start);
    }
    
    pos = data.find("\"gpa\":");
    if (pos != string::npos) {
        size_t start = pos + 6;
        size_t end = data.find_first_of(",}", start);
        student.gpa = stof(data.substr(start, end - start));
    }
    
    pos = data.find("\"marks\":");
    if (pos != string::npos) {
        size_t start = pos + 8;
        size_t end = data.find_first_of(",}", start);
        student.marks = stoi(data.substr(start, end - start));
    } else {
        student.marks = 0;
    }
    
    // Save to file
    ofstream file(FILENAME, ios::app);
    if (file.is_open()) {
        file << student.id << "|" << student.name << "|" << student.age << "|" 
             << student.course << "|" << student.email << "|" << student.gpa << "|" << student.marks << "\n";
        file.close();
        
        string studentData = "{\"id\":" + to_string(student.id) + ",\"name\":\"" + student.name + "\"}";
        return createJSONResponse("success", "Student added successfully", studentData);
    }
    
    return createJSONResponse("error", "Failed to add student");
}

string getAllStudentsAPI() {
    vector<Student> students = loadStudents();
    
    string jsonArray = "[";
    for (size_t i = 0; i < students.size(); i++) {
        jsonArray += "{";
        jsonArray += "\"id\":" + to_string(students[i].id) + ",";
        jsonArray += "\"name\":\"" + students[i].name + "\",";
        jsonArray += "\"age\":" + to_string(students[i].age) + ",";
        jsonArray += "\"course\":\"" + students[i].course + "\",";
        jsonArray += "\"email\":\"" + students[i].email + "\",";
        jsonArray += "\"gpa\":" + to_string(students[i].gpa) + ",";
        jsonArray += "\"marks\":" + to_string(students[i].marks);
        jsonArray += "}";
        
        if (i < students.size() - 1) {
            jsonArray += ",";
        }
    }
    jsonArray += "]";
    
    return createJSONResponse("success", "Students retrieved", jsonArray);
}

string searchStudentAPI(int id) {
    vector<Student> students = loadStudents();
    
    for (const auto& student : students) {
        if (student.id == id) {
            string studentData = "{";
            studentData += "\"id\":" + to_string(student.id) + ",";
            studentData += "\"name\":\"" + student.name + "\",";
            studentData += "\"age\":" + to_string(student.age) + ",";
            studentData += "\"course\":\"" + student.course + "\",";
            studentData += "\"email\":\"" + student.email + "\",";
            studentData += "\"gpa\":" + to_string(student.gpa) + ",";
            studentData += "\"marks\":" + to_string(student.marks);
            studentData += "}";
            
            return createJSONResponse("success", "Student found", studentData);
        }
    }
    
    return createJSONResponse("error", "Student not found");
}

string updateStudentAPI(const string& data) {
    vector<Student> students = loadStudents();
    
    // Extract ID
    int updateId = 0;
    size_t pos = data.find("\"id\":");
    if (pos != string::npos) {
        size_t start = pos + 5;
        size_t end = data.find(",", start);
        updateId = stoi(data.substr(start, end - start));
    }
    
    bool found = false;
    for (auto& student : students) {
        if (student.id == updateId) {
            found = true;
            
            // Update fields
            pos = data.find("\"name\":\"");
            if (pos != string::npos) {
                size_t start = pos + 8;
                size_t end = data.find("\"", start);
                student.name = data.substr(start, end - start);
            }
            
            pos = data.find("\"age\":");
            if (pos != string::npos) {
                size_t start = pos + 6;
                size_t end = data.find(",", start);
                student.age = stoi(data.substr(start, end - start));
            }
            
            pos = data.find("\"course\":\"");
            if (pos != string::npos) {
                size_t start = pos + 10;
                size_t end = data.find("\"", start);
                student.course = data.substr(start, end - start);
            }
            
            pos = data.find("\"email\":\"");
            if (pos != string::npos) {
                size_t start = pos + 9;
                size_t end = data.find("\"", start);
                student.email = data.substr(start, end - start);
            }
            
            pos = data.find("\"gpa\":");
            if (pos != string::npos) {
                size_t start = pos + 6;
                size_t end = data.find_first_of(",}", start);
                student.gpa = stof(data.substr(start, end - start));
            }
            
            pos = data.find("\"marks\":");
            if (pos != string::npos) {
                size_t start = pos + 8;
                size_t end = data.find_first_of(",}", start);
                student.marks = stoi(data.substr(start, end - start));
            }
            
            saveStudents(students);
            break;
        }
    }
    
    if (found) {
        return createJSONResponse("success", "Student updated successfully");
    }
    
    return createJSONResponse("error", "Student not found");
}

string deleteStudentAPI(int id) {
    vector<Student> students = loadStudents();
    size_t originalSize = students.size();
    
    students.erase(
        std::remove_if(students.begin(), students.end(), 
                       [id](const Student& s) { return s.id == id; }),
        students.end()
    );
    
    if (students.size() < originalSize) {
        saveStudents(students);
        return createJSONResponse("success", "Student deleted successfully");
    }
    
    return createJSONResponse("error", "Student not found");
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
        return 1001;
    }
    
    int maxId = 0;
    for (const auto& student : students) {
        if (student.id > maxId) {
            maxId = student.id;
        }
    }
    
    return maxId + 1;
}

string createJSONResponse(const string& status, const string& message, const string& data) {
    string json = "{";
    json += "\"status\":\"" + status + "\",";
    json += "\"message\":\"" + message + "\"";
    
    if (!data.empty()) {
        json += ",\"data\":" + data;
    }
    
    json += "}";
    return json;
}
