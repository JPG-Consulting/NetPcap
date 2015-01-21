// NetPcap.h

#pragma once

#include <Device.h>
#include <msclr\marshal_cppstd.h>
#include "Packet.h"

using namespace System;

namespace NetPcap {

	public ref class Device
	{

	public:
		delegate void OnDataArrivalDelegate();
		event OnDataArrivalDelegate ^OnDataArrival;

		~Device()
		{
			delete _device;
			_device = 0;
		}

		property String^ Name
		{
			String^ get() { return gcnew String(_device->Name().c_str()); }
		}

		property String^ Description
		{
			String^ get() { return gcnew String(_device->Description().c_str()); }
		}

		property String^ Filter
		{
			String^ get() { return gcnew String(_device->Filter().c_str()); }
			void set(String^ value) {
				_device->Filter( msclr::interop::marshal_as<std::string>(value) );
			}
		}

		property Boolean^ IsOpen
		{
			Boolean^ get() { return _device->IsOpen(); }
		}

		void BeginCapture() { BeginCapture(-1); }
		void BeginCapture(Int32 cnt);

		void EndCapture()
		{
			_device->EndCapture();
		}

		void Close()
		{
			_device->Close();
		}

		Int32^ NextPacket(NetPcap::Packet^ %packet);

		void Open()
		{
			_device->Open();
		}

		String^ ToString() override
		{
			return Name;
		}

		static array<Device^>^ GetAllNetworkInterfaces();

	public:
		Device::Device(CppPcap::Device device)
		{
			_device = new CppPcap::Device(device);
		}

		
		static void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
		{

		}
		
	private:
		CppPcap::Device * _device;
	};
}
