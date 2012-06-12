#ifndef OS_INCLUDE_FS_DIRECOTRY_H_
#define OS_INCLUDE_FS_DIRECOTRY_H_
namespace os { namespace fs {
struct DirData;
class Dir;
class DirEntry {
  friend class Directory;
  friend struct DirData;
  friend class Dir;
 public :
  ~DirEntry() {
    free(name_);
    free(dir_);
    free(full_path_);
  };
  const char* name() const { return name_; };
  const char* abspath() const { return full_path_; };
  const char* dirname() const { return dir_; };
  bool IsDir() const {return is_dir_;}
  bool IsFile() const {return !is_dir_;}  
 private :
  DirEntry(){}
  DirEntry(const char* path, const char* dir, const char* fullpath, bool is_dir)
      : is_dir_(is_dir) {
    name_ = os::Strdup(path);
    dir_ = os::Strdup(dir);
    full_path_ = os::Strdup(fullpath);
  }
  void Swap(const char* path, const char* dir, const char* fullpath, bool is_dir) {
    free(name_);
    free(dir_);
    free(full_path_);
    name_ = os::Strdup(path);
    dir_ = os::Strdup(dir);
    full_path_ = os::Strdup(fullpath);
    is_dir_ = is_dir;
  }
  bool is_dir_;
  char* name_;
  char* dir_;
  char* full_path_;
};

class directory_iterator : public std::iterator<std::forward_iterator_tag, const DirEntry*> {
 public :
  directory_iterator(const char* path, bool recursive = false);
  directory_iterator(const std::string& path, bool recursive = false);
  directory_iterator(const Path& path_info, bool recursive = false);
  template <typename FilterT>
  directory_iterator(const char* path, FilterT f, bool recursive = false)
      : recursive_(recursive),
        dir_data_(NULL),
        current_entry_(NULL),
        filter_base_(new Filter<FilterT>(f)) {Initialize(path);}
  
  directory_iterator();
  directory_iterator(const directory_iterator&);
  ~directory_iterator();
  const directory_iterator& operator = (const directory_iterator&);
  const DirEntry* operator*() const;
  const DirEntry* operator->() const;
  directory_iterator& operator ++();
  bool operator !=(const directory_iterator&) const;
  static const directory_iterator& end();
 private :
  class FilterBase {
   public :
    virtual bool Invoke(DirEntry* ent) = 0;
  };
  template <typename T>
  class Filter : public FilterBase {
   public :
    Filter(T fn)
    : fn_(fn) {}
    virtual bool Invoke(DirEntry* ent) {return Dereferrence<T>::Get(fn_)(ent);}
   private :
    T fn_;
  };
  void Initialize(const char* path);
  void CreateDirEnt();
  void ReadDirectory(bool);
  bool recursive_;
  std::vector<std::string> sub_;
  DirData* dir_data_;
  DirEntry* current_entry_;
  FilterBase* filter_base_;
};
}}
#endif
