alias c := config
alias b := build
alias r := run

config:
    CC=gcc meson setup build .
    ln -sf build/compile_commands.json ./compile_commands.json

build:
    ninja -C build

run ex:
    build/{{ ex }}

clean:
    rm -fr build
