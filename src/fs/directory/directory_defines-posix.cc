#include <fs/directory/directory_defines-posix.h>

namespace os { namespace fs {

bool Dir::Open(const char* path, DirData* data) {
  os::fs::Path path_info(path);
  os::fs::Stat stat(path_info.absolute_path());
  if (stat.IsExist()) {
    data->dirpath.assign(path_info.absolute_path());
    data->dir = opendir(path_info.absolute_path());
    memset(&(data->entry), 0, sizeof(data->entry));
    memset(&(data->result), 0, sizeof(data->result));
    bool success = readdir_r(data->dir, &(data->entry), &(data->result)) == 0 && data->dir != NULL;
    while (success && (strcmp(data->result->d_name, ".") == 0 || strcmp(data->result->d_name, "..") == 0)) {
      success = (readdir_r(data->dir, (&data->entry), &(data->result)) == 0 && data->result != NULL);
    }
    return success;
  } else {
    data->dir = NULL;
    return false;
  }
}

void Dir::Close(DirData* data) {
  if (data->dir != NULL) {
    closedir(data->dir);
  }
  data->dir = NULL;
  data->result = NULL;
  memset(&(data->entry), 0, sizeof(data->entry));
}

bool Dir::NextEntry(DirData* data) {
  return readdir_r(data->dir, &(data->entry), &(data->result)) == 0 && data->result != NULL;
}

DirEntry* Dir::CreateFirstEntry(DirData* data, memory::Pool* pool) {
  if (data->dir != NULL) {
    std::string abspath;
    os::SPrintf(&abspath, "%s/%s", data->dirpath.c_str(), data->result->d_name);
    os::fs::Stat stat(abspath.c_str());
    bool is_dir = stat.IsDir();
    os::fs::Path file_path_info(abspath.c_str());
    os::fs::Path dir_path_info(data->dirpath.c_str());
    return new(pool) DirEntry(data->result->d_name, dir_path_info.absolute_path(), file_path_info.absolute_path(), is_dir);
  } else {
    return NULL;
  }
}

DirEntry* Dir::CreateNextEntry(DirData* data, memory::Pool* pool) {
  if (data->dir != NULL) {
    std::string abspath;
    os::SPrintf(&abspath, "%s/%s", data->dirpath.c_str(), data->result->d_name);
    os::fs::Stat stat(abspath.c_str());
    bool is_dir = stat.IsDir();
    os::fs::Path path_info(abspath.c_str());
    return new(pool) DirEntry(data->result->d_name, path_info.directory(), path_info.absolute_path(), is_dir);
  } else {
    return NULL;
  }
}

}}
