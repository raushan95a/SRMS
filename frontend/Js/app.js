// API Configuration
const API_BASE_URL = 'http://localhost:8080/api';

// API Functions

/**
 * Fetch all students from the backend
 * @returns {Promise<Array>} Array of student objects
 */
async function fetchStudents() {
    try {
        const response = await fetch(`${API_BASE_URL}/students`);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        const result = await response.json();
        
        if (result.status === 'success') {
            return result.data;
        } else {
            throw new Error(result.message || 'Failed to fetch students');
        }
    } catch (error) {
        console.error('Error fetching students:', error);
        if (error.message.includes('Failed to fetch')) {
            throw new Error('Cannot connect to server. Please ensure the backend server is running on port 8080.');
        }
        throw error;
    }
}

/**
 * Add a new student
 * @param {Object} studentData - Student information
 * @returns {Promise<Object>} Result object
 */
async function addStudent(studentData) {
    try {
        const response = await fetch(`${API_BASE_URL}/students`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(studentData)
        });
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        const result = await response.json();
        
        if (result.status === 'success') {
            return result;
        } else {
            throw new Error(result.message || 'Failed to add student');
        }
    } catch (error) {
        console.error('Error adding student:', error);
        if (error.message.includes('Failed to fetch')) {
            throw new Error('Cannot connect to server. Please ensure the backend server is running on port 8080.');
        }
        throw error;
    }
}

/**
 * Search for a student by ID
 * @param {number} id - Student ID
 * @returns {Promise<Object>} Student object
 */
async function searchStudent(id) {
    try {
        const response = await fetch(`${API_BASE_URL}/students/${id}`);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        const result = await response.json();
        
        if (result.status === 'success') {
            return result.data;
        } else {
            throw new Error(result.message || 'Student not found');
        }
    } catch (error) {
        console.error('Error searching student:', error);
        if (error.message.includes('Failed to fetch')) {
            throw new Error('Cannot connect to server. Please ensure the backend server is running on port 8080.');
        }
        throw error;
    }
}

/**
 * Update student information
 * @param {number} id - Student ID
 * @param {Object} studentData - Updated student information
 * @returns {Promise<Object>} Result object
 */
async function updateStudent(id, studentData) {
    try {
        const response = await fetch(`${API_BASE_URL}/students/${id}`, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(studentData)
        });
        
        const result = await response.json();
        
        if (result.status === 'success') {
            return result;
        } else {
            throw new Error(result.message || 'Failed to update student');
        }
    } catch (error) {
        console.error('Error updating student:', error);
        throw error;
    }
}

/**
 * Delete a student
 * @param {number} id - Student ID
 * @returns {Promise<Object>} Result object
 */
async function deleteStudent(id) {
    try {
        const response = await fetch(`${API_BASE_URL}/students/${id}`, {
            method: 'DELETE'
        });
        
        const result = await response.json();
        
        if (result.status === 'success') {
            return result;
        } else {
            throw new Error(result.message || 'Failed to delete student');
        }
    } catch (error) {
        console.error('Error deleting student:', error);
        throw error;
    }
}

// Utility Functions

/**
 * Format GPA to 2 decimal places
 * @param {number} gpa - GPA value
 * @returns {string} Formatted GPA
 */
function formatGPA(gpa) {
    return parseFloat(gpa).toFixed(2);
}

/**
 * Validate email format
 * @param {string} email - Email address
 * @returns {boolean} True if valid
 */
function validateEmail(email) {
    const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return re.test(email);
}

/**
 * Validate GPA range
 * @param {number} gpa - GPA value
 * @returns {boolean} True if valid
 */
function validateGPA(gpa) {
    return gpa >= 0 && gpa <= 10.0;
}

/**
 * Show notification message
 * @param {string} message - Message to display
 * @param {string} type - Message type (success, error, info)
 */
function showNotification(message, type = 'info') {
    // Create notification element
    const notification = document.createElement('div');
    notification.className = `notification notification-${type}`;
    notification.textContent = message;
    notification.style.cssText = `
        position: fixed;
        top: 20px;
        right: 20px;
        padding: 1rem 1.5rem;
        border-radius: 6px;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        z-index: 9999;
        animation: slideIn 0.3s ease;
        max-width: 400px;
    `;
    
    // Set colors based on type
    if (type === 'success') {
        notification.style.background = '#10b981';
        notification.style.color = 'white';
    } else if (type === 'error') {
        notification.style.background = '#ef4444';
        notification.style.color = 'white';
    } else {
        notification.style.background = '#3b82f6';
        notification.style.color = 'white';
    }
    
    document.body.appendChild(notification);
    
    // Remove after 5 seconds
    setTimeout(() => {
        notification.style.animation = 'slideOut 0.3s ease';
        setTimeout(() => {
            document.body.removeChild(notification);
        }, 300);
    }, 5000);
}

// Add CSS animations
const style = document.createElement('style');
style.textContent = `
    @keyframes slideIn {
        from {
            transform: translateX(100%);
            opacity: 0;
        }
        to {
            transform: translateX(0);
            opacity: 1;
        }
    }
    
    @keyframes slideOut {
        from {
            transform: translateX(0);
            opacity: 1;
        }
        to {
            transform: translateX(100%);
            opacity: 0;
        }
    }
`;
document.head.appendChild(style);

// Export functions for use in HTML pages
window.fetchStudents = fetchStudents;
window.addStudent = addStudent;
window.searchStudent = searchStudent;
window.updateStudent = updateStudent;
window.deleteStudent = deleteStudent;
window.formatGPA = formatGPA;
window.validateEmail = validateEmail;
window.validateGPA = validateGPA;
window.showNotification = showNotification;
