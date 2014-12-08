Totient's Peer
==============


## Wake Up

When a peer arises it must report to the tracker about all pieces it has. For this task it reads the 'pieces/list' file, and for each line sends the following message to the tracker.

- [0] - add, reserved word to identify the request.
- [1] - SHA1, message digest of piece.
- [2] - ip
- [3] - port
