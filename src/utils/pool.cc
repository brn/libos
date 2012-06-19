#include <utilities.h>
#include <thread.h>
namespace os {namespace memory {
//Check alignment.
size_t Pool::Align(size_t siz, size_t alignment) {
  return (siz + (alignment - 1)) & ~(alignment - 1);
}

//Free all allocated block.
void Pool::Free() {
  //Call Allocated destructor.
  //Not free.
  if (head_) {
    Allocated* block = head_;
    Allocated* next = block->next_;
    while (1) {
      //Call virtual destructor.
      block->~Allocated();
      if (block->allocated_ == 1) {
        free(block);
      }
      if (next) {
        block = next;
        next->prev_ = 0;
        next = next->next_;
      } else {
        break;
      }
    }
    head_ = current_ = 0;
  }
  if (head_block_ != NULL) {
    Block* tmp = head_block_;
    while (tmp != NULL) {
      Block* next = tmp->next;
      if (tmp->malloced & (1 == 1)) {
        tmp = reinterpret_cast<Block*>(reinterpret_cast<char*>(tmp) - tmp->size);
        free(tmp);
      }
      tmp = next;
    }
  }
  //Free all chunk.
  if (head_chunk_) {
    while (1) {
      Chunk<kDefaultSize>* next = head_chunk_->next();
      delete head_chunk_;
      head_chunk_ = next;
      if (!head_chunk_) {
        break;
      }
    }
  }
}

static os::thread_specific_ptr<Pool> ptr_;

inline Pool* Pool::Local() {
  Pool* pool = reinterpret_cast<Pool*>(ptr_.get());
  if (pool == NULL) {
    pool = new Pool;
    ptr_.reset(pool);
  }
  return pool;
}
}}
