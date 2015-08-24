#ifndef _CS540_MAP_HPP__
#define _CS540_MAP_HPP__

#include <cstdlib>
#include <stdexcept>
#include <utility>

namespace cs540{

  /* Map Template Class */
  template <typename Key_T, typename Mapped_T> 
  class Map{
  private:
    /* Member */
    typedef std::pair<const Key_T, Mapped_T> ValueType;

    class BNode{
    public:
      BNode *_left;
      BNode *_right;

      BNode *_sentinel;
      BNode *_next;
      BNode *_prev;
      
    public:
      ValueType data;
      
      BNode(){}
      BNode(ValueType const &node) : _left(NULL), _right(NULL), data(node) { }
      
      Mapped_T* search(const Key_T &key){
	if(key == std::get<0>(data)){
	  return &std::get<1>(data);
	}else if(key < std::get<0>(data)){   
	  if(_left == NULL){
	    return NULL;
	  }else{
	    return _left->search(key);
	  }
	}else if(key > std::get<0>(data)){
	  if(_right ==  NULL){
	    return NULL;
	  }else{
	    return _right->search(key);
	  }
	}
	return NULL;
      }
      
      BNode *find(const Key_T &key){
	if(key == std::get<0>(data)){
	  return this;
	}else if(key < std::get<0>(data)){   
	  if(_left == NULL){
	    return NULL;
	  }else{
	    return _left->find(key);
	  }
	}else if(key > std::get<0>(data)){
	  if(_right ==  NULL){
	    return NULL;
	  }else{
	    return _right->find(key);
	  }
	}
	return NULL;
      }
      
      const BNode *find(const Key_T &key) const{
	if(key == std::get<0>(data)){
	  return this;
	}else if(key < std::get<0>(data)){   
	  if(_left == NULL){
	    return NULL;
	  }else{
	    return _left->find(key);
	  }
	}else if(key > std::get<0>(data)){
	  if(_right ==  NULL){
	    return NULL;
	  }else{
	    return _right->find(key);
	  }
	}
	return NULL;
      }
      
      std::pair<BNode *, bool> insert(ValueType const &data){
	if (std::get<0>(data) == std::get<0>(this->data))
	  return std::make_pair(this, false);
	else if (std::get<0>(data) < std::get<0>(this->data)) {
	  if (_left == NULL) {
	      _left = new BNode(data);
	      return std::make_pair(_left, true);
	  } else
	    return _left->insert(data);
	} else if (std::get<0>(data) > std::get<0>(this->data)) {
	  if (_right == NULL) {
	    _right = new BNode(data);
	    return std::make_pair(_right, true);
	  } else
	    return _right->insert(data);
	}
	return std::make_pair(this, false);
      }
      
      BNode *remove(Key_T key, BNode *parent){
	if (key < std::get<0>(data)) {
	  if (_left != NULL){
	    return _left->remove(key, this);
	  }else{
	    return NULL;
	  }
	} else if (key > std::get<0>(data)) {
	  if (_right != NULL){
	    return _right->remove(key, this);
	  }else{
	    return NULL;
	  }
	} else {
	  if (_left != NULL && _right != NULL) {
	    Key_T *mod = const_cast<Key_T *>(&std::get<0>(data));
	    *mod = _right->minValue();
	    return _right->remove(std::get<0>(data), this);
	  } else if (parent->_left == this) {
	    parent->_left = (_left != NULL) ? _left : _right;
	    return this;
	  } else if (parent->_right == this) {
	    parent->_right = (_left != NULL) ? _left : _right;
	    return this;
	  }
	}
	return NULL;
      }

      void deleteTree(const BNode *leaf){
	if(leaf != NULL){
	  deleteTree(leaf->_left);
	  deleteTree(leaf->_right);
	  delete leaf;
	}
      }
      
