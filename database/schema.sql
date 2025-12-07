-- crime_intelligence_system.sql

CREATE DATABASE IF NOT EXISTS crime_db;
USE crime_db;

-- 1. CRIMINALS TABLE (Stored in B-Tree)
CREATE TABLE criminals (
    criminal_id INT AUTO_INCREMENT PRIMARY KEY,
    cnic VARCHAR(15) UNIQUE NOT NULL,
    name VARCHAR(100) NOT NULL,
    age INT,
    gender ENUM('Male', 'Female', 'Other'),
    address TEXT,
    crime_type VARCHAR(50),
    status ENUM('Active', 'Arrested', 'Wanted', 'Released'),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_cnic (cnic),
    INDEX idx_name (name),
    INDEX idx_status (status)
);

-- 2. FINGERPRINTS TABLE (Hash Table equivalent)
CREATE TABLE fingerprints (
    fingerprint_id VARCHAR(64) PRIMARY KEY,  -- SHA256 hash
    criminal_id INT NOT NULL,
    scanned_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (criminal_id) REFERENCES criminals(criminal_id) ON DELETE CASCADE,
    INDEX idx_fingerprint (fingerprint_id)
);

-- 3. WEAPONS TABLE (Hash Table equivalent)
CREATE TABLE weapons (
    serial_number VARCHAR(50) PRIMARY KEY,
    weapon_type VARCHAR(50),
    criminal_id INT,
    registered_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (criminal_id) REFERENCES criminals(criminal_id) ON DELETE SET NULL,
    INDEX idx_serial (serial_number)
);

-- 4. CRIME REPORTS TABLE (Queue equivalent)
CREATE TABLE crime_reports (
    report_id INT AUTO_INCREMENT PRIMARY KEY,
    report_code VARCHAR(20) UNIQUE NOT NULL,
    title VARCHAR(200) NOT NULL,
    description TEXT,
    location VARCHAR(255),
    reporter_name VARCHAR(100),
    status ENUM('Pending', 'Processing', 'Resolved', 'Archived') DEFAULT 'Pending',
    priority ENUM('Low', 'Medium', 'High') DEFAULT 'Medium',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    processed_at TIMESTAMP NULL,
    INDEX idx_status (status),
    INDEX idx_created (created_at)
);

-- 5. ADMIN ACTIONS TABLE (Stack equivalent for undo)
CREATE TABLE admin_actions (
    action_id INT AUTO_INCREMENT PRIMARY KEY,
    admin_id INT,
    action_type ENUM('INSERT', 'UPDATE', 'DELETE'),
    table_name VARCHAR(50),
    record_id INT,
    old_data JSON,
    new_data JSON,
    performed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    undone BOOLEAN DEFAULT FALSE,
    INDEX idx_admin (admin_id),
    INDEX idx_undone (undone)
);

-- 6. CRIME PATTERNS TABLE
CREATE TABLE crime_patterns (
    pattern_id INT AUTO_INCREMENT PRIMARY KEY,
    pattern_type VARCHAR(50),
    location VARCHAR(255),
    frequency INT,
    common_time TIME,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 7. ADMINS TABLE
CREATE TABLE admins (
    admin_id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    full_name VARCHAR(100),
    role ENUM('Super Admin', 'Data Entry', 'Viewer') DEFAULT 'Data Entry',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Insert default admin (password: admin123)
INSERT INTO admins (username, password_hash, full_name, role) 
VALUES ('admin', '$2b$10$YourHashedPasswordHere', 'System Administrator', 'Super Admin');

-- Insert sample data
INSERT INTO criminals (cnic, name, age, gender, address, crime_type, status) VALUES
('42101-1234567-8', 'John Smith', 32, 'Male', '123 Main St, Karachi', 'Robbery', 'Wanted'),
('42101-2345678-9', 'Sarah Khan', 28, 'Female', '456 Market Rd, Lahore', 'Fraud', 'Arrested'),
('42101-3456789-0', 'Ali Ahmed', 45, 'Male', '789 Park Ave, Islamabad', 'Assault', 'Active');

INSERT INTO crime_reports (report_code, title, description, location, priority) VALUES
('FIR-2024-001', 'Bank Robbery', 'Armed robbery at National Bank', 'Gulshan, Karachi', 'High'),
('FIR-2024-002', 'Car Theft', 'Toyota Corolla stolen from parking', 'DHA, Lahore', 'Medium'),
('FIR-2024-003', 'Cyber Fraud', 'Online banking fraud reported', 'Blue Area, Islamabad', 'High');
