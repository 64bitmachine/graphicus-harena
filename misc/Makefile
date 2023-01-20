# library
OPENGL_LIBRARY 	= -lGL
GLEW_LIBRARY 	= -lGLEW
GLFW_LIBRARY 	= -lglfw
PTHREAD_LIBRARY = -lpthread
LIBS = $(OPENGL_LIBRARY) $(GLEW_LIBRARY) $(GLFW_LIBRARY) $(PTHREAD_LIBRARY)

# flags
LDFLAGS		= -L/usr/local/lib
CPPFLAGS 	= -g -std=c++0x -I/usr/local/include -I./include

# application
BIN = bin/application

# header files and source files
INCLUDE_FILES	=	contextmanager.h
SRC_FILES		= 	application.cpp contextmanager.cpp

# header folder and source folder
INCLUDE		= $(addprefix include/,$(INCLUDE_FILES))
SRC			= $(addprefix src/,$(SRC_FILES))

# build the application
all: $(BIN)

$(BIN): $(SRC) $(INCLUDE)
	g++ $(CPPFLAGS) $(SRC) -o $(BIN) $(LDFLAGS) $(LIBS)

# clean the application and object files
clean:
	rm -f *.o $(BIN)