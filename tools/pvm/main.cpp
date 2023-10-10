#include <iostream>

#include <CLI/CLI.hpp>

int main(int argc, char **argv) {
  CLI::App app{"PlumbusVM"};
  CLI11_PARSE(app, argc, argv);
  return 0;
}
