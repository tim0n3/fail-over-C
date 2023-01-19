#!/bin/bash

# Check if script is running as root or with sudo
if [ "$EUID" -ne 0 ]
  then echo "Please run as root or with sudo"
  exit
fi

# Check if /usr/bin/ folder exists
if [ ! -d "/usr/bin/" ]; then
  # Create /usr/bin/ folder
  mkdir /usr/bin/
  # Set folder permissions
  chmod 755 /usr/bin/
  echo "/usr/bin/ folder created and permissions set"
else
  echo "/usr/bin/ folder exists"
fi

# Copy inetmonit to /usr/bin/
cp inetmonit /usr/bin/

# Check if copy was successful
if [ $? -eq 0 ]; then
  echo "inetmonit successfully copied to /usr/bin/"
else
  echo "Error: inetmonit was not copied to /usr/bin/"  | tee -a errors-installer.log
  exit 1
fi

# Set file permissions for inetmonit
chmod 755 /usr/bin/inetmonit

# Copy inetmonit.service to /etc/systemd/system/
cp inetmonit.service /etc/systemd/system/

# Check if copy was successful
if [ $? -eq 0 ]; then
  echo "inetmonit.service successfully copied to /etc/systemd/system/"
else
  echo "Error: inetmonit.service was not copied to /etc/systemd/system/"  | tee -a errors-installer.log
  exit 1
fi

# Set file permissions for inetmonit.service
chmod 644 /etc/systemd/system/inetmonit.service

# Reload systemd
systemctl daemon-reload

# Enable and start the service
systemctl enable inetmonit
systemctl start inetmonit

if [ $? -eq 0 ]; then
  echo "inetmonit.service successfully started"
else
  echo "Error: inetmonit.service failed to start"  | tee -a errors-installer.log
  exit 1
fi

# Check file permissions for inetmonit
if [ $(stat -c %a /usr/bin/inetmonit) -eq 755 ]; then
    echo "Permissions for inetmonit set correctly"
else
    echo "Error: Failed to set permissions for inetmonit" | tee -a errors-installer.log
    exit 1
fi

# Check file permissions for inetmonit.service
if [ $(stat -c %a /etc/systemd/system/inetmonit.service) -eq 644 ]; then
    echo "Permissions for inetmonit.service set correctly"
else
    echo "Error: Failed to set permissions for inetmonit.service" | tee -a errors-installer.log
    exit 1
fi

echo "inetmonit installation and setup completed successfully"
