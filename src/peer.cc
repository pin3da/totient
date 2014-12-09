#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <set>

#include "utils.cc"

using namespace std;
using namespace zmqpp;

#define PIECES_PATH  "./pieces/list"

void wake_up(socket &tracker) {
  set<string> pieces;
  ifstream i_file(PIECES_PATH);
  string line;
  while (getline(i_file, line)) {
    pieces.insert(line);
  }

  remove(PIECES_PATH);

  ofstream o_file(PIECES_PATH);

  message request;
  request << ADD << "localhost" << "6668" << pieces.size();
  for (string piece : pieces) {
    o_file << piece << endl;
    request << piece;
  }

  tracker.send(request);

  i_file.close();
  o_file.close();


}

int main(int argc, char **argv) {

  if (argc < 3) {
    cout << "Usage: " << argv[0] << " tracker_ip tracker_port";
    exit(1);
  }

  const string tracker_endpoint = string("tcp://") + argv[1] + ":" + argv[2];

  context ctx;
  socket tracker(ctx, socket_type::dealer);
  tracker.connect(tracker_endpoint);

  wake_up(tracker);

  return 0;
}
