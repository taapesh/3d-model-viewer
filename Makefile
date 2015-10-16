# Name: Arya Mohades
# UTEID: am52293
# UTCS ID: am611

# Makefile for

all: modelviewer.cpp
	g++ -std=c++11 -O2 -o modelviewer modelviewer.cpp -lGL -lGLU -lglut

clean:
	$(RM) modelviewer