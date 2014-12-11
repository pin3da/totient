#include <bits/stdc++.h>
#include <zmqpp/zmqpp.hpp>
#include "utils.cc"

using namespace std;
using namespace zmqpp;

unordered_map<string, set<pair<string, string>>> parts;

void add_rem_peer(message &request, bool add = true) {
  string ip, port, sha;
  size_t sha_number;

  request >> ip >> port >> sha_number;

  cout << "parts : " << sha_number << endl;
  while (sha_number-- > 0) {
    request >> sha;
    if (add) {
      cout << string_color("Added part " + sha) << endl;
      parts[sha].insert({ip, port});
    } else {
      cout << string_color("Deleted part " + sha) << endl;
      parts[sha].erase({ip, port});
    }
  }

}

void search_part(context &ctx, message &request) {
  string sha, ip, port;

  request >> sha >> ip >> port;

  cout << string_color("Search request : " + sha + " from " + ip + " on " + port) << endl;
  string temp_peer_endpoint = "tcp://" + ip + ":" + port;
  socket temp_peer(ctx, socket_type::dealer);
  temp_peer.connect(temp_peer_endpoint);

  message response;

  cout << string_color("Num of peers : " + to_string(parts[sha].size())) << endl;
  response << SEARCH << sha << parts[sha].size();
  for (auto it : parts[sha])
    response << it.first << it.second;

  temp_peer.send(response);
  cout << string_color("Sent message to " + temp_peer_endpoint) << endl;
  temp_peer.disconnect(temp_peer_endpoint);
}

void dispatch_peer(context &ctx, message &request) {
  string id, command;
  request >> id >> command;

  if (command == ADD) {
    add_rem_peer(request);
  } else if (command == REM) { //Losing my religion
    add_rem_peer (request, false);
  } else if (command == SEARCH){
    search_part (ctx, request);
  }
}


int main(int argc, char ** argv) {

  if (argc < 2) {
    cout << "Usage : " << argv[0] << " port" << endl;
    exit(1);
  }

  const string port = argv[1];
  const string peer_endpoint = "tcp://*:" + port;
  cout << string_color("Running tracker at port " + port) << endl;

  context ctx;

  socket peers(ctx, socket_type::router);
  peers.bind(peer_endpoint);

  poller pol;
  pol.add(peers);

  message request;

  while (true){
    if (pol.poll()){
      if (pol.has_input(peers)){
        peers.receive(request);
        dispatch_peer(ctx, request);
      }
    }
  }

  return 0;
}


