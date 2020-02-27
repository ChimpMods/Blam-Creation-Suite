#pragma once

template<typename T, find_offset_func find_offset>
class /*[[deprecated("DataEx is deprecated. Use reference_symbol instead.")]]*/ DataEx : public c_data_reference_base
{
public:
	DataEx(const DataEx&) = delete;
	DataEx()
		: c_data_reference_base(_engine_type_not_set, _build_not_set, sizeof(T), 0, find_offset)
	{

	}

	T* ptr() const
	{
		T* ptr = reinterpret_cast<T*>(m_pPtr);
		return ptr;
	}

	volatile T* volatile_ptr() const
	{
		return static_cast<volatile T*>(ptr());
	}

	T& PointerReference() const
	{
		return *ptr();
	}

	T& operator=(T value)
	{
		T& reference = PointerReference();
		reference = value;
		return reference;
	}

	T* operator->()
	{
		return ptr();
	}

	T const* operator->() const
	{
		return ptr();
	}

	operator T& () const
	{
		return PointerReference();
	}

	operator bool() const
	{
		T& r = PointerReference();
		return (bool)r;
	}
};
