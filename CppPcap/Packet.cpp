#include "Packet.h"

extern "C" {
	#include <string.h>	// memset(), memcpy()
	#include <stdlib.h>	// malloc()
}

namespace CppPcap {


	Packet::Packet()
		:
		m_header(),
		mp_data(NULL),
		m_managed(false)
	{
		memset(&m_header, 0, sizeof(m_header));
	}


	Packet::Packet(u_char const* data, Length length)
		:
		m_header(),
		mp_data(const_cast<u_char*>(data)),
		m_managed(false)
	{
		memset(&(m_header.ts), 0, sizeof(m_header.ts));
		m_header.len = m_header.caplen = length;
	}


	// [virtual]
	Packet::~Packet()
	{
		if (m_managed && mp_data) {
			free(reinterpret_cast<void*>(mp_data));
			mp_data = NULL;
		}
	}


	void Packet::manage()
	{
		if (m_managed || mp_data == NULL)
			return;
		void* dest = malloc(m_header.caplen);	//XXX error?
		memcpy(dest, __get_void_const_p(), m_header.caplen);
		mp_data = reinterpret_cast<u_char*>(dest);
		m_managed = true;
	}


	void const* Packet::__get_void_const_p()
	{
		void* p = reinterpret_cast<void*>(mp_data);
		return const_cast<void const*>(p);
	}

}