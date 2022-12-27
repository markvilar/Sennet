# syntax=docker/dockerfile:1
FROM ubuntu:20.04

MAINTAINER markvilar EMAIL martin.kvisvik.larsen@hotmail.com


# Install dependencies
ENV DEBIAN_FRONTEND="noninteractive" TZ="Etc/UTC"
RUN apt update -y
RUN apt install -y \
    cmake \
    gcc \
    clang \
    python3-dev \
    python3-pip \ 
    pkg-config \
    libgl-dev \
    libx11-xcb-dev \
    libfontenc-dev \
    libice-dev \
    libsm-dev \
    libxaw7-dev \
    libxcomposite-dev \
    libxcursor-dev \
    libxdamage-dev \
    libxi-dev \
    libxinerama-dev \
    libxkbfile-dev \
    libxmuu-dev \
    libxrandr-dev \
    libxres-dev \
    libxss-dev \
    libxtst-dev \
    libxv-dev \
    libxvmc-dev \
    libxxf86vm-dev \
    libxcb-render0-dev \
    libxcb-render-util0-dev \
    libxcb-xkb-dev \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-keysyms1-dev \
    libxcb-randr0-dev \
    libxcb-shape0-dev \
    libxcb-sync-dev \
    libxcb-xfixes0-dev \
    libxcb-xinerama0-dev \
    libxcb-dri3-dev \
    uuid-dev \
    libxcb-util-dev \
    libxcb-util0-dev 

# Install conan
RUN pip3 install conan

# Set up conan profile for system requirement installation
RUN conan profile new sudo_install --detect
RUN conan profile update conf.tools.system.package_manager:mode=install \
    sudo_install
RUN conan profile update conf.tools.system.package_manager:sudo=True \
    sudo_install

# Copy source code and set up working directory
RUN mkdir /pine
COPY . /pine/
WORKDIR /pine

# Generate build for pine
RUN cd /pine
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build pine
RUN cmake --build build
