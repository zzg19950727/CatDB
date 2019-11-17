#include "buffer.h"

using namespace CatDB::Common;

Buffer::Buffer(u32 len, u8 * ptr)
	:length(len),
	buf(ptr),
	is_dirty(false)
{
}

Buffer::~Buffer()
{
	if(length > 0 && buf)
		delete[] buf;
}

Buffer_s Buffer::make_buffer(u32 size)
{
	u8* ptr = new u8[size];
	memset(ptr, 0, size);
	return Buffer_s(new Buffer(size, ptr));
}