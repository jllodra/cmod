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

#include <stdlib.h>
#include <string.h>

#include "playlist.h"

void create_playlist(item *head, item *tail)
{
  head->p = -1; head->prev = NULL; head->next = tail;
  tail->p = -2; tail->prev = head; tail->next = NULL;
}

item *add(item *head, char *file, item *tail)
{
  item *curr;
  char *str;

  curr = (item*)malloc(sizeof(item));
  str = (char*)malloc(sizeof(char)*(strlen(file)+1));
  strcpy(str, file);
  curr->file = str;

  if(is_empty(head, tail)) { // empty list
    head->next = curr;
    tail->prev = curr;
    curr->prev = head;
    curr->next = tail;
    curr->p = 0;
  } else {
    curr->prev = tail->prev;
    curr->next = tail;
    (tail->prev)->next = curr;
    curr->p = (tail->prev->p) + 1;
    tail->prev = curr;
  }

  return curr;
}

int del(item *head, int p, item *tail)
{
  item *curr;
  item *temp;

  if(!is_empty(head, tail)) {
    curr = head->next;
    while((curr->p != p) && (curr != tail)) {
      curr = curr->next;
    }
  } else {
    return 0;
  }

  if(curr == tail) {
    return 0; // not found
  }

  (curr->next)->prev = curr->prev;
  (curr->prev)->next = curr->next;

  temp = curr->next;
  free(curr->file);
  free(curr);

  while(temp != tail) {
    (temp->p)--;
    temp = temp->next;
  }

  return 1;
}

int del_all(item *head, item *tail)
{
  item *curr;
  item *temp;
  int ret = 0;

  curr = head->next;
  while(curr != tail) {
    //delete one
    (curr->next)->prev = curr->prev;
    (curr->prev)->next = curr->next;
    temp = curr->next;
    free(curr->file);
    free(curr);
    curr = temp; // next
    ret++;
  }
  return ret;
}

item *get(item *head, int p, item *tail)
{
  item *curr;

  if(!is_empty(head, tail)) {
    curr = head->next;
    while((curr->p != p) && (curr != tail)) {
      curr = curr->next;
    }
  } else {
    curr = NULL;
  }

  if(curr == tail) {
    curr = NULL;
  }

  return curr;
}

int is_empty(item *head, item *tail)
{
  return (head->next == tail);
}




