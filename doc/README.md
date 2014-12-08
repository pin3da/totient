Totient
=======

The idea behind this project is to share files quickly, for this task every file is split into little 'pieces'.

To keep information about this 'pieces', a special metadata is required. This metadata is not fully compliant with the bittorrent protocol, but is based on it [Bittorrent Ref](http://bittorrent.org/beps/bep_0003.html).


## .totient description

- **name** : Suggested name to save the file.
- **piece_length** : Number of bytes in each piece the file is split into.
- **length** : The length of the file, in bytes.
- **pieces** : String whose length is a multiple of 20. It is to be subdivided into strings of length 20, each of which is the SHA1 hash of the piece at the corresponding index.

**NOTE :** Unlike bittorrent's description, this metainfo file (.totient) just keeps track the info for *one* file, not a whole
directory.

## Node's description

Works as a peer in the network, see the full description [here](http://github.com/pin3da/totient/tree/master/doc/node.md)

## Tracker's description

At the first time you need to know who has the parts that interest you. This is the reason to introduce a "centralized" component called Tracker. See the full description [here](http://github.com/pin3da/totient/tree/master/doc/tracker.md)
