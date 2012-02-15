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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <ncurses.h>
#include <limits.h>
#include <libgen.h>
#include <unistd.h>

#include "bassmod.h"
#include "filesystem.h"
#include "utils.h"
#include "cmod.h"

WINDOW *browser_win = NULL;
WINDOW *playlist_win = NULL;
int active_win = BROWSER_WIN;
int status = STOPPED;
BOOL random_play = FALSE; // random mode
BOOL color;
int p_b = 0; // item selected in browser window
int p_p = -1; // item selected in playlist window
int c_p_p = -1; // item playing in playlist window
int nfiles = 0; // number of files in the browser window
int nsongs = 0; // number of songs in the playlist window
char **entries = NULL; // directory files
char *current_path = NULL;
char progname[] = "[cmod 0.1 by herotyc]";
DWORD bass_loop = BASS_MUSIC_STOPBACK;
DWORD volume;
DWORD amplify = 50;
item playlist_h;
item playlist_t;

int main(int argc, char *argv[])
{
  char *buff;
  int ch;
  int i;

  initscr();
  cbreak();
  noecho();
  nonl();
  curs_set(0);
  // nodelay(stdscr, TRUE);
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);

  if((color = has_colors())) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_CYAN, COLOR_BLUE);
  }

  signal(SIGINT, cleanup);

  init_windows();
  init_bass();

  create_playlist(&playlist_h, &playlist_t);

  if(argc>1) {
    if(strncmp("/", argv[1], 1) != 0) {
      error("argument must be an absolute path", FALSE);
    }
    if(dirname(opendir(argv[1]))) { // is a dir
      if(add(&playlist_h, argv[1], &playlist_t)) {
        p_p++;
        c_p_p++;
        nsongs++;
        active_win = PLAYLIST_WIN;
        bass_loop = BASS_MUSIC_LOOP;
        play(argv[1], PLAYING_PLAYLIST);
      }
      current_path = strdup((char*)dirname(argv[1]));
    } else {
      error("argument not valid", FALSE);
    }
  } else {
    current_path = (char*)malloc(sizeof(char)*PATH_MAX);
    current_path = (char*)getcwd(current_path, PATH_MAX);
  }

  entries = refresh_browser(current_path, 0);

  mvprintw(0, 0, "%s", current_path);

  repaint_windows();

  while((ch = getch()) != 'q') {
    switch(ch) {
      case 'a': // add song to playlist
          if(active_win == BROWSER_WIN) {
            if(!is_dir(current_path, entries[p_b])) {
              buff = mystrcat(current_path,entries[p_b],1);
              if(add(&playlist_h, buff, &playlist_t)) {
                if(p_p == -1) p_p++;
                nsongs++;
                repaint_windows();
              }
              free(buff);
            }
          }
        break;
      case 'e': // add all songs in current dir
          if(active_win == BROWSER_WIN) {
            for(i=0; i<nfiles; i++) {
              if(!is_dir(current_path, entries[i])) {
                buff = mystrcat(current_path,entries[i],1);
                if(add(&playlist_h, buff, &playlist_t)) {
                  if(p_p == -1) p_p++;
                  nsongs++;
                }
                free(buff);
              }
            }
          }
          repaint_windows();
        break;
      case 'c': // recursive add
          if(active_win == BROWSER_WIN) {
            //printw("---%s+++", current_path);
            add_recursive(current_path, entries, nfiles);
            repaint_windows();
            //printw("---%s", current_path);
          }
        break;
      case 'd': // remove one song from playlist
          if(active_win == PLAYLIST_WIN) {
            if(del(&playlist_h, p_p, &playlist_t)) {
              nsongs--;
              if(p_p > nsongs - 1) {
                p_p = nsongs - 1;
              }
              if(p_p < c_p_p) {
                c_p_p--;
              }
              //stop();
              repaint_windows();
            }
          }
        break;
      case 'r': // remove all songs from playlist
          if(del_all(&playlist_h, &playlist_t)) {
            p_p = -1;
            c_p_p = -1;
            nsongs = 0;
            repaint_windows();
          }
        break;
      case ' ': // start playback or browse filesystem
      case 13: //enter key
          if(active_win == BROWSER_WIN) {
            if(strcmp(entries[p_b], "..\0") == 0) {
              current_path = dirname(current_path);
              p_b = 0;
              entries = refresh_browser(current_path, 0);
            } else {
              if(is_dir(current_path, entries[p_b])) {
                current_path = dir_up(current_path, entries[p_b]);
                p_b = 0;
                entries = refresh_browser(current_path, 0);
              } else {
                buff = mystrcat(current_path,entries[p_b],1);
                play(buff, PLAYING_BROWSER);
                free(buff);
                c_p_p = -1;
                if(p_b < nfiles-1) {
                  p_b++;
                }
              }
            }
            refresh_path();
          } else { // PLAYLIST_WIN
            if(nsongs != 0) {
              play(get(&playlist_h, p_p, &playlist_t)->file, PLAYING_PLAYLIST);
              c_p_p = p_p;
              if(p_p < nsongs - 1) {
                p_p++;
              }
            }
          }
          repaint_windows();
        break;

      case 's': // stop playback
          stop();
          c_p_p = -1;
          repaint_windows();
          clear_statusbar(NULL);
        break;

      case 'n': // next song
          next();
        break;

      case 'p': // prev song
          prev();
        break;

      case 'g': // go to current song
          p_p = c_p_p;
          repaint_windows();
        break;

      case 'l': // toggle loop
          loop();
          stop();
          c_p_p = -1;
          repaint_windows();
        break;

      case 'z': // toggle random
          random_mode();
        break;

      case '.': // seek forward
          seek(1);
        break;

      case ',': // seek backward
          seek(-1);
        break;

      case '+':
          volume_up();
        break;

      case '-':
          volume_down();
        break;

      case '>':
          amplify_up();
        break;

      case '<':
          amplify_down();
        break;

      case KEY_UP:
        if(active_win == BROWSER_WIN) {
          if(p_b > 0) {
            p_b--;
          }
        } else {
          if(p_p > 0) {
            p_p--;
          }
        }
        repaint_windows();
        break;

      case KEY_DOWN:
        if(active_win == BROWSER_WIN) {
          if(p_b < nfiles-1) {
            p_b++;
          }
        } else {
          if(p_p < nsongs - 1) {
            p_p++;
          }
        }
        repaint_windows();
        break;

      case KEY_LEFT:
      case KEY_RIGHT:
          switch_activewindow();
          repaint_windows();
        break;

      case KEY_HOME:
          if(active_win == BROWSER_WIN) {
            p_b = 0;
          } else {
            if(p_p != -1) p_p = 0;
          }
          repaint_windows();
        break;

      case KEY_END:
          if(active_win == BROWSER_WIN) {
            p_b = nfiles-1;
          } else {
            if(p_p != -1) p_p = nsongs - 1;
          }
          repaint_windows();
        break;

      case KEY_NPAGE:
        if(active_win == BROWSER_WIN) {
          if((p_b + (browser_win->_maxy - 2)) < nfiles-1 ) {
            p_b += browser_win->_maxy - 1;
          } else {
            p_b = nfiles-1;
          }
        } else {
          if((p_p + (playlist_win->_maxy - 2)) < nsongs) {
            p_p += playlist_win->_maxy - 1;
          } else {
            p_p = nsongs - 1;
          }
        }
        repaint_windows();
        break;

      case KEY_PPAGE:
        if(active_win == BROWSER_WIN) {
          if((p_b - (browser_win->_maxy - 2)) > 0) {
            p_b -= browser_win->_maxy - 1;
          } else {
            p_b = 0;
          }
        } else {
          if((p_p - (playlist_win->_maxy - 2)) > 0) {
            p_p -= playlist_win->_maxy - 1;
          } else {
            if(p_p != -1) p_p = 0;
          }
        }
        repaint_windows();
        break;
    }

    //usleep(10);

    //update_status();

  }

  cleanup();

  return 0;

}

