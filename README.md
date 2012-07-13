rgbd2blend is a simple bridge between [RGBDtoolkit](http://rgbdtoolkit.com/) and [Blender](http://www.blender.org/). 

building requires [openframeworks](https://github.com/openframeworks/openFrameworks) and [ofxRGBDepth](https://github.com/obviousjim/),
using requires a kinect, RGBDtoolkit, blender, and the [blender-meshfoot script](http://blenderartists.org/forum/showthread.php?252844-Script-Meshfoot-OBJ-Sequence-Manager-%28For-Blender-2.6.2%29). 

record with your DSLR+kinect using RGBDtoolkit
convert by dragging a group of pngs from the depth folder created by RGBDtoolkit into the app window
the .obj sequence will be output to /tmp/

texturing isnt yet supported in the converter, but the point is to just do it in blender.
video tutorial coming soon..
   