      Key_T minValue() {
	if (_left == NULL)
            return std::get<0>(data);
	else
	  return _left->minValue();
      }
      
    };

    
    typedef class BinaryTree{      
    public:
      
      size_t _size;
      BNode *root;
      
    public:
      BinaryTree() : _size(0), root(NULL){ }
      ~BinaryTree(){ delete root->_sentinel; root->deleteTree(root); }

      Mapped_T &search(Key_T const &key) const{
	if(root != NULL){
	  Mapped_T *mapped = root->search(key);
	  if(mapped != NULL){
	    return *mapped;
	  }
	}
	throw std::out_of_range("BinaryTree::search() : index is out of range");
      }

      Mapped_T &operator[](const Key_T &key){
	if(root != NULL){
	  Mapped_T *mapped = root->search(key);
	  if(mapped != NULL){
	    return *mapped;
	  }
	}
	throw std::out_of_range("BinaryTree::search() : index is out of range");
      }

      BNode *find(const Key_T &key){
	return root->find(key);
      }

      BNode *find(const Key_T &key) const{
	return root->find(key);
      }

      void insertList(BNode *node){
	root->_sentinel->_prev->_next = node;
	node->_prev = root->_sentinel->_prev;
	root->_sentinel->_prev = node;
	node->_next = root->_sentinel;
      }

      std::pair<BNode *, bool> insert(ValueType const &val){
	if (root == NULL) {
	  root = new BNode(val);
	  root->_sentinel = new BNode();
	  _size = 1;
	  root->_sentinel->_next = root->_sentinel->_prev = root;
	  root->_sentinel->_next->_prev = root->_sentinel;
	  root->_sentinel->_prev->_next = root->_sentinel;
	  return std::pair<BNode *, bool>(root, true);
	} else {
	  BNode *node = std::get<0>(root->insert(val));
	  if(node != NULL){
	    ++_size;
	    insertList(node);
	    return std::pair<BNode *, bool>(node, true);
	  }else{
	    return std::pair<BNode *, bool>(node, false);
	  }
	}
      }

      template <typename IT_T>
      void insert(IT_T range_beg, IT_T range_end){
	for(IT_T it = range_beg; it != range_end; ++it){
	  insert(*it);
	}
      }

      bool remove(const Key_T &key) {
	if (root != NULL){
	  if (std::get<0>(root->data) == key) {
	    BNode auxRoot(std::pair<Key_T, Mapped_T>(0, 0));
	    auxRoot._left = root;
	    BNode *removedNode = root->remove(key, &auxRoot);
	    root = auxRoot._left;
	    if (removedNode != NULL) {
	      --_size;
	      delete removedNode;
	      return true;
	    } else
	      return false;
	  } else {
	    BNode* removedNode = root->remove(key, NULL);
	    if (removedNode != NULL) {
	      removedNode->_next->_prev = removedNode->_prev;
	      removedNode->_prev->_next = removedNode->_next;
	      _size--;
	      delete removedNode;
	      return true;
	    } else
	      return false;
	  }
	}
	return false;
      }

      void clear(){
	if(root != NULL){
	  root->deleteTree(root->_left);
	  root->deleteTree(root->_right);
	}
      }

      size_t size() const{
	return _size;
      }
    } BinaryTree;
    
    
    
    
    
    
    
    BinaryTree _M_t;

  public:
    /* Public Member Functions and Comparison Operators of Map */
    Map(){ }
    Map(const Map &cpy_map){ insert(cpy_map.begin(), cpy_map.end()); }
    Map(Map && mv_map) : _M_t(std::move(mv_map._M_t)){ }
    Map &operator=(const Map &cpy_map){ insert(cpy_map.begin(), cpy_map.end()); return *this; }
    Map &operator=(Map &&mv_map) { _M_t = std::move(mv_map._M_t); return *this;}
    Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> ini_list){ insert(ini_list.begin(), ini_list.end());};
    ~Map(){ }
    
    /* Size */
    size_t size() const{return _M_t.size();}
    bool empty() const{return  _M_t.size() == 0;}

