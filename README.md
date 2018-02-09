# Omicron
The Omicron c++ engine, successor to the Argent java engine

## Features
* Designed for Cross-platform usage
* Multi-threaded Entity-Component System
* Shader-based material system
* Worlds defined in XML file
  * Defines skybox cubemap textures, and the day-night colour map
  * Defines the systems enabled in a given world
  * Can define a template entity that can be used when defining entities
    * When using a template, any definition within the entity will take precidence over the definition in the template
  * Components can be added to entities with ease
* Lua integration
* OVR support
* Event-bus system to allow for cross-thread communication
* Extensible input system
* Dynamic lighting (No shadows yet)
