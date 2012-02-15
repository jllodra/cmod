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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "filesystem.h"
#include "utils.h"

char **get_dir(char *dirname, int *num_files)
{
  DIR *dir = NULL;
  char buff[PATH_MAX];
  struct dirent *entry = NULL;
  char **entries = NULL;
  int items = 0;

  if(dirname) {
    if (!(dir = opendir(dirname))) return entries;
  } else {

    if (!(dir = opendir(getcwd(buff, PATH_MAX)))) return entries;
  }

  while((entry = readdir(dir))) {
    if(filter_ok(dirname, entry->d_name)){
      entries = (char**)realloc(entries, (items+1) * sizeof(char *));
      entries[items] = (char*)malloc((strlen(entry->d_name)+1) * sizeof(char));
      strcpy(entries[items], entry->d_name);
      items++;
    }
  }

  //entries = (char**)realloc(entries, (items+1) * sizeof(char *));
  //entries[items] = NULL;

  *num_files = items;

  closedir(dir);

  return entries;

}

int filter_ok(char *path, char *entry)
{
  int last_dot = 0;
  int i = 0;

  if(strcmp(entry, "..\0") == 0)
    return 1;

  if(strncmp(entry, ".", 1) == 0)
    return 0;

  while(*(entry + i) != '\0') {
    if(*(entry + i) == '.') {
      last_dot = i;
    }
    i++;
  }

  if(last_dot != 0) {
    if((*(entry + last_dot + 1) == 'i'  ||
        *(entry + last_dot + 1) == 'I') &&
       (*(entry + last_dot + 2) == 't'  ||
        *(entry + last_dot + 2) == 'T') &&
       (*(entry + last_dot + 3) == '\0')) {
        return 1;
       }
    if((*(entry + last_dot + 1) == 'x'  ||
        *(entry + last_dot + 1) == 'X') &&
       (*(entry + last_dot + 2) == 'm'  ||
        *(entry + last_dot + 2) == 'M') &&
       (*(entry + last_dot + 3) == '\0')) {
        return 1;
       }
    if((*(entry + last_dot + 1) == 'i'  ||
        *(entry + last_dot + 1) == 'I') &&
       (*(entry + last_dot + 2) == 't'  ||
        *(entry + last_dot + 2) == 'T') &&
       (*(entry + last_dot + 3) == '\0')) {
        return 1;
       }
    if((*(entry + last_dot + 1) == 'm'  ||
        *(entry + last_dot + 1) == 'M') &&
       (*(entry + last_dot + 2) == 'o'  ||
        *(entry + last_dot + 2) == 'O') &&
       (*(entry + last_dot + 3) == 'd'  ||
        *(entry + last_dot + 3) == 'D') &&
       (*(entry + last_dot + 4) == '\0')) {
         return 1;
       }
    if((*(entry + last_dot + 1) == 's'  ||
        *(entry + last_dot + 1) == 'S') &&
        *(entry + last_dot + 2) == '3'  &&
       (*(entry + last_dot + 3) == 'm'  ||
        *(entry + last_dot + 3) == 'M') &&
       (*(entry + last_dot + 4) == '\0')) {
         return 1;
       }

  }

  if(is_dir(path, entry)) {
    return 1;
  }

  return 0;

}

int is_dir(char *path, char *entry)
{
  char *temp;
  struct stat buf;
  int ret = 0;

  temp = mystrcat(path, entry, 1);
  if(stat(temp, &buf) == 0) {
    if(S_ISDIR(buf.st_mode)) {
      ret = 1;
    }
  }

  free(temp);

  return ret;
}

char *dir_up(char *path, char *entry)
{
  if(strcmp(path, "/") != 0) {
    //path = realloc(path, sizeof(char)*(strlen(path)+1+strlen(entry)+1));
    path = strcat(path, "/");
    path = strcat(path, entry);
  } else {
    //path = realloc(path, sizeof(char)*(strlen(path)+strlen(entry)+1));
    path = strcat(path, entry);
  }

  //free(path);
  //path = strdup(temp);

  //free(temp);
  return path;
}
