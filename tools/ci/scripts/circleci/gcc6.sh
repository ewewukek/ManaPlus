#!/bin/bash

echo "CI nodes: ${CIRCLE_NODE_TOTAL}"
echo "Current node: ${CIRCLE_NODE_INDEX}"

case $CIRCLE_NODE_INDEX in
    0)
        export IMAGE=i386/debian:stretch
        export COMMAND="./tools/ci/jobs/gcc6.sh --enable-werror --without-opengl --with-sdl2"
        export PACKAGES="gcc-6 g++-6 ${LIBS_PACKAGES} ${SDL2}"
        ./tools/ci/scripts/docker_run.sh
    ;;
    1)
        export IMAGE=debian:stretch
        export COMMAND="./tools/ci/jobs/gcc6.sh --enable-werror --without-opengl"
        export PACKAGES="gcc-6 g++-6 ${LIBS_PACKAGES} ${SDL1}"
        ./tools/ci/scripts/docker_run.sh
    ;;
    2)
        export IMAGE=i386/debian:stretch
        export COMMAND="./tools/ci/jobs/gcc6.sh --enable-werror --without-opengl"
        export PACKAGES="gcc-6 g++-6 ${LIBS_PACKAGES} ${SDL1}"
        ./tools/ci/scripts/docker_run.sh
    ;;
    3)
        export IMAGE=debian:stretch
        export COMMAND="./tools/ci/jobs/gcc6.sh --enable-werror --without-opengl --with-sdl2"
        export PACKAGES="gcc-6 g++-6 ${LIBS_PACKAGES} ${SDL2}"
        ./tools/ci/scripts/docker_run.sh
    ;;
    *)
        echo unknown node
        exit 1
    ;;
esac
