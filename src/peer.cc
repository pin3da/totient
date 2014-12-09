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

// Begin peer state

string address, port, tracker_ip, tracker_port;

// End peer state

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
  request << ADD << address << port << pieces.size();
  for (string piece : pieces) {
    o_file << piece << endl;
    request << piece;
  }

  tracker.send(request);

  i_file.close();
  o_file.close();
}

bool  share_file(socket &tracker, string &filename) {
  filename = "files/" + filename;
  if (!file_exists(filename))
    return false;

  string command = "./totient_generator.sh " + filename + " " + tracker_ip + " " + tracker_port;
  cerr << string_color(command, BLUE) << endl;
  system(command.c_str());

  ifstream totient_file(filename + ".totient");
  string tracker_url, name;
  int piece_length, length;
  totient_file >> tracker_url >> name >> piece_length >> length;
  size_t num_parts = (length + piece_length - 1 ) / piece_length;

  message request;

  request << ADD << address << port << num_parts;

  string part;
  while (totient_file >> part) {
    request << part;
  }

  tracker.send(request);

  return true;
}

void download_file(socket &tracker, const string &filename) {

}

void play_song(const string &filename) {

}

int main(int argc, char **argv) {

  if (argc < 5) {
    cout << "Usage: " << argv[0] << "address port tracker_ip tracker_port";
    exit(1);
  }

  address = argv[1];
  port    = argv[2];
  tracker_ip = argv[3];
  tracker_port = argv[4];
  const string tracker_endpoint = string("tcp://") + tracker_ip + ":" + tracker_port;

  context ctx;
  socket tracker(ctx, socket_type::dealer);
  tracker.connect(tracker_endpoint);

  wake_up(tracker);

  while (true) {
    string command;

    cout << "Totient P2P file sharing." << endl;
    cin >> command;

    if ((command == "q") or (command == "quit"))
      break;

    string filename;
    if (command == "share") {
      cout << "Enter the name of the file that you want to share (must be in the files dir)" << endl;
      cin >> filename;
      if (share_file(tracker, filename))
        cout << string_color("The file was successfully shared", GREEN) << endl;
      else
        cout << string_color("The file does not exist", RED) << endl;
    } else if (command == "download") {
      cout << "Enter the name of the file that you want to download (must be in the totient dir)" << endl;
      cin >> filename;
      download_file(tracker, filename);
    } else if (command == "play") {
      cout << "Enter the name of the file that you want to hear (must bein the files dir)" << endl;
      cin >> filename;
      play_song(filename);
    }
  }

  cout << "Bye bye" << endl;

  return 0;
}
