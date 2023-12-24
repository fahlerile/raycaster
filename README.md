# raycaster

A raycaster engine written in C. End goal - a simple game.

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
- [ ] Add documents
- [ ] Textured floor/ceiling
- [ ] Entities

