# Use an Ubuntu base image
FROM ubuntu:22.04

# Set noninteractive mode to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt update && apt install -y \
    software-properties-common \
    wget \
    cmake \
    make \
    g++ \
    clang \
    git \
    openmpi-bin \
    libopenmpi-dev \
    libhdf5-dev \
    libcurl4-openssl-dev \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# Install the latest CMake (>= 3.23.0)
RUN wget -qO- "https://github.com/Kitware/CMake/releases/download/v4.0.0-rc5/cmake-4.0.0-rc5-linux-x86_64.sh" -O /tmp/cmake.sh \
    && chmod +x /tmp/cmake.sh \
    && /tmp/cmake.sh --prefix=/usr/local --skip-license \
    && rm -f /tmp/cmake.sh

# Verify CMake version
RUN cmake --version

# Set OpenMPI environment variables
ENV PATH="/usr/lib/openmpi/bin:$PATH"
#ENV LD_LIBRARY_PATH="/usr/lib/openmpi/lib:$LD_LIBRARY_PATH"

# Set working directory
WORKDIR /workspace

# Install NetCDF-C
RUN git clone --depth=1 https://github.com/Unidata/netcdf-c.git /opt/netcdf-c \
    && cd /opt/netcdf-c \
    && mkdir build && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local \
    && make -j$(nproc) && make install \
    && rm -rf /opt/netcdf-c

# Install SEACAS
RUN git clone --depth=1 https://github.com/sandialabs/seacas.git /opt/seacas \
    && cd /opt/seacas \
    && mkdir build && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local \
    && make -j$(nproc) && make install \
    && rm -rf /opt/seacas

# Default entrypoint
CMD ["/bin/bash"]
