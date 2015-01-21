
#include "Device.h"
#include "PcapException.h"
//#include <assert.h> 

namespace CppPcap {

	Device::Device(pcap_if_t const& iface)
		:
		_name(iface.name ? iface.name : ""),
		_description(iface.description ? iface.description : ""),
		_flags(iface.flags)
	{
		_handle = NULL;
#ifdef HAVE_REMOTE
		_remoteAuth = NULL;
#endif
		//for (pcap_addr_t* a = iface.addresses; a; a = a->next)
		//	m_addrs.push_back(*a);
	}

	Device::Device(Device const& dev)
		:
		_name(dev._name),
		_description(dev._description),
		_flags(dev._flags),
#ifdef HAVE_REMOTE
		_remoteAuth(dev._remoteAuth),
#endif
		_handle(dev._handle)
	{ 
	}


	Device::~Device(void)
	{
		if (_handle != NULL) {
			pcap_close(_handle);
			_handle = NULL;
		}
	}

	void Device::BeginCapture(pcap_handler callback)
	{
		BeginCapture(-1, callback);
	}

	void Device::BeginCapture(int cnt, pcap_handler callback)
	{
		int result;

		result = pcap_loop(_handle, cnt, callback, NULL);

		if (result == 0) {
			// cnt is exhausted
			return;
		} else if (result == -1) {
			throw new PcapException(pcap_geterr(_handle));
		} else if (result == -2) {
			// loop terminated due to a call to pcap_breakloop() 
			return;
		}
	}

	void Device::EndCapture()
	{
		pcap_breakloop(_handle);
	}

	void Device::Close(void)
	{
		if (_handle != NULL) {
			pcap_close(_handle);
			_handle = NULL;
		}
	}

	int Device::NextPacket(Packet& packet)
	{
		int result;
		struct pcap_pkthdr *header;
		const u_char *pkt_data;
		Packet *p = NULL;

		switch ((result = pcap_next_ex(_handle, &header, &pkt_data))) {
			case 1:	// the packet has been read without problems
				p = new Packet(*header, pkt_data);
				packet = *p;
				break;
			case 0: // the timeout set with pcap_open_live() has elapsed. In this case pkt_header and pkt_data don't point to a valid packet.
				break;
			case -1: //an error occurred
				throw new PcapException(pcap_geterr(_handle));
				break;
			case -2: // EOF was reached reading from an offline capture
				break;
		}

		return result;
	}

	void Device::Open(void)
	{
		char errbuf[PCAP_ERRBUF_SIZE + 1];
#ifndef HAVE_REMOTE
		_handle = pcap_open_live(_name.c_str(), 65536, 1, 1000, errbuf);
#else
		_handle = pcap_open(_name.capacity_str(), 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, _remoteAuth, errbuf);
#endif
		if (_handle == NULL) {
			throw PcapException(errbuf);
			return;
		}

		// Compile the filter if any
		if (_filter.size() > 0) {
			struct bpf_program fp;

			if (pcap_compile(_handle, &fp, strdup(_filter.c_str()), 0, 0) == -1) {
				pcap_close(_handle);
				_handle = NULL;
				throw new PcapException( pcap_geterr(_handle) );
				return;
			}

			if (pcap_setfilter(_handle, &fp) == -1) {
				pcap_close(_handle);
				_handle = NULL;
				throw new PcapException(pcap_geterr(_handle));
				return;
			}
		}

	}

	// [static]
	std::vector<Device> Device::find_all()
	{
		pcap_if_t* ifs;
		char errbuf[PCAP_ERRBUF_SIZE + 1];

#ifndef HAVE_REMOTE
		if (pcap_findalldevs(&ifs, errbuf) < 0)
#else
		if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &ifs, errbuf) < 0)
#endif
			throw PcapException(errbuf);

		std::vector<Device> devs;

		for (pcap_if_t* i = ifs; i; i = i->next)
			devs.push_back(*i);

		pcap_freealldevs(ifs);

		return devs;
	}

#ifdef HAVE_REMOTE

	std::vector<Device> Device::find_all(std::string const& source)
	{
		return find_all(source, NULL);
	}

	std::vector<Device> Device::find_all(std::string const& source, struct pcap_rmtauth * auth)
	{
		pcap_if_t* ifs;
		char errbuf[PCAP_ERRBUF_SIZE + 1];

		// TODO: is this save?
		_remoteAuth = auth;

		if (pcap_findalldevs_ex(strdup(source.c_str()), auth, &ifs, errbuf) < 0)
			throw PcapException(errbuf);

		std::vector<Device> devs;

		for (pcap_if_t* i = ifs; i; i = i->next)
			devs.push_back(*i);

		pcap_freealldevs(ifs);

		return devs;
	}

#endif

} // namespace CppPcap