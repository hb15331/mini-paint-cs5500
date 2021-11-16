# wxWidgets

For this year, I am also opening up the project to users who want to use wxWidgets with their projects.

You *can* embed SFML within a wxWidgets application, but that will take some work as shown here: https://www.walknsqualk.com/post/011-sfml-embedded-in-wxWidgets/

Otherwise I recommend you can more simply have a separate wxWidget window running alongside your SFML application in a separate background thread.

wxWidgets is a good example of taking a 'large' component that you have not written, and having to learn how to use it by reading online documentation, samples, and the source code. :)

## Helpful resources

* wxWidgets website 
	* [https://www.wxwidgets.org/](https://www.wxwidgets.org/)
* My Youtube Series on wxWidgets
	* [My series on wxWidgets](https://www.youtube.com/playlist?list=PLvv0ScY6vfd9QZOUbQ2YMghuxWnFoWuiU)
* Series of videos on wxWidgets Programming
	* [Cross Platform Graphical User Interfaces in C++](https://www.youtube.com/watch?v=FOIbK4bJKS8)
	* [More Cross Platform Graphical User Interfaces in C++: Custom Controls](https://www.youtube.com/watch?v=FwUGeV2fnfM)

## Description

wxWidgets is a professional GUI library. You can use this instead of nuklear GUI this year. I have included some instructions and a simple example for Linux users to get started. Because this is an experiment this year to include wxWidgets, I may provide more resources as the semester proceeds.

## Compilation

### Linux Users

Linux users are likely to have to do something like this to build the sample.

```sh
g++ sfml.cpp -o prog `./wx-config --cppflags --libs` -lsfml-system -lsfml-graphics -lsfml-window
```
or
```sh
g++ sfml.cpp -o prog `./wx-config --cppflags --libs` `pkg-config --cflags --libs gtk+-3.0` -lsfml-system -lsfml-graphics -lsfml-window
```

You may also need to install some libraries and dependencies like gtk+ `sudo apt-get install libgtk-3-dev`

### Your task

0. Modify or create a build script to compile for your platform. 
	- Note: I have provided a linux example above which you can modify.
	- Eventually you should incorporate this build into your final project in your CMakeLists.txt
1. Add a color selector such that the GUI can be used to change the SFML window background.
3. (Optional) Cleanup the code to provide a cleaner abstraction for using the GUI.
	- My sample code is from the SFML hello world and the wxWidgets hello world examples. Start by splitting them into separate files.

## Testing

- Make sure every time you change a color the change is reflected on the SFML window.

# Submission/Deliverables

- Task 1
  - A modified [main.cpp](./main.cpp)
	
### Submission

- Commit all of your files to github, including any additional files you create.
- Do not commit any binary files unless told to do so.
- Do not commit any 'data' files generated when executing a binary.

# Rubric

You (and any partner(s)) will receive the same grade from a scale of 0-2. Both partners should commit the same code.

- 0 for no work completed by the deadline
- 1 for some work completed, but something is not working properly
- 2 for a completed lab (with possible 'going further' options completed)

# Going Further

An optional task(if any) that will reinforce your learning throughout the semester--this is not graded.

1. Try exploring the other widgets available.

# F.A.Q. (Instructor Anticipated Questions)

1. Q: wxWidgets is not working on mac/windows/linux?
	- A See if you can build the files from the included script.
