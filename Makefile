CC = gcc

SRC_DIR = src
OBJ_DIR = build/obj
PDC_DIR = PDCurses-3.9

CFLAGS = -Wall -Wextra -O2 \
	-I$(SRC_DIR) \
	-I$(SRC_DIR)/c_db \
	-I$(SRC_DIR)/setup \
	-I$(SRC_DIR)/login \
	-I$(SRC_DIR)/ui \
	-I$(PDC_DIR)

TARGET = test.exe

# Object files
MAIN_OBJ   = $(OBJ_DIR)/main.o
SETUP_OBJ  = $(OBJ_DIR)/setup.o
LOGIN_OBJ  = $(OBJ_DIR)/login.o
UI_OBJ     = $(OBJ_DIR)/ui.o
SQLITE_OBJ = $(OBJ_DIR)/sqlite3.o

PDC_LIB = $(PDC_DIR)/wincon/pdcurses.a

# =====================
# BUILD TARGET
# =====================
all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(SETUP_OBJ) $(LOGIN_OBJ) $(UI_OBJ) $(SQLITE_OBJ)
	$(CC) $^ $(PDC_LIB) -o $@

# =====================
# COMPILE RULES
# =====================

# main.c
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# setup.c
$(OBJ_DIR)/setup.o: $(SRC_DIR)/setup/setup.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# login.c
$(OBJ_DIR)/login.o: $(SRC_DIR)/login/login.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ui.c
$(OBJ_DIR)/ui.o: $(SRC_DIR)/ui/ui.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# sqlite3.c
$(OBJ_DIR)/sqlite3.o: $(SRC_DIR)/c_db/sqlite3.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# =====================
# CLEAN
# =====================
clean:
	@if exist "$(OBJ_DIR)\main.o" del /Q "$(OBJ_DIR)\main.o"
	@if exist "$(OBJ_DIR)\setup.o" del /Q "$(OBJ_DIR)\setup.o"
	@if exist "$(OBJ_DIR)\login.o" del /Q "$(OBJ_DIR)\login.o"
	@if exist "$(OBJ_DIR)\ui.o" del /Q "$(OBJ_DIR)\ui.o"
	@if exist "$(OBJ_DIR)\sqlite3.o" del /Q "$(OBJ_DIR)\sqlite3.o"
	@if exist "$(TARGET)" del /Q "$(TARGET)"

clean-all:
	@if exist "$(OBJ_DIR)" del /Q "$(OBJ_DIR)\*.o"
	@if exist "$(TARGET)" del /Q "$(TARGET)"
