# project LaserBoy
# James Lehman
# laserboy.org
#
# Apple Silicon prerequisites:
#   xcode-select --install
#   brew install boost fltk jpeg libpng zlib

RM       = rm -f
STRIP    = strip
CPP      = g++
FLTK_CONFIG ?= fltk-config
BREW_PREFIX ?= $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)
INC      = -I$(BREW_PREFIX)/include
LIB      = -L$(BREW_PREFIX)/lib
LIBS     = $(shell $(FLTK_CONFIG) --ldflags) -lboost_filesystem

CPPFLAGS = -D__WITH_FLTK__ \
           -std=c++17 \
           -O3 \
           -Wall \
           -Wextra \
           -Wno-unused-parameter \
           -Wno-implicit-fallthrough \
           -Wno-missing-field-initializers \
           -Wno-misleading-indentation \
           -Wno-unused-but-set-variable \
           -Wno-mismatched-new-delete \
           -Wno-unused-command-line-argument \
           -Wno-unsupported-floating-point-opt \
           -static \
           $(shell $(FLTK_CONFIG) --cxxflags) \
           $(INC) \
           $(LIB) \
           $(LIBS)

HEADERS   = LaserBoy_includes.hpp \
            LaserBoy_macros.hpp \
            LaserBoy_common.hpp \
            LaserBoy_utility.hpp \
            LaserBoy_oscillator.hpp \
            LaserBoy_font.hpp \
            LaserBoy_bmp.hpp \
            LaserBoy_3D_short.hpp \
            LaserBoy_color.hpp \
            LaserBoy_vertex.hpp \
            LaserBoy_segment.hpp \
            LaserBoy_3D_double.hpp \
            LaserBoy_real_vertex.hpp \
            LaserBoy_real_segment.hpp \
            LaserBoy_real_segment_set.hpp \
            LaserBoy_ild_header.hpp \
            LaserBoy_frame.hpp \
            LaserBoy_frame_set.hpp \
            LaserBoy_palette.hpp \
            LaserBoy_palette_set.hpp \
            LaserBoy_wave.hpp \
            LaserBoy_FLTK_GUI.hpp \
            LaserBoy_space.hpp \
            LaserBoy_TUI.hpp

OBJ      = ../build/LaserBoy.om1 \
           ../build/LaserBoy_common.om1 \
           ../build/LaserBoy_oscillator.om1 \
           ../build/LaserBoy_frame.om1 \
           ../build/LaserBoy_frame_effects.om1 \
           ../build/LaserBoy_selected_frames_effects.om1 \
           ../build/LaserBoy_frame_set.om1 \
           ../build/LaserBoy_frame_set_effects.om1 \
           ../build/LaserBoy_ild_header.om1 \
           ../build/LaserBoy_segment.om1 \
           ../build/LaserBoy_palette.om1 \
           ../build/LaserBoy_palette_set.om1 \
           ../build/LaserBoy_real_segment.om1 \
           ../build/LaserBoy_real_segment_set.om1 \
           ../build/LaserBoy_wave.om1 \
           ../build/LaserBoy_space.om1 \
           ../build/LaserBoy_TUI.om1 \
           ../build/LaserBoy_FLTK_GUI.om1 \
           ../build/LaserBoy_bmp.om1 \
           ../build/LaserBoy_font.om1

BIN      = ../LaserBoy

all: $(BIN) strip

strip:
	$(STRIP) $(BIN)

