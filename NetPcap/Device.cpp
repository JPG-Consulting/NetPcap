// This is the main DLL file.

#include "stdafx.h"

#include <vector>
#include "Device.h"
#include <Packet.h>

namespace NetPcap {

	void Device::BeginCapture(Int32 cnt)
	{
		//_device->BeginCapture(cnt, Device::packet_handler );
	}

	Int32^ Device::NextPacket(NetPcap::Packet^ %packet)
	{
		Int32^ result;
		CppPcap::Packet cpppcap_packet;

		result = _device->NextPacket(cpppcap_packet);
		packet = gcnew NetPcap::Packet(cpppcap_packet);
		return result;
	}

	array<Device^>^ Device::GetAllNetworkInterfaces()
	{
		std::vector<CppPcap::Device> devsVec = CppPcap::Device::find_all();

		array<Device^>^ devices = gcnew array<Device^>(devsVec.size());
		for (int i = 0; i < devsVec.size(); i++)
		{
			devices[i] = gcnew Device(devsVec[i]);
		}

		return devices;
	}

}