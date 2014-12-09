#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#define ADD "add"

enum COLOR { RED, GREEN, BLUE, GRAY};

std::string string_color(const std::string &s, COLOR color = GRAY) {
  std::string ret;
  if (color == RED)
    ret = "\033[0;31m";
  if (color == GREEN)
    ret = "\033[0;32m";
  if (color == BLUE)
    ret = "\033[0;34m";
  if (color == GRAY)
    ret = "\033[0;37m";

  return ret + s + "\033[0m";

}

bool file_exists(const std::string &filename) {
  std::cout << "Looking for " << filename << std::endl;
  std::ifstream ifile(filename, std::ifstream::binary);
  bool good = !ifile.fail();
  ifile.close();
  return good;
}
/*
namespace tracker {

  struct peer {
    std::string id, ip, port;
    peer() {}
    std::string encode() {}
    static peer decode(const std::string &benc) {}
  };

  struct request {
    std::string info_hash, peer_id, ip, port;
    std::string encode() {}
    static request decode(const std::string &benc) {}
  };

  struct response {
    int interval;
    std::vector<peer> peers;
    std::string encode() {}
    static request decode(const std::string &benc) {}
  };
}

namespace totient {

  struct metainfo {
    std::string announce, name, pieces;
    int piece_lenght, length;
    metainfo() {}
    std::string encode() {}
    static metainfo decode(const std::string &benc) {}
  };
}
*/
