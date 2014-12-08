#include <bits/stdc++.h>
#include <zmqpp/zmqpp.hpp>
#include "utils.cc"

using namespace std;
using namespace zmqpp;

unordered_map<string, set<pair<string, string>>> parts;

void add_rem_peer (message &incmsg, bool add = true){
  string ip, port, sha;
  int sha_number;
  
  incmsg >> ip;
  incmsg >> port;
  incmsg >> sha_number;
  
  while(sha_number > 0){
    incmsg >> sha;
    if (add)
      parts[sha].insert({ip, port});
    else
      parts[sha].erase({ip, port});
    sha_number--;  
  }
  
}

void search_part(message &incmsg){
  string sha, ip, port;
  incmsg >> sha;
  incmsg >> ip;
  incmsg >> port;
  
  
  context ctx;
  string temp_peer_endpoint = "tcp://" + ip + ":" + port;
  socket temp_peer(ctx, socket_type::dealer);
  temp_peer.connect(temp_peer_endpoint);
  
  message outmsg;  
  
  if (parts[sha].size() > 0){
    outmsg << parts[sha].size();
    for(set<pair<string, string>>::iterator it = parts[sha].begin(); it != parts[sha].end(); ++it){
      outmsg << it->first;
      outmsg << it->second;
    }    
  } else {
    outmsg << "NF";
  }
  
  temp_peer.send(outmsg);
}

void dispatch_peer(message &incmsg){
  string id, command;
  incmsg >> id;
  incmsg >> command;
  
  if (command == "add") {
    add_rem_peer(incmsg);
  } else if (command == "rem") { //Losing my religion
    add_rem_peer (incmsg, false);
  } else if (command == "search"){
    search_part (incmsg);
  }
}


int main(){
  const string peer_endpoint = "tcp://*:6667";
  context ctx;
  
  socket peers(ctx, socket_type::router);
  peers.bind(peer_endpoint);
  
  poller pol;  
  pol.add(peers);
  
  message incmsg;
  
  while (true){
    if (pol.poll()){
      if (pol.has_input(peers)){
        peers.receive(incmsg);
        dispatch_peer(incmsg);
      }
    }
  }
  
  return 0;
}


