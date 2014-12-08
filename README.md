
Totient
=======

Totient is a peer-to-peer file sharing project based on [bittorrent](http://www.bittorrent.com/),
is built using [ZeroMQ](http://zeromq.org/) to connect things and [ZMQPP](https://github.com/zeromq/zmqpp) to make this easier.

The project just have two components, [nodes](http://github.com/pin3da/totient/tree/master/doc/node.md) and
[trackers](http://github.com/pin3da/totient/tree/master/doc/node.md). Nodes and trackers are connected in a
non-restricted way, so if you want to connect to the Totient cloud you just need to know one tracker's address.


## Dependencies
- [Install ZeroMQ](http://zeromq.org/intro:get-the-software)
- [Install ZMQPP](https://github.com/zeromq/zmqpp)



## Compile

    # To compile nodes and trackers
    make
    # To compile nodes
    make node
    # To compile trackers
    make trackers

## Clean

    make clean

## Documentation

See the full documentation at [doc](https://github.com/pin3da/totient/tree/master/doc)

## Contribute

Feel free to fork, modify and send us a pull request. We will respond as soon as possible.

______

Manuel Pineda - Carlos González.
