TOOL_CC  = gcc
TOOL_CPP = g++
TOOL_AR =  ar

OBJ_EXT = .o
H_EXT = .h
CPP_EXT = .cpp
C_EXT = .c
LIB_EXT = .a
EXEC_EXT = 

LIB = lib

LD_FLAGS = -g -O3
CC_FLAGS_EASY = -g -O3 -DFONTS_USE_SBC_COMPRESSION

COMMON_LIBS = ../../common-libs/common
GRAPHICS_LIB =  ../Graphics
SURFACE = $(GRAPHICS_LIB)/Surface
BITMAP = $(GRAPHICS_LIB)/bitmap
DRAWING = $(GRAPHICS_LIB)/Drawing
FONTS = $(GRAPHICS_LIB)/Fonts
TESTCODE = $(GRAPHICS_LIB)/TestCode
JSON_LIB = $(COMMON_LIBS)/../libjson
XJSON_LIB = $(COMMON_LIBS)/../libXjson
BASE64 = $(COMMON_LIBS)/../Base64
SCANBACK = $(COMMON_LIBS)/../Scanback
LINUXFB = $(GRAPHICS_LIB)/LinuxFB
JETCAT = $(GRAPHICS_LIB)/JetCat
JETCAT_MODULES = $(GRAPHICS_LIB)/JetCat-Modules
NETWORK = $(COMMON_LIBS)/../network
TESTCODELINUX = $(GRAPHICS_LIB)/TestCodeLinux
CALIBRATOR = $(GRAPHICS_LIB)/CJCalibrator
MESSAGE_BOX = $(GRAPHICS_LIB)/MessageBox
CJFBCAPTURE = $(GRAPHICS_LIB)/CJFBCapture
CJFBMPDRAW = $(GRAPHICS_LIB)/CJBitmapDraw
FRAMEBUFF = $(GRAPHICS_LIB)/framebuf

BUILD_DIR = ../build

ifeq ($(FRAMEBUF), SDL)
SDL_PATH = `sdl-config --cflags --libs`
SDL_LIB = $(BUILD_DIR)/$(LIB)framebuf$(LIB_EXT)
SDL_LIB_LINK = -lframebuf
CC_FLAGS = $(CC_FLAGS_EASY) -DUSE_SDL_FRAMEBUF
else
SDL_PATH =
SDL_LIB = $(BUILD_DIR)/$(LIB)framebuf$(LIB_EXT)
SDL_LIB_LINK = -lframebuf
CC_FLAGS = $(CC_FLAGS_EASY)
endif

all: $(BUILD_DIR)/$(LIB)common$(LIB_EXT) $(BUILD_DIR)/$(LIB)surface$(LIB_EXT) $(BUILD_DIR)/$(LIB)drawing$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)fonts$(LIB_EXT) $(BUILD_DIR)/$(LIB)json$(LIB_EXT)  $(BUILD_DIR)/$(LIB)xjson$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)scanback$(LIB_EXT) $(BUILD_DIR)/$(LIB)jetcat$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)jetcat_modules$(LIB_EXT) $(SDL_LIB) \
	$(BUILD_DIR)/TestCode$(EXEC_EXT)

linux: all $(BUILD_DIR)/$(LIB)linuxfb$(LIB_EXT) $(BUILD_DIR)/TestCodeLinux$(EXEC_EXT) $(BUILD_DIR)/cjcalibrate$(EXEC_EXT) \
	$(BUILD_DIR)/messagebox$(EXEC_EXT) $(BUILD_DIR)/cjfb_capture$(EXEC_EXT) \
	$(BUILD_DIR)/cjbitmapdraw$(EXEC_EXT)

