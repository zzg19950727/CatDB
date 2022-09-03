#ifndef PAGE_H
#define PAGE_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Common {
		DECLARE(Buffer);
	}
	namespace Storage {
		DECLARE(Page);
		using Common::Buffer_s;
		using Common::Row_s;
		using Common::RowDesc;

		struct PageHeader
		{
			u32 page_checksum;	//校验和
			u32 page_offset;	//表空间内的偏移，虚拟地址，以16K压缩地址，真正的地址需要向左移14位
			u32 page_pre;		//上一页
			u32 page_next;		//下一页
			u32 row_count;		//存放的行数
			u32 free_offset;	//空闲空间偏移
			u32 free_size;		//空闲空间大小
			KV_STRING(
				K(page_checksum),
				K(page_offset),
				K(page_pre),
				K(page_next),
				K(row_count),
				K(free_offset),
				K(free_size)
			);
		};

		struct RowInfo
		{
			u16 offset;
			KV_STRING(
				K(offset)
			);
		};

		class RawRecord
		{
		public:
			u16 column_count;	//列数量
			u16 column_offset[1];	//每列数据所在的偏移
			u32 size()const;
			bool is_null(u32 i);
			void set_null(u32 i);
			u16 get_offset(u32 i);
			static RawRecord* make_raw_record(void* ptr);
		};

		class Page
		{
		public:
			~Page();
			static Page_s make_page();
			static Page_s make_page(u32 page_offset, const RowDesc &row_desc);
			//用于快速扫描所有行数据
			u32 open();
			u32 reset();
			u32 close();

			u32 get_next_row(Row_s& row);
			u32 insert_row(const Row_s& row);
			u32 get_row(u32 row_id, Row_s& row);
			u32 delete_row(u32 row_id);

			inline u32 page_size()const { return PAGE_SIZE; }
			inline u32 page_offset()const { return page_header_->page_offset; }
			inline u32 next_page_offset()const { return page_header_->page_next; }
			inline Buffer_s& page_buffer() { return buffer_; }
			inline bool is_page_dirty() const { return is_dirty; }
			inline void write_page_to_disk() { is_dirty = false; }
			void set_sample_value(int64_t value) { sample_value = value; }
			void clear_page(u32 offset);

			/* row_id的定义：前22位为当前页偏移的虚拟地址，
			* 页实际偏移等于虚拟地址左移14位
			* 后10位为页内row_id从0x0 ~ 0x3ff
			* 由于page_offset是地址的所以row_id整体是递增且唯一的，
			* 同时可以根据row_id直接定位到page的位置*/
			inline static u64 get_real_page_offset(u32 page_offset)
			{
				u64 offset = static_cast<u64>(page_offset);
				return (offset << 14);
			}
			inline static u32 get_virtual_page_offset(u64 page_offset)
			{
				page_offset >>= 14;
				if (page_offset > 0x7FFFFFFF) {
					return 0;
				}
				u32 offset = static_cast<u32>(page_offset);
				return offset;
			}
			inline static u32 get_page_offset_from_row_id(u32 row_id)
			{
				return (row_id >> 10);
			}
			inline u32 get_idx_from_row_id(u32 row_id)const
			{
				return page_header_->row_count - (row_id & 0x3FF) - 1;
			}
			inline u32 get_row_id_from_idx(u32 idx) const
			{
				return idx | (page_header_->page_offset << 10);
			}

			KV_STRING(
				K(page_header_)
			);

		public:
			Page(const Buffer_s& buffer);
			u32 deserialize_row(RowInfo* row_info, u32 row_id, Row_s& row)const;
			u32 serialize_row(const Row_s& row, u32& store_len);
			u32 search_row(u32 row_id, RowInfo*& info)const;
			u32 set_row_deleted(RowInfo* info);
			bool is_row_deleted(RowInfo* info)const;
			u32 row_width(const Row_s& row)const;
			
			RowDesc 		row_desc;
			Buffer_s		buffer_;		//数据所在内存
			PageHeader* 	page_header_;	//页信息
			u8* 			records_space_;	//记录数据起始地址
			u8* 			free_space_;	//空闲空间起始地址
			RowInfo* 		row_info_;		//每条记录的信息
			u32 			row_idx_;	//用于快速扫描所有行数据
			bool 			is_dirty;	//保证脏数据写回表空间
			int64_t			sample_value;

		private:
			DISALLOW_COPY_AND_ASSIGN(Page)
		};
	}
}
#endif //PAGE_H