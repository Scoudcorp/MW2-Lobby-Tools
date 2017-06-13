# MW2 Lobby Tools by Scoud

MW2 Lobby Tools (MW2LT) are a set of tools to enhance the MW2 lobby experience by giving power back to the host.
Also, this was my first cpp project ever, so the code could be much much much better.

## New (!)
My research (idb from ida) are now stored here. Feel free to use it. Note: some information in the IDBs can be wrong).

## Features
* **General**
  * Console unlock
  * Force host
* **Lobby**
  * Change map, gamemode and gamerules in public matches
  * Force start the game
  * Change intermission time
  * Toggle between private and public match
  * 18 players
* **Admin**
  * **Ingame menu**
    * Change configurations
    * Monitor clients
    * Kick clients
    * Tempban clients
    * Punish clients
  * **Enforcement**
    * Replace/strip/kill weapons and attachments
    * Replace/strip/kill perks
* **Ingame**
  * Killstreak notifications
  * Deathstreak notifications
  * 80fov mode
* **DVAR**
  * Patch `jump_height`, `bg_fallDamageMaxHeight` and `bg_fallDamageMinHeight`, `gravity`, `speed` so that you can be a sanic bunny again

## Usage
1. Compile (needs detours)
2. Start the latest steam version of `Call of Duty: Modern Warfare 2 - Multiplayer` (v1.2.211).
3. Inject the generated dll into `iw4mp.exe` using your favorite injector.
4. Enjoy
