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

#include "utils.h"

char *strpad(char *text, int length)
{
  char *ret;
  int i = 0;

  ret = (char*)malloc(sizeof(char)*length + 1);



  if(strlen(text) < length) {

    while (*(text + i) != '\0') {
        *(ret + i) = *(text + i);
        i++;
      }

    while (i < length) {
      *(ret + i) = ' ';
      i++;
    }

  } else { // truncate
    strncpy(ret, text, length);
    for(i=length-3; i<length; i++) {
      *(ret + i) = '.';
    }
  }

  *(ret + i) = '\0';

  return ret;
}

char *mystrcat(char *s1, char *s2, int bar)
{
  char *temp;
  int i = 0;
  int j = 0;

  temp = (char*)malloc(sizeof(char)*(strlen(s1)+1+strlen(s2)+1));
  while(*(s1 + i) != '\0') {
    *(temp + j) = *(s1 + i);
    i++; j++;
  }
  if(bar) {
    *(temp + (j++)) = '/';
  }
  i = 0;
  while(*(s2 + i) != '\0') {
    *(temp + j) = *(s2 + i);
    i++; j++;
  }
  *(temp + j) = '\0';

  return temp;
}

char *name(char *filename)
{
  char *str;
  int i, j;

  str = strdup(filename);

  i = 0;
  j = 0; // Line added to remove garbage value
    
  while(*(str + i) != '\0') {
    if(*(str + i) == '/') {
      j = i + 1;
    }
    i++;
  }

  i = 0;

  while(*(str + j) != '\0') {
    *(str + i) = *(str + j);
    j++;
    i++;
  }

  *(str + i) = '\0';

  str = (char*)realloc(str, sizeof(char)*i + 1);

  return str;

}

