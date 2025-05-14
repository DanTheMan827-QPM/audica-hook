# Audica Hook

A significantly stripped down version of the beatsaber-hook library for Audica on the Quest.

This is a conversion / update of the [original package](https://github.com/sc2ad/audica-hook) by [sc2ad](https://github.com/sc2ad) for use in the QPM package ecosystem.

- `qpm s build` to build.
- `qpm s clean` to build with a clean build path.
- `qpm s qmod` to package qmod.
- `qpm s copy` to copy the mod to the headset and (re)start the game with logging.
- `qpm s deepclean` to clean all artifacts and downloaded dependencies from the project directory.

## Acknowledgements

- [sc2ad](https://github.com/sc2ad): Wrote the original [audica-hook](https://github.com/sc2ad/audica-hook)

This wouldn't have been possible without a few people who have helped immensely.

- [emulamer](https://github.com/emulamer/): Provided support, tested, and in general just a great person to talk to. This wouldn't exist at all if not for him motivating me to keep working.
- [jakibaki](https://github.com/jakibaki/): Created the library that actually supports all of the mod loading, as well as a lot of support when developing (and bearing with my annoying questions).
