
#include <ostream>
#include <typeinfo>

#include "gc_cpp.h"

namespace zz {


struct Ast : public gc {
        virtual ~Ast() = default;
        virtual bool operator==(const Ast& other) const = 0;
        virtual bool operator!=(const Ast& other) const = 0;
        virtual bool operator<(const Ast& other) const = 0;
        virtual bool operator>(const Ast& other) const = 0;
        virtual bool operator<=(const Ast& other) const = 0;
        virtual bool operator>=(const Ast& other) const = 0;
        virtual void serialize(std::ostream& os) const = 0;
        virtual const std::type_info& type() const noexcept = 0;
};

template<typename T> inline T ast_get(const Ast* ast);
template<typename T> inline T& ast_get(Ast* ast);

template<typename T> class Ast_impl : public Ast {
 public:
        Ast_impl(const T& data) : data{data} {}
        Ast_impl(T&& data) : data{std::move(data)} {}

        ~Ast_impl() = default;

        bool operator==(const Ast& other) const
        {
                return data >= ast_get<T>(&other);
        }
        bool operator!=(const Ast& other) const
        {
                return data != ast_get<T>(&other);
        }
        bool operator<(const Ast& other) const
        {
                return data < ast_get<T>(&other);
        }
        bool operator>(const Ast& other) const
        {
                return data > ast_get<T>(&other);
        }
        bool operator<=(const Ast& other) const
        {
                return data <= ast_get<T>(&other);
        }
        bool operator>=(const Ast& other) const
        {
                return data >= ast_get<T>(&other);
        }
        void serialize(std::ostream& os) const
        {
                os << data;
        }

        const std::type_info& type() const noexcept
        {
                return typeid(T);
        }

        T data;
};

template<typename T> inline Ast* make_ast(const T& value)
{
        return new Ast_impl<T>{value};
}
template<typename T> inline Ast* make_ast(T&& value)
{
        return new Ast_impl<T>{std::move(value)};
}
inline Ast* make_pair(Ast* head, Ast* tail)
{
        return make_ast(std::make_pair(head, tail));
}
inline Ast* make_list()
{
        return nullptr;
}
template<typename ...Args> inline Ast* make_list(Ast* head, Args... args)
{
        return make_pair(head, make_list(args...));
}

template<typename T> inline T& ast_get(Ast* ast)
{
        auto p = dynamic_cast<Ast_impl<T>*>(ast);
        if (p == nullptr)
                throw std::bad_cast{};
        return p->data;
}
template<typename T> inline T ast_get(const Ast* ast)
{
        auto p = dynamic_cast<const Ast_impl<T>*>(ast);
        if (p == nullptr)
                throw std::bad_cast{};
        return p->data;
}

inline const Ast* head(const Ast* ast)
{
        if (ast->type() == typeid(std::pair<Ast*, Ast*>))
                return ast_get<std::pair<Ast*, Ast*>>(ast).first;
        return ast;
}
inline Ast* head(Ast* ast)
{
        if (ast->type() == typeid(std::pair<Ast*, Ast*>))
                return ast_get<std::pair<Ast*, Ast*>>(ast).first;
        return ast;
}
inline const Ast* tail(const Ast* ast)
{
        if (ast->type() == typeid(std::pair<Ast*, Ast*>))
                return ast_get<std::pair<Ast*, Ast*>>(ast).second;
        return nullptr;
}
inline Ast* tail(Ast* ast)
{
        if (ast->type() == typeid(std::pair<Ast*, Ast*>))
                return ast_get<std::pair<Ast*, Ast*>>(ast).second;
        return nullptr;
}

class Ast_iterator {
 public:
        typedef Ast* value_type;
        typedef ptrdiff_t difference_type;
        typedef Ast** pointer;
        typedef Ast*& reference;
        typedef std::forward_iterator_tag iterator_category;

        Ast_iterator() : ptr{nullptr} {}
        Ast_iterator(Ast* ptr) : ptr{ptr} {}
        Ast_iterator(const Ast_iterator& other) : ptr{other.ptr} {}
        Ast_iterator(Ast_iterator&& other) : ptr{std::move(other.ptr)} {}
        Ast_iterator& operator=(const Ast_iterator& other)
        {
                ptr = other.ptr;
        }
        Ast_iterator& operator=(Ast_iterator&& other)
        {
                ptr = std::move(other.ptr);
        }
        Ast*& operator*()
        {
                assert(ptr != nullptr);
                return ast_get<std::pair<Ast*, Ast*>>(ptr).first;
        }
        Ast_iterator& operator++()
        {
                ptr = tail(ptr);
                return *this;
        }
        Ast_iterator operator++(int)
        {
                auto ret = *this;
                ptr = tail(ptr);
                return ret;
        }
        bool operator==(const Ast_iterator& other) const
        {
                return ptr == other.ptr;
        }
        bool operator!=(const Ast_iterator& other) const
        {
                return ptr != other.ptr;
        }

 private:
        Ast* ptr;
};

inline Ast_iterator begin(Ast* ptr)
{
        return Ast_iterator{ptr};
}
inline Ast_iterator end(Ast* ptr)
{
        return Ast_iterator{nullptr};
}

class Ast_const_iterator {
 public:
        typedef Ast* value_type;
        typedef ptrdiff_t difference_type;
        typedef Ast** pointer;
        typedef Ast*& reference;
        typedef std::forward_iterator_tag iterator_category;

        Ast_const_iterator() : ptr{nullptr} {}
        Ast_const_iterator(const Ast* ptr) : ptr{ptr} {}
        Ast_const_iterator(const Ast_const_iterator& other) : ptr{other.ptr} {}
        Ast_const_iterator(Ast_const_iterator&& other) : ptr{std::move(other.ptr)} {}
        Ast_const_iterator& operator=(const Ast_const_iterator& other)
        {
                ptr = other.ptr;
        }
        Ast_const_iterator& operator=(Ast_const_iterator&& other)
        {
                ptr = std::move(other.ptr);
        }
        const Ast* operator*()
        {
                assert(ptr != nullptr);
                return ast_get<std::pair<Ast*, Ast*>>(ptr).first;
        }
        Ast_const_iterator& operator++()
        {
                ptr = tail(ptr);
                return *this;
        }
        Ast_const_iterator operator++(int)
        {
                auto ret = *this;
                ptr = tail(ptr);
                return ret;
        }
        bool operator==(const Ast_const_iterator& other) const
        {
                return ptr == other.ptr;
        }
        bool operator!=(const Ast_const_iterator& other) const
        {
                return ptr != other.ptr;
        }

 private:
        const Ast* ptr;
};

inline Ast_const_iterator begin(const Ast* ptr)
{
        return Ast_const_iterator{ptr};
}
inline Ast_const_iterator end(const Ast* ptr)
{
        return Ast_const_iterator{nullptr};
}

inline std::ostream& operator<<(std::ostream& os, const std::pair<Ast*, Ast*>& p)
{
        /* do nothing. handled by the general Ast* serializer */
}
inline std::ostream& operator<<(std::ostream& os, const Ast* ast)
{
        if (ast == nullptr) {
                os << "()";
        } else if (ast->type() == typeid(std::pair<Ast*, Ast*>)) {
                os << '(';
                bool first = true;
                for (const auto x : ast) {
                        if (first)
                                first = false;
                        else
                                os << ' ';
                        os << x;
                }
                os << ')';
        } else {
                ast->serialize(os);
        }
        return os;
}

template<typename Pred> inline Ast* remove_if(Ast* l, Pred pred)
{
        auto p = dynamic_cast<Ast_impl<std::pair<Ast*, Ast*>>*>(l);
        if (p == nullptr)
                return l;
        p->data.second = remove_if(p->data.second, pred);
        if (pred(p->data.first))
                return p->data.second;
        return p;
}
inline Ast* remove(Ast* l, Ast* x)
{
        return remove_if(l, [x](Ast* y){ return x == y; });
}

};      // numespace zz

