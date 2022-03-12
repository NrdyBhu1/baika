#define CLR_R(x)   (((x) & 0xff0000) >> 16)
#define CLR_G(x)   (((x) & 0x00ff00) >>  8)
#define CLR_B(x)   (((x) & 0x0000ff) >>  0)
#define CLR_16(x)  ((double)(x) / 0xff)
#define CLR_GDK(x) (const GdkRGBA){ .red = CLR_16(CLR_R(x)), \
                                    .green = CLR_16(CLR_G(x)), \
                                    .blue = CLR_16(CLR_B(x)), \
                                    .alpha = 0 }
#define FONT "mononoki Nerd Font 15"
#define DEFAULT_SHELL "/usr/bin/fish"
#define STARTUP_DIR "/home/bhuvan"
#define HORIZONTAL_PADDING 5
#define VERTICAL_PADDING 5
#define ALLOW_HYPERLINK TRUE
#define CURSOR_BLINK VTE_CURSOR_BLINK_ON 
#define CURSOR_SHAPE VTE_CURSOR_SHAPE_BLOCK 
#define MOUSE_AUTOHIDE TRUE
