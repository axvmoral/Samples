/**
 * @file linked_list_raw_pointers.h
 *
 * The beginnings of a doubly linked list class, using raw pointers.
 *
 * @author Axel V. Morales Sanchez for CS 19, asmorales@jeff.cabrillo.cis.edu
 */
#ifndef LINKED_LIST_RAW_POINTERS_H_
#define LINKED_LIST_RAW_POINTERS_H_

#include <initializer_list>
#include <iostream>

namespace cs19 {

/**
 * Class LinkedList is an archetypal doubly linked list. The public API exposes some standard list
 * operations. Node structures and internal list details are kept private.
 *
 * Also note that all member functions are defined inside the class declaration. (No separate .h and
 * .cpp/.cc files.) Template classes are usually defined this way.
 *
 * @tparam T type of the list elements
 */
template <typename T>
class LinkedList {
  struct Node;  // forward declaration for our private Node type

 public:
  LinkedList(){
  }
  /** Constructs a list with a copy of each of the elements in `init_list`, in the same order. */
  LinkedList(std::initializer_list<T> init_list) {
    for (const T& val : init_list)
      this->push_back(val);
  }
  LinkedList(const LinkedList<T> &another) {
    for (Node* pos = another.head_; pos != NULL; pos = pos->next) {
      this->push_back(pos->data);
    }
  }
  /** Destroys each of the contained elements, and deallocates all memory allocated by this list. */
  ~LinkedList() {
     while (this->head_) {
       Node* old_head = this->head_;
       this->head_ = old_head->next;
       delete old_head;
    }
  }
  std::size_t size() const {
    return this->size_;
  }
  bool empty() const {
    return !static_cast<bool>(this->size_);
  }
  T & front() const {
    if (this->head_ != NULL) {
      return this->head_->data;
    } else {
      throw std::domain_error("List Empty");
    }
  }
  T & back() const {
    if (this->head_ != NULL) {
      return this->tail_->data;
    } else {
      throw std::domain_error("List Empty");
    }
  }
  /** Appends a copy of `val` to this list. */
  void push_back(const T& val) {
    Node* new_node = new Node{val};
    if (this->size_ == 0) {
      this->head_ = this->tail_ = new_node;
    } else {
      this->tail_->next = new_node;
      new_node->prev = this->tail_;
      this->tail_ = new_node;
    }
    ++this->size_;
  }
  /**  Prepends a copy of `val` to this list. */
  void push_front(const T& val) {
    Node* new_node = new Node{val};
    if (this->size_ == 0) {
      this->head_ = this->tail_ = new_node;
    } else {
      new_node->next = this->head_;
      this->head_->prev = new_node;
      this->head_ = new_node;
    }
    ++this->size_;
  }
  void pop_front() {
    if (this->head_ != NULL) {
      Node* del = this->head_;
      this->head_ = this->head_->next;
      if (this->head_ != NULL) {
        this->head_->prev = NULL;
      } else {
        this->tail_ = nullptr;
      }
      delete del;
      --this->size_;
    }
  }
  void pop_back() {
    if (this->tail_ != NULL) {
      Node* del = this->tail_;
      this->tail_ = this->tail_->prev;
      if (this->tail_ != NULL) {
        this->tail_->next = nullptr;
      } else {
        this->head_ = nullptr;
      }
      delete del;
      --this->size_;
    }
  }
  void resize(std::size_t n) {
    for (; this->size_ > n;) {
      this->pop_back();
    }
  }
  void resize(std::size_t n, const T& fill_value) {
    for (; this->size_ > n;) {
      this->pop_back();
    }
    for (; this->size_ < n;) {
      this->push_back(fill_value);
    }
  }
  void clear() {
    for (; this->size_ != 0;) {
      this->pop_front();
    }
  }
  void remove(const T &val) {
    for (Node* pos = this->head_; pos != NULL;) {
      if (pos->data == val) {
        Node* temp = pos->next;
        if (pos == this->head_) {
          this->head_ = pos->next;
        }
        if (pos->next != NULL) {
          pos->next->prev = pos->prev;
        }
        if (pos->prev != NULL) {
          pos->prev->next = pos->next;
        }
        if (pos == this->tail_) {
          this->tail_ = pos->prev;
        }
        delete pos;
        --this->size_;
        pos = temp;
      } else {
        pos = pos->next;
      }
    }
  }
  void unique() {
    if (this->size_ > 1) {
      for (Node* out = this->head_; out != NULL; out = out->next) {
        Node* in = out->next;
        while (in != NULL) {
          if (out->data == in->data) {
            Node* new_in = in->next;
            if (in->next != NULL) {
              in->next->prev = in->prev;
            } else {
              this->tail_ = in->prev;
            }
            if (in->prev != NULL) {
              in->prev->next = in->next;
            }
            delete in;
            --this->size_;
            in = new_in;
          } else {
            in = in->next;
          }
        }
      }
    }
  }
  void reverse() {
    Node* forward = this->head_;
    Node* backward = this->tail_;
    int lim = this->size_ / 2;
    for (; lim > 0;) {
      T temp = forward->data;
      forward->data = backward->data;
      backward->data = temp;
      forward = forward->next;
      backward = backward->prev;
      --lim;
    }
  }
  LinkedList& operator=(std::initializer_list<T> init_list) {
    if (init_list.size() < this->size_) {
      this->resize(init_list.size());
    }
    Node* apos = this->head_;
    auto bpos = init_list.begin();
    for (std::size_t i = 0; bpos != init_list.end(); ++i) {
      if (i < this->size_) {
        apos->data = *bpos;
        apos = apos->next;
      } else {
        this->push_back(*bpos);
      }
      ++bpos;
    }
    return *this;
  }
  LinkedList& operator=(const LinkedList& another) {
    if (another.size() < this->size_) {
      this->resize(another.size());
    }
    Node* apos = this->head_;
    Node* bpos = another.head_;
    for (std::size_t i = 0; bpos != NULL; ++i) {
      if (i < this->size_) {
        apos->data = bpos->data;
        apos = apos->next;
      } else {
        this->push_back(bpos->data);
      }
      bpos = bpos->next;
    }
    return *this;
  }
  bool operator==(const LinkedList& another) {
    if (another.size() != this->size_) {
      return false;
    } else {
      Node* apos = this->head_;
      for (Node* bpos = another.head_; bpos != NULL; bpos = bpos->next) {
        if (apos->data != bpos->data) {
          return false;
        }
        apos = apos->next;
      }
      return true;
    }
  }
  bool operator!=(const LinkedList& another) {
    if (another.size() != this->size_) {
      return true;
    } else {
      Node* apos = this->head_;
      for (Node* bpos = another.head_; bpos != NULL; bpos = bpos->next) {
        if (apos->data != bpos->data) {
          return true;
        }
        apos = apos->next;
      }
      return false;
    }
  }
  /** Inserts this list into an ostream, with the format `[element1, element2, element3, ...]` */
  friend std::ostream& operator<<(std::ostream& out, const LinkedList& list) {
    out << '[';
    for (Node* cur = list.head_; cur; cur = cur->next) {
      out << cur->data;
      if (cur->next)
        out << ", ";
    }
    out << ']';
    return out;
  }

 private:
  struct Node {
    T data;
    Node* next = nullptr;
    Node* prev = nullptr;
  };
  Node* head_ = nullptr;
  Node* tail_ = nullptr;
  std::size_t size_ = 0;
};

}  // namespace cs19

#endif  // LINKED_LIST_RAW_POINTERS_H_