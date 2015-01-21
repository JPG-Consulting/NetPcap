#ifndef __CPPPCAP_PACKET_H__
#define __CPPPCAP_PACKET_H__ 1

extern "C" {
	#include <pcap.h>
}

namespace CppPcap {

	class Packet {
	public:

		///
		///	@brief	Length of a packet (number of bytes)
		///
		typedef bpf_u_int32 Length;

		///
		///	@brief	Header of a packet
		///
		//typedef struct pcap_pkthdr Header;

		///
		///	@brief	Unit of packet data (1 byte)
		///
		//typedef u_char Data;


		///
		/// @brief	Creates an empty Packet
		///
		Packet();


		///
		///	@brief	Creates a packet from raw data
		///	@param	data	A pointer to the packet data
		///	@param	length	The length of the packet data (in bytes)
		///
		Packet(u_char const* data, Length length);


		///
		/// @brief	Creates a Packet object as a copy from another
		///
		Packet(Packet const& packet)
			:
			m_header(packet.m_header),
			mp_data(packet.mp_data),
			m_managed(false)
		{ }


		///
		///	@brief	Creates a Packet giving its Header and Data
		///	@param	header	The Header of the Packet
		///	@param	data	A pointer to the Packet Data
		///
		Packet(struct pcap_pkthdr const& header, u_char const* data)
			:
			m_header(header),
			mp_data(const_cast<u_char*>(data)),
			m_managed(false)
		{ }


		///
		/// @brief	Assign another Packet object's contents to this one
		///
		Packet& operator=(Packet const& packet)
		{
			if (this != &packet) {
				m_header = packet.m_header;
				mp_data = packet.mp_data;
				m_managed = false;
			}
			return *this;
		}


		///
		///	@brief	Destroys a Packet object
		///
		virtual ~Packet();


		///
		///	@brief	Let the user manage the memory occupied by the payload of
		///		the Packet
		///
		///	By default the packet data (as returned by get_data()), is owned by
		///	libpcap. This means that that data is not persistent among
		///	successive calls of a Pcap::Handler.@n
		///	To overcome this limitation one can create a Packet as a copy of the
		///	incoming packet, and call manage() on it.@n
		///	All the memory allocated by manage() is released when the Packet is
		///	destroyed
		///
		void manage();


		///
		/// @brief	Gets the time when the packet was captured, in seconds
		///
		long get_seconds() const
		{
			return m_header.ts.tv_sec;
		}


		///
		/// @brief	Gets the time when the packet was captured, in miliseconds
		///
		long get_miliseconds() const
		{
			return m_header.ts.tv_usec;
		}


		///
		/// @brief	Gets the capture length
		///	@return The number of bytes of the packet that are available
		///		from the capture
		///
		Length get_capture_length() const
		{
			return m_header.caplen;
		}


		///
		/// @brief	Gets the length of the packet
		///	@return	 The length of the packet, in bytes, which might be
		///		more than the number of bytes available from the
		///		capture (given by get_capture_length()), if the length of the
		///		packet is larger than the maximum number of bytes to capture.
		///
		Length get_length() const
		{
			return m_header.len;
		}


		///
		/// @brief	Gets  a pointer to the data (payload) of the Packet
		///
		///	Note that unless the Packet is managed by the user (by calling
		///	manage()), the Data pointer is owned by libpcap
		///
		u_char const* Data() const
		{
			return mp_data;
		}


		///
		///	@brief	Gets the Header of the Packet
		///
		struct pcap_pkthdr const& Header() const
		{
			return m_header;
		}


	private:

		void const* __get_void_const_p();

		struct pcap_pkthdr	m_header;
		u_char*	mp_data;
		bool	m_managed;
	};

}

#endif // __CPPPCAP_PACKET_H__