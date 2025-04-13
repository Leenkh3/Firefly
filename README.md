# Firefly

- this repository is for the team project of production software engineering course

## Dependencies

The Firefly project requires several libraries and tools to build properly. 

### Required Dependencies
- C/C++ compiler (GCC 7+ or equivalent)
- CMake 3.15+
- Lua 5.3+ (with development libraries)
- zlib (development version)
- m4 macro processor

### Optional Dependencies
- Fortran compiler (for certain scientific components)
- Google Test (for running tests - automatically downloaded)

### Installing Dependencies

#### Automatic Installation (Linux/macOS)

We provide a script to automatically install all required dependencies:

```bash
# Make the script executable
chmod +x scripts/install_dependencies.sh

# Run the script
./scripts/install_dependencies.sh
```

#### Manual Installation

Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential cmake liblua5.3-dev libgtest-dev m4 zlib1g-dev gfortran
```

Fedora/RHEL/CentOS:
```bash
sudo dnf install cmake gcc gcc-c++ gcc-gfortran lua-devel gtest-devel m4 zlib-devel
```

Arch Linux:
```bash
sudo pacman -S cmake gcc gfortran lua gtest m4 zlib
```

macOS:
```bash
brew update
brew install cmake lua googletest m4 zlib gcc
```

### Checking Dependencies

To check if you have all required dependencies installed:

```bash
./scripts/check_dependencies.sh
```

## Build Instructions

### Building with CMake
```bash
# Create a build directory
mkdir -p build && cd build

# Configure the project
cmake ..

# Build
make
```

### Building with specific options
```bash
# Skip building tests
cmake -DFIREFLY_BUILD_TESTS=OFF ..

# Skip building Exodus (for faster builds during development)
cmake -DFIREFLY_SKIP_EXODUS=ON ..

# Skip dependencies that require Fortran
cmake -DFIREFLY_SKIP_NETCDF=ON -DFIREFLY_SKIP_EXODUS=ON ..

# Specify Lua location if it's installed in a non-standard location
cmake -DLUA_INCLUDE_DIR=/path/to/lua/include -DLUA_LIBRARIES=/path/to/lua/lib/liblua.so ..
```

## Contributors:

- Martin Takács
- Miklós Vajkai
- Aili Torvinen
- Zoltán Kaszai
- Leen Alkhouri
- József Bakosi
- Dániel Doma
- Ali Hasan
- Hana Bejaoui
- Husayn Zaweedi
- Erkin Bektenov
- Sahir Kaju
- Aqib Shahzad
- Obay Daoudi


#### To compile the C program with Lua support:

```fish
gcc user_input.c -o fileprogram -llua -lm -ldl
```

OR

```
gcc -std=c99 -o user_input user_input.c -llua -lm -ldl
```
