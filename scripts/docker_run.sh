#!/usr/bin/bash

# Expose the X server to the host
xhost +local:root

# Run container
docker run \
    -it --privileged \
    -e DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    pine:latest 
