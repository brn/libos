#ifdef PLATFORM_WIN32
#if DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
#include "../include/fs.h"
#include "../include/utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
class t {
 public :
  bool operator()(os::fs::DirEntry* ent) {
    os::fs::Path path(ent->abspath());
    if (path.HasExtension()) {
      return strcmp(path.extension(), "js") == 0;
    }
    return false;
  }
};
typedef bool (callback)(os::fs::DirEntry*);
void test_func( callback fn){
  printf("ok\n");
}

int main (int argc, char** args) {
#ifdef PLATFORM_WIN32
  ::_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
  printf("%s\n", os::fs::Path::current_directory());
  /*const char* target = (argc > 1)? args[1] : "../";
  os::fs::DirectoryIterator it(target, true);
  for (; it != os::fs::DirectoryIterator::end(); ++it) {
     os::Printf("%s\n", it->abspath());
     }*/
  os::fs::MakeDirectory(args[1], 0777);
  //os::fs::File file("os.gyp", "r");
  //std::cout << file << std::endl;
  //os::Printf("%s\n", os::fs::Read("test/test.cc").c_str());
  /*os::fs::CopyTree("test", "test2");
  printf("%s\n", os::fs::Path::current_directory());
  os::fs::Remove("test2");*/
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
