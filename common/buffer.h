#ifndef BUFFER_H
#define BUFFER_H
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Buffer);

		class Buffer
		{
		public:
			~Buffer();
			static Buffer_s make_buffer(u32 size);
			u32 length;
			u8* buf;
			bool is_dirty;
		private:
			Buffer(u32 len, u8* ptr);
		private:
			DISALLOW_COPY_AND_ASSIGN(Buffer)
		};
	}
}

#endif	//BUFFER_H