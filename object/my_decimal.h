/* Copyright (c) 2005, 2013, Oracle and/or its affiliates.
Copyright (c) 2011, 2014, SkySQL Ab.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA */

/**
@file

It is interface module to fixed precision decimals library.

Most functions use 'uint mask' as parameter, if during operation error
which fit in this mask is detected then it will be processed automatically
here. (errors are E_DEC_* constants, see include/decimal.h)

Most function are just inline wrappers around library calls
*/

#ifndef my_decimal_h
#define my_decimal_h
#include "decimal.h"
#include "type.h"
#include "log.h"
#define DECIMAL_LONGLONG_DIGITS 22
#define DECIMAL_LONG_DIGITS 10
#define DECIMAL_LONG3_DIGITS 8
/** maximum length of buffer in our big digits (uint32). */
#define DECIMAL_BUFF_LENGTH 9
/* the number of digits that my_decimal can possibly contain */
#define DECIMAL_MAX_POSSIBLE_PRECISION (DECIMAL_BUFF_LENGTH * 9)
/**
maximum guaranteed precision of number in decimal digits (number of our
digits * number of decimal digits in one our big digit - number of decimal
digits in one our big digit decreased by 1 (because we always put decimal
point on the border of our big digits))
With normal precession we can handle 65 digits. MariaDB can store in
the .frm up to 63 digits.  By default we use DECIMAL_NOT_SPECIFIED digits
when converting strings to decimal, so we don't want to set this too high.
To not use up all digits for the scale we limit the number of decimals to 38.
*/
#define DECIMAL_MAX_PRECISION (DECIMAL_MAX_POSSIBLE_PRECISION - 9)
#define DECIMAL_MAX_SCALE 38
#define DECIMAL_NOT_SPECIFIED 39

/**
maximum length of string representation (number of maximum decimal
digits + 1 position for sign + 1 position for decimal point, no terminator)
*/
#define DECIMAL_MAX_STR_LENGTH (DECIMAL_MAX_POSSIBLE_PRECISION + 2)
/**
maximum size of packet length.
*/
#define DECIMAL_MAX_FIELD_SIZE DECIMAL_MAX_PRECISION
#define CHARSET_INFO void

/**
my_decimal class limits 'decimal_t' type to what we need in MySQL.

It contains internally all necessary space needed by the instance so
no extra memory is needed. One should call fix_buffer_pointer() function
when he moves my_decimal objects in memory.
*/
class my_decimal :public decimal_t
{
	decimal_digit_t buffer[DECIMAL_BUFF_LENGTH];
	void init()
	{
		len = DECIMAL_BUFF_LENGTH;
		buf = buffer;
	}
	void fix_buffer_pointer() { buf = buffer; }

public:
	my_decimal(const my_decimal &rhs) : decimal_t(rhs)
	{
		init();
		for (uint i = 0; i < DECIMAL_BUFF_LENGTH; i++)
			buffer[i] = rhs.buffer[i];
	}

	my_decimal& operator=(const my_decimal &rhs)
	{
		if (this == &rhs)
			return *this;
		decimal_t::operator=(rhs);
		for (uint i = 0; i < DECIMAL_BUFF_LENGTH; i++)
			buffer[i] = rhs.buffer[i];
		fix_buffer_pointer();
		return *this;
	}

	my_decimal()
	{
		init();
		decimal_make_zero(static_cast<decimal_t*>(this));
	}

	my_decimal(const uchar *bin, int size, int prec, int scale)
	{
		init();
		memcpy(&prec, bin, sizeof(prec));
		memcpy(&scale, bin + sizeof(prec), sizeof(scale));
		bin2decimal(bin + sizeof(prec) + sizeof(prec), this, prec + scale, scale);
	}

	my_decimal(const String& str)
	{
		init();
		char* end;
		end = (char*)(str.c_str() + str.size());
		internal_str2dec(str.c_str(), this, &end, 0);
	}

	my_decimal(const char* str, int size)
	{
		init();
		char* end = (char*)(str + size);
		internal_str2dec(str, this, &end, 0);
	}

	my_decimal(longlong value)
	{
		init();
		longlong2decimal(value, this);
	}

	my_decimal(double value)
	{
		init();
		double2decimal(value, this);
	}

	~my_decimal()
	{

	}

	bool sign() const { return decimal_t::sign; }
	void sign(bool s) { decimal_t::sign = s; }
	uint precision() const { return intg + frac; }
	uint scale() const { return frac; }
	uint size()const{return sizeof(intg) + sizeof(frac) + decimal_bin_size(intg + frac, frac);}
	void minus()
	{
		decimal_t::sign = !decimal_t::sign;
	}
	void set_zero()
	{
		/*
		We need the up-cast here, since my_decimal has sign() member functions,
		which conflicts with decimal_t::sign
		(and decimal_make_zero is a macro, rather than a funcion).
		*/
		decimal_make_zero(static_cast<decimal_t*>(this));
	}

	bool is_zero()const
	{
		return decimal_is_zero(this);
	}

	int cmp(const my_decimal& other) const
	{
		return decimal_cmp(this, &other);
	}

	bool to_bool() const
	{
		return !decimal_is_zero(this);
	}

	double to_double() const
	{
		double res;
		decimal2double(this, &res);
		return res;
	}

	longlong to_longlong() const
	{
		long long value = 0;
		decimal2longlong(this, &value);
		return value;
	}

	String to_string() const
	{
		char tmp[DECIMAL_MAX_STR_LENGTH] = { 0 };
		int len = DECIMAL_MAX_STR_LENGTH;
		decimal2string(this, tmp, &len, 0, 0, '0');
		return String(tmp, len);
	}

	int to_binary(uchar *bin) const
	{
		memcpy(bin, &intg, sizeof(intg));
		memcpy(bin + sizeof(intg), &frac, sizeof(frac));
		return decimal2bin(this, bin + sizeof(intg) + sizeof(intg), intg+frac, frac);
	}

	/** Swap two my_decimal values */
	void swap(my_decimal &rhs)
	{
		swap_variables(my_decimal, *this, rhs);
	}
	
	int cast_to_decimal(int prec, int scale, my_decimal &res)
	{
		int ret = 0;
		if (prec < scale) {
			return E_DEC_ERROR;
		}
		if (prec - scale != intg) {
			my_decimal temp;
			max_decimal(prec-scale+DECIMAL_MAX_SCALE, DECIMAL_MAX_SCALE, &temp);
			decimal_mod(this, &temp, &res);
		}
		if (scale != res.frac) {
			my_decimal temp = res;
			ret = decimal_round(&temp, &res, scale, HALF_UP);
		}
		return ret;
	}

	int cast_to_char(char* buf, int &len)
	{
		return decimal2string(this, buf, &len, 0, 0, '0');
	}

	static int my_decimal_add(my_decimal& res, const my_decimal& a, const my_decimal& b)
	{
		return decimal_add(&a, &b, &res);
	}

	static int my_decimal_sub(my_decimal& res, const my_decimal& a, const my_decimal& b)
	{
		return decimal_sub(&a, &b, &res);
	}

	static int my_decimal_mul(my_decimal& res, const my_decimal& a, const my_decimal& b)
	{
		return decimal_mul(&a, &b, &res);
	}

	static int my_decimal_div(my_decimal& res, const my_decimal& a, const my_decimal& b, int div_scale_inc)
	{
		return decimal_div(&a, &b, &res, div_scale_inc);
	}

	static int my_decimal_mod(my_decimal& res, const my_decimal& a, const my_decimal& b)
	{
		return decimal_mod(&a, &b, &res);
	}
};

#endif /*my_decimal_h*/