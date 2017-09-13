/*
 *  Copyright 2008-2013 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


/*! \file host_vector.h
 *  \brief A dynamically-sizable array of elements which reside in the "host" memory space
 */

#pragma once

#include <thrust/detail/config.h>
#include <memory>
#include <thrust/detail/vector_base.h>
#include <vector>
#include <utility>

namespace thrust
{

// forward declaration of device_vector
template<typename T, typename Alloc> class device_vector;

/*! \addtogroup container_classes Container Classes
 *  \addtogroup host_containers Host Containers
 *  \ingroup container_classes
 *  \{
 */

/*! A \p host_vector is a container that supports random access to elements,
 *  constant time removal of elements at the end, and linear time insertion
 *  and removal of elements at the beginning or in the middle. The number of
 *  elements in a \p host_vector may vary dynamically; memory management is
 *  automatic. The memory associated with a \p host_vector resides in the memory
 *  space of the host associated with a parallel device.
 *
 *  \see http://www.sgi.com/tech/stl/Vector.html
 *  \see device_vector
 */
template<typename T, typename Alloc = std::allocator<T> >
  class host_vector
    : public detail::vector_base<T,Alloc>
{
  private:
    typedef detail::vector_base<T,Alloc> Parent;

  public:
    /*! \cond
     */
    typedef typename Parent::size_type  size_type;
    typedef typename Parent::value_type value_type;
    /*! \endcond
     */

    /*! This constructor creates an empty \p host_vector.
     */
    __host__
    host_vector(void)
      :Parent() {}

    /*! The destructor erases the elements.
     */
    //  Define an empty destructor to explicitly specify
    //  its execution space qualifier, as a workaround for nvcc warning
    __host__
    ~host_vector(void) {}

    /*! This constructor creates a \p host_vector with the given
     *  size.
     *  \param n The number of elements to initially create.
     */
    __host__
    explicit host_vector(size_type n)
      :Parent(n) {}

    /*! This constructor creates a \p host_vector with copies
     *  of an exemplar element.
     *  \param n The number of elements to initially create.
     *  \param value An element to copy.
     */
    __host__
    explicit host_vector(size_type n, const value_type &value)
      :Parent(n,value) {}

    /*! Copy constructor copies from an exemplar \p host_vector.
     *  \param v The \p host_vector to copy.
     */
    __host__
    host_vector(const host_vector &v)
      :Parent(v) {}

  #if __cplusplus >= 201103L
    /*! Move constructor moves from another host_vector.
     *  \param v The host_vector to move.
     */
     __host__
    host_vector(host_vector &&v)
      :Parent(std::move(v)) {}
  #endif

  /*! Assign operator copies from an exemplar \p host_vector.
   *  \param v The \p host_vector to copy.
   */
  __host__
  host_vector &operator=(const host_vector &v)
  { Parent::operator=(v); return *this; }

  #if __cplusplus >= 201103L
    /*! Move assign operator moves from another host_vector.
     *  \param v The host_vector to move.
     */
     __host__
     host_vector &operator=(host_vector &&v)
     { Parent::operator=(std::move(v)); return *this; }
  #endif

    /*! Copy constructor copies from an exemplar \p host_vector with different type.
     *  \param v The \p host_vector to copy.
     */
    template<typename OtherT, typename OtherAlloc>
    __host__
    host_vector(const host_vector<OtherT,OtherAlloc> &v)
      :Parent(v) {}

    /*! Assign operator copies from an exemplar \p host_vector with different type.
     *  \param v The \p host_vector to copy.
     */
    template<typename OtherT, typename OtherAlloc>
    __host__
    host_vector &operator=(const host_vector<OtherT,OtherAlloc> &v)
    { Parent::operator=(v); return *this; }

    /*! Copy constructor copies from an exemplar <tt>std::vector</tt>.
     *  \param v The <tt>std::vector</tt> to copy.
     */
    template<typename OtherT, typename OtherAlloc>
    __host__
    host_vector(const std::vector<OtherT,OtherAlloc> &v)
      :Parent(v) {}

    /*! Assign operator copies from an exemplar <tt>std::vector</tt>.
     *  \param v The <tt>std::vector</tt> to copy.
     */
    template<typename OtherT, typename OtherAlloc>
    __host__
    host_vector &operator=(const std::vector<OtherT,OtherAlloc> &v)
    { Parent::operator=(v); return *this;}

    /*! Copy constructor copies from an exemplar \p device_vector with possibly different type.
     *  \param v The \p device_vector to copy.
     */
    template<typename OtherT, typename OtherAlloc>
    __host__
    host_vector(const device_vector<OtherT,OtherAlloc> &v);

    /*! Assign operator copies from an exemplar \p device_vector.
     *  \param v The \p device_vector to copy.
     */
    template<typename OtherT, typename OtherAlloc>
    __host__
    host_vector &operator=(const device_vector<OtherT,OtherAlloc> &v)
    { Parent::operator=(v); return *this; }

    /*! This constructor builds a \p host_vector from a range.
     *  \param first The beginning of the range.
     *  \param last The end of the range.
     */
    template<typename InputIterator>
    __host__
    host_vector(InputIterator first, InputIterator last)
      :Parent(first, last) {}

// declare these members for the purpose of Doxygenating them
// they actually exist in a derived-from class
#if 0
    /*! \brief Resizes this vector to the specified number of elements.
     *  \param new_size Number of elements this vector should contain.
     *  \param x Data with which new elements should be populated.
     *  \throw std::length_error If n exceeds max_size().
     *
     *  This method will resize this vector to the specified number of
     *  elements.  If the number is smaller than this vector's current
     *  size this vector is truncated, otherwise this vector is
     *  extended and new elements are populated with given data.
     */
    void resize(size_type new_size, const value_type &x = value_type());

    /*! Returns the number of elements in this vector.
     */
    size_type size(void) const;

    /*! Returns the size() of the largest possible vector.
     *  \return The largest possible return value of size().
     */
    size_type max_size(void) const;

    /*! \brief If n is less than or equal to capacity(), this call has no effect.
     *         Otherwise, this method is a request for allocation of additional memory. If
     *         the request is successful, then capacity() is greater than or equal to
     *         n; otherwise, capacity() is unchanged. In either case, size() is unchanged.
     *  \throw std::length_error If n exceeds max_size().
     */
    void reserve(size_type n);

    /*! Returns the number of elements which have been reserved in this
     *  vector.
     */
    size_type capacity(void) const;

    /*! This method shrinks the capacity of this vector to exactly
     *  fit its elements.
     */
    void shrink_to_fit(void);

    /*! \brief Subscript access to the data contained in this vector_dev.
     *  \param n The index of the element for which data should be accessed.
     *  \return Read/write reference to data.
     *
     *  This operator allows for easy, array-style, data access.
     *  Note that data access with this operator is unchecked and
     *  out_of_range lookups are not defined.
     */
    reference operator[](size_type n);

    /*! \brief Subscript read access to the data contained in this vector_dev.
     *  \param n The index of the element for which data should be accessed.
     *  \return Read reference to data.
     *
     *  This operator allows for easy, array-style, data access.
     *  Note that data access with this operator is unchecked and
     *  out_of_range lookups are not defined.
     */
    const_reference operator[](size_type n) const;

    /*! This method returns an iterator pointing to the beginning of
     *  this vector.
     *  \return mStart
     */
    iterator begin(void);

    /*! This method returns a const_iterator pointing to the beginning
     *  of this vector.
     *  \return mStart
     */
    const_iterator begin(void) const;

    /*! This method returns a const_iterator pointing to the beginning
     *  of this vector.
     *  \return mStart
     */
    const_iterator cbegin(void) const;

    /*! This method returns a reverse_iterator pointing to the beginning of
     *  this vector's reversed sequence.
     *  \return A reverse_iterator pointing to the beginning of this
     *          vector's reversed sequence.
     */
    reverse_iterator rbegin(void);

    /*! This method returns a const_reverse_iterator pointing to the beginning of
     *  this vector's reversed sequence.
     *  \return A const_reverse_iterator pointing to the beginning of this
     *          vector's reversed sequence.
     */
    const_reverse_iterator rbegin(void) const;

    /*! This method returns a const_reverse_iterator pointing to the beginning of
     *  this vector's reversed sequence.
     *  \return A const_reverse_iterator pointing to the beginning of this
     *          vector's reversed sequence.
     */
    const_reverse_iterator crbegin(void) const;

    /*! This method returns an iterator pointing to one element past the
     *  last of this vector.
     *  \return begin() + size().
     */
    iterator end(void);

    /*! This method returns a const_iterator pointing to one element past the
     *  last of this vector.
     *  \return begin() + size().
     */
    const_iterator end(void) const;

    /*! This method returns a const_iterator pointing to one element past the
     *  last of this vector.
     *  \return begin() + size().
     */
    const_iterator cend(void) const;

    /*! This method returns a reverse_iterator pointing to one element past the
     *  last of this vector's reversed sequence.
     *  \return rbegin() + size().
     */
    reverse_iterator rend(void);

    /*! This method returns a const_reverse_iterator pointing to one element past the
     *  last of this vector's reversed sequence.
     *  \return rbegin() + size().
     */
    const_reverse_iterator rend(void) const;

    /*! This method returns a const_reverse_iterator pointing to one element past the
     *  last of this vector's reversed sequence.
     *  \return rbegin() + size().
     */
    const_reverse_iterator crend(void) const;

    /*! This method returns a const_reference referring to the first element of this
     *  vector.
     *  \return The first element of this vector.
     */
    const_reference front(void) const;

    /*! This method returns a reference pointing to the first element of this
     *  vector.
     *  \return The first element of this vector.
     */
    reference front(void);

    /*! This method returns a const reference pointing to the last element of
     *  this vector.
     *  \return The last element of this vector.
     */
    const_reference back(void) const;

    /*! This method returns a reference referring to the last element of
     *  this vector_dev.
     *  \return The last element of this vector.
     */
    reference back(void);

    /*! This method returns a pointer to this vector's first element.
     *  \return A pointer to the first element of this vector.
     */
    pointer data(void);

    /*! This method returns a const_pointer to this vector's first element.
     *  \return a const_pointer to the first element of this vector.
     */
    const_pointer data(void) const;

    /*! This method resizes this vector to 0.
     */
    void clear(void);

    /*! This method returns true iff size() == 0.
     *  \return true if size() == 0; false, otherwise.
     */
    bool empty(void) const;

    /*! This method appends the given element to the end of this vector.
     *  \param x The element to append.
     */
    void push_back(const value_type &x);

    /*! This method erases the last element of this vector, invalidating
     *  all iterators and references to it.
     */
    void pop_back(void);

    /*! This method swaps the contents of this vector_base with another vector.
     *  \param v The vector with which to swap.
     */
    void swap(host_vector &v);

    /*! This method removes the element at position pos.
     *  \param pos The position of the element of interest.
     *  \return An iterator pointing to the new location of the element that followed the element
     *          at position pos.
     */
    iterator erase(iterator pos);

    /*! This method removes the range of elements [first,last) from this vector.
     *  \param first The beginning of the range of elements to remove.
     *  \param last The end of the range of elements to remove.
     *  \return An iterator pointing to the new location of the element that followed the last
     *          element in the sequence [first,last).
     */
    iterator erase(iterator first, iterator last);

    /*! This method inserts a single copy of a given exemplar value at the
     *  specified position in this vector.
     *  \param position The insertion position.
     *  \param x The exemplar element to copy & insert.
     *  \return An iterator pointing to the newly inserted element.
     */
    iterator insert(iterator position, const T &x); 

    /*! This method inserts a copy of an exemplar value to a range at the
     *  specified position in this vector.
     *  \param position The insertion position
     *  \param n The number of insertions to perform.
     *  \param x The value to replicate and insert.
     */
    void insert(iterator position, size_type n, const T &x);

    /*! This method inserts a copy of an input range at the specified position
     *  in this vector.
     *  \param position The insertion position.
     *  \param first The beginning of the range to copy.
     *  \param last  The end of the range to copy.
     *
     *  \tparam InputIterator is a model of <a href="http://www.sgi.com/tech/stl/InputIterator.html>Input Iterator</a>,
     *                        and \p InputIterator's \c value_type is a model of <a href="http://www.sgi.com/tech/stl/Assignable.html">Assignable</a>.
     */
    template<typename InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);

    /*! This version of \p assign replicates a given exemplar
     *  \p n times into this vector.
     *  \param n The number of times to copy \p x.
     *  \param x The exemplar element to replicate.
     */
    void assign(size_type n, const T &x);

    /*! This version of \p assign makes this vector a copy of a given input range.
     *  \param first The beginning of the range to copy.
     *  \param last  The end of the range to copy.
     *
     *  \tparam InputIterator is a model of <a href="http://www.sgi.com/tech/stl/InputIterator">Input Iterator</a>.
     */
    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last);

    /*! This method returns a copy of this vector's allocator.
     *  \return A copy of the alloctor used by this vector.
     */
    allocator_type get_allocator(void) const;
#endif // end doxygen-only members
}; // end host_vector

/*! \}
 */

} // end thrust

#include <thrust/detail/host_vector.inl>

