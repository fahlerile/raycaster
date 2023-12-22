# raycaster

A raycaster engine written in C. End goal - a simple game.

#

## TODO
- [x] Add player and map construction
- [x] Implement `readMapFromFile` (`.map` file parser)
- [ ] Fix horizontal raycasting for rays facing -Y
- [ ] Fix vertical raycasting for rays facing -X
- [ ] Refactor/etc. the `.map` file parser
    - [ ] Do not crash if invalid input
    - [ ] Refactor?
- [ ] Implement `castAndDrawRays` for "air" and colored `Block`'s
- [ ] Add event reaction (`updateContext`) (movement, look around, etc.)
- [ ] Add textured `Block`'s

