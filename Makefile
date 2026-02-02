CC      := gcc
# Menambahkan -DNCURSES_STATIC untuk stabilitas di Windows
CFLAGS  := -Wall -Wextra -DNCURSES_STATIC -I./src -I./src/c_db -I./src/controller -I./src/login -I./src/setup -I./src/ui
LDFLAGS := -lsqlite3 -lncursesw

TARGET  := build/test.exe
OBJDIR  := build/obj
SRCDIR  := src

SRC := src/main.c src/controller/controller.c src/login/login.c src/setup/setup.c src/ui/ui.c
OBJ := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
SQLITE_OBJ := $(OBJDIR)/sqlite3.o

FIX_PATH = $(subst /,\,$1)

all: $(TARGET)

$(TARGET): $(OBJ) $(SQLITE_OBJ)
	@echo [LINKING] Membuat file eksekusi...
	@if not exist "build" mkdir "build"
	@$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo [COMPILING] $<...
	@if not exist "$(call FIX_PATH,$(dir $@))" mkdir "$(call FIX_PATH,$(dir $@))"
	@$(CC) $(CFLAGS) -c $< -o $@

$(SQLITE_OBJ): src/c_db/sqlite3.c
	@echo [WAITS] Mengompilasi SQLite3 (Sabar, ini lama)...
	@if not exist "$(OBJDIR)" mkdir "$(OBJDIR)"
	@$(CC) $(CFLAGS) -c $< -o $@

run: all
	@echo [RUNNING]...
	@./$(TARGET)

clean:
	@echo [CLEANING]...
	@for %%f in ($(call FIX_PATH,$(OBJ))) do @if exist %%f del /q %%f
	@if exist $(call FIX_PATH,$(TARGET)) del /q $(call FIX_PATH,$(TARGET))

.PHONY: all run clean