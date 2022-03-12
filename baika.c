/***************************************************************************
 * Filename: baika.c
 * Description: A terminal emulator
 *
 *  MIT License
 *  Copyright (c) 2022 NrdyBhu1
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h> 
#include <vte/vte.h>
#include "config.h"

GtkWidget *window;
#define EXIT_SUCCESS 0  
#define EXIT_FAILURE 1  
  
static void daemonize(void)  
{  
    pid_t pid, sid;  
    int fd;   
  
    /* already a daemon */  
    if ( getppid() == 1 ) return;  
  
    /* Fork off the parent process */  
    pid = fork();  
    if (pid < 0)    
    {     
        exit(EXIT_FAILURE);  
    }     
  
    if (pid > 0)    
    {     
        exit(EXIT_SUCCESS); /*Killing the Parent Process*/  
    }     
  
    /* At this point we are executing as the child process */  
  
    /* Create a new SID for the child process */  
    sid = setsid();  
    if (sid < 0)    
    {  
        exit(EXIT_FAILURE);  
    }  
  
    /* Change the current working directory. */  
    if ((chdir("/")) < 0)  
    {  
        exit(EXIT_FAILURE);  
    }  
  
  
    fd = open("/dev/null",O_RDWR, 0);  
  
    if (fd != -1)  
    {  
        dup2 (fd, STDIN_FILENO);  
        dup2 (fd, STDOUT_FILENO);  
        dup2 (fd, STDERR_FILENO);  
  
        if (fd > 2)  
        {  
            close (fd);  
        }  
    }  
  
    /*resettign File Creation Mask */  
    umask(027);  
}

static void
child_title_changed(VteTerminal* terminal, gpointer user_data) {
    const char* title = vte_terminal_get_window_title(VTE_TERMINAL(terminal));
    gtk_window_set_title(GTK_WINDOW(window), title);
}

static void
child_ready(VteTerminal* terminal, GPid pid, GError* error, gpointer user_data) {
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}

int
main(int argc, char* argv[]) {
    GtkWidget *terminal;
    // GdkPixbuf* icon;

    // init gtk window
    daemonize();
    gtk_init(&argc, &argv);
    terminal = vte_terminal_new();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // icon = gdk_pixbuf_new_from_resource("baika.png", NULL);
    // gtk_window_set_icon(GTK_WINDOW(window), icon);
    // gtk_window_set_icon_from_file(GTK_WINDOW(window), "baika.png", NULL);
    gtk_window_set_icon_name(GTK_WINDOW(window), "terminal");
    
    // start the shell
    gchar **shellcmd = (gchar *[]) { "/bin/sh", "-c", DEFAULT_SHELL, NULL };
    PangoFontDescription* font = pango_font_description_from_string(FONT);
    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
        VTE_PTY_DEFAULT,
        STARTUP_DIR,           /* working directory */
        shellcmd,       /* command           */
        NULL,           /* enviroment        */
        0,              /* spawn flags       */
        NULL, NULL,     /* child setup       */
        NULL,           /* child pid         */
        -1,             /* timeout           */
        NULL,           /* cancellable       */
        child_ready,    /* callback          */
        NULL);          /* user data         */

    vte_terminal_set_font(VTE_TERMINAL(terminal), font);
    vte_terminal_set_allow_hyperlink(VTE_TERMINAL(terminal), ALLOW_HYPERLINK);
    vte_terminal_set_cursor_blink_mode(VTE_TERMINAL(terminal), CURSOR_BLINK);
    vte_terminal_set_cursor_shape(VTE_TERMINAL(terminal), CURSOR_SHAPE);
    vte_terminal_set_mouse_autohide(VTE_TERMINAL(terminal), MOUSE_AUTOHIDE);
    vte_terminal_set_colors(VTE_TERMINAL(terminal),
        &CLR_GDK(0xffffff),
        &(GdkRGBA){ .alpha = 0.85 },
        (const GdkRGBA[]){
            CLR_GDK(0x111111),
            CLR_GDK(0xd36265),
            CLR_GDK(0xaece91),
            CLR_GDK(0xe7e18c),
            CLR_GDK(0x5297cf),
            CLR_GDK(0x963c59),
            CLR_GDK(0x5E7175),
            CLR_GDK(0xbebebe),
            CLR_GDK(0x666666),
            CLR_GDK(0xef8171),
            CLR_GDK(0xcfefb3),
            CLR_GDK(0xfff796),
            CLR_GDK(0x74b8ef),
            CLR_GDK(0xb85e7b),
            CLR_GDK(0xA3BABF),
            CLR_GDK(0xffffff)
    }, 16);

    // set quit on quit
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);
    g_signal_connect(terminal, "window-title-changed", G_CALLBACK(child_title_changed), NULL);

    // put everything together and run it
    gtk_container_add(GTK_CONTAINER(window), terminal);
    gtk_widget_show_all(window);
    gtk_main();
}
