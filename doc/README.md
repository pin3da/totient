Totient
=======

The idea behind this project is to share files quickly, for this task every file is split into little 'pieces'.

To keep information about this 'pieces', a special metadata is required. This metadata is not fully compliant with the bittorrent protocol, but is based on it; [Bittorrent Ref](http://bittorrent.org/beps/bep_0003.html).

### Bencoding

- Strings are length-prefixed base ten followed by a colon and the string. For example 4:spam corresponds to 'spam'.

- Integers are represented by an 'i' followed by the number in base 10 followed by an 'e'. For example i3e corresponds
to 3 and i-3e corresponds to -3. Integers have no size limitation. i-0e is invalid. All encodings with a leading zero,
such as i03e, are invalid, other than i0e, which of course corresponds to 0.

- Lists are encoded as an 'l' followed by their elements (also bencoded) followed by an 'e'. For example l4:spam4:eggse
corresponds to ['spam', 'eggs'].

- Dictionaries are encoded as a 'd' followed by a list of alternating keys and their corresponding values followed by an 'e'.
For example, d3:cow3:moo4:spam4:eggse corresponds to {'cow': 'moo', 'spam': 'eggs'} and d4:spaml1:a1:bee corresponds to
 {'spam': ['a', 'b']}. Keys must be strings and appear in sorted order (sorted as raw strings, not alphanumerics).

## .totient description

Is a bencoding dictionary with the following keys

```json
{
  "announce": "The URL of the tracker.",
  "info": {
    "name": "Suggested name to save the file.",
    "piece_length": "Number of bytes in each piece the file is split into.",
    "length": "The length of the file, in bytes.",
    "pieces": "String whose length is a multiple of 20. It is to be subdivided
      into strings of length 20, each of which is the SHA1 hash of the piece at
      the corresponding index."
  }
}
```

**NOTES :**
- Unlike bittorrent's description, this metainfo file (.totient) just keeps track the info for *one* file, not a whole directory.
- We are NOT using JSON notation for the metainfo file, is just to show where the keys are placed.

## Peer's description

Just another peer in the ~~wall~~ network, see the full description [here](http://github.com/pin3da/totient/tree/master/doc/peer.md)

## Tracker's description

At the first time you need to know who has the parts that interest you. This is the reason to introduce a "centralized" component called Tracker. See the full description [here](http://github.com/pin3da/totient/tree/master/doc/tracker.md)


## Communication

### Tracker request

Tracker requests are a bencoded dictionary that MUST have the following keys:

```json
{
  "info_hash":  "The 20 byte sha1 hash of the bencoded form of the info value from the metainfo file.",
  "peer_id": "A string of length 20 which this downloader uses as its id. Each downloader generates
    its own id at random at the start of a new download.",
  "ip": "An optional parameter giving the IP (or dns name) which this peer is at",
  "port": "The port number this peer is listening on. Common behavior is for a downloader to try
    to listen on port 6881 and if that port is taken try 6882, then 6883, etc. and give up after 6889.",
}
```
### Tracker response

Tracker responses are a bencoded dictionary that MUST have the following keys:

```json
{
  "interval": "Number of seconds the downloader should wait between regular rerequests",
  "peers": [
    {
      "id": "peer's self-selected ID",
      "ip": "IP address or dns name as a string",
      "port": "Port number"
    }
  ]
}
```

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

