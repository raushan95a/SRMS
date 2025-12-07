# Student Record Management System (SRMS)

## Project Overview
SRMS is a comprehensive Student Record Management System with a C++ backend and web-based frontend. It provides complete CRUD operations (Create, Read, Update, Delete) for managing student records efficiently.

## Features
- **Add Students**: Register new students with personal and academic details
- **View Students**: Browse all registered students in a table format
- **Search Students**: Find specific students by ID
- **Update Students**: Modify existing student information
- **Delete Students**: Remove student records from the system
- **Persistent Storage**: File-based storage using `students.txt`

## Project Structure
```
cc project/
├── backend/
│   ├── main.cpp          # Standalone console application
│   ├── server.cpp        # HTTP server for web frontend
│   └── students.txt      # Data storage file (auto-created)
├── frontend/
│   ├── index.html        # Home page
│   ├── add-student.html  # Add student form
│   ├── view-students.html # View/Update/Delete students
│   ├── search-student.html # Search functionality
│   ├── css/
│   │   └── style.css     # Stylesheet
│   └── js/
│       └── app.js        # API communication
└── README.md
```

## Technologies Used
### Backend
- C++ (Console & HTTP Server)
- File handling for data persistence
- Winsock2 for networking (Windows)

### Frontend
- HTML5
- CSS3 (Modern responsive design)
- JavaScript (Fetch API for backend communication)

## Installation & Setup

### Prerequisites
- MinGW-w64 (GCC compiler for Windows)
- Visual Studio Code or any C++ IDE
- Modern web browser

### Backend Setup

#### Option 1: Console Application
1. Compile the standalone application:
   ```powershell
   g++ backend/main.cpp -o backend/srms.exe
   ```

2. Run the application:
   ```powershell
   .\backend\srms.exe
   ```

#### Option 2: Web Server (For Frontend Integration)
1. Compile the server:
   ```powershell
   g++ backend/server.cpp -o backend/server.exe -lws2_32
   ```

2. Start the server:
   ```powershell
   .\backend\server.exe
   ```
   The server will run on `http://localhost:8080`

### Frontend Setup
1. Ensure the backend server is running
2. Open `frontend/index.html` in your web browser
3. Start managing student records!

## Usage Guide

### Console Application
1. Launch `srms.exe`
2. Use the menu to:
   - Press 1: Add new student
   - Press 2: View all students
   - Press 3: Search student by ID or name
   - Press 4: Update student information
   - Press 5: Delete student
   - Press 6: Exit

### Web Application
1. **Home Page**: Overview and navigation
2. **Add Student**: Fill the form and submit
3. **View Students**: See all students, edit or delete
4. **Search**: Enter student ID to find details

## Data Format
Student records are stored in `students.txt` with the following format:
```
ID|Name|Age|Course|Email|GPA
```
Example:
```
1001|John Doe|20|Computer Science|john@example.com|3.75
```

## Configuration
To change the server port, edit `server.cpp`:
```cpp
const int PORT = 8080; // Change this value
```

To change the API endpoint in frontend, edit `js/app.js`:
```javascript
const API_BASE_URL = 'http://localhost:8080/api';
```

## UI Features
- Modern, responsive design
- Clean and intuitive interface
- Real-time form validation
- Success/Error notifications
- Mobile-friendly layout
- Modal dialogs for updates

## Data Validation
- Name: Required, text input
- Age: 16-100 years
- Course: Required, text input
- Email: Valid email format
- GPA: 0.0-10.0 scale

## Troubleshooting

### Backend Issues
- **Port already in use**: Change the PORT constant
- **Compilation errors**: Ensure MinGW is properly installed
- **File access errors**: Check write permissions in backend folder

### Frontend Issues
- **API connection failed**: Ensure backend server is running
- **CORS errors**: Server includes CORS headers; check browser console
- **Data not loading**: Verify API_BASE_URL in app.js

## File Descriptions

### Backend Files
- `main.cpp`: Standalone console application with menu-driven interface
- `server.cpp`: HTTP server handling API requests from frontend
- `students.txt`: Text file storing all student records

### Frontend Files
- `index.html`: Landing page with features overview
- `add-student.html`: Form for adding new students
- `view-students.html`: Table view with edit/delete functionality
- `search-student.html`: Search interface by student ID
- `css/style.css`: Complete styling with modern design
- `js/app.js`: API functions and utility helpers

## API Endpoints

### GET /api/students
Retrieve all students

### POST /api/students
Add a new student
```json
{
  "name": "John Doe",
  "age": 20,
  "course": "Computer Science",
  "email": "john@example.com",
  "gpa": 3.75
}
```

### GET /api/students/{id}
Get student by ID

### PUT /api/students/{id}
Update student information

### DELETE /api/students/{id}
Delete student

## Future Enhancements
- Authentication and authorization
- Export to CSV/PDF
- Advanced search filters
- Student performance analytics
- Batch operations
- Database integration (MySQL/PostgreSQL)

## Development
Built with separation of concerns:
- Backend handles all business logic and data storage
- Frontend provides user-friendly interface
- Clear API contract between layer

---
**SRMS** - Making student record management simple and efficient! 📚✨
