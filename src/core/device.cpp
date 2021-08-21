#include "device.h"

#include <regex>
#include <exception>
#include <array>
#include <utility>

namespace welkin
{
    std::string DeviceTypeName(DeviceType d, bool lower_case)
    {
        switch (d)
        {
        case DeviceType::CPU:
            return lower_case ? "cpu" : "CPU";
        case DeviceType::CUDA:
            return lower_case ? "cuda" : "CUDA";
        default:
            return "";
        }
    }

    bool isValidDeviceType(DeviceType d)
    {
        switch (d)
        {
        case DeviceType::CPU:
        case DeviceType::CUDA:
            return true;
        default:
            return false;
        }
    }

    std::ostream &operator<<(std::ostream &os, DeviceType type)
    {
        os << DeviceTypeName(type, true);
        return os;
    }

    DeviceType parse_type(const std::string_view &device_string)
    {
        const std::array<std::pair<std::string, DeviceType>, static_cast<size_t>(DeviceType::COMPILE_TIME_MAX_DEVICE_TYPES)> types =
            {{{"cpu", DeviceType::CPU},
              {"cuda", DeviceType::CUDA}}};
        auto device = std::find_if(
            types.begin(), types.end(), [device_string](const std::pair<std::string, DeviceType> &p)
            { return p.first == device_string; });
        if (device != types.end())
        {
            return device->second;
        }
        return DeviceType::CPU;
    }

    Device::Device(const std::string &device_string) : Device(DeviceType::CPU)
    {
        WELKIN_CHECK(!device_string.empty(), "device_string must not be empty!");
        const std::regex regex("([a-zA-Z_]+)(?::([1-9]\\d*|0))?");
        std::smatch match;
        WELKIN_CHECK(std::regex_match(device_string, match, regex), "invalid device_string: %s", device_string.c_str());
        m_type = parse_type(match[1].str());
        if (match[2].matched)
        {
            try
            {
                m_index = static_cast<DeviceIndex>(std::stoi(match[2].str()));
            }
            catch (const std::exception &)
            {
                WELKIN_INFO("could not parse device index: %s, in device string: %s", match[2].str().c_str(), device_string.c_str());
            }
        }
    }

    std::string Device::str() const noexcept
    {
        std::string str = DeviceTypeName(type(), true);
        if (has_index())
        {
            str.push_back(':');
            str.append(std::to_string(index()));
        }
        return str;
    }

    std::ostream &operator<<(std::ostream &os, const Device &device)
    {
        os << device.str();
        return os;
    }
}