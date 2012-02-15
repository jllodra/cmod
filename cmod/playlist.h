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

struct t_item {
  struct t_item *prev;
  int p;      // position (starting at 0)
  char *file; // complete path
  struct t_item *next;
};

typedef struct t_item item;

//head *item = NULL;
//tail *item = NULL;

void create_playlist(item *head, item *tail);
item *add(item *head, char *file, item *tail);
int del(item *head, int p, item *tail);
int del_all(item *head, item *tail);
item *get(item *head, int p, item *tail);
int is_empty(item *head, item *tail);


