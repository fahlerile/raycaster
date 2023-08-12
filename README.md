# raycaster

My simple raycaster engine written in C.

# TODO

- Update `utils.h:is_oob` macro to fully replace `if` statements in ray casting functions (so no `||` there, only `is_oob`)
- Hide 2D map (show it if M was pressed, add ability to turn this feature off if `DEBUG` macros is not defined)
- Create a simple game
    - Ability to move between "floors" (maps)
    - Add enemies
    - Add GUI
    - Add guns/swords/something to kill enemies
    - Create a couple of levels (maximum five)
    - Some final boss?
    - Congratulation screen for the player
