#include <string>
#include <vector>

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
