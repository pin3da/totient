Totient
=======

The idea behind this project is to share files quickly, for this task every file is split into little 'pieces'.

To keep information about this 'pieces', a special metadata is required. This metadata is not compliant with the bittorrent protocol, but is based on it; [Bittorrent Ref](http://bittorrent.org/beps/bep_0003.html).


## .totient description

Regular file that MUST have the following info. (Order is very important)

[line number] - desc
- [0] - announce: The URL of the tracker.
- [1] - name: Suggested name to save the file.
- [2] - piece_length: Number of bytes in each piece the file is split into.
- [3] - length: The length of the file, in bytes.
- [4 - N] - SHA1 message digest of each piece


**NOTES :**
- Unlike bittorrent's description, this metainfo file (.totient) just keeps track the info for *one* file, not a whole directory.

## Peer's description

Just another peer in the ~~wall~~ network, see the full description [here](http://github.com/pin3da/totient/tree/master/doc/peer.md)

## Tracker's description

At the first time you need to know who has the parts that interest you. This is the reason to introduce a "centralized" component called Tracker. See the full description [here](http://github.com/pin3da/totient/tree/master/doc/tracker.md)


## Communication

### Tracker request

Tracker requests MUST have the following parts:

- [0] - info_hash: The 40 byte sha1 hash of the part.
- [1] - ip: IP (or dns name) which this peer is at
- [2] - port: The port number this peer is listening on. Common behavior is for a downloader to try  to listen on port 6881 and if that port is taken try 6882, then 6883, etc. and give up after 6889.

### Tracker response

Tracker responses MUST have the following parts:
- [0] - peers_length: Number of peers.

for i in peers

- [1 + (3 * i)] - ip: IP address or dns name as a string
- [1 + (3 * i + 1)] - port: Port number


### Peer request

Peer request MUST have the following parts.

- [0] : "piece", word to identify the request kind.
- [1] : SHA1, SHA1 of piece.
- [2] : offset
- [3] : chunk_size

### Peer response

Peer response MUST have the followin parts.

- [0] : state, "OK" if the part was found, "NF" otherwise.
- [1] : if [0] is "OK" must have an array with the data.


## Sockets' definition



        __________
       | TRACKER  |
       |          |
       |          |
       |          |
       |_/|\____|_|
          |     |
      ROUTER   DEALER
       (bind)  (connect)
          |     |
          |     |
          |     |
          |     |
          |     |
          |     |
          |     |
      DEALER   ROUTER
    (connect)  (bind)
          |     |
        __|____ | _                          ___________
       |  |    \|/ |  DEALER        ROUTER  |   PEER    |
       |           | (connect)       (bind) |           |
       |         ----------------------------->         |
       |           |                        |           |
       |           |                        |           |
       | PEER    <----------------------------          |
       |__________ | ROUTER         DEALER  |___________|
                     (bund)        (connect)