clean:
	$(RM) ../build/*.om1 $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIB) $(LIBS)

../build/LaserBoy.om1: LaserBoy.cpp $(HEADERS)
	$(CPP) -c LaserBoy.cpp -o ../build/LaserBoy.om1 $(CPPFLAGS)

../build/LaserBoy_FLTK_GUI.om1: LaserBoy_FLTK_GUI.cpp $(HEADERS)
	$(CPP) -c LaserBoy_FLTK_GUI.cpp -o ../build/LaserBoy_FLTK_GUI.om1 $(CPPFLAGS)

../build/LaserBoy_TUI.om1: LaserBoy_TUI.cpp $(HEADERS)
	$(CPP) -c LaserBoy_TUI.cpp -o ../build/LaserBoy_TUI.om1 $(CPPFLAGS)

../build/LaserBoy_space.om1: LaserBoy_space.cpp $(HEADERS)
	$(CPP) -c LaserBoy_space.cpp -o ../build/LaserBoy_space.om1 $(CPPFLAGS)

../build/LaserBoy_frame_set_effects.om1: LaserBoy_frame_set_effects.cpp $(HEADERS)
	$(CPP) -c LaserBoy_frame_set_effects.cpp -o ../build/LaserBoy_frame_set_effects.om1 $(CPPFLAGS)

../build/LaserBoy_selected_frames_effects.om1: LaserBoy_selected_frames_effects.cpp $(HEADERS)
	$(CPP) -c LaserBoy_selected_frames_effects.cpp -o ../build/LaserBoy_selected_frames_effects.om1 $(CPPFLAGS)

../build/LaserBoy_frame_effects.om1: LaserBoy_frame_effects.cpp $(HEADERS)
	$(CPP) -c LaserBoy_frame_effects.cpp -o ../build/LaserBoy_frame_effects.om1 $(CPPFLAGS)

../build/LaserBoy_palette_set.om1: LaserBoy_palette_set.cpp $(HEADERS)
	$(CPP) -c LaserBoy_palette_set.cpp -o ../build/LaserBoy_palette_set.om1 $(CPPFLAGS)

../build/LaserBoy_palette.om1: LaserBoy_palette.cpp $(HEADERS)
	$(CPP) -c LaserBoy_palette.cpp -o ../build/LaserBoy_palette.om1 $(CPPFLAGS)

../build/LaserBoy_frame_set.om1: LaserBoy_frame_set.cpp $(HEADERS)
	$(CPP) -c LaserBoy_frame_set.cpp -o ../build/LaserBoy_frame_set.om1 $(CPPFLAGS)

../build/LaserBoy_frame.om1: LaserBoy_frame.cpp $(HEADERS)
	$(CPP) -c LaserBoy_frame.cpp -o ../build/LaserBoy_frame.om1 $(CPPFLAGS)

../build/LaserBoy_ild_header.om1: LaserBoy_ild_header.cpp LaserBoy_ild_header.hpp LaserBoy_common.hpp LaserBoy_macros.hpp LaserBoy_includes.hpp
	$(CPP) -c LaserBoy_ild_header.cpp -o ../build/LaserBoy_ild_header.om1 $(CPPFLAGS)

../build/LaserBoy_bmp.om1: LaserBoy_bmp.cpp LaserBoy_bmp.hpp LaserBoy_font.hpp LaserBoy_color.hpp LaserBoy_common.hpp LaserBoy_macros.hpp LaserBoy_includes.hpp
	$(CPP) -c LaserBoy_bmp.cpp -o ../build/LaserBoy_bmp.om1 $(CPPFLAGS)

../build/LaserBoy_font.om1: LaserBoy_font.cpp LaserBoy_font.hpp
	$(CPP) -c LaserBoy_font.cpp -o ../build/LaserBoy_font.om1 $(CPPFLAGS)

../build/LaserBoy_segment.om1: LaserBoy_segment.cpp $(HEADERS)
	$(CPP) -c LaserBoy_segment.cpp -o ../build/LaserBoy_segment.om1 $(CPPFLAGS)

../build/LaserBoy_real_segment.om1: LaserBoy_real_segment.cpp $(HEADERS)
	$(CPP) -c LaserBoy_real_segment.cpp -o ../build/LaserBoy_real_segment.om1 $(CPPFLAGS)

../build/LaserBoy_real_segment_set.om1: LaserBoy_real_segment_set.cpp $(HEADERS)
	$(CPP) -c LaserBoy_real_segment_set.cpp -o ../build/LaserBoy_real_segment_set.om1 $(CPPFLAGS)

../build/LaserBoy_wave.om1: LaserBoy_wave.cpp LaserBoy_wave.hpp LaserBoy_common.hpp LaserBoy_macros.hpp LaserBoy_includes.hpp
	$(CPP) -c LaserBoy_wave.cpp -o ../build/LaserBoy_wave.om1 $(CPPFLAGS)

../build/LaserBoy_oscillator.om1: LaserBoy_oscillator.cpp LaserBoy_oscillator.hpp LaserBoy_common.hpp LaserBoy_includes.hpp
	$(CPP) -c LaserBoy_oscillator.cpp -o ../build/LaserBoy_oscillator.om1 $(CPPFLAGS)

../build/LaserBoy_common.om1: LaserBoy_common.cpp LaserBoy_common.hpp LaserBoy_macros.hpp LaserBoy_includes.hpp
	$(CPP) -c LaserBoy_common.cpp -o ../build/LaserBoy_common.om1 $(CPPFLAGS)
