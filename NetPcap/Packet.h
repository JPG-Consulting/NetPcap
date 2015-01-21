// NetPcap.h

#pragma once

#include <Packet.h>

using namespace System;

namespace NetPcap {

	public ref class Packet
	{

	public:

		~Packet()
		{
			delete _packet;
			_packet = 0;
		}

		property UInt32 CaptureLength
		{
			UInt32 get() { return _packet->Header().caplen; }
		}

		property UInt32 Length
		{
			UInt32 get() { return _packet->Header().len; }
		}

		property array<Byte>^ Data
		{
			array<Byte>^ get() {
				_packet->manage();

				int count = _packet->Header().caplen;
				if (count == 0) {
					System::Collections::Generic::List<Byte>^ list = gcnew System::Collections::Generic::List<Byte>();
					return list->ToArray();
				}

				array<Byte>^ managedBytes = gcnew array<Byte>(count);
				System::Runtime::InteropServices::Marshal::Copy((IntPtr)const_cast<unsigned char*>(_packet->Data()), managedBytes, 0, count);
				return managedBytes;
			}
		}


	public:
		Packet::Packet(CppPcap::Packet packet)
		{
			bpf_u_int32 clen = packet.Header().caplen;
			const u_char * data = packet.Data();
			_packet = new CppPcap::Packet(packet);
		}

	private:
		CppPcap::Packet * _packet;
	};
}