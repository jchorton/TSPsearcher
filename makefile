######################################################################
# cs325-400-W17	Analysis of Algorithms	04 March 2017	Project 4
# Jonathan Horton	hortonjo@oregonstate.edu
# Chris Kearns		kearnsc@oregonstate.edu
# Dustin Pack		packdu@oregonstate.edu
# File: makefile
# Description: makefile for chagneMaker project.
# Edit the variables under "USER SECTION" as needed.
# To execute the makefile on unix-y systems, type 'make all'.
######################################################################
CXX = g++
CXXFLAGS = -std=c++0x
CXXFLAGS += -Wall
CXXFLAGS += -g

####################
### USER SECTION ###
####################

# SRCS is a standard name for the source files (.cpp). 
SRC1 = search.cpp
#SRC2 = 
#SRC3 = 
#SRC4 = 
#SRC5 = 
#SRC6 = 
#SRC7 = 
#SRC8 = 
SRCS = ${SRC1}

# HEADERS is a standard name for the header files (.hpp). 
#HEADER1 = 
#HEADER2 = 
#HEADER3 = 
#HEADER4 = 
#HEADER5 = 
#HEADER6 = 
#HEADER7 = 
HEADERS = ${HEADER1}

# Your executable files. 
# Edit as needed.
PROG1 = tspSearch
# PROG2 = 
# PROG3 = 
PROGS = ${PROG1}

#####################
### BUILD SECTION ###
#####################
# Typing 'make' in terminal calls the first build available.
# In this case, default.

default:
	${CXX} ${CXXFLAGS} ${SRCS} ${HEADERS} -o ${PROG1}

# Typing 'make all' in terminal calls this build.
all:
	${CXX} ${CXXFLAGS} ${SRCS} ${HEADERS} -o ${PROG1}

# CLEAN
# Typing 'make clean' calls this build.
# It's designed to clean up the folder.
# Be careful with this, edit as needed.
# rm means remove file.
# -f means --force or you don't need access privelege to the file, only the parent directory and no "confirm remove" is offered.
# ${PROGS} means remove the executable file(s) specified above.
# *.o means remove the object files created by the compile process for linking.
# *~ means remove temp files. 
clean:
	rm -f ${PROGS} *.o *~
