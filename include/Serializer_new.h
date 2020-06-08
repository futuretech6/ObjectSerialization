#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "Base64.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <type_traits>

#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace ser {
    const std::string sep = " ";

    template <typename Type>
    typename std::enable_if<std::is_arithmetic<Type>::value ||
                            std::is_same<Type, std::string>::value>::type
    Serializer(const Type &, std::ostream &);
    template <typename Type1, typename Type2>
    void Serializer(const std::pair<Type1, Type2> &, std::ostream &);
    template <typename Type>
    void Serializer(const std::vector<Type> &, std::ostream &);
    template <typename Type>
    void Serializer(const std::list<Type> &, std::ostream &);
    template <typename Type>
    void Serializer(const std::set<Type> &, std::ostream &);
    template <typename Type1, typename Type2>
    void Serializer(const std::map<Type1, Type2> &, std::ostream &);
    template <typename Type>
    void Serializer(const std::unique_ptr<Type> &, std::ostream &);
    template <typename Type>
    void Serializer(const std::unique_ptr<Type[]> &, std::ostream &, size_t len = 1);
    template <typename Type>
    void Serializer(const std::shared_ptr<Type> &, std::ostream &);
    template <typename Type>
    void Serializer(const std::shared_ptr<Type[]> &, std::ostream &, size_t len = 1);
    template <typename Type>
    void Serializer(const Type *obj, std::ostream &buf, size_t len = 1);

    /* Arithmetic || string */
    template <typename Type>
    typename std::enable_if<std::is_arithmetic<Type>::value ||
                            std::is_same<Type, std::string>::value>::type
    Serializer(const Type &obj, std::ostream &buf) {
        buf << obj << sep;
    };

    /* pair */
    template <typename Type1, typename Type2>
    void Serializer(const std::pair<Type1, Type2> &obj, std::ostream &buf) {
        Serializer(obj.first, buf);
        Serializer(obj.second, buf);
    };

    /* vector */
    template <typename Type>
    void Serializer(const std::vector<Type> &obj, std::ostream &buf) {
        buf << obj.size() << sep;
        for (auto &i : obj)
            Serializer(i, buf);
    }

    /* list */
    template <typename Type>
    void Serializer(const std::list<Type> &obj, std::ostream &buf) {
        buf << obj.size() << sep;
        for (auto &i : obj)
            Serializer(i, buf);
    }

    /* set */
    template <typename Type>
    void Serializer(const std::set<Type> &obj, std::ostream &buf) {
        buf << obj.size() << sep;
        for (auto &i : obj)
            Serializer(i, buf);
    }

    /* map */
    template <typename Type1, typename Type2>
    void Serializer(const std::map<Type1, Type2, std::less<Type1>,
                        std::allocator<std::pair<const Type1, Type2>>> &obj,
        std::ostream &buf) {
        buf << obj.size() << sep;
        for (auto &i : obj)
            Serializer<Type1, Type2>(i, buf);
    }

    /* unique_ptr */
    template <typename Type>
    void Serializer(const std::unique_ptr<Type> &obj, std::ostream &buf) {
        Serializer(*obj, buf);
    }
    template <typename Type>
    void Serializer(const std::unique_ptr<Type[]> &obj, std::ostream &buf, size_t len) {
        buf << len << sep;
        for (auto i = 0; i < len; i++)
            Serializer(obj[i], buf);
    }

    /* shared_ptr */
    template <typename Type>
    void Serializer(const std::shared_ptr<Type> &obj, std::ostream &buf) {
        Serializer(*obj, buf);
    }
    template <typename Type>
    void Serializer(const std::shared_ptr<Type[]> &obj, std::ostream &buf, size_t len) {
        buf << len << sep;
        for (auto i = 0; i < len; i++)
            Serializer(obj[i], buf);
    }

    /* ptr */
    template <typename Type>
    void Serializer(const Type *obj, std::ostream &buf, size_t len) {
        buf << len << sep;
        for (auto i = 0; i < len; i++)
            Serializer(obj[i], buf);
    }

}  // namespace ser

namespace des {

