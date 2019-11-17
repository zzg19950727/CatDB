﻿#ifndef PAGE_H
#define PAGE_H
#include "type.h"
//PAGE_SIZE跟row_id的结构设计相关，改大后注意是否向前兼容row_id
//详见row_id设计
#define PAGE_SIZE 16384

namespace CatDB {
	namespace Common {
		DECLARE(Buffer);
		DECLARE(Row);
	}
	namespace Storage {
		DECLARE(Page);
		using Common::Buffer_s;
		using Common::Row_s;

		//page的文件信息
		struct FileHeader
		{
			u32 page_checksum;	//校验和
			u32 table_id;		//所属表空间
			u32 page_offset;	//表空间内的偏移，虚拟地址，以16K压缩地址，真正的地址需要向左移14位
			u32 page_pre;		//上一页
			u32 page_next;		//下一页
		};
		/* row_id的定义：前22位为当前页偏移的虚拟地址，
		 * 页实际偏移等于虚拟地址左移14位
		 * 后10位为页内row_id从0x0 ~ 0x3ff
		 * 由于page_offset是地址的所以row_id整体是递增且唯一的，
		 * 同时可以根据row_id直接定位到page的位置*/
		u64 get_real_page_offset(u32 page_offset);
		u32 get_virtual_page_offset(u64 page_offset);
		u32 get_page_offset_from_row_id(u32 row_id);
		u32 get_beg_row_id_from_page_offset(u32 page_offset);

		struct PageHeader
		{
			u32 beg_row_id;		//存放记录的起始行
			u32 end_row_id;		//存放记录的结束行
			u32 row_count;		//存放的行数
			u32 free_offset;	//空闲空间偏移
			u32 free_size;		//空闲空间大小
		};

		struct RowInfo
		{
			u32 row_id;
			u32 offset;
		};

		class RawRecord
		{
		public:
			u32 column_count;	//列数量
			u32 column_offset[1];	//每列数据所在的偏移
			u32 size()const;
			static RawRecord* make_raw_record(void* ptr);
		};

		class Page
		{
		public:
			~Page();
			static Page_s make_page(
				const String& database,
				const String& table,
				Buffer_s& buffer);
			static Page_s make_page(
				const String& database,
				const String& table,
				Buffer_s& buffer,
				u32 table_id,
				u32 page_offset,
				u32 page_pre,
				u32 page_next,
				u32 beg_row_id);
			//用于快速扫描所有行数据
			u32 open();
			u32 get_next_row(Row_s& row);
			bool have_row()const;
			u32 reset();
			u32 close();

			u32 get_free_space()const;
			u32 select_row(u32 row_id, Row_s& row)const;
			u32 insert_row(u32& row_id, const Row_s& row);
			u32 update_row(u32 row_id, Row_s& row);
			u32 delete_row(u32 row_id);

			u32 page_size()const;
			u32 page_offset()const;
			u32 next_page_offset()const;
			const Buffer_s& page_buffer()const;
			void reset_page();
			u32 row_id_exists(u32 row_id)const;
			bool have_free_space_insert(const Row_s& row);

		private:
			Page(const Buffer_s& buffer);
			u32 project_all_column(RawRecord* record, Row_s& row)const;
			u32 project_row(RowInfo* row_info, Row_s& row)const;
			u32 update_none_fix_row(u32 row_id, Row_s& row);
			u32 write_row(RawRecord* record, const Row_s& row);
			u32 search_row(u32 row_id, RowInfo*& info)const;
			u32 set_row_id_deleted(u32& row_id);
			bool row_id_deleted(u32 row_id)const;
			u32 row_width(const Row_s& row)const;

			String database, table;
			Buffer_s		buffer_;		//数据所在内存
			FileHeader* file_header_;	//文件信息
			PageHeader* page_header_;	//页信息
			u8* records_space_;	//记录数据起始地址
			u8* free_space_;	//空闲空间起始地址
			RowInfo* row_info_;		//每条记录的信息

			//用于快速扫描所有行数据
			u32 row_idx_;

		private:
			DISALLOW_COPY_AND_ASSIGN(Page)
		};
	}
}
#endif //PAGE_H