#ifndef OS_INCLUDE_FS_PATH_H_
#define OS_INCLUDE_FS_PATH_H_
#include <string>
namespace os { namespace fs {
class Path {
 public :
  Path(const char* path);
  ~Path();
  const char* raw_path() const { return raw_; }
  const char* absolute_path() const { return fullpath_; }
  const char* filename() const { return filename_; }
  const char* directory() const { return directory_; }
  const char* extension() const {return ext_;}
  bool HasFilename() const { return filename_ != NULL; }
  bool HasDirectory() const { return directory_ != NULL; }
  bool HasAbsolutePath() const { return fullpath_ != NULL; }
  bool HasExtension() const { return ext_ != NULL; }
  static const char* current_directory();
  static const char* home_directory();
  static const char* relative_path(const char* base, const char* dest, std::string* buf);
  static void NormalizePath(const char* path, std::string* buf);
 private :
  static void ReleaseCurrentDirectory();
  static void ReleaseHomeDirectory();
  char* raw_;
  char* fullpath_;
  char* filename_;
  char* directory_;
  char* ext_;
  static char* current_dir_;
  static char* user_home_;
};
}}
#endif