    template <typename Type>
    typename std::enable_if<std::is_arithmetic<Type>::value ||
                            std::is_same<Type, std::string>::value>::type
    Deserializer(Type &, std::istream &);
    template <typename Type1, typename Type2>
    void Deserializer(std::pair<Type1, Type2> &, std::istream &);
    template <typename Type>
    void Deserializer(std::vector<Type> &, std::istream &);
    template <typename Type>
    void Deserializer(std::list<Type> &, std::istream &);
    template <typename Type>
    void Deserializer(std::set<Type> &, std::istream &);
    template <typename Type1, typename Type2>
    void Deserializer(std::map<Type1, Type2> &, std::istream &);
    template <typename Type>
    void Deserializer(std::unique_ptr<Type> &, std::istream &);
    template <typename Type>
    void Deserializer(std::unique_ptr<Type[]> &, std::istream &, size_t len = 1);
    template <typename Type>
    void Deserializer(std::shared_ptr<Type> &, std::istream &);
    template <typename Type>
    void Deserializer(std::shared_ptr<Type[]> &, std::istream &, size_t len = 1);
    template <typename Type>
    void Deserializer(Type *, std::istream &, size_t len = 1);

    /* Arithmetic || string */
    template <typename Type>
    typename std::enable_if<std::is_arithmetic<Type>::value ||
                            std::is_same<Type, std::string>::value>::type
    Deserializer(Type &obj, std::istream &buf) {
        buf >> obj;
    }

    /* pair */
    template <typename Type1, typename Type2>
    void Deserializer(std::pair<Type1, Type2> &obj, std::istream &buf) {
        Type1 tmp1;
        Type2 tmp2;
        Deserializer(tmp1, buf);
        Deserializer(tmp2, buf);
        obj = std::make_pair(tmp1, tmp2);
    };

    /* vector */
    template <typename Type>
    void Deserializer(std::vector<Type> &obj, std::istream &buf) {
        size_t len;
        buf >> len;
        obj.resize(len);
        for (auto &i : obj)
            Deserializer(i, buf);
    }

    /* list */
    template <typename Type>
    void Deserializer(std::list<Type> &obj, std::istream &buf) {
        size_t len;
        buf >> len;
        obj.resize(len);
        for (auto &i : obj)
            Deserializer(i, buf);
    }

    /* set */
    template <typename Type>
    void Deserializer(std::set<Type> &obj, std::istream &buf) {
        size_t len;
        buf >> len;
        for (auto i = 0; i < len; i++) {
            Type tmp;
            Deserializer(tmp, buf);
            obj.insert(tmp);
        }
    }

    /* map */
    template <typename Type1, typename Type2>
    void Deserializer(std::map<Type1, Type2> &obj, std::istream &buf) {
        size_t len;
        buf >> len;
        for (auto i = 0; i < len; i++) {
            Type1 tmp1;
            Type2 tmp2;
            Deserializer(tmp1, buf);
            Deserializer(tmp2, buf);
            obj.insert(std::make_pair(tmp1, tmp2));
        }
    }

    /* unique_ptr */
    template <typename Type>
    void Deserializer(std::unique_ptr<Type> &obj, std::istream &buf) {
        obj = std::unique_ptr<Type>(new Type);
        Deserializer(obj, buf);
    }
    template <typename Type>
    void Deserializer(std::unique_ptr<Type[]> &obj, std::istream &buf, size_t len) {
        obj = std::unique_ptr<Type[]>(new Type[len]);
        for (auto i = 0; i < len; i++)
            Deserializer(obj[i], buf);
    }

    /* shared_ptr */
    template <typename Type>
    void Deserializer(std::shared_ptr<Type> &obj, std::istream &buf) {
        obj = std::shared_ptr<Type>(new Type);
        Deserializer(obj, buf);
    }
    template <typename Type>
    void Deserializer(std::shared_ptr<Type[]> &obj, std::istream &buf, size_t len) {
        obj = std::unique_ptr<Type>(new Type[len]);
        for (auto i = 0; i < len; i++)
            Deserializer(obj[i], buf);
    }

    /* ptr */
    template <typename Type>
    void Deserializer(Type *obj, std::istream &buf, size_t len) {
        obj = new Type[len];
        for (auto i = 0; i < len; i++)
            Deserializer(obj[i], buf);
    }
}  // namespace des

#endif
