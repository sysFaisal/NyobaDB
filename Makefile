# =================================================================
# CONFIGURATION
# =================================================================
CC = gcc

MSYS_ROOT = C:/msys64/mingw64

SRC_DIR = src
OBJ_DIR = build\obj

CFLAGS = -Wall -Wextra -O2 \
    -I$(MSYS_ROOT)/include \
    -I$(MSYS_ROOT)/include/ncurses \
    -I$(SRC_DIR) \
    -I$(SRC_DIR)/c_db \
    -I$(SRC_DIR)/setup \
    -I$(SRC_DIR)/login \
    -I$(SRC_DIR)/ui

LIBS = -L$(MSYS_ROOT)/lib -lncursesw -lkernel32 -luser32 -lgdi32 -lwinmm -ladvapi32

TARGET = build\test.exe

OBJS = $(OBJ_DIR)\main.o \
       $(OBJ_DIR)\setup.o \
       $(OBJ_DIR)\login.o \
       $(OBJ_DIR)\ui.o \
       $(OBJ_DIR)\sqlite3.o

# =================================================================
# BUILD TARGETS
# =================================================================
all: prepare $(TARGET)

# Membuat folder build jika belum ada
prepare:
	if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"

# Proses Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# =================================================================
# RUN PROGRAM (di terminal VSCode)
# =================================================================
run: all
	@echo.
	@echo [MENJALANKAN PROGRAM...]
	"$(TARGET)"

# =================================================================
# COMPILE RULES
# =================================================================
$(OBJ_DIR)\main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)\setup.o: $(SRC_DIR)/setup/setup.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)\login.o: $(SRC_DIR)/login/login.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)\ui.o: $(SRC_DIR)/ui/ui.c
	$(CC) $(CFLAGS) -c $< -o $@

# SQLite3: compile hanya kalau belum ada
$(OBJ_DIR)\sqlite3.o: $(SRC_DIR)/c_db/sqlite3.c
	if not exist $(OBJ_DIR)\sqlite3.o $(CC) -O2 -I$(SRC_DIR)/c_db -c $< -o $@

# =================================================================
# CLEANUP
# =================================================================
clean:
	del /Q $(OBJ_DIR)\main.o 2>nul
	del /Q $(OBJ_DIR)\setup.o 2>nul
	del /Q $(OBJ_DIR)\login.o 2>nul
	del /Q $(OBJ_DIR)\ui.o 2>nul
	del /Q $(TARGET) 2>nul
	@echo Clean selesai, sqlite3.o tetap ada.

clean-all:
	if exist build rmdir /S /Q build
	@echo Semua folder build dihapus, termasuk sqlite3.o.

.PHONY: all prepare run clean clean-all
