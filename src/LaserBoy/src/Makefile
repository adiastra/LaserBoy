# ────────────────────────────────────────────────────────────
#  LaserBoy‑FLTK • Ubuntu‑24.10 Makefile
#  Produces:
#      • objects  →  ../build/*.oux
#      • binary   →  ../LaserBoy
# ────────────────────────────────────────────────────────────

BIN         := ../LaserBoy
CXX         ?= g++
STRIP       ?= strip -s

INC_PATHS   := -I/usr/local/include
LIB_PATHS   := -L/usr/local/lib

CXXFLAGS    = -std=c++17                      \
              -O3                             \
              -pipe                           \
              -Wall -Wextra                   \
              -frounding-math                 \
              -Wno-unused-parameter           \
              -Wno-implicit-fallthrough       \
              -Wno-missing-field-initializers \
              -Wno-misleading-indentation     \
              -Wno-mismatched-new-delete      \
              -Wno-unused-but-set-variable    \
              $(INC_PATHS)

BOOST_LIBS  = -lboost_system                  \
              -lboost_filesystem              \
              -lboost_thread                  \
              -lboost_chrono

FLTK_LIBS   = -lfltk                          \
              -lfltk_images                   \
              -lfltk_jpeg                     \
              -lfltk_png

IMG_LIBS    = -ljpeg                          \
              -lpng                           \
              -lz

X11_LIBS    = -lX11                           \
              -lXext                          \
              -lXft                           \
              -lfontconfig                    \
              -lXrender                       \
              -lXfixes                        \
              -lXcursor                       \
              -lXinerama

SYS_LIBS    = -pthread                        \
              -ldl

LDFLAGS     = $(LIB_PATHS)                    \
              $(BOOST_LIBS)                   \
              $(FLTK_LIBS)                    \
              $(IMG_LIBS)                     \
              $(X11_LIBS)                     \
              $(SYS_LIBS)

SRC         := $(wildcard LaserBoy*.cpp)
OBJ         := $(patsubst %.cpp,../build/%.oux,$(SRC))
DEP         := $(OBJ:.oux=.dux)

all: $(BIN) strip

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

../build/%.oux: %.cpp
	@mkdir -p ../build
	$(CXX) $(CXXFLAGS) -MD -MF ../build/$*.dux -c $< -o $@

-include $(DEP)

strip:
	@if [ -f $(BIN) ]; then $(STRIP) $(BIN); fi

clean:
	rm -f ../build/*.oux ../build/*.dux $(BIN)

.PHONY: all clean strip


#   apt update
#   apt install -y build-essential git cmake pkg-config

#   apt install -y libboost-system-dev libboost-filesystem-dev libboost-thread-dev libboost-chrono-dev

#   apt install -y libjpeg-dev libpng-dev zlib1g-dev

#   apt install -y libx11-dev libxext-dev libxft-dev libfontconfig1-dev libxrender-dev libxfixes-dev libxcursor-dev libxinerama-dev

#   FLTK
#   ./configure \
#     --prefix=/usr/local \
#     --enable-static \
#     --disable-shared \
#     --enable-localjpeg \
#     --enable-localpng \
#     --enable-localzlib

#   make

#   make install