void init_bass()
{
  if (!BASSMOD_Init(-1,44100,0))
    error("Can't initialize device", TRUE);

  volume = BASSMOD_GetVolume();
}

void CALLBACK end_song(HSYNC handle, DWORD data, DWORD user)
{
  if(bass_loop != BASS_MUSIC_LOOP) {
    clear_statusbar(NULL);
    if(status == PLAYING_PLAYLIST) {
      next(); // play next song
    } else {
      stop();
    }
  }
}

void play(char *song, int source)
{
  char songinfo[100];

  stop(); // if playing

  if (BASSMOD_MusicLoad(FALSE, song, 0, 0, BASS_PARMS|bass_loop)) {
    BASSMOD_MusicSetSync(BASS_SYNC_END, 0, &end_song, 0);
    BASSMOD_MusicPlay();
    snprintf(songinfo, 100, "playing \"%s\" [%d orders]", BASSMOD_MusicGetName(), BASSMOD_MusicGetLength(FALSE));
    clear_statusbar(songinfo);
  }

  status = source;

}

void stop()
{
  if (BASSMOD_MusicIsActive() != BASS_ACTIVE_STOPPED) {
    if(!BASSMOD_MusicStop()) {
      error("Can't stop the file", TRUE);
    }

  }

  BASSMOD_MusicFree();

  status = STOPPED;
}