  /* Public Member Functions of Iterator */
    class Iterator{
    public:
      Iterator(){};
      Iterator(BNode *nd) : _ptr(nd){ }
      Iterator(const Iterator &it) : _ptr(it._ptr){ }
      ~Iterator(){ }
      Iterator &operator=(const Iterator &it){_ptr = it._ptr; return *this;}
      Iterator &operator++(){_ptr = _ptr->_next; return *this;}
      Iterator &operator++(int){Iterator *tmp = this; _ptr = _ptr->_next; return *tmp;}
      Iterator &operator--(){_ptr = _ptr->_prev; return *this;}
      Iterator &operator--(int){Iterator *tmp = this; _ptr = _ptr->_prev; return *tmp;}
      ValueType &operator*() const{return _ptr->data;}
      ValueType &operator->() const{return _ptr->data;}

      BNode *_ptr;
    };

    /* Public Member Functions of ConstIterator */
    class ConstIterator{
    public:
      ConstIterator(){};
      ConstIterator(const BNode *nd) : _ptr(nd){ }
      ConstIterator(const ConstIterator &cit) : _ptr(cit._ptr){ }
      ConstIterator(const Iterator &it) : _ptr(it._ptr){ }
      ~ConstIterator(){ }
      ConstIterator &operator=(const ConstIterator &it){_ptr = it._ptr; return *this;}
      ConstIterator &operator++(){_ptr = _ptr->_next; return *this;}
      ConstIterator &operator++(int){ConstIterator *tmp = this; _ptr = _ptr->_next; return *tmp;}
      ConstIterator &operator--(){_ptr = _ptr->_prev; return *this;}
      ConstIterator &operator--(int){ConstIterator *tmp = this; _ptr = _ptr->_prev; return *tmp;}
      const ValueType &operator*() const{return _ptr->data;}
      const ValueType &operator->() const{return _ptr->data;}

      const BNode *_ptr;
    };

    /* Public Member Functions of ReverseIterator */
    class ReverseIterator{
    public:
      ReverseIterator(){};
      ReverseIterator(BNode *nd) : _rptr(nd){ }
      ReverseIterator(const ReverseIterator &rit) : _rptr(rit._rptr){ };
      ReverseIterator(const Iterator &it) : _rptr(it._ptr){ }
      ~ReverseIterator(){ }
      ReverseIterator &operator=(const ReverseIterator &rit){_rptr = rit._rptr; return *this;}
      ReverseIterator &operator++(){ReverseIterator *tmp = this; _rptr = _rptr->_prev; return *tmp;}
      ReverseIterator &operator++(int){_rptr = _rptr->_prev; return *this;}
      ReverseIterator &operator--(){ReverseIterator *tmp = this; _rptr = _rptr->_next; return *tmp;}
      ReverseIterator &operator--(int){_rptr = _rptr->_next; return *this;}
      ValueType &operator*() const{return _rptr->data;}
      ValueType &operator->() const{return _rptr->data;}

      BNode *_rptr;
    };

    /* Iterators */
    Iterator begin(){return Iterator(_M_t.root->_sentinel->_next);}
    Iterator end(){return Iterator(_M_t.root->_sentinel);}
    ConstIterator begin() const{return ConstIterator(_M_t.root->_sentinel->_next);}
    ConstIterator end() const{return ConstIterator(_M_t.root->_sentinel);}
    ReverseIterator rbegin(){return ReverseIterator(_M_t.root->_sentinel->_prev);}
    ReverseIterator rend(){return ReverseIterator(_M_t.root->_sentinel);}

    /* Element Access */
    Iterator find(const Key_T &key){
      Iterator f_it;
      f_it._ptr = _M_t.find(key);
      if(f_it._ptr != NULL){
	return f_it;
      }else{
	return end();
      }
    }

    ConstIterator find(const Key_T &key) const{
      Iterator f_it;
      f_it._ptr = _M_t.find(key);
      if(f_it._ptr != NULL){
	return f_it;
      }else{
	return end();
      }
    }