clean:
	rm $(BUILD_DIR)/*

# Make a custom build of CommonLibs

$(BUILD_DIR)/common_utilities$(OBJ_EXT): $(COMMON_LIBS)/common_utilities$(CPP_EXT) $(COMMON_LIBS)/common_utilities$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS)

$(BUILD_DIR)/common_events$(OBJ_EXT): $(COMMON_LIBS)/common_events$(CPP_EXT) $(COMMON_LIBS)/common_events$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS)

$(BUILD_DIR)/common_swap$(OBJ_EXT): $(COMMON_LIBS)/common_swap$(CPP_EXT) $(COMMON_LIBS)/common_swap$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS)

$(BUILD_DIR)/base64$(OBJ_EXT): $(BASE64)/Base64$(CPP_EXT) $(BASE64)/Base64$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(BASE64)

$(BUILD_DIR)/databuffer$(OBJ_EXT): $(COMMON_LIBS)/databuffer$(CPP_EXT) $(COMMON_LIBS)/databuffer$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(BASE64)

$(BUILD_DIR)/nettcp$(OBJ_EXT): $(NETWORK)/nettcp$(CPP_EXT) $(NETWORK)/nettcp$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(NETWORK) -I$(NETWORK)/..

$(BUILD_DIR)/$(LIB)common$(LIB_EXT): $(BUILD_DIR)/common_swap$(OBJ_EXT) $(BUILD_DIR)/common_events$(OBJ_EXT) \
	$(BUILD_DIR)/common_utilities$(OBJ_EXT) $(BUILD_DIR)/base64$(OBJ_EXT) $(BUILD_DIR)/databuffer$(OBJ_EXT) \
	$(BUILD_DIR)/nettcp$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

# Surface library

$(BUILD_DIR)/surface$(OBJ_EXT): $(SURFACE)/surface$(CPP_EXT) $(SURFACE)/surface$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE)

$(BUILD_DIR)/bmp_surface$(OBJ_EXT): $(SURFACE)/bmp_surface$(CPP_EXT) $(SURFACE)/bmp_surface$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE)

$(BUILD_DIR)/easy_draw$(OBJ_EXT): $(SURFACE)/easy_draw$(CPP_EXT) $(SURFACE)/easy_draw$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE)

$(BUILD_DIR)/bmp$(OBJ_EXT): $(BITMAP)/bmp$(CPP_EXT) $(BITMAP)/bmp$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(BITMAP)

$(BUILD_DIR)/$(LIB)surface$(LIB_EXT): $(BUILD_DIR)/bmp$(OBJ_EXT) $(BUILD_DIR)/surface$(OBJ_EXT) $(BUILD_DIR)/bmp_surface$(OBJ_EXT) $(BUILD_DIR)/easy_draw$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

# Drawing library

$(BUILD_DIR)/bitmask$(OBJ_EXT): $(DRAWING)/bitmask$(CPP_EXT) $(DRAWING)/bitmask$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING)

$(BUILD_DIR)/brush$(OBJ_EXT): $(DRAWING)/brush$(CPP_EXT) $(DRAWING)/brush$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING)

$(BUILD_DIR)/sprite$(OBJ_EXT): $(DRAWING)/sprite$(CPP_EXT) $(DRAWING)/sprite$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING)

$(BUILD_DIR)/transparency$(OBJ_EXT): $(DRAWING)/transparency$(CPP_EXT) $(DRAWING)/transparency$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING)

$(BUILD_DIR)/viewport$(OBJ_EXT): $(DRAWING)/viewport$(CPP_EXT) $(DRAWING)/viewport$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING)

$(BUILD_DIR)/scale$(OBJ_EXT): $(DRAWING)/scale$(CPP_EXT) $(DRAWING)/scale$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING)

$(BUILD_DIR)/$(LIB)drawing$(LIB_EXT): $(BUILD_DIR)/bitmask$(OBJ_EXT) $(BUILD_DIR)/brush$(OBJ_EXT) $(BUILD_DIR)/sprite$(OBJ_EXT) \
	$(BUILD_DIR)/transparency$(OBJ_EXT) $(BUILD_DIR)/viewport$(OBJ_EXT) $(BUILD_DIR)/scale$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

# Fonts library

$(BUILD_DIR)/character$(OBJ_EXT): $(FONTS)/character$(CPP_EXT) $(FONTS)/character$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS)

$(BUILD_DIR)/Font$(OBJ_EXT): $(FONTS)/Font$(CPP_EXT) $(FONTS)/Font$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS)

$(BUILD_DIR)/FIndex$(OBJ_EXT): $(FONTS)/FIndex$(CPP_EXT) $(FONTS)/FIndex$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS)

$(BUILD_DIR)/FontSet$(OBJ_EXT): $(FONTS)/FontSet$(CPP_EXT) $(FONTS)/FontSet$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS)

$(BUILD_DIR)/$(LIB)fonts$(LIB_EXT): $(BUILD_DIR)/character$(OBJ_EXT) $(BUILD_DIR)/Font$(OBJ_EXT) $(BUILD_DIR)/FIndex$(OBJ_EXT) \
	$(BUILD_DIR)/FontSet$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

# libJSON

$(BUILD_DIR)/Serializable$(OBJ_EXT): $(JSON_LIB)/Serializable$(CPP_EXT) $(JSON_LIB)/Serializable$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(JSON_LIB)

$(BUILD_DIR)/Parseable$(OBJ_EXT): $(JSON_LIB)/Parseable$(CPP_EXT) $(JSON_LIB)/Parseable$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(JSON_LIB)

$(BUILD_DIR)/Json$(OBJ_EXT): $(JSON_LIB)/Json$(CPP_EXT) $(JSON_LIB)/Json$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(JSON_LIB)

$(BUILD_DIR)/$(LIB)json$(LIB_EXT): $(BUILD_DIR)/Serializable$(OBJ_EXT) $(BUILD_DIR)/Parseable$(OBJ_EXT) $(BUILD_DIR)/Json$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

# libXJSON

$(BUILD_DIR)/xjson_defs$(OBJ_EXT): $(XJSON_LIB)/xjson_defs$(CPP_EXT) $(XJSON_LIB)/xjson_defs$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(JSON_LIB) -I $(XJSON_LIB)

$(BUILD_DIR)/xjson_vocabulary$(OBJ_EXT): $(XJSON_LIB)/xjson_vocabulary$(CPP_EXT) $(XJSON_LIB)/xjson_vocabulary$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(JSON_LIB) -I $(XJSON_LIB)

$(BUILD_DIR)/xjson_conv$(OBJ_EXT): $(XJSON_LIB)/xjson_conv$(CPP_EXT) $(XJSON_LIB)/xjson_conv$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(JSON_LIB) -I $(XJSON_LIB)

$(BUILD_DIR)/$(LIB)xjson$(LIB_EXT): $(BUILD_DIR)/xjson_conv$(OBJ_EXT) $(BUILD_DIR)/xjson_defs$(OBJ_EXT) $(BUILD_DIR)/xjson_vocabulary$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

# Scanback

$(BUILD_DIR)/rle$(OBJ_EXT): $(SCANBACK)/rle$(CPP_EXT) $(SCANBACK)/rle$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SCANBACK)

$(BUILD_DIR)/bitmem$(OBJ_EXT): $(SCANBACK)/bitmem$(C_EXT) $(SCANBACK)/bitmem$(H_EXT)
	$(TOOL_CC) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SCANBACK)

$(BUILD_DIR)/rlesbc$(OBJ_EXT): $(SCANBACK)/rlesbc$(CPP_EXT) $(SCANBACK)/rlesbc$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SCANBACK)

$(BUILD_DIR)/scanback$(OBJ_EXT): $(SCANBACK)/scanback$(CPP_EXT) $(SCANBACK)/scanback$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SCANBACK)

$(BUILD_DIR)/$(LIB)scanback$(LIB_EXT): $(BUILD_DIR)/scanback$(OBJ_EXT) $(BUILD_DIR)/rle$(OBJ_EXT) $(BUILD_DIR)/rlesbc$(OBJ_EXT) $(BUILD_DIR)/bitmem$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

# CompoLin
$(BUILD_DIR)/fb_surface$(OBJ_EXT): $(LINUXFB)/fb_surface$(CPP_EXT) $(LINUXFB)/fb_surface$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(LINUXFB) -I $(SURFACE) -I $(LINUXFB) $(SDL_PATH)

$(BUILD_DIR)/touchscreenlistener$(OBJ_EXT): $(LINUXFB)/touchscreenlistener$(CPP_EXT) $(LINUXFB)/touchscreenlistener$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(LINUXFB)

$(BUILD_DIR)/$(LIB)linuxfb$(LIB_EXT): $(BUILD_DIR)/fb_surface$(OBJ_EXT) $(BUILD_DIR)/touchscreenlistener$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

# JetCat

$(BUILD_DIR)/background$(OBJ_EXT): $(JETCAT)/background$(CPP_EXT) $(JETCAT)/background$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/caption$(OBJ_EXT): $(JETCAT)/caption$(CPP_EXT) $(JETCAT)/caption$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/drawable$(OBJ_EXT): $(JETCAT)/drawable$(CPP_EXT) $(JETCAT)/drawable$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/frame$(OBJ_EXT): $(JETCAT)/frame$(CPP_EXT) $(JETCAT)/frame$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/global$(OBJ_EXT): $(JETCAT)/global$(CPP_EXT) $(JETCAT)/global$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/progress$(OBJ_EXT): $(JETCAT)/progress$(CPP_EXT) $(JETCAT)/progress$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/shadow$(OBJ_EXT): $(JETCAT)/shadow$(CPP_EXT) $(JETCAT)/shadow$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/statusbar$(OBJ_EXT): $(JETCAT)/statusbar$(CPP_EXT) $(JETCAT)/statusbar$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/textout$(OBJ_EXT): $(JETCAT)/textout$(CPP_EXT) $(JETCAT)/textout$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/imagebox$(OBJ_EXT): $(JETCAT)/imagebox$(CPP_EXT) $(JETCAT)/imagebox$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/inputs$(OBJ_EXT): $(JETCAT)/inputs$(CPP_EXT) $(JETCAT)/inputs$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/window$(OBJ_EXT): $(JETCAT)/window$(CPP_EXT) $(JETCAT)/window$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/evtfilter$(OBJ_EXT): $(JETCAT)/evtfilter$(CPP_EXT) $(JETCAT)/evtfilter$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/placement$(OBJ_EXT): $(JETCAT)/placement$(CPP_EXT) $(JETCAT)/placement$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/label$(OBJ_EXT): $(JETCAT)/label$(CPP_EXT) $(JETCAT)/label$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/buttons$(OBJ_EXT): $(JETCAT)/buttons$(CPP_EXT) $(JETCAT)/buttons$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/tabview$(OBJ_EXT): $(JETCAT)/tabview$(CPP_EXT) $(JETCAT)/tabview$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/menu$(OBJ_EXT): $(JETCAT)/menu$(CPP_EXT) $(JETCAT)/menu$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/focus$(OBJ_EXT): $(JETCAT)/focus$(CPP_EXT) $(JETCAT)/focus$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/cursor$(OBJ_EXT): $(JETCAT)/cursor$(CPP_EXT) $(JETCAT)/cursor$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/scroller$(OBJ_EXT): $(JETCAT)/scroller$(CPP_EXT) $(JETCAT)/scroller$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/editbox$(OBJ_EXT): $(JETCAT)/editbox$(CPP_EXT) $(JETCAT)/editbox$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/keypad$(OBJ_EXT): $(JETCAT)/keypad$(CPP_EXT) $(JETCAT)/keypad$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/messagebox$(OBJ_EXT): $(JETCAT)/messagebox$(CPP_EXT) $(JETCAT)/messagebox$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT)

$(BUILD_DIR)/$(LIB)jetcat$(LIB_EXT): $(BUILD_DIR)/background$(OBJ_EXT) $(BUILD_DIR)/frame$(OBJ_EXT) \
	$(BUILD_DIR)/caption$(OBJ_EXT) $(BUILD_DIR)/drawable$(OBJ_EXT) $(BUILD_DIR)/shadow$(OBJ_EXT)  \
	$(BUILD_DIR)/global$(OBJ_EXT) $(BUILD_DIR)/progress$(OBJ_EXT) $(BUILD_DIR)/imagebox$(OBJ_EXT) \
	$(BUILD_DIR)/statusbar$(OBJ_EXT) $(BUILD_DIR)/textout$(OBJ_EXT) $(BUILD_DIR)/window$(OBJ_EXT) \
	$(BUILD_DIR)/evtfilter$(OBJ_EXT) $(BUILD_DIR)/inputs$(OBJ_EXT) $(BUILD_DIR)/placement$(OBJ_EXT) \
	$(BUILD_DIR)/label$(OBJ_EXT) $(BUILD_DIR)/buttons$(OBJ_EXT) $(BUILD_DIR)/tabview$(OBJ_EXT) \
	$(BUILD_DIR)/menu$(OBJ_EXT) $(BUILD_DIR)/focus$(OBJ_EXT) $(BUILD_DIR)/cursor$(OBJ_EXT) \
        $(BUILD_DIR)/scroller$(OBJ_EXT) $(BUILD_DIR)/editbox$(OBJ_EXT) $(BUILD_DIR)/keypad$(OBJ_EXT) \
        $(BUILD_DIR)/messagebox$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

#########################################################################################################################################
#### JetCat Modules
#########################################################################################################################################

$(BUILD_DIR)/touch$(OBJ_EXT): $(JETCAT_MODULES)/touch$(CPP_EXT) $(JETCAT_MODULES)/touch$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT) -I$(JETCAT_MODULES)

$(BUILD_DIR)/$(LIB)jetcat_modules$(LIB_EXT): $(BUILD_DIR)/touch$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

#########################################################################################################################################
#### Framebuff
#########################################################################################################################################

$(BUILD_DIR)/framebuf$(OBJ_EXT): $(FRAMEBUFF)/framebuf$(CPP_EXT) $(FRAMEBUFF)/framebuf$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT) -I$(JETCAT_MODULES) $(SDL_PATH)

ifeq ($(FRAMEBUF), SDL)

$(BUILD_DIR)/sdl_surface$(OBJ_EXT): $(FRAMEBUFF)/sdl_surface$(CPP_EXT) $(FRAMEBUFF)/sdl_surface$(H_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(JETCAT) -I$(JETCAT_MODULES) $(SDL_PATH)

$(BUILD_DIR)/$(LIB)framebuf$(LIB_EXT): $(BUILD_DIR)/sdl_surface$(OBJ_EXT) $(BUILD_DIR)/framebuf$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

else

$(BUILD_DIR)/$(LIB)framebuf$(LIB_EXT): $(BUILD_DIR)/framebuf$(OBJ_EXT)
	$(TOOL_AR) rvs $@ $^

endif

#########################################################################################################################################
#### JetCat Calibrate
#########################################################################################################################################

$(BUILD_DIR)/jcc_linux_main$(OBJ_EXT): $(CALIBRATOR)/linux_main$(CPP_EXT) 
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(LINUXFB) -I $(JETCAT) -I $(JETCAT_MODULES) $(SDL_PATH)


$(BUILD_DIR)/cjcalibrate$(EXEC_EXT): $(BUILD_DIR)/$(LIB)common$(LIB_EXT) $(BUILD_DIR)/$(LIB)surface$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)drawing$(LIB_EXT) $(BUILD_DIR)/$(LIB)fonts$(LIB_EXT) $(BUILD_DIR)/$(LIB)json$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)xjson$(LIB_EXT) $(BUILD_DIR)/$(LIB)scanback$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)jetcat$(LIB_EXT) $(BUILD_DIR)/$(LIB)jetcat_modules$(LIB_EXT) \
	$(BUILD_DIR)/jcc_linux_main$(OBJ_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -o $@ -pthread $(LD_FLAGS) \
	$(BUILD_DIR)/jcc_linux_main$(OBJ_EXT)  \
	-L$(BUILD_DIR) 			 \
        -ljetcat_modules                 \
        -ljetcat                         \
	$(SDL_LIB_LINK)			 \
        -llinuxfb                        \
	-lfonts 			 \
	-ldrawing 			 \
	-lsurface 			 \
	-ljson	 			 \
	-lxjson				 \
	-lscanback                       \
	-lcommon			 \
	$(SDL_PATH)


#########################################################################################################################################
#### MessageBox
#########################################################################################################################################

$(BUILD_DIR)/mbox_linux_main$(OBJ_EXT): $(MESSAGE_BOX)/linux_main$(CPP_EXT) 
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(LINUXFB) -I $(JETCAT) -I $(JETCAT_MODULES) $(SDL_PATH)


$(BUILD_DIR)/messagebox$(EXEC_EXT): $(BUILD_DIR)/$(LIB)common$(LIB_EXT) $(BUILD_DIR)/$(LIB)surface$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)drawing$(LIB_EXT) $(BUILD_DIR)/$(LIB)fonts$(LIB_EXT) $(BUILD_DIR)/$(LIB)json$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)xjson$(LIB_EXT) $(BUILD_DIR)/$(LIB)scanback$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)jetcat$(LIB_EXT) $(BUILD_DIR)/$(LIB)jetcat_modules$(LIB_EXT) \
	$(BUILD_DIR)/mbox_linux_main$(OBJ_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -o $@ -pthread $(LD_FLAGS) \
	$(BUILD_DIR)/mbox_linux_main$(OBJ_EXT)  \
	-L$(BUILD_DIR) 			 \
        -ljetcat_modules                 \
        -ljetcat                         \
	$(SDL_LIB_LINK)			 \
        -llinuxfb                        \
	-lfonts 			 \
	-ldrawing 			 \
	-lsurface 			 \
	-ljson	 			 \
	-lxjson				 \
	-lscanback                       \
	-lcommon			 \
	$(SDL_PATH)

#########################################################################################################################################
#### Linux Frame Buffer Capture
#########################################################################################################################################

$(BUILD_DIR)/fbcapture_linux_main$(OBJ_EXT): $(CJFBCAPTURE)/linux_main$(CPP_EXT) 
	$(TOOL_CPP) $(CC_FLAGS_EASY) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(LINUXFB) -I $(JETCAT) -I $(JETCAT_MODULES) $(SDL_PATH)


$(BUILD_DIR)/cjfb_capture$(EXEC_EXT): $(BUILD_DIR)/$(LIB)common$(LIB_EXT) $(BUILD_DIR)/$(LIB)surface$(LIB_EXT) \
	$(BUILD_DIR)/fbcapture_linux_main$(OBJ_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -o $@ -pthread $(LD_FLAGS) \
	$(BUILD_DIR)/fbcapture_linux_main$(OBJ_EXT)  \
	-L$(BUILD_DIR) 			 \
	$(SDL_LIB_LINK)			 \
        -llinuxfb                        \
	-lsurface 			 \
	-lcommon			 \
	$(SDL_PATH)

#########################################################################################################################################
#### Linux Bitmap Draw
#########################################################################################################################################

$(BUILD_DIR)/bmpdraw_linux_main$(OBJ_EXT): $(CJFBMPDRAW)/linux_main$(CPP_EXT) 
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(LINUXFB) -I $(JETCAT) -I $(JETCAT_MODULES) $(SDL_PATH)


$(BUILD_DIR)/cjbitmapdraw$(EXEC_EXT): $(BUILD_DIR)/$(LIB)common$(LIB_EXT) $(BUILD_DIR)/$(LIB)surface$(LIB_EXT) \
	$(BUILD_DIR)/bmpdraw_linux_main$(OBJ_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -o $@ -pthread $(LD_FLAGS) \
	$(BUILD_DIR)/bmpdraw_linux_main$(OBJ_EXT)  \
	-L$(BUILD_DIR) 			 \
	$(SDL_LIB_LINK)			 \
        -llinuxfb                        \
	-lsurface 			 \
	-lcommon			 \
	$(SDL_PATH)

#########################################################################################################################################
#### Test Code
#########################################################################################################################################

$(BUILD_DIR)/TestCode$(OBJ_EXT): $(TESTCODE)/TestCode$(CPP_EXT) 
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(TESTCODE)

$(BUILD_DIR)/TestCodeLinux$(OBJ_EXT): $(TESTCODELINUX)/TestCodeLinux$(CPP_EXT) 
	$(TOOL_CPP) $(CC_FLAGS) -c -o $@ $< -I $(COMMON_LIBS) -I $(SURFACE) -I $(DRAWING) -I $(FONTS) -I $(LINUXFB) -I $(TESTCODELINUX) $(SDL_PATH)

$(BUILD_DIR)/TestCode$(EXEC_EXT): $(BUILD_DIR)/$(LIB)common$(LIB_EXT) $(BUILD_DIR)/$(LIB)surface$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)drawing$(LIB_EXT) $(BUILD_DIR)/$(LIB)fonts$(LIB_EXT) $(BUILD_DIR)/$(LIB)json$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)xjson$(LIB_EXT) $(BUILD_DIR)/$(LIB)scanback$(LIB_EXT) \
	$(BUILD_DIR)/TestCode$(OBJ_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -o $@ -pthread $(LD_FLAGS) \
	$(BUILD_DIR)/TestCode$(OBJ_EXT)  \
	-L$(BUILD_DIR) 			 \
	-lfonts 			 \
	-ldrawing 			 \
	-lsurface 			 \
	-ljson	 			 \
	-lxjson				 \
	-lscanback                       \
	-lcommon

$(BUILD_DIR)/TestCodeLinux$(EXEC_EXT): $(BUILD_DIR)/$(LIB)common$(LIB_EXT) $(BUILD_DIR)/$(LIB)surface$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)drawing$(LIB_EXT) $(BUILD_DIR)/$(LIB)fonts$(LIB_EXT) $(BUILD_DIR)/$(LIB)json$(LIB_EXT) \
	$(BUILD_DIR)/$(LIB)xjson$(LIB_EXT) $(BUILD_DIR)/$(LIB)scanback$(LIB_EXT) \
	$(BUILD_DIR)/TestCodeLinux$(OBJ_EXT)
	$(TOOL_CPP) $(CC_FLAGS) -o $@ -pthread $(LD_FLAGS) \
	$(BUILD_DIR)/TestCodeLinux$(OBJ_EXT)  \
	-L$(BUILD_DIR) 			 \
        -llinuxfb                        \
	$(SDL_LIB_LINK)			 \
	-lfonts 			 \
	-ldrawing 			 \
	-lsurface 			 \
	-ljson	 			 \
	-lxjson				 \
	-lscanback                       \
	-lcommon			 \
	$(SDL_PATH)

