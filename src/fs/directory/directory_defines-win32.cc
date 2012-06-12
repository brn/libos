#include <fs/directory/directory_defines-win32.h>
namespace os { namespace fs {
bool Dir::Open(const char* path, DirData* data) {
  os::fs::Path path_info(path);
  os::fs::Stat stat(path_info.absolute_path());
  bool success = false;
  if (stat.IsExist()) {
    data->dirpath.assign(path_info.absolute_path());
    std::string buf;
    os::SPrintf(&buf, "%s/*", path_info.absolute_path());
    data->h_find = FindFirstFile(buf.c_str(), &(data->ffdata));
    success = data->h_find != INVALID_HANDLE_VALUE;
    if (success) {
      while (strcmp(data->ffdata.cFileName, ".") == 0 || strcmp(data->ffdata.cFileName, "..") == 0) {
        success = FindNextFile(data->h_find, &(data->ffdata));
        if (!success) {break;}
      }
    }
  } else {
    data->h_find = INVALID_HANDLE_VALUE;
    memset(&(data->ffdata), 0, sizeof(data->ffdata));
  }
  return success;
}

void Dir::Close(DirData* data) {
  if (data->h_find != INVALID_HANDLE_VALUE) {
    FindClose(data->h_find);
  }
}

bool Dir::NextEntry(DirData* data) {
  return FindNextFile(data->h_find, &(data->ffdata));
}

DirEntry* Dir::CreateFirstEntry(DirData* data) {
  if (data->h_find != INVALID_HANDLE_VALUE) {
    bool is_dir = data->ffdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    std::string abspath;
    os::SPrintf(&abspath, "%s/%s", data->dirpath.c_str(), data->ffdata.cFileName);
    return new DirEntry(data->ffdata.cFileName, data->dirpath.c_str(), abspath.c_str(), is_dir);
  } else {
    return NULL;
  }
}

DirEntry* Dir::CreateNextEntry(DirData* data, DirEntry* entry) {
  if (data->h_find != INVALID_HANDLE_VALUE) {
    bool is_dir = data->ffdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    std::string abspath;
    os::SPrintf(&abspath, "%s/%s", data->dirpath.c_str(), data->ffdata.cFileName);
    entry->Swap(data->ffdata.cFileName, data->dirpath.c_str(), abspath.c_str(), is_dir);
    return entry;
  } else {
    return NULL;
  }
}

}}

