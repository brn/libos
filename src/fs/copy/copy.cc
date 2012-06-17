#include <string>
#include <utilities.h>
#include <fs.h>
namespace os { namespace fs {

bool Copy(const char* src, const char* dest) {
  Stat stat(src);
  if (stat.IsExist() && !stat.IsDir()) {
    File src_file(src, "rb");
    File dest_file(dest, "w+b");
    if (src_file.IsValid() && dest_file.IsValid()) {
      dest_file.Write(src_file.Read());
      return true;
    } else {
      return false;
    }
  }
  return false;
}

bool Copy(const std::string& src, const std::string& dest) {
  return Copy(src.c_str(), dest.c_str());
}

bool CopyTree(const char* src, const char* dest) {
  Stat stat(src);
  if (stat.IsExist()) {
    if (!stat.IsDir()) {
      return Copy(src, dest);
    }
    Path src_info(src);
    Path dest_info(dest);
    if (!MakeDirectory(dest_info.absolute_path(), 0777)) {
      return false;
    }
    DirectoryIterator it(src_info.absolute_path());
    for (; it != DirectoryIterator::end(); ++it) {
      const char* path = it->abspath();
      if (it->IsDir()) {
        std::string buf;
        os::SPrintf(&buf, "%s/%s", dest_info.absolute_path(), it->name());
        if (!CopyTree(path, buf.c_str())) {
          return false;
        }
      } else {
        std::string buf;
        os::SPrintf(&buf, "%s/%s", dest_info.absolute_path(), it->name());
        if (!Copy(path, buf.c_str())) {
          return false;
        }
      }
    }
    return true;
  }
}

bool CopyTree(const std::string& src, const std::string& dest) {
  return CopyTree(src.c_str(), dest.c_str());
}

}}
