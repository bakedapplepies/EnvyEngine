#pragma once

#include <cstdint>
#include <vector>

#include <Envy/core/Defines.h>

ENVY_NAMESPACE_START

template <typename T>
class Resource
{
public:
    Resource<T>()
    {}

    Resource<T>(uint32_t index, std::vector<T>* vector)
    {
        m_index = index;
        m_vector = vector;
    }

    ~Resource<T>()
    {}

    Resource<T>(const Resource<T>&) = default;
    Resource<T>& operator=(const Resource<T>&) = default;
    
    Resource<T>(Resource<T>&& other) noexcept
    {
        m_index = other.m_index;
        m_vector = other.m_vector;

        other.m_index = -1;
        other.m_vector = nullptr;
    }

    Resource<T>& operator=(Resource<T>&& other) noexcept
    {
        m_index = other.m_index;
        m_vector = other.m_vector;

        other.m_index = -1;
        other.m_vector = nullptr;

        return *this;
    }

    const inline T* operator->() const
    {
        return &m_vector->at(m_index);
    }
    
    inline T* operator->()
    {
        return &m_vector->at(m_index);
    }

    bool Usable() const
    {
        return static_cast<bool>(m_vector);
    }

public:
    static Resource<T> empty;

private:
    uint32_t m_index = -1;
    std::vector<T>* m_vector = nullptr;
};

template <typename T>
Resource<T> Resource<T>::empty = Resource<T>(0, nullptr);

template <typename T>
class ResourceVector
{
public:
    ResourceVector<T>()
    {}

    ~ResourceVector<T>()
    {}

    ResourceVector<T>(const ResourceVector<T>&) = delete;
    ResourceVector<T>& operator=(const ResourceVector<T>&) = delete;

    ResourceVector<T>(ResourceVector<T>&& other) noexcept
    {
        m_vector = std::move(other.m_vector);
    }

    ResourceVector<T>& operator=(ResourceVector<T>&& other) noexcept
    {
        m_vector = std::move(other.m_vector);

        return *this;
    }
    
    inline void AddBack(T&& v)
    {
        m_vector.emplace_back(std::forward<T>(v));
    }

    inline const Resource<T> GetBack() const
    {
        return Resource<T>(m_vector.size() - 1, &m_vector);
    }

    inline Resource<T> GetBack()
    {
        return Resource<T>(m_vector.size() - 1, &m_vector);
    }

    inline const Resource<T> GetAt(uint32_t i) const
    {
        return Resource<T>(i, &m_vector);
    }

    inline Resource<T> GetAt(uint32_t i)
    {
        return Resource<T>(i, &m_vector);
    }

    inline size_t Size() const
    {
        return m_vector.size();
    }

    inline void Clear()
    {
        m_vector.clear();
    }

private:
    // std::vector<T> m_vector;
    mutable std::vector<T> m_vector;
};

ENVY_NAMESPACE_END