FROM ubuntu:22.04

RUN apt-get update  &&  \
    apt-get install -y  \
                        \
    g++-10              \
    clang++-12          \
                        \
    git                 \
    ninja-build         \
    make                \
    cmake &&            \
    pip install Jinja

WORKDIR /tmp