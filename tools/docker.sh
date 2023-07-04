#!/usr/bin/bash

first=$1

echo "$first"

# Build image
if [ "$first" = "build" ]; then 
    docker build . -t pine:latest

# Run image
elif [ "$first" = "run" ]; then
    # Expose the X server to the host
    xhost +local:root
    # Run container
    docker run -it --privileged -e DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix \
        pine:latest 

# Compose
elif [ "$first" = "compose" ]; then
    docker-compose up   
else
    echo "no valid command provided"
fi