    Mapped_T &at(const Key_T &key){return _M_t.search(key);}
    const Mapped_T &at(const Key_T &key) const{return _M_t.search(key);}
    Mapped_T &operator[](const Key_T &key){ return _M_t[key];  }
	
    /* Modifiers */
    std::pair<Iterator, bool> insert(const ValueType &data){ 
      std::pair<BNode *, bool> node = _M_t.insert(data);
      return std::make_pair(Iterator(std::get<0>(node)), std::get<1>(node));
    }
    template <typename IT_T>
    void insert(IT_T range_beg, IT_T range_end){_M_t.insert(range_beg, range_end);}
    Iterator insert(std::pair<const Key_T, Mapped_T> && data){ return insert(std::move(data));  }
    void erase(Iterator pos){_M_t.remove(std::get<0>(*pos));}
    void remove(const Key_T &key) {_M_t.remove(key);};
    void clear() {_M_t.clear();}

    /* Comparison Operators for Iterators */
    friend bool operator==(const Iterator &it1, 
			   const Iterator &it2){return it1._ptr == it2._ptr;}
    
    friend bool operator==(const ConstIterator &cit1, 
			   const ConstIterator &cit2){return cit1._ptr == cit2._ptr;}
    
    friend bool operator==(const Iterator &it, 
			   const ConstIterator &cit){return it._ptr == cit._ptr;}
    
    friend bool operator==(const ConstIterator &cit, 
			   const Iterator &it){return cit._ptr == it._ptr;}
    
    friend bool operator!=(const Iterator &it1, 
			   const Iterator &it2){return it1._ptr != it2._ptr;}
    
    friend bool operator!=(const ConstIterator &cit1, 
			   const ConstIterator &cit2){return cit1._ptr != cit2._ptr;}
    
    friend bool operator!=(const Iterator &it, 
			   const ConstIterator &cit){return it._ptr != cit._ptr;}
    
    friend bool operator!=(const ConstIterator &cit, 
			   const Iterator &it){return cit._ptr != it._ptr;}
    
    friend bool operator==(const ReverseIterator &ri1, 
			   const ReverseIterator &ri2){return ri1._rptr == ri2._rptr;}
    
    friend bool operator!=(const ReverseIterator &ri1, 
			   const ReverseIterator &ri2){return ri1._rptr != ri2._rptr;}
  };

    /* Comparison */
    template <typename Key_T, typename Mapped_T> 
    bool operator==(const Map<Key_T, Mapped_T> &m1, const Map<Key_T, Mapped_T> &m2){
      if(m1.size() == m1.size()){
	for(typename Map<Key_T, Mapped_T>::ConstIterator it = m1.begin(), it2 = m2.begin(); it != m1.end() && it2 != m2.end(); ++it, ++it2){
	  if(std::get<0>(*it) != std::get<0>(*it2) || std::get<1>(*it) != std::get<1>(*it2)){
	    return false;
	  }
	}
	return true;
      }
      return false;
    }

    template <typename Key_T, typename Mapped_T> 
    bool operator!=(const Map<Key_T, Mapped_T> &m1, const Map<Key_T, Mapped_T> &m2){
      return !(m1 == m2);
    }

    template <typename Key_T, typename Mapped_T> 
    bool operator<(const Map<Key_T, Mapped_T> &m1, const Map<Key_T, Mapped_T> &m2){
      if(m1.size() < m1.size()){
	return true;
      }else if(m1.size() > m1.size()){
	return false;
      }

      for(typename Map<Key_T, Mapped_T>::ConstIterator it = m1.begin(), it2 = m2.begin(); it != m1.end() && it2 != m2.end(); ++it, ++it2){
	if(std::get<0>(*it) == std::get<0>(*it2)){
	  if(std::get<1>(*it) < std::get<1>(*it2)){
	    return true;
	  }else if(std::get<1>(*it) > std::get<1>(*it2)){
	    return false;
	  }
	}else if(std::get<0>(*it) < std::get<0>(*it2)){
	  return true;
	}else{
	  return false;
	}
      }
      return false;
    }	
}

#endif
