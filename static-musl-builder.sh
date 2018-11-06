#!/bin/bash

export WORKDIR=$(pwd)

docker run --rm -it -v "${PWD}":"${WORKDIR}" radupopescu/musl-builder \
    sh -c "cd ${WORKDIR} && mkdir build-musl && cd build-musl && cmake -D CMAKE_EXE_LINKER_FLAGS=\"-static\" ../ && make && chown -R $(id -u):$(id -g) "$WORKDIR"/build-musl"
