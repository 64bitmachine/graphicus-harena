# graphicus-harena

<!-- install -->
## Install / Setup

```bash
sudo apt-get install libglfw3-dev
sudo apt-get install libglew-dev
sudo apt-get install mesa-utils
sudo apt-get install libglm-dev
sudo apt-get install cmake
```

<!-- faq -->
## FAQ
**Q:** which is better glfw3 or freeglut for graphics programming ?

**A:** Both GLFW3 and freeglut are libraries that can be used for graphics programming. Which one is better for you will depend on your specific needs and preferences.

GLFW3 is a modern, cross-platform library for creating windows and contexts for OpenGL graphics. It is focused on providing a simple, low-level API for creating windows and contexts, and for handling user input.

Freeglut, on the other hand, is an open-source alternative to the OpenGL Utility Toolkit (GLUT) library. It provides a similar API for creating windows and contexts for OpenGL graphics, but it also includes additional features such as support for multiple windows, support for stereo rendering, and support for the creation of pop-up menus.

In general, if you are looking for a simple, low-level library for creating windows and contexts for OpenGL graphics, GLFW3 may be a good choice. If you need additional features such as support for multiple windows or pop-up menus, freeglut may be a better option. Ultimately, the best choice will depend on your specific needs and preferences.

**Q:** which is better glew or glad for graphics programming ?

**A:** Both GLEW and GLAD are OpenGL extension loaders, which means they help with loading OpenGL functions that may not be supported by the graphics driver. In terms of performance, GLEW is known to be faster and more efficient, but GLAD is easier to use and offers more features, such as the ability to generate custom extension loaders. Ultimately, the choice between GLEW and GLAD depends on the specific needs and preferences of the developer.
