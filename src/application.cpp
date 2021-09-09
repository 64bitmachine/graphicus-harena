#include<iostream>
#include<contextmanager.h>
#include<preprocessing.h>

int main()
{
    cg::ContextManager cm;
    cm.init();

    cg::initGLEW();
    cg::initCallBacks(&cm);

    cg::printGraphicsDetails();
    glViewport(0, 0, cm.getWidth(), cm.getHeight());

    while(!glfwWindowShouldClose(cm.getContext()))
    {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(cm.getContext());
    }

    glfwTerminate();

    return 0;
}