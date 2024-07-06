# I2C Userspace Linux Library
# Author: Giuseppe Giglio <g.giglio001@gmail.com>
# This makefile is used to compile the library and a test program contained in /examples.
# The library is compiled as both a static and shared library
# while the test program is compiled as an executable.

# Define the compiler toolchain (use CROSS_COMPILE variable to specify the toolchain)
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -Werror -std=gnu99 -g

# Define the library name
LIB_NAME = libi2c_userspace

# Define the library source files folder
LIB_SRC_DIR = src

# Define the library header files folder
LIB_INC_DIR = include

# Define the library object files folder
LIB_OBJ_DIR = obj

# Define the static library output folder
LIB_STATIC_DIR = build/static

# Define the shared library output folder
LIB_SHARED_DIR = build/shared

# Define the test program output folder
LIB_TEST_DIR = build/test

# Define the test program source files folder
TEST_SRC_DIR = examples/PCA9865_I2CReadWrite

# Define the library source files
LIB_SRC = $(wildcard $(LIB_SRC_DIR)/*.c)

# Define the library object files
LIB_OBJ = $(patsubst $(LIB_SRC_DIR)/%.c, $(LIB_OBJ_DIR)/%.o, $(LIB_SRC))

#define compilation targets
all: static shared

# Compile the library as a static library
static:
	@mkdir -p $(LIB_OBJ_DIR)
	@mkdir -p $(LIB_STATIC_DIR)
	$(CC) $(CFLAGS) -c $(LIB_SRC) -I$(LIB_INC_DIR) -o $(LIB_OBJ)
	ar rcs $(LIB_STATIC_DIR)/$(LIB_NAME).a $(LIB_OBJ)

# Compile the library as a shared library
shared:
	@mkdir -p $(LIB_OBJ_DIR)
	@mkdir -p $(LIB_SHARED_DIR)
	$(CC) $(CFLAGS) -fPIC -c $(LIB_SRC) -I$(LIB_INC_DIR) -o $(LIB_OBJ)
	$(CC) -shared -o $(LIB_SHARED_DIR)/$(LIB_NAME).so $(LIB_OBJ)

clean:
	rm -rf $(LIB_OBJ_DIR) $(LIB_STATIC_DIR) $(LIB_SHARED_DIR) $(LIB_TEST_DIR)
