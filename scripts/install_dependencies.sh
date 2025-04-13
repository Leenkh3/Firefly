#!/bin/bash

# Make script exit on first error
set -e

# Detect OS
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
elif [ -f /etc/debian_version ]; then
    OS="debian"
elif [ -f /etc/redhat-release ]; then
    OS="rhel"
elif [ "$(uname)" == "Darwin" ]; then
    OS="macos"
else
    OS="unknown"
fi

echo "Detected OS: $OS"

# Install dependencies based on OS
case $OS in
    ubuntu|debian)
        echo "Installing dependencies for Debian/Ubuntu..."
        sudo apt-get update
        sudo apt-get install -y \
            build-essential \
            cmake \
            liblua5.3-dev \
            libgtest-dev \
            m4 \
            zlib1g-dev \
            gfortran
        ;;
    fedora|rhel|centos)
        echo "Installing dependencies for Fedora/RHEL/CentOS..."
        sudo dnf install -y \
            cmake \
            gcc \
            gcc-c++ \
            gcc-gfortran \
            lua-devel \
            gtest-devel \
            m4 \
            zlib-devel
        ;;
    arch|manjaro)
        echo "Installing dependencies for Arch Linux..."
        sudo pacman -S --needed \
            cmake \
            gcc \
            gfortran \
            lua \
            gtest \
            m4 \
            zlib
        ;;
    macos)
        echo "Installing dependencies for macOS..."
        brew update
        brew install \
            cmake \
            lua \
            googletest \
            m4 \
            zlib \
            gcc
        ;;
    *)
        echo "Unsupported OS. Please install the following dependencies manually:"
        echo "- CMake (3.15+)"
        echo "- C and C++ compiler"
        echo "- Fortran compiler (gfortran)"
        echo "- Lua development libraries"
        echo "- Google Test"
        echo "- m4 macro processor"
        echo "- zlib development libraries"
        exit 1
        ;;
esac

echo "All dependencies installed successfully!"