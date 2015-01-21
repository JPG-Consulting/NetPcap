#ifndef __CPPPCAP_EXCEPTION_H__
#define __CPPPCAP_EXCEPTION_H__ 1

#include <string>
#include <stdexcept>

namespace CppPcap {

	class PcapException : public std::runtime_error
	{
	public:

		///
		/// @brief	Creates an Exception object
		///	@param	msg	The error message
		///
		PcapException(std::string const& msg = "")
			: std::runtime_error(msg) { }
	};

}

#endif // __CPPPCAP_EXCEPTION_H__