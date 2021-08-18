#include "device.h"

namespace welkin
{
    std::string DeviceTypeName(DeviceType d, bool lower_case = false)
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

    DeviceType parse_type(const std::string &device_string)
    {
        const std::array<std::pair<std::string, DeviceType>, static_cast<size_t>(DeviceType::COMPILE_TIME_MAX_DEVICE_TYPES)> types =
            {
                {"cpu" : DeviceType::CPU},
                {"cuda" : DeviceType::CUDA}};
        std::array < std::pair<std::string, DeviceType>::const_iterator device = std::find_if(
            types.begin(), types.end(), [device_string](const std::pair<std::string, DeviceType> &p)
            { return p.first == device_string; });
        if (device != types.end())
        {
            return device->second;
        }
    }

    Device::Device(const std::string &device_string)
    {

    }

    std::string Device::str() const noexcept
    {
        std::string str = DeviceTypeName(type(), true);
        if (has_index())
        {
            str.push_back(':');
            str.append(to_string(index()));
        }
        return str;
    }

    std::ostream &operator<<(std::ostream &os, const Device &device)
    {
        os << device.str();
        return os;
    }
}