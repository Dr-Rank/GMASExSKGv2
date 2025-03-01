# Basic Motion Matching / Motion Warping Example Plugin

To use this, check the repository out into the `Plugins` directory of the GMAS + GMCEx template project. Once you've either loaded the template project or created a project from it, go to the `/Plugins/GMCE_Motion/Maps` folder and open `MotionMatchTestMap`; if you don't see the plugins contents, you may need to go to the settings for your Content Browser and select "Show Plugin Content" in the options.

**You must use the `motion-matching` branch of the template, currently.** This requires a number of changes -- both to the default template and to GMAS and GMCEx -- which have not been fully vetted and brought live yet.

Other than the new Motion Animation Component in GMCEx, the interesting stuff is probably in `/Plugins/GMCE_Motion/Blueprints/SBP_Traversal_Base`; almost all of the traversal logic is there, save for the actual "find things in the environment" logic, which is done via the two subclasses.

Not everything in this project is 100% clean; the turn-in-place logic could be improved, the motion matching animation blueprint C++ parent class could be tidied up considerably, etc. At present, if the turn-in-place logic borrowed from GASP is being janky, just aim (right mouse button or left gameplay trigger) to force the character to re-align with the capsule before traversing. I'll probably change it later to force a turn-in-place before traversing at some point, but that can also be an exercise for the curious here.

And I think it's enough that folks can start poking at it, and tinker with improvements on their own. :)