# BATTLESHIPS MULTIPLAYER
## A Game by Tharen Candi and Rafael Riber
## EE-310 - Fall Semester 2022-2023

### Gameplay
Warning: the game cannot start properly until a WiFi hotspot with SSID "MES-NDS" without encryption is available. This is indicated by the "Loading WIFI" prompt on the homescreen.
On launch, choose if you would like to host a game or join a game. Either the touchscreen to select, or press A to join or B to host.
Each player then places their ships, moving them with the keypad and rotating them with the B key. Press A to confirm.

Once the game starts, each player can place a shot at the target location by pressing A.
If there is ever a packet drop after placing a shot, the text at the top of the player's gameboard will indicate it, and the START key can be pressed to retry sending the shot message to the enemy. 
On very rare occasions there may be too much packet loss and the game must be restarted. This is inherent to UDP.

On game over, a text indicates if you have won or lost and the game statistics are updated. After a brief moment, you automatically return to the homescreen, ready to start a new game whenever you like !

#### Note: Tested on the latest version of libnds as of Dec. 2022
