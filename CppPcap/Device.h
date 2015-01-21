#ifndef __CPPPCAP_DEVICE_H__
#define __CPPPCAP_DEVICE_H__ 1

#include <vector>
extern "C" {
	#include <pcap.h>
}
#include "Packet.h"


namespace CppPcap {

	class Device {

	public:

		Device(Device const& dev);

		///
		///	@brief	Destroys a Device object
		///
		virtual ~Device();

		///
		///	@brief	Gets the name of the device
		///
		std::string const& Name() const
		{
			return _name;
		}


		///
		///	@brief	Gets a description of the device
		///	@return	A human-readable description of the device, or an empty
		///		string ("") if a description is not available for this device
		///
		std::string const& Description() const
		{
			return _description;
		}

		///
		///	@brief	Gets or sets description of the device
		///
		std::string const& Filter() const { return _filter; }								// Getter
		void Filter(std::string const& filter_expression) { _filter = filter_expression; }	// Setter

		///
		///	@brief	Returns whether the device is open
		///
		bool IsOpen() const
		{
			return ((_handle == NULL) ? false : true);
		}

		void BeginCapture(pcap_handler 	callback);
		void BeginCapture(int cnt, pcap_handler callback);

		void EndCapture();

		int NextPacket(Packet& packet);

		void Close(void);

		///
		///	@brief	Open the device
		///
		void Open(void);

		///
		///	@brief	Gets all the available devices
		///
		///	Gets all the devices suitable to be used to make packet capture
		///	with PcapLive
		///
		static std::vector<Device> find_all(void);

#ifdef HAVE_REMOTE
		///
		///	@brief	Gets all the available devices
		///
		///	Gets all the devices suitable to be used to make packet capture
		///	with PcapLive
		///
		static std::vector<Device> find_all(std::string const& source);

		///
		///	@brief	Gets all the available devices
		///
		///	Gets all the devices suitable to be used to make packet capture
		///	with PcapLive
		///
		static std::vector<Device> find_all(std::string const& source, struct pcap_rmtauth * auth);

#endif

	private:

		Device(pcap_if_t const& iface);

	private:
		std::string			_name;		/* name to hand to "pcap_open_live()" */
		std::string			_description;	/* textual description of interface, or NULL */
		bpf_u_int32			_flags;	/* PCAP_IF_ interface flags */

		pcap_t *	_handle;

		std::string			_filter;

#ifdef HAVE_REMOTE
		struct pcap_rmtauth * _remoteAuth;
#endif


	};

} // namespace CppPcap

#endif // __CPPPCAP_DEVICE_H__