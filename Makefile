TARGET_NAME = jpeg2000
PATH_BUILD = bin
PATH_SRC = src
PATH_OBJ = obj

LIBRARIES = 


PATH_INCLUDE =	-Iinclude

PATH_LIBRARIES = 

#lister les fichiers à compiler
OBJ_FILES = $(PATH_OBJ)/main.o \
			$(PATH_OBJ)/signal.o \
			$(PATH_OBJ)/signalprocessing.o

GCC_OPTS = -std=c++11 -Wall

.PHONY: all

all: $(TARGET_NAME)

$(TARGET_NAME): $(OBJ_FILES)
	g++ $(GCC_OPTS) -o $(PATH_BUILD)/$(TARGET_NAME) $(OBJ_FILES) $(PATH_LIBRARIES) $(LIBRARIES)

$(PATH_OBJ)/%.o: $(PATH_SRC)/%.cpp
	@echo "Compiling $<"
	g++ $(GCC_OPTS) -c $< -o $@ $(PATH_INCLUDE)


clean:
	rm -rf $(PATH_OBJ)/*.o