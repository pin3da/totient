#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <thread>
#include <set>
#include <SFML/Audio.hpp>
#include "utils.cc"

using namespace std;
using namespace zmqpp;

#define PIECES_PATH  "./pieces/list"


string address, port, tracker_ip, tracker_port;


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
  filename = "./files/" + filename;
  if (!file_exists(filename))
    return false;

  string command = "./totient_generator.sh " + filename + " " + tracker_ip + " " + tracker_port;
  cerr << string_color(command, BLUE) << endl;
  system(command.c_str());


  totient::entry totient_file(filename + ".totient");

  message request;

  request << ADD << address << port << totient_file.pieces.size();

  for (size_t i = 0; i < totient_file.pieces.size(); ++i)
    request << totient_file.pieces[i];

  tracker.send(request);

  return true;
}

bool download_file(socket &tracker, unordered_map<string, totient::entry> &downloads, string &filename,
      socket &download_t) {
  filename = "./totient/" + filename + ".totient";
  if (!file_exists(filename))
    return false;

  if (downloads.count(filename) == 0) {
    totient::entry entry(filename);
    downloads[filename] = entry;
  }

  message request;
  request << "push";

  download_t.send(request);

  return true;
}

void play_song(const string &filename) {

}

void download_thread(void * _ctx) {
  context *ctx= (context *)_ctx;
  socket cli(*(ctx), socket_type::dealer);
  cli.connect("inproc://download");

  while (true) {
    message request;
    cli.receive(request);
    string command;
    request >> command;
    if (command == "quit")
      break;

    else {
      cout << string_color("@@@ Received request in download thread", GREEN) << endl;
    }
  }
}

void play_thread(void *_ctx){
  sf::Music music;
  vector<string> playlist;
  bool playflag = true;

  context *ctx = (context*)_ctx;
  socket cli(*(ctx), socket_type::dealer);
  cli.connect("inproc://playlist");
  size_t pos = 0;

  poller pol;
  pol.add(cli);
  while (true) {
    if (playlist.size() > 0 and pos < playlist.size()) {
      string name = playlist[pos];
      if (music.getStatus() == 0 and music.openFromFile("files/" + name) and playflag){
        music.play();
        pos++;
      }
    }

    if (pol.poll(100)) {
      if (pol.has_input(cli)) {
        message incmsg;
        cli.receive(incmsg);
        string command;
        incmsg >> command;
        if (command == "quit")
          break;
        if (command == "add") {
          string filename;
          incmsg >> filename;
          playlist.push_back(filename);
        } else if (command == "next") {
          music.stop();
        } else if (command == "prev") {
          pos = pos - 2;
          if (pos < 0)
            pos = 0;
          music.stop();
        } else if (command == "stop" and playflag) {
          playflag = false;
          pos--;
          if (pos < 0)
            pos = 0;
          music.stop();
        } else if(command == "play" and not playflag) {
          playflag = "true";
        } else if(command == "del" and playlist.size() > 0) {
          pos--;
          if (pos < 0)
            pos = 0;
          playlist.erase(playlist.begin() + pos);
          music.stop();
        } else if(command == "pause" and playflag) {
          music.pause();
        } else if(command == "play" and playflag and playlist.size() > 0) {
          music.play();
        }
      }
    }
  }
}

int main(int argc, char **argv) {

  if (argc < 5) {
    cout << "Usage: " << argv[0] << "address port tracker_ip tracker_port" << endl;
    exit(1);
  }

  cout << string_color(string("Running peer at ") + argv[1] + " on port " + argv[2]) << endl;
  cout << string_color(string("Default tracker at ") + argv[3] + " on port " + argv[4]) << endl;


  address = argv[1];
  port    = argv[2];
  tracker_ip = argv[3];
  tracker_port = argv[4];
  const string tracker_endpoint = string("tcp://") + tracker_ip + ":" + tracker_port;

  context ctx;
  socket tracker(ctx, socket_type::dealer);
  tracker.connect(tracker_endpoint);

  wake_up(tracker);


  // Peer state
  unordered_map<string, totient::entry> downloads;
  // End peer state

  socket download_t(ctx, socket_type::dealer);
  download_t.bind("inproc://download");
  socket playlist_t(ctx, socket_type::dealer);
  playlist_t.bind("inproc://playlist");

  thread download_task(download_thread, (void *) &ctx);
  thread playlist_task(play_thread, (void *) &ctx);

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
      if (download_file(tracker, downloads, filename, download_t))
        cout << string_color("Download in process", GREEN) << endl;
      else
        cout << string_color("The file does not exist", RED) << endl;
    } else if (command == "add") {
      cout << "Enter the name of the file that you want to hear (must be in the files dir)" << endl;
      cin >> filename;
      message p_command;
      p_command << command << filename;
      playlist_t.send(p_command);
    } else if (command == "next" or "prev" or "stop" or "play" or "del" or "pause"){
      message p_command;
      p_command << command;
      playlist_t.send(p_command);
    }
  }


  message request;
  request << "quit";
  download_t.send(request);
  request << "quit";
  playlist_t.send(request);

  download_task.join();
  playlist_task.join();

  cout << "Bye bye" << endl;

  return 0;
}
