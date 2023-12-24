# raycaster

A raycaster engine written in C. End goal - a simple game.

## `.map` files

Uses 0-based indexing everywhere. Consists of 4 sections:
- Width, height, player's default values
- Textures
    - `path/to/texture`
- Blocks
    - `BlockTypeAir`
        - `0` (SHOULD BE IN EVERY MAP AS FIRST BLOCK!)
    - `BlockTypeColored`
        - `1 red green blue`
    - `BlockTypeTextured`
        - `2 texture_index`
- Map
    - Comma-separated indices of blocks

Example `.map` file:
```
MAP
map_width map_height player_default_x player_default_y player_default_see_angle
#textures
path/to/texture_0
...
path/to/texture_n
#blocks
0
1 red green blue
2 texture_index
#map
block_index,block_index,...,
...
...
```

## TODO
- [x] Add player and map construction
- [x] Implement `readMapFromFile` (`.map` file parser)
- [x] Implement `castAndDrawRays` for "air" and colored `Block`'s
- [x] Fix horizontal raycasting for rays facing -Y
- [x] Fix vertical raycasting for rays facing -X
- [x] Refactor raycasting functions
    - Splitted something in separate functions
    - Declared a lot of things `static`
- [x] Add textured `Block`'s rendering
- [x] ~~Refactor/etc. the `.map` file parser~~
- [x] Add event reaction (`updateContext`) (movement, look around, etc.)
- [x] Fix bugs
    - [x] Incorrect rendering of textures if too close
    - [x] Fix visual bug when too small angles
    - [x] Random segfaults (I guess I fixed them)
- [x] Add documentation
- [x] Fix SEGV bug at angles close to 0/2pi
- [x] Simultaneous movement (e.g. A+W (forward and left) at the same time)
- [x] Change camera controls from mouse to keyboard (arrows)
- [x] Fix yet another SEGV (probably)
- [ ] Textured floor/ceiling
- [ ] Entities

