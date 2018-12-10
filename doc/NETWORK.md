# Quick Curver - Multiplayer - Specification

## Abstract

This document defines the Quick Curver multiplayer.
It describes how multiple instances of Quick Curver communicate in order
to provide a fluent online multiplayer.

## Introduction

Quick Curver uses TCP and UDP to communicate with different instances.
TCP is used for most packet types, but UDP is used for the broadcasting
of Curver data due to performance reasons.

## Conventions and Definitions

The words "MUST", "MUST NOT", "SHOULD", "REQUIRED", "RECOMMENDED",
"OPTIONAL" and "MAY" are used in this document. These keywords are to be
understood as defined in [RFC2119].

Client: The instance instantiating a Quick Curver connection request.

Server: The instance listening for incoming connection requests.

DISCONNECT: The event on closing the TCP connection between server and
client.

## Overview

The actual game is run on the server. All logic is computed on the
server and on the server only. The client only sends keyboard inputs to
the server, which makes it impossible to cheat as long as the server
runs a sane version of Quick Curver.
The client gets back all visual data, which is computed by the server,
which includes data for curvers, item positions etc...

An online game is opened from an initial local game and only then a
server is started. The server listens for incoming client connections
and adds players to the local running game on demand by its own.

A client requests a connection simply by sending a TCP connection
request to the server. If the server accepts the connection, the client
must send a Ping packet via UDP to the server. The server replies with a
Pong packet and only then the player has successfully joined the game.

Any party may at any time close the TCP socket, which closes the session
for the client.

If the server receives a DISCONNECT from the client, this is interpreted
as the client purposefully leaving the game. The client may at any time
rejoin the game.

If the client receives a DISCONNECT from the server, this is interpreted
as the server purposefully kicking the client from the game. The client
MAY try to immediately reconnect, however the server MAY as well block
future incoming connections from the client, if it did in fact
purposefully kick the client.

Once a stable connection has been established, there are different
phases of the game:

* Waiting for the game to start
* The actual game being played

Server and client may exchange chat messages at any time.
Chat messages are sent by either instance to a server. The server then
distributes the chat message to every client instance.

## Packet Types

Quick Curver uses a common packet type across all packets, that
encapsulates all packets in one common format.

The format of all packets is defined by the following outline: Each
packet begins with the following 1-byte header:

```
 0               1
 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|T|T|T|U|U|U|R|S|    .....
-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
```

The first three bits denote the packet type (T). The next 5 bits define
special flags. The following table defines how the flags are interpreted:

```
------------------
| Flag | Meaning |
|----------------|
|   S  | Start   |
|   R  | Reset   |
|   U  | Unused  |
------------------
```

The interpretation of the type information depends on whether the packet
was sent by a server or a client. The fact whether the sender is a
server or a client is simply deduced by the fact whether the instance
itself is a server or a client. The following table defines how the type
information is interpreted:

```
-------------------------------------------------------
| Type | Binary | Sent from server | Sent from client |
|-----------------------------------------------------|
|   0  |  000   | Chat Message     | Chat Message     |
|   1  |  001   | Playermodel Edit | Playermodel Edit |
|   2  |  010   | Curver Data      | Curver Rotation  |
|   3  |  011   | Item Data        | Ping             |
|   4  |  100   | Settings         | ---------------- |
|   5  |  101   | Pong             | ---------------- |
-------------------------------------------------------
```

The following section defines every single packet by its own.
Each packet already comes with the general 1-byte header at the very
beginning. The following only describes what comes after the first byte.

### Chat Message from server

Username and message as QString.

### Chat Message from client
The client simply sends the message as a serialized QString. The server
knows the username of the client already, so there is no need to send
the username.

### Playermodel Edit from server

The server sends every setting bundled to every client.
This means in particular that it first sends the number of players as an
unsigned integer, and after that for each player the following properties (in
that order):

* Username
* Color
* Round score
* Total score
* Controller
* Alive

### Playermodel Edit from client

The player sends any change to the server and the server determines
what curver the player belongs to and changes the settings accordingly.
After that, the server sends back the new settings to all clients.
The settings that the client sends are (in this order):

* Username
* Color

### Curver Data from server

First the id of the curver that the segment belongs to, then a new
segment flag (set or unset), then the amount of points, then the segment data.

### Curver Rotation from client

The client sends the new rotation direction of the curver that is
controlled by this client. A 1-byte integer is sufficient for this
purpose, so an uint8_t is simply serialized, where the different values
are interpreted as follows:

```
--------------------
| Value | Rotation |
|------------------|
|    0  |    Left  |
|    1  |    None  |
|    2  |   Right  |
--------------------
```

Any other value than the above mentioned is undefined behaviour.

### Item Data from server

The server sends the following data (in this order):

* spawned (Item spawned or triggered)
* Sequence Number (unsigned int)
* Index of the kind of item
* Position as QPointF
* Allowed users as uint8_t
* index of collector as int (if spawned, this must be -1)

### Ping packet from client

The client sends the current time.

### Settings from server

The server sends the following data:

* Game dimension as QPoint

### Pong packet from client

The server sends the current time as received from the corresponding
Ping packet.
