#!/bin/bash

# Set colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Checking Firefly dependencies...${NC}"
echo ""

# Check for CMake
echo -n "CMake: "
if command -v cmake >/dev/null 2>&1; then
    VERSION=$(cmake --version | head -n1 | cut -d" " -f3)
    echo -e "${GREEN}Found v$VERSION${NC}"
else
    echo -e "${RED}Not found${NC}"
fi

# Check for C/C++ compiler
echo -n "GCC: "
if command -v gcc >/dev/null 2>&1; then
    VERSION=$(gcc --version | head -n1 | grep -o -E '[0-9]+\.[0-9]+\.[0-9]+')
    echo -e "${GREEN}Found v$VERSION${NC}"
else
    echo -e "${RED}Not found${NC}"
fi

# Check for Fortran compiler
echo -n "Fortran: "
if command -v gfortran >/dev/null 2>&1; then
    VERSION=$(gfortran --version | head -n1 | grep -o -E '[0-9]+\.[0-9]+\.[0-9]+')
    echo -e "${GREEN}Found v$VERSION${NC}"
else
    echo -e "${RED}Not found${NC}"
fi

# Check for Lua
echo -n "Lua: "
if command -v lua >/dev/null 2>&1; then
    VERSION=$(lua -v | cut -d" " -f2)
    echo -e "${GREEN}Found v$VERSION${NC}"
else
    echo -e "${RED}Not found${NC}"
fi

# Check for Lua development files
echo -n "Lua dev headers: "
if [ -f /usr/include/lua.h ] || [ -f /usr/include/lua5.3/lua.h ] || [ -f /usr/local/include/lua.h ] || [ -f /usr/local/include/lua5.3/lua.h ]; then
    echo -e "${GREEN}Found${NC}"
else
    echo -e "${RED}Not found${NC}"
fi

# Check for m4
echo -n "m4: "
if command -v m4 >/dev/null 2>&1; then
    VERSION=$(m4 --version | head -n1 | cut -d" " -f4)
    echo -e "${GREEN}Found v$VERSION${NC}"
else
    echo -e "${RED}Not found${NC}"
fi

# Check for zlib
echo -n "zlib dev: "
# This is a simple check - could be improved
if [ -f /usr/include/zlib.h ] || [ -f /usr/local/include/zlib.h ]; then
    echo -e "${GREEN}Found${NC}"
else
    echo -e "${RED}Not found${NC}"
fi

echo ""
echo -e "${YELLOW}Summary:${NC}"
echo "If any dependencies are missing, run the install_dependencies.sh script:"
echo "  bash scripts/install_dependencies.sh"
echo ""
echo "For more detailed information, see the README.md"