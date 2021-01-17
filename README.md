Group Project: 
I had (Treavor Gagne) a big hand in programming the game mechanics and designing the
overall system architecture. Created map, players, hit detection, spawning, shooting,
movements, and debugging various portions of the game. Used C++ to write modular code
of classes to make the game extensible for further content in the future.


Description:

TDR will be a 2 dimensional top down point of view battle royale shooter.  TDRwill 
be a no time limit game with 2 to 150 player lobbies with the winner beingthe last 
player standing. All players will have the same weapon with infiniteammo, fire rate 
and bullet speed, so all players are on equal power levels forthe entire game. The
game map is a square arena with various walls to providecover from enemy bullets. At
the start of the game players will be spawned across the board randomly, but away from 
one another to prevent spawn kills. Overall, TDR strives to be a game where skill is
the biggest factor to winning and losing.

When using the client, players will have a small field of vision around them instead
of the entire map. As such, when a player moves around, so does their field of vision
of the map to find and eliminate other players. 

The game server will be managed via a command line for simplicity, and so that it can 
be run on a headless server. The client will be interacted with via a graphical interface 
and will provide a menu where players can enter an IPaddress and port to connect to.

The game will use 2 network ports. The first port is a TCP port, and will be used for 
important info such as game synchronization (waiting for players to join and starting 
the game), and communication of player inputs.  On the serverthis port will be static 
and set through a configuration file.

The second port is a UDP port opened on each client. The port will be chosenrandomly 
on each client and passed to the server via the TCP connection. The server will store
these ports with  each connection object and use them whensending all game information
to clients such as their current position, health,and other player positions and bullets. 
Using UDP allows for more frequentupdates and faster response times because there is no 
overhead to managing theconnection. The server will send various pieces of information 
about the game to all clients, at specified intervals depending on how important the 
information is. Bullet and wall collisions are the most important, so they must be sent 
often.

MEMBERS: 
Dana Bell (V00832577)
James Lacroix (V00897338)
Treavor Gagne (V00890643)
