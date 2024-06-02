#!/bin/bash

# Update package lists
echo "Updating package lists..."
sudo apt update

# Install Node.js and npm
echo "Installing Node.js and npm..."
sudo apt install -y nodejs npm

# Install uuid package
echo "Installing uuid package..."
npm install uuid

# Run the server
echo "Running the server..."
node server.mjs

