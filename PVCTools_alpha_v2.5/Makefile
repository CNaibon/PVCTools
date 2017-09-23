DIR_INC = ./include
DIR_HEAD_SRC = ./src/headnode
DIR_CHILD_SRC = ./src/childnode
DIR_HEAD_OBJ = ./obj/headnode
DIR_CHILD_OBJ = ./obj/childnode
DIR_BIN = ./bin

HEAD_SRC = $(wildcard ${DIR_HEAD_SRC}/*.cpp)
HEAD_OBJ = $(patsubst %.cpp,${DIR_HEAD_OBJ}/%.o,$(notdir ${HEAD_SRC}))
CHILD_SRC = $(wildcard ${DIR_CHILD_SRC}/*.cpp)
CHILD_OBJ = $(patsubst %.cpp,${DIR_CHILD_OBJ}/%.o,$(notdir ${CHILD_SRC}))

HEAD_TARGET = PCVTools
CHILD_TARGET = $(DIR_BIN)/main
TARGET = $(HEAD_TARGET) $(CHILD_TARGET)

CC = g++
CFLAGS = -g -Wall -fopenmp -I${DIR_INC} -MMD -MT $@ -MF $@.d

all: $(TARGET)
	@echo "--------The installation has been completed!---------"

${HEAD_TARGET}:${HEAD_OBJ}
	$(CC) -fopenmp -MMD -MT $@ -MF $@.d $(HEAD_OBJ)  -o $@

${DIR_HEAD_OBJ}/%.o:${DIR_HEAD_SRC}/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

${CHILD_TARGET}:${CHILD_OBJ}
	$(CC) -fopenmp -MMD -MT $@ -MF $@.d $(CHILD_OBJ)  -o $@

${DIR_CHILD_OBJ}/%.o:${DIR_CHILD_SRC}/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

.PHONY:clean
clean:
	rm -fr $(DIR_HEAD_OBJ)/*.o
	rm -fr $(DIR_CHILD_OBJ)/*.o