
#include <algorithm>
#include <stdexcept>
#include <string>

#include "gc_cpp.h"

namespace zz {

class Ast;

class Ast {
 private:
        struct Base : public gc {
                virtual bool eq(const Base* other) const = 0;
                virtual bool ne(const Base* other) const = 0;
                virtual bool lt(const Base* other) const = 0;
                virtual bool gt(const Base* other) const = 0;
                virtual bool le(const Base* other) const = 0;
                virtual bool ge(const Base* other) const = 0;
        };

        template<typename T> struct Impl : public Base {
                T data;
                Impl(T data) : data{data} {}
                static Impl<T>* cast(Base *p)
                {
                        auto i = dynamic_cast<Impl<T>*>(p);
                        if (i == nullptr)
                                throw std::runtime_error("Bad AST cast");
                        return i;
                }
                static const Impl<T>* cast(const Base *p)
                {
                        auto i = dynamic_cast<const Impl<T>*>(p);
                        if (i == nullptr)
                                throw std::runtime_error("Bad AST cast");
                        return i;
                }
                bool eq(const Base* other) const
                {
                        return data == cast(other)->data;
                }
                bool ne(const Base* other) const
                {
                        return data != cast(other)->data;
                }
                bool lt(const Base* other) const
                {
                        return data < cast(other)->data;
                }
                bool gt(const Base* other) const
                {
                        return data > cast(other)->data;
                }
                bool le(const Base* other) const
                {
                        return data <= cast(other)->data;
                }
                bool ge(const Base* other) const
                {
                        return data >= cast(other)->data;
                }
        };

        typedef Impl<std::pair<Ast, Ast>> Pair;

 public:
        class iterator {
         public:
                iterator() = default;
                iterator(Ast* ptr) : ptr{ptr} {}
                const Ast& operator*() const
                {
                        if (ptr == nullptr)
                                throw std::runtime_error("Bad iterator");
                        auto p = dynamic_cast<Pair*>(ptr->impl);
                        return p ? p->data.first : *ptr;
                }
                Ast& operator*()
                {
                        if (ptr == nullptr)
                                throw std::runtime_error("Bad iterator");
                        auto p = dynamic_cast<Pair*>(ptr->impl);
                        return p ? p->data.first : *ptr;
                }
                iterator& operator++()
                {
                        if (ptr == nullptr)
                                throw std::runtime_error("Bad iterator");
                        auto p = dynamic_cast<Pair*>(ptr->impl);
                        ptr = p ? &p->data.second : nullptr;
                        return *this;
                }
                iterator operator++(int)
                {
                        auto ret = *this;
                        ++*this;
                        return ret;
                }
                bool operator==(const iterator& other) const
                {
                        return ptr == other.ptr;
                }
                bool operator!=(const iterator& other) const
                {
                        return ptr != other.ptr;
                }

         private:
                Ast* ptr = nullptr;
        };

        class const_iterator {
         public:
                const_iterator() = default;
                const_iterator(const Ast* ptr) : ptr{ptr} {}
                const Ast& operator*() const
                {
                        if (ptr == nullptr)
                                throw std::runtime_error("Bad iterator");
                        auto p = dynamic_cast<const Pair*>(ptr->impl);
                        return p ? p->data.first : *ptr;
                }
                const_iterator& operator++()
                {
                        if (ptr == nullptr)
                                throw std::runtime_error("Bad iterator");
                        auto p = dynamic_cast<const Pair*>(ptr->impl);
                        ptr = p ? &p->data.second : nullptr;
                        return *this;
                }
                const_iterator operator++(int)
                {
                        auto ret = *this;
                        ++*this;
                        return ret;
                }
                bool operator==(const const_iterator& other) const
                {
                        return ptr == other.ptr;
                }
                bool operator!=(const const_iterator& other) const
                {
                        return ptr != other.ptr;
                }

         private:
                const Ast* ptr = nullptr;
        };

        template<typename T> friend Ast make_ast(const T& data);
        template<typename T> friend T& ast_cast(Ast& a);

        Ast() : impl{nullptr} {}
        template<typename T> Ast(T data) : impl{new Impl<T>{data}} {}
        Ast(Ast head, Ast tail) : impl{new Pair{std::make_pair(head, tail)}} {}
        Ast(const Ast& other) : impl{other.impl} {}
        Ast(Ast&& other) : impl{std::move(other.impl)} {}

        ~Ast() = default;

        Ast& operator=(const Ast& other)
        {
                impl = other.impl;
                return *this;
        }
        Ast& operator=(Ast&& other)
        {
                impl = std::move(other.impl);
                return *this;
        }

        operator bool() const { return impl != nullptr; }
        template<typename T> T& get() { return Impl<T>::cast(impl)->data; }

        bool operator==(const Ast& other) const { return impl->eq(other.impl); }
        bool operator!=(const Ast& other) const { return impl->ne(other.impl); }
        bool operator<(const Ast& other) const { return impl->lt(other.impl); }
        bool operator>(const Ast& other) const { return impl->gt(other.impl); }
        bool operator<=(const Ast& other) const { return impl->le(other.impl); }
        bool operator>=(const Ast& other) const { return impl->ge(other.impl); }

        iterator begin() { return iterator{this}; }
        iterator end() { return iterator{nullptr}; }
        const_iterator begin() const { return const_iterator{this}; }
        const_iterator end() const { return const_iterator{nullptr}; }

        Ast head() const
        {
                return *begin();
        }
        Ast tail() const
        {
                auto p = dynamic_cast<Pair*>(impl);
                if (p == nullptr)
                        return Ast{};
                return p->data.second;
        }
        iterator insert(iterator it, Ast x)
        {
                *it->ptr = Ast{x, *it->ptr};
                return it;
        }

        Ast operator[](int i) const
        {
                auto it{begin()};
                auto it_end{end()};
                while (it != it_end) {
                        if (i == 0)
                                return *it;
                        --i;
                        ++it;
                }
                return Ast{};
        }
        Ast& operator[](int i)
        {
                auto it{begin()};
                auto it_end{end()};
                while (it != it_end) {
                        if (i == 0)
                                return *it;
                        --i;
                        ++it;
                }
                throw std::runtime_error{"List out of range"};
        }

 private:
        Base* impl;
};

};      // numespace zz
