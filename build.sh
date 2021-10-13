#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd ${DIR}

function corecount {
    getconf _NPROCESSORS_ONLN 2>/dev/null || sysctl -n hw.ncpu
}

source build.options

GLFW_CMAKE_ARGS="                        \
-DCMAKE_BUILD_TYPE=Debug               \
-DGLFW_BUILD_DOCS=False                  \
-DGLFW_BUILD_TESTS=False                 \
-DGLFW_BUILD_EXAMPLES=False              \
-DCMAKE_INSTALL_PREFIX=${DIR}/glfw/build "

if ! [ -z ${wayland} ]; then
    GLFW_CMAKE_ARGS+="-DGLFW_USE_WAYLAND=True "
fi

if ! [ -d glfw ]; then
    git clone https://github.com/kammerdienerb/glfw
    cd glfw
    mkdir -p build
    rm -rf build/*
    cd build
    cmake .. ${GLFW_CMAKE_ARGS}
    make -j$(corecount)
    make install
fi

cd ${DIR}

echo "Compiling leaf.."

CFG="-g -O0"
# CFG="-O3"
gcc -Wall -Iglfw/build/include -c src/internal.c     ${CFG} -o src/internal.o     &
gcc -Wall -Iglfw/build/include -c src/video.c        ${CFG} -o src/video.o        &
gcc -Wall -Iglfw/build/include -c src/input.c        ${CFG} -o src/input.o        &
gcc -Wall -Iglfw/build/include -c src/options.c      ${CFG} -o src/options.o      &
gcc -Wall -Iglfw/build/include -c src/font.c         ${CFG} -o src/font.o         &
gcc -Wall -Iglfw/build/include -c src/leaf.c         ${CFG} -o src/leaf.o         &
wait

LD_FLAGS="-lGL -Lglfw/build/lib -lglfw3 -lm -lpthread -ldl "

if ! [ -z ${wayland} ]; then
    LD_FLAGS+="-lwayland-client -lwayland-cursor -lwayland-egl "
fi

gcc src/*.o ${LD_FLAGS} -o leaf

rm -f src/*.o

echo "Done."
