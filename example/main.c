#define RGFW_IMPLEMENTATION
#define RFONT_IMPLEMENTATION
#define RLGL_IMPLEMENTATION

#if !defined(RFONT_RENDER_LEGACY) && !defined(RFONT_RENDER_RLGL)
#define GL_GLEXT_PROTOTYPES
#endif

#ifdef RFONT_RENDER_LEGACY
#define GRAPHICS_API_OPENGL_11
#endif

#ifdef RFONT_RENDER_RLGL
#include "rlgl.h"
#define glColor4ub rlColor4ub
#endif

#if !defined(RFONT_RENDER_LEGACY) && !defined(RFONT_RENDER_RLGL) && defined(_WIN32)
#define GLAD_MALLOC malloc
#define GLAD_FREE free

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"
#endif

#include "RGFW.h"

#include "RFont.h"

#include <stdbool.h>

int main(int argc, char **argv) {
    RGFW_setGLVersion(3, 3);
    
    RGFW_window* win = RGFW_createWindow((argc > 1) ? argv[1] : "window", 200, 200, 1000, 500, 0);

    #if defined(RFONT_RENDER_RLGL) && !defined(RFONT_RENDER_LEGACY)
    rlLoadExtensions((void*)RGFW_getProcAddress);        
    rlglInit(win->w, win->h);  

    rlDrawRenderBatchActive();      // Update and draw internal render batch
    #endif

    #if !defined(RFONT_RENDER_LEGACY) && !defined(RFONT_RENDER_RLGL) && defined(_WIN32)
    gladLoadGL((GLADloadfunc)RGFW_getProcAddress);
    #endif

    RFont_init(win->w, win->h);
    RFont_font* font = RFont_font_init("/usr/share/fonts/TTF/DejaVuSans.ttf");

    bool running = true;

    while (running) {
        while(RGFW_window_checkEvent(win)) {   
            if (win->event.type == RGFW_quit) {
                running = false;
                break;
            }
        }

        glClearColor(100, 100, 100, 255);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glColor4ub(255, 0, 0, 255);
        RFont_draw_text(font, "abcdefghijklmnopqrstuvwxyz\n1234567890@.<>,/?\\|[{]}", 0, 100, 60);
        RFont_draw_text(font, "`~!#$%^&*()_-=+", 0, 220, 60);
        RFont_draw_text(font, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 320, 60);

        RFont_draw_text(font, "RFont_draw_text();", 0, 400, 60);

        #if defined(RFONT_RENDER_RLGL)
        rlDrawRenderBatchActive();      // Update and draw internal render batch
        #endif


        RGFW_window_swapBuffers(win);
        
        #if defined(RFONT_RENDER_RLGL) && !defined(RFONT_RENDER_LEGACY)
        rlSetFramebufferSize(win->w, win->h);  
        RFont_update_framebuffer(win->w, win->h);
        #endif
    }


    #if defined(RFONT_RENDER_RLGL) && !defined(RFONT_RENDER_LEGACY)
    rlglClose();
    #endif
    
    RGFW_window_close(win);

    return 0;
}