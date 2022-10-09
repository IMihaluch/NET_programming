

#include <buffer.hpp>
#include <string.h>
#include <string>

namespace imp
{

Buffer::Buffer(const unsigned short int size) : _size(size)
{
    _buffer = std::make_unique<std::byte[]>(_size);
}

const unsigned short int Buffer::getSize() const
{
    return _size;
}

const std::string Buffer::getString() const
{
    return reinterpret_cast<char*>(_buffer.get());
}

void Buffer::clear()
{
    bzero(_buffer.get(), _size);
}

Buffer::operator char*()
{
    return reinterpret_cast<char*>(_buffer.get());
}

}
