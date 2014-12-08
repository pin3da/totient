Totient's Tracker
=================


### Requests

#### ADD

Adds a part (or parts) if it doesn't exist, and adds the sepecified peer to that part (or parts).

- [0] - "add" reserved word to identify the request.
- [1] - ip.
- [2] - port.
- [3] - number of pieces.
- [4 - N] - message digest for each piece.

#### Remove

Removes a peer from an specifical part (or parts), needed when the peer decides to leave the network.

- [0] - "rem", a reserved word.
- [1] - ip.
- [2] - port.
- [3] - Number of pieces.
- [4 - N] - message digest for each piece.
