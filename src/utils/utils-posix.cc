/**
 *@author Taketoshi Aono
 *@fileOverview
 *@license
 *Copyright (c) 2011 Taketoshi Aono
 *Licensed under the BSD.
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 *associated doc umentation files (the "Software"), to deal in the Software without restriction,
 *including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 *subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all copies or
 *substantial portions ofthe Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 *TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 *CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *DEALINGS IN THE SOFTWARE.
 */
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <utime.h>
#include <utils/utils.h>

namespace os {

void Strerror(std::string* buf, int err) {
  char buffer[95];
  strerror_r(err, buffer, 95);
  buf->assign(buffer);
}

void Printf(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

void SPrintf(std::string* buffer, const char* format, ...) {
  va_list args;
  va_start(args, format);
  char* buf = NULL;
  vasprintf(&buf, format, args);
  buffer->assign(buf);
  free(buf);
}

void VSPrintf(std::string* buffer, const char* format, va_list args) {
  char* buf = NULL;
  vasprintf(&buf, format, args);
  buffer->assign(buf);
  free(buf);
}

void VFPrintf(FILE* fp, const char* format, va_list arg) {
  vfprintf(fp, format, arg);
}

void FPrintf(FILE* fp, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(fp, format, args);
  va_end(args);
}

FILE* FOpen(const char* filename, const char* mode) {
  FILE* fp = fopen(filename, mode);
  return fp;
}

void FClose(FILE* fp) {
  fclose(fp);
}

void GetEnv(std::string* buf, const char* env) {
  char* ret = getenv(env);
  if (ret != NULL) {
    buf->assign(ret);
    free(ret);
  }
}

bool Sleep(int nano_time) {
  struct timespec req;
  struct timespec rem;
  if (nano_time > 1000) {
    req.tv_sec = nano_time / 1000;
    nano_time = (nano_time << 1) / 10;
  } else {
    req.tv_sec = 0;
  }
  req.tv_nsec = nano_time;
  int ret = nanosleep(&req, &rem);
  return ret == 0 || ret == EINTR;
}

int Utime(const char* path) {
  K_ERRNO = utime(path, NULL);
  return K_ERRNO;
}

time_t Time(time_t* time) {
  return ::time(time);
}

int Asctime(std::string* buf, tm* tm) {
  char buffer[27];
  asctime_r(tm, buffer);
  buf->assign(buffer);
  return 0;
}

int LocalTime(tm* t, time_t* time) {
  localtime_r(time, t);
  return 0;
}

void AtExit(void(*callback)()) {
  atexit(callback);
}

void GetLastError(std::string* buf) {
  Strerror(buf, K_ERRNO);
}

FILE* POpen(const char* name, const char* mode) {
  return popen(name, mode);
}

void PClose(FILE* fp) {
  pclose(fp);
}

char* Strdup(const char* path) {
  return strdup(path);
}
}
