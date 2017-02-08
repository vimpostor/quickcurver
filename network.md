This file documents, how network is handled internally in Quickcurver and how far the implementation progressed.

# Client

## General
- [ ] At each and every state the client will properly handle not getting any answer back by timing out after 5 seconds
- [ ] A closed socket error will cause the client to leave as well

When the user entered the ip and port in the Join dialog, the client tries joining the server:

## Joining
- [x] Try establishing a TCP connection to the server
- [x] If the client may join, the server answers with "[ACCEPTED]" on TCP and the client has to finally send "[JOIN]" on UDP
- [x] If the client may not join, the server answers with "[REJECTED]" on TCP and the client will leave
- [x] If "[JOIN]" on UDP reaches the server, the server answers with "[JOINED]" on UDP, which is proof for the client that he finally successfully joined
- [ ] The client then transmits his settings (such as username etc) to the server using TCP
- [ ] The client will immediately get settings back from the server (TCP), which have to be handled

Once the client is joined, the client has to wait for the server to start the game:

## Waiting for the beginning of the game
- [ ] Since other players may join during the waiting time, the server may respond at any time with new settings (TCP). The client has to handle these messages
- [x] Once the server starts the game, the client receives a "[STARTED]" message over TCP, which will cause the client to open the game page and end the waiting phase

When the server started the game, the client will have to be able to handle live game data:

## During the game
### Game data
- [x] Depending on what direction is pressed, the client will send "[LEFT]", "[RIGHT]" or "[NONE]" to the server (UDP) as soon as the direction changes
- [x] The client will periodically get the update of the position of a curver over UDP with the header "HEAD" and handle it
- [x] The client will periodically get updates of curver segment data with the header "POS" over UDP and handle it
- [ ] The client will have to be able to handle incoming "ITEM" messages which contain the position of new items

### Chat messages
- [x] The client may send chat messages to the server (TCP) at any time with the header "[MESSAGE]" followed by the username and the message
- [x] The client may receive chat messages from the server (TCP) at any time with header "[MESSAGE]" followed by the username and the message which have to be displayed

### Leaving
- [ ] The server may close the game at any time, but will gracefully send a "[TERMINATE]" message over TCP to the client causing the client to leave the game
- [ ] The server may kick the client at any time, which will trigger a "[KICKED]" message over TCP causing the client to leave the game
- [ ] The client may leave the game at any time, but will gracefully send a "[LEFT]" message over TCP to the server

# Server
tbd...
