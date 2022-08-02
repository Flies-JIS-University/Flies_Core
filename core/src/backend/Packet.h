#pragma once

#include <string>
#include <vector>

namespace Flies {
	class Packet {
	public:
		std::string m_deviceId;
		uint16_t m_version = 2;
	public:
		Packet(const std::string& deviceId);
		Packet(const std::string&& deviceId);
		~Packet();
		// TODO copy move

		virtual uint8_t* encode() const;
		static  Packet decode(const uint8_t* const packet);
	};

}


