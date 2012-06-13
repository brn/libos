#ifndef INCLUDE_FS_INCLUDE_FS_IO_H_
#define INCLUDE_FS_INCLUDE_FS_IO_H_

namespace os { namespace fs {

class File {
 public :
  File(const char* path, const char* mode);
  ~File();
  void Read(std::string* buffer);
  void Read(int size, std::string* buffer);
  std::string Read();
  std::string Read(int size);
  void Write(const char* str);
  void Write(const char* str, int size);
  void Write(const std::string& str);
  void Write(const std::string& str, int size);
  void Close();
  const char* path() const;
 private :
  char* path_;
  FILE* fp_;
};

void Read(const char* path, std::string* buffer);
std::string Read(const char* path);
void Write(const char* path);
void Write(const char* path, int size);
void Write(const char* path, const std::string& str);
void Write(const char* path, const std::string& str, int size);

}}

#endif
