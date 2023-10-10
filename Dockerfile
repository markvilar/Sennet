# syntax=docker/dockerfile:1
FROM ubuntu:20.04

MAINTAINER markvilar EMAIL martin.kvisvik.larsen@hotmail.com

# Environment variables
ENV DEBIAN_FRONTEND="noninteractive" TZ="Etc/UTC"

# Install system dependencies
RUN apt update -y --fix-missing
RUN apt install -y \
    sudo \
    cmake \
    gcc-10 \
    g++-10 \
    clang-10 \
    clang-12 \
    ninja-build \
    python3-dev \
    python3-pip \
    pkg-config 

# Install conan
RUN pip3 install conan

# Copy source code and set up working directory
RUN mkdir /pine
COPY . /pine/
WORKDIR /pine
RUN cd /pine

# Create default profile
RUN conan profile detect

# Install dependencies and invoke build
RUN conan build . --profile ./conan/profiles/linux_ninja_clang-12 --build missing
