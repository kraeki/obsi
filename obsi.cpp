#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> // strerror
#include <stdlib.h> // system, realpath
#include <limits.h> // realpath
#include <string>   // string

#include <dirent.h> // opendir
#include <sys/stat.h> // stat
#include <map>


#define EVENT_LEN (sizeof(inotify_event))
#define BUFFER_LEN ( 1024 * EVENT_LEN )

#define CHECK( v ) \
if( ev->mask & (v) ) \
  printf( "info: event has %-20s %s\n", (#v), file.c_str( ))

std::map<int, std::string> dir_watched;

int isDir( const char *file_path )
{
  struct stat s;
  stat( file_path, &s );
  return S_ISDIR( s.st_mode );
}

int isFile( const char *file_path )
{
  struct stat s;
  stat( file_path, &s );
  return S_ISREG( s.st_mode );
}

bool addWatchRecursively( int fd,  const char* topdir )
{
  DIR *dir;
  char b[512];
  struct dirent *ent;
  bool ret = true;

  dir = opendir( topdir );
  if( dir == NULL )
  {
    fprintf( stderr, "error: failed to walk directory '%s'\n", topdir );
    return false;
  }

  while( (ent = readdir( dir )) != NULL )
  {
    // skip "." or ".."
    if( strcmp( ent->d_name, "." ) == 0 || strcmp( ent->d_name, ".." ) == 0 )
      continue;

    char entpath[FILENAME_MAX];
    entpath[0] = '\0';
    strcat( entpath, topdir );
    strcat( entpath, "/" );
    strcat( entpath, ent->d_name );

    if( !isDir( entpath )) // directory
      continue;
    ret &= addWatchRecursively( fd, entpath );
  }
  closedir(dir);

  // add watch
  //printf( "\n\t add watch for dir \"%s\"\n", topdir );
  int wd = inotify_add_watch( fd, topdir, IN_ALL_EVENTS );
  if( wd < 0 )
  {
    printf( "error: could not add %s to watch list", topdir );
    return false;
  }
  dir_watched.insert( std::pair<int, std::string>( wd, topdir ));

  return ret;
}

void run( const char *watch, const char *script )
{
  int fd = inotify_init( );
  if( fd < 0 )
  {
    printf( "error: inotify_init( ) failed" );
    return;
  }
  // add watch recursively
  if( !addWatchRecursively( fd, watch ))
  {
    printf( "error: add watch failed\n" );
    return;
  }

  int bread = 0;
  inotify_event ev;
  while( true )
  {
    char buffer[BUFFER_LEN];
    bread = read( fd, buffer, BUFFER_LEN );
    if( bread < 0 )
    {
      printf( "error: could not read: %d %s\n", errno, strerror( errno ));
      return;
    }

    int i = 0;
    while( i < bread )
    {
      inotify_event *ev = (inotify_event*)buffer + i;
      if( ev->len > 0 )
      {
        //printf( "i=%d, event occured on (%d) %s\n", i, ev->len, ev->name );
        char tmp[20];
        sprintf( tmp, "%d", ev->mask );
        std::string file = dir_watched[ev->wd] + "/" + std::string( ev->name );

        std::string command( script );
        command += " " + std::string( tmp ) + " " + file;
        int ret = system( command.c_str( ));

        CHECK( IN_ACCESS );
        CHECK( IN_ATTRIB );
        CHECK( IN_CLOSE_WRITE );
        CHECK( IN_CLOSE_NOWRITE );
        CHECK( IN_CREATE );
        CHECK( IN_DELETE );
        CHECK( IN_DELETE_SELF );
        CHECK( IN_MODIFY );
        CHECK( IN_MOVE_SELF );
        CHECK( IN_MOVED_FROM );
        CHECK( IN_MOVED_TO );
        CHECK( IN_OPEN );
        // add or remove watchdirs
        if( ev->mask & IN_DELETE_SELF )
        {
          dir_watched.erase( ev->wd );
        }
        else if( ev->mask & IN_CREATE )
        {
          if( !isDir( file.c_str( )))
            break;
          if( !addWatchRecursively( fd, file.c_str( )))
          {
            printf( "error: add watch failed\n" );
            return;
          }
        }
      }
      i += EVENT_LEN;
    }
  }
  close( fd );
}

int main( int argc, char** arg )
{
  if( argc < 3 )
  {
    printf( "usage: obsi <script> <observe-dir>\n" );
    return 1;
  }
  char *watch = realpath( arg[2], NULL );
  if( !watch )
  {
    printf( "error: directory not found: %s\n", arg[2] );
    free( watch );
    return 1;
  }

  if( !watch )
  {
    printf( "error: directory not found: %s\n", arg[2] );
    free( watch );
    return 1;
  }

  if( !isFile( arg[1] ))
  {
    printf( "error: file not found: %s\n", arg[1] );
    return 1;
  }

  run( watch, arg[1] );

  free( watch );
  return 0;
}

