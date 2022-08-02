#include "Packet.h"
#include <algorithm>

namespace Flies {

	Packet::Packet(const std::string& deviceId) {
	}
	Packet::Packet(const std::string&& deviceId) {
	}



	uint8_t* Packet::encode() const
	{
		std::vector<uint8_t> buff;
		std::copy(m_deviceId.c_str(), m_deviceId.c_str() + m_deviceId.length() + 1, buff.begin() + buff.size());

		return 	(uint8_t*) &buff;
	}

	Packet Packet::decode(const uint8_t* const packet)
	{
		return Packet("hello");
	}

}
