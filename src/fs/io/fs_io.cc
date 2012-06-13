#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <utilities.h>
#include <fs_include/fs_io.h>
namespace os { namespace fs {

File::File(const char* path, const char* mode) {
  fp_ = os::FOpen(path, mode);
  path_ = os::Strdup(path);
}

File::~File() {
  free(path_);
  if (fp_ != NULL) {
    Close();
  }
}

void InternalReadAll(std::stringstream* st, FILE* fp) {
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  rewind(fp);
  for (int i = 0; i < size;) {
    char buf[64];
    int read = fread(&buf, 1, 63, fp);
    buf[read] = 0;
    (*st) << buf;
    i += read;
  }
  rewind(fp);
}

void InternalReadByte(std::stringstream* st, FILE* fp, int size) {
  char ch;
  int count = -1;
  while ((ch = fgetc(fp)) != NULL) {
    if (size > count) {
      (*st) << ch;
    }
    count++;
  }
}

void File::Read(std::string* buffer) {
  if (fp_ != NULL) {
    std::stringstream st;
    InternalReadAll(&st, fp_);
    buffer->assign(st.str());
  }
}

std::string File::Read() {
  if (fp_ != NULL) {
    std::stringstream st;
    InternalReadAll(&st, fp_);
    return st.str();
  }
  return std::string("");
}

void File::Read(int size, std::string* buffer) {
  if (fp_ != NULL) {
    std::stringstream st;
    InternalReadByte(&st, fp_, size);
    buffer->assign(st.str());
  }
}

std::string File::Read(int size) {
  if (fp_ != NULL) {
    std::stringstream st;
    InternalReadByte(&st, fp_, size);
    return st.str();
  }
  return std::string("");
}

void File::Write(const char* str) {
  fwrite(str, sizeof(char), strlen(str), fp_);
}

void File::Write(const char* str, int size) {
  fwrite(str, sizeof(char), size, fp_);
}

void File::Write(const std::string& str) {
  fwrite(str.c_str(), sizeof(char), str.size(), fp_);
}

void File::Write(const std::string& str, int size) {
  fwrite(str.c_str(), sizeof(char), size, fp_);
}

void File::Close() {
  if (fp_ != NULL) {
    os::FClose(fp_);
    fp_ = NULL;
  }
}

void Read(const char* path, std::string* buffer) {
  File file(path, "rb");
  file.Read(buffer);
}

std::string Read(const char* path) {
  FILE *fp = os::FOpen(path, "rb");
  if (fp != NULL) {
    std::stringstream st;
    InternalReadAll(&st, fp);
    return st.str();
  }
  return std::string("");
}

void Write(const char* path, const char* str) {
  File file(path, "wb");
  file.Write(str);
}

void Write(const char* path, const char* str, int size) {
  File file(path, "wb");
  file.Write(str, size);
}

void Write(const char* path, const std::string& str) {
  File file(path, "wb");
  file.Write(str);
}

void Write(const char* path, const std::string& str, int size) {
  File file(path, "wb");
  file.Write(str, size);
}

}}
