/***************************************************************************
 *   Copyright (C) 2006 by josep ll.   *
 *   jlg@agiaonline.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//#define DEBUG

#define BASS_PARMS BASS_MUSIC_RAMPS|BASS_MUSIC_SURROUND|BASS_MUSIC_CALCLEN|BASS_MUSIC_POSRESET

#define BOOL int

#ifndef TRUE
  #define TRUE  0
#endif
#ifndef FALSE
  #define FALSE 1
#endif

#define BROWSER_WIN 0
#define PLAYLIST_WIN 1

#define STOPPED 0
#define PLAYING_BROWSER 1
#define PLAYING_PLAYLIST 2

#include "playlist.h"

void init_bass();
void CALLBACK end_song(HSYNC handle, DWORD data, DWORD user);

void play(char *song, int source);
void stop();
void next();
void prev();
void loop();
void random_mode();
void seek(int d);
void volume_up();
void volume_down();
void amplify_up();
void amplify_down();

void add_recursive(char *path, char **ent, int nfil);

void init_windows();
char **refresh_browser(char *path, int begin_pos);
void refresh_playlist(int begin_pos);
void repaint_browser();
void repaint_playlist();
void switch_activewindow();
void repaint_windows();
void show_help();
void hide_help();
//void update_status();
void refresh_path();
void clear_statusbar(char msg[]);

char *strpad(char *text, int length);
void cleanup();
void error(char *text, BOOL bass);

extern WINDOW *browser_win;
extern WINDOW *playlist_win;
extern BOOL random_play; // random mode
extern BOOL color;
extern int active_win;
extern int status;
extern int p_b; // item selected in browser window
extern int p_p; // item selected in playlist window
extern int c_p_p; // item playing in playlist window
extern int nfiles; // number of files in the browser window
extern int nsongs; // number of songs in the playlist window
extern char **entries; // directory files
extern char *current_path;
extern char progname[];
extern DWORD bass_loop;
extern DWORD volume;
extern DWORD amplify;
extern item playlist_h;
extern item playlist_t;
