
## The Rolling Dice Game

> Advanced System Programming Project - University of Windsor

 **Multiplayer dice game based on client-server architecture.**

## Run Locally

 1. Clone the repository: 
`git clone https://github.com/nihalmpatel/rolling-dice-game`

2. cd into project directory:
`cd rolling-dice-game`

3. Open terminal and run **server** program (server.c):
`gcc server.c -o server`
`./server <<PORT_NO>>`

4. Open terminal and run **client** program (client.c):
`gcc client.c -o client`
`./client <<SERVER_IP>> <<SERVER_PORT_NO>>`

**Preview:**
![preview](https://nihalpatel.com/images/project/dice-game.png)