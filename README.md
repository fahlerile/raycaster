# raycaster

My simple raycaster engine written in C.

# TODO

- Fix ray casting if player in square coordinates are not `(0.5, 0.5)`
    - Is my math wrong? Try to replace `(1 - in_sq_x)` with `in_sq_q`, just like in https://www.youtube.com/watch?v=eOCQfxRQ2pY video.
- Add Framerate-safe player movement
- Textures
- Make ray casting functions more simple (abstract away similar code blocks in `cast_horizontal_ray` and `cast_vertical_ray`)
- Create a simple game
