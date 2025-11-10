# Super Mario Galaxy Remake

## Stages

Stage types:
- normal stages (get the star)
- Luma stages
- comet stages

    * timer comets (get the star before the timer ends)
    * purple coins comet (collect all purple coins)
    * daredevil comet (one health)

Comet stages can be **combined** (daredevil and timer for example)

Load, unload objects in a stage, restart the stage -> reload all objects (some might have disappeared).

Should have only one player and one star (displayed or not) in the stage.

## Scenes

3D Objects -> can collide with player, hitbox on each object
Special objects can change the directional velocities of the player, or trigger actions
    -> bounce on enemies, reduce/increase player health, take the star. 

2D Objects -> displayed as billboards (always in front of the player)
    => black holes for example

Skybox -> always displayed in front of the player as a gigantic untouchable sphere.

## Head-up display

2D Information to display to the real player:

- current coins / starbits (stage ones if a stage is loaded, total ones if in hubworld)

    * always displayed

- player health
    
    * should be displayed if the player is not at full health or just went back to full health.

- player lives

    * should be displayed if the player didn't move intentionally.

- special comet data (time / purple coins)

    * always displayed

## Special Objects

- Pull stars
- Purple coins
- Timer extender
- Black holes
- Level stars

## Gravity

- Gravity box on planets
    
    * attraction force (direction, magnitude)
    * pull radius/box

- Gravity box on black holes

    * pull radius/box
    * initial tangential velocity
    * number of spirals
    * event horizon radius of black holes