void next()
{
  int r;

  if(status == PLAYING_PLAYLIST) {
    if(random_play) {
      if(nsongs > 0) {
        while((r = (rand() % nsongs ) + 1) == c_p_p + 1);
        c_p_p = r - 1;
        play(get(&playlist_h, c_p_p, &playlist_t)->file, PLAYING_PLAYLIST);
      }
    } else {
      if((c_p_p < nsongs - 1) && (c_p_p != -1)) {
        c_p_p++;
        play(get(&playlist_h, c_p_p, &playlist_t)->file, PLAYING_PLAYLIST);
      } else {
        c_p_p = -1;
        stop();
        clear_statusbar(NULL);
      }
    }
    repaint_windows();
  }
}

void prev()
{
  int r;
  if(status == PLAYING_PLAYLIST) {
    if(random_play) {
      if(nsongs > 0) {
        while((r = (rand() % nsongs ) + 1) == c_p_p + 1);
        c_p_p = r - 1;
      }
    } else {
      if(c_p_p > 0) {
        c_p_p--;
      }
    }
    play(get(&playlist_h, c_p_p, &playlist_t)->file, PLAYING_PLAYLIST);
    repaint_windows();
  }
}

void loop()
{
  if(bass_loop == BASS_MUSIC_LOOP) {
    bass_loop = BASS_MUSIC_STOPBACK;
    clear_statusbar("loop mode: off");
  } else {
    bass_loop = BASS_MUSIC_LOOP;
    clear_statusbar("loop mode: on");
  }
}

void random_mode()
{
  if(random_play = !random_play) {
    clear_statusbar("random: on");
  } else {
    clear_statusbar("random: off");
  }
}

void seek(int d)
{
  short order, length;

  order = BASSMOD_MusicGetPosition();
  length = BASSMOD_MusicGetLength(FALSE);

  if(d == 1) { // forward
    if(order < length) {
      BASSMOD_MusicSetPosition(++order);
    }
  } else { // backward
    if(order > 0) {
      BASSMOD_MusicSetPosition(--order);
    }
  }

}

void volume_up()
{
  char buff[50];

  volume = BASSMOD_GetVolume();
  if(volume < 100) {
    BASSMOD_SetVolume(++volume);
  }
  snprintf(buff, 50, "volume: %d", volume);
  clear_statusbar(buff);
}

void volume_down()
{
  char buff[50];

  volume = BASSMOD_GetVolume();
  if(volume > 0) {
    BASSMOD_SetVolume(--volume);
  }
  snprintf(buff, 50, "volume: %d", volume);
  clear_statusbar(buff);
}

void amplify_up()
{
  char buff[50];

  if(amplify < 100) {
    //BASSMOD_MusicSetAmplify(++amplify);
  }
  snprintf(buff, 50, "amplify: %d", amplify);
  clear_statusbar(buff);
}

void amplify_down()
{
  char buff[50];

  if(amplify > 0) {
    //BASSMOD_MusicSetAmplify(--amplify);
  }
  snprintf(buff, 50, "amplify: %d", amplify);
  clear_statusbar(buff);
}

void add_recursive(char *path, char **ent, int nfil)
{
  char **e;
  char *buff;
  int i;
  int item = 0;
  int nf;

  for(i=0; i<nfil; i++) {
    if(!is_dir(path, ent[i])) {
      buff = mystrcat(path,ent[i],1);
      if(add(&playlist_h, buff, &playlist_t)) {
        if(p_p == -1) p_p++;
        nsongs++;
      }
      free(buff);
    }  else { // is a dir
      if(strncmp(ent[i], ".." ,2) != 0) { // d'oh!
        buff = mystrcat(path, ent[i], 1);
        if(e = get_dir(buff, &nf)) {
          add_recursive(buff, e, nf);
          while(item < nf) {
            free(e[item++]);
          }
          free(e);
        }
        free(buff);
      }
    }
  }
}

void init_windows()
{
  int browser_width, playlist_width;

  browser_win = newwin(LINES - 2, COLS/2, 1, 0);
  playlist_win = newwin(LINES - 2, COLS/2, 1, COLS/2);

  refresh();
}

