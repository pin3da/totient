#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <set>

using namespace std;
using namespace zmqpp;

#define PIECES_PATH  "./pieces/list"

void wake_up() {
  set<string> pieces;
  ifstream i_file(PIECES_PATH);
  string line;
  while (getline(i_file, line)) {
    pieces.insert(line);
  }

  remove(PIECES_PATH);

  ofstream o_file(PIECES_PATH);

  for (string piece : pieces)
    o_file << piece << endl;

  i_file.close();
  o_file.close();


}

int main(int argc, char **argv) {
  wake_up();

  return 0;
}
