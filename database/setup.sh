#!/bin/bash
# Setup database quickly

echo "Setting up Crime Intelligence Database..."

# Install MySQL if not present
if ! command -v mysql &> /dev/null; then
    echo "MySQL not found. Installing..."
    sudo apt install -y mysql-server
    sudo systemctl start mysql
    sudo systemctl enable mysql
fi

# Secure MySQL installation (optional - remove for quick setup)
# sudo mysql_secure_installation

# Create database and user
sudo mysql -e "
CREATE DATABASE IF NOT EXISTS crime_db;
CREATE USER IF NOT EXISTS 'crime_user'@'localhost' IDENTIFIED BY 'CrimePass123!';
GRANT ALL PRIVILEGES ON crime_db.* TO 'crime_user'@'localhost';
FLUSH PRIVILEGES;
"

# Import schema
sudo mysql crime_db < schema.sql

echo "Database setup complete!"
echo "Database: crime_db"
echo "Username: crime_user"
echo "Password: CrimePass123!"
echo "Host: localhost"