char **refresh_browser(char *path, int begin_pos)
{
  char *str = NULL;
  int p_item, item;
  int max_items;
  int attrs = A_NORMAL;

  wclear(browser_win);

  if(active_win == BROWSER_WIN) {
    if(color) {
      wattron(browser_win, COLOR_PAIR(1));
      wbkgd(browser_win, COLOR_PAIR(1));
    }
    wattrset(browser_win, A_BOLD);
  } else if (color) {
    wattron(browser_win, COLOR_PAIR(2));
    wbkgd(browser_win, COLOR_PAIR(2));
  }

  box(browser_win, 0, 0);

  if(path != NULL) {
    if(nfiles > 0) {
     item = 0;
      while(item < nfiles) {
        free(entries[item++]);
      }
    free(entries);
    }
    entries = get_dir(path, &nfiles);
  }

  item = 0;
  p_item = begin_pos;

  max_items = browser_win->_maxy - 1;

  while((p_item < nfiles) && item < max_items) {
    if((p_b >= 0) && (p_b == p_item)) {
      attrs = A_REVERSE;
    }
    str = strpad(entries[p_item], browser_win->_maxx - 1);
    if(is_dir(current_path, entries[p_item])) {
      attrs |= A_BOLD;
    }
    wattrset(browser_win, attrs);
    mvwaddstr(browser_win, item + 1, 1, str);
    free(str);
    attrs = A_NORMAL;
    p_item++;
    item++;
  }

  wnoutrefresh(browser_win);
  wnoutrefresh(playlist_win);
  doupdate();

  return entries;
}

void refresh_playlist(int begin_pos)
{
  int i, p_item;
  int max_items;
  char *str, *str2;
  item *curr;

  wclear(playlist_win);

  if(active_win == PLAYLIST_WIN) {
    if(color) {
      wattron(playlist_win, COLOR_PAIR(1));
      wbkgd(playlist_win, COLOR_PAIR(1));
    }
    wattrset(playlist_win, A_BOLD);
  } else if (color) {
    wattron(playlist_win, COLOR_PAIR(2));
    wbkgd(playlist_win, COLOR_PAIR(2));
  }

  box(playlist_win, 0, 0);
  wattrset(playlist_win, A_NORMAL);

  max_items = playlist_win->_maxy - 1;

  i = 0;
  p_item = begin_pos;

  while((curr = get(&playlist_h, p_item, &playlist_t)) && i < max_items) {
    if((c_p_p >= 0) && (c_p_p == p_item)) {
      wattrset(playlist_win, A_BOLD);
    }
    if((p_p >= 0) && (p_p == p_item)) {
      wattrset(playlist_win, A_REVERSE);
    }
    str2 = name(curr->file);
    str = strpad(str2, playlist_win->_maxx - 1);
    mvwaddstr(playlist_win, i + 1, 1, str);
    free(str2);
    free(str);
    wattrset(playlist_win, A_NORMAL);
    p_item++;
    i++;
  }

  wnoutrefresh(playlist_win);
  wnoutrefresh(browser_win);
  doupdate();
}

void switch_activewindow() {
  if(active_win == BROWSER_WIN) {
    active_win = PLAYLIST_WIN;
  } else {
    active_win = BROWSER_WIN;
  }
}

void repaint_windows()
{
  int begin_pos = 0;

  begin_pos = (p_b / (browser_win->_maxy - 1)) * (browser_win->_maxy - 1);
  refresh_browser(NULL, begin_pos);

  begin_pos = (p_p / (playlist_win->_maxy - 1)) * (playlist_win->_maxy - 1);
  refresh_playlist(begin_pos);

}

/*void update_status()
{
  int i;
  //unsigned int row;
  //unsigned short order;

  if(status == STOPPED) {
    for(i=0;i<(COLS - strlen(progname)); i++){
      mvprintw(0, i, " ");
    }
  }

  //if(status == PLAYING_BROWSER || status == PLAYING_PLAYLIST) {
  //  order = BASSMOD_MusicGetPosition();
  //  row = BASSMOD_MusicGetPosition() >> 16;
  //  mvprintw(0, 0, "order: %d, row:%3d ", order, row);
  //}



}*/

void refresh_path()
{
  int i;
  // clear the status bar (up)
  for(i=0;i<COLS; i++){
    mvprintw(0, i, " ");
  }

  mvprintw(0, 0, "%s", current_path);

}

void clear_statusbar(char msg[])
{
  int i;

  for(i=0;i<COLS; i++){
    mvprintw(LINES-1, i, " ");
  }

  if (msg) {
    mvprintw(LINES-1, 0, "%s", msg);
  }

  refresh();

}

void cleanup()
{
  if(current_path) free(current_path);
  signal(SIGINT, SIG_DFL);
  del_all(&playlist_h, &playlist_t);
  endwin();
  BASSMOD_Free();
  printf("thanks for using %s\n", progname);
}

/* display error messages */
void error(char *text, BOOL bass)
{
  cleanup();

  if(bass) {
    printf("Error(%d): %s\n", BASSMOD_ErrorGetCode(), text);
  } else {
    printf("Error: %s\n", text);
  }
  exit(1);
}
