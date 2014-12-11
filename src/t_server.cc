#include <set>
#include <string>
#include <sstream>
#include <unordered_map>
#include <zmqpp/zmqpp.hpp>
#include "utils.cc"

using namespace std;
using namespace zmqpp;

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Usage " << argv[0] << " port " << endl;
    exit (1);
  }

  string port = argv[1];
  cout << string_color("Server running on port: " + port + "\n");

  context ctx;
  socket frontend(ctx, socket_type::router);
  frontend.bind("tcp://*:" + port);

  poller pol;
  pol.add(frontend);

  while (true) {
    if (pol.poll()) {
      if (pol.has_input(frontend)) {
        message request;
        frontend.receive(request);
        string id, filename;
        request >> id;
        request >> filename;
        cout << id << endl << filename << endl;
        if (filename == "new"){
          string data;
          request >> filename >> data;
          ofstream totient("./totient/" + filename + ".totient");
          totient << data;
          totient.close();
          message outmsg;
          outmsg << id << "OK";
          frontend.send(outmsg);
        } else{
          filename = "./totient/" + filename;
          message response;
          cout << "before" << endl;
          response << id;
          cout << "after" << endl;
          cout << "Received request : " << filename << endl;
          if (file_exists(filename)) {
            ifstream file;
            response << "OK";
            file.open(filename);
            stringstream buffer;
            buffer << file.rdbuf();
            response << buffer.str();
            file.close();
          } else {
            response << "NF";
          }
          frontend.send(response);
        }
      }
    }
  }

}
