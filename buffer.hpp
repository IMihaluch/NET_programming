

#pragma once

#include <memory>

namespace imp
{

class Buffer
{
private:
    const unsigned short int _size;
    std::unique_ptr<std::byte[]> _buffer;
public:
    Buffer(const unsigned short int size);
    const unsigned short int getSize() const;
    const std::string getString() const;
    void clear();
    operator char*();
};

}
