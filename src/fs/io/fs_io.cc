#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <utilities.h>
#include <defines.h>
#include <fs/fs_io.h>
#include <fs/fs_stat.h>
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

void InternalReadAll(std::string* string, FILE* fp, const char* path) {
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  if (size == 0) {
    Stat stat(path);
    size = stat.Size();
    if (size == 0) {
      string->append("");
      return;
    }
  }
  rewind(fp);
  char* read_buf = new char[size];
  string->reserve(size);
  for (int i = 0; i < size;) {
    int read = fread(read_buf + i, sizeof(char), size - i, fp);
    i += read;
  }
  read_buf[size - 1] = 0;
  string->append(read_buf);
  delete []read_buf;
  rewind(fp);
}

void InternalReadByte(std::stringstream* st, FILE* fp, int size) {
  char ch;
  int count = -1;
  while ((ch = fgetc(fp)) != NULL) {
    if (size > count) {
      (*st) << ch;
    } else {
      break;
    }
    count++;
  }
}

void File::Read(std::string* buffer) {
  if (fp_ != NULL) {
    InternalReadAll(buffer, fp_, path_);
  }
}

std::string File::Read() {
  if (fp_ != NULL) {
    std::string string;
    InternalReadAll(&string, fp_, path_);
    return string;
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

void File::Write(char ch) {
  if (fp_ != NULL) {
    char write[2];
    write[0] = ch;
    write[1] = 0;
    fwrite(write, sizeof(char), 1, fp_);
  }
}

void File::Write(const char* str) {
  if (fp_ != NULL) {
    fwrite(str, sizeof(char), strlen(str), fp_);
  }
}

void File::Write(const char* str, int size) {
  if (fp_ != NULL) {
    fwrite(str, sizeof(char), size, fp_);
  }
}

void File::Write(const std::string& str) {
  if (fp_ != NULL) {
    fwrite(str.c_str(), sizeof(char), str.size(), fp_);
  }
}

void File::Write(const std::string& str, int size) {
  if (fp_ != NULL) {
    fwrite(str.c_str(), sizeof(char), size, fp_);
  }
}

void File::Close() {
  if (fp_ != NULL) {
    os::FClose(fp_);
    fp_ = NULL;
  }
}

File& File::operator << (const char* str) {
  Write(str);
  return *this;
}

File& File::operator << (const std::string& str) {
  Write(str);
  return *this;
}

File& File::operator << (char ch) {
  Write(ch);
  return *this;
}

File& File::operator >> (std::string& buf) {
  Read(&buf);
  return *this;
}

bool File::IsValid() const {return fp_ != NULL;}

void Read(const char* path, std::string* buffer) {
  File file(path, "rb");
  file.Read(buffer);
}

std::string Read(const char* path) {
  FILE *fp = os::FOpen(path, "rb");
  if (fp != NULL) {
    std::string string;
    InternalReadAll(&string, fp, path);
    return string;
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

std::ostream& operator << (std::ostream& os, os::fs::File& file) {
  std::string str;
  file.Read(&str);
  os << str;
  return os;
}

