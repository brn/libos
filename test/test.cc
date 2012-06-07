#include "../include/fs.h"
#include "../include/utilities.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** args) {
  printf("%s\n", os::fs::Path::current_directory());
  const char* target = (argc > 1)? args[1] : "../";
  os::fs::directory_iterator it(target, true);
  for (; it != os::fs::directory_iterator::end(); ++it) {
    os::Printf("%s\n", it->abspath());
  }
  /*
  int i;

  const char *dirname = target;
  struct dirent **namelist;
  int r = scandir(dirname, &namelist, NULL, NULL);
  if(r == -1) {
    err(EXIT_FAILURE, "%s", dirname);
  }
  (void) printf ("%d\n", r);
  for (i = 0; i < r; ++i) {
    if (strcmp(namelist[i]->d_name, "..") == 0 || strcmp(namelist[i]->d_name, ".") == 0) {
      continue;
    }
    if (namelist[i]->d_type == DT_DIR) {
      char **argv = new char*[2];
      argv[0] = new char[100];
      argv[1] = new char[100];
      strcpy(argv[0], args[0]);
      sprintf(argv[1], "%s/%s", dirname, namelist[i]->d_name);
      printf("%s\n", argv[1]);
      main(argc, argv);
      delete []argv;
    }
    (void) printf ("%s\n", namelist[i]->d_name);
    free(namelist[i]);
  }
  free(namelist);*/
  return 0;
}
