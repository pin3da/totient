Totient's Peer
==============


## Wake Up

When a peer arises it must report to the tracker about all pieces it has. For this task it reads the 'pieces/list' file
and sends an [ADD request](https://github.com/pin3da/totient/blob/master/doc/tracker.md#add) to the tracker, this add request contains all the parts contained in the 'pieces/list' file, this is asumed to be a case when the peer got disconnected after being sharing something and decided to reconnect.

## Sharing a File

When a peer decides to share a file it most be previously placed in the 'files' folder, after that you must specify explicitly that you want to share that specifical file, this will call a bash script that will 'slice and dice' the files, add it to the 'files folder' as well as the list file in the same folder, and sends the report of the new parts to the tracker, it also creates .totient file and sends it to the .totient tracker (not yet).

The identity is managed by zmq so it will be automatically appended as a message arrives.

## Leaving the network

You can explicitly tell a peer to leave the network, which it will do after finishing downloading the current part (if not it will leave immediately) sending a [Remove request](https://github.com/pin3da/totient/blob/master/doc/tracker.md#remove) to the tracker.

#Requests

## Sending a part

The only petitions peers take care of is to send a specifical part to a specifical peer (lazy bastards), in order to this the message sent from the other peer MUST contain:

- [0] - The keyword 'send'.
- [1] - The SHA of the part it needs.




