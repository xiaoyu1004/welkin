#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include <string>
#include <string_view>
#include <ostream>

#include "exception.h"

namespace welkin
{
    // an index represent a specific device
    using DeviceIndex = int8_t;

    enum class DeviceType : int8_t
    {
        CPU = 0,
        CUDA = 1,

        // NB: If you add more devices:
        //  - Change the implementations of DeviceTypeName and isValidDeviceType
        //    in DeviceType.cpp
        //  - Change the number below
        COMPILE_TIME_MAX_DEVICE_TYPES = 2,
    };

    constexpr DeviceType KCPU = DeviceType::CPU;
    constexpr DeviceType KCUDA = DeviceType::CUDA;

    // define explicit int constant
    constexpr int COMPILE_TIME_MAX_DEVICE_TYPES = static_cast<int>(DeviceType::COMPILE_TIME_MAX_DEVICE_TYPES);

    static_assert(COMPILE_TIME_MAX_DEVICE_TYPES <= 2, "You seem to be adding a lot of new DeviceTypes.");

    std::string DeviceTypeName(DeviceType d, bool lower_case = false);

    bool isValidDeviceType(DeviceType d);

    std::ostream &operator<<(std::ostream &stream, DeviceType type);

    // Represents a a compute device on which a tensor is located
    class Device
    {
    public:
        Device(DeviceType type, DeviceIndex index = -1) : m_type(type), m_index(index)
        {
            validate();
        }

        Device(const std::string &device_string);

        bool operator==(const Device &other) const noexcept
        {
            return m_type == other.m_type && m_index == other.m_index;
        }

        bool operator!=(const Device &other) const noexcept
        {
            return !(*this == other);
        }

        // set the device index
        void set_index(DeviceIndex index)
        {
            m_index = index;
        }

        DeviceType type() const noexcept
        {
            return m_type;
        }

        DeviceIndex index() const noexcept
        {
            return m_index;
        }

        // return true if has a non-defaualt index
        bool has_index() const noexcept
        {
            return m_index != -1;
        }

        bool is_cuda() const noexcept
        {
            return m_type == DeviceType::CUDA;
        }

        bool is_cpu() const noexcept
        {
            return m_type == DeviceType::CPU;
        }

        std::string str() const noexcept;

    private:
        DeviceType m_type;
        DeviceIndex m_index;

        void validate()
        {
            WELKIN_CHECK(m_index == -1 || m_index >= 0, "Device index must be -1 or non-negative, got %d", (int)m_index);
            WELKIN_CHECK(!is_cpu() || m_index <= 0, "CPU device index must be -1 or zero, got %d", (int)m_index);
        }
    };

    std::ostream &operator<<(std::ostream &os, const Device &device);
}

namespace std
{
    template <>
    struct hash<welkin::DeviceType>
    {
        size_t operator()(welkin::DeviceType k) const
        {
            return std::hash<int>()(static_cast<int>(k));
        }
    };

    template <>
    struct hash<welkin::Device>
    {
        size_t operator()(const welkin::Device &d) const noexcept
        {
            static_assert(sizeof(welkin::DeviceType) == 1, "DeviceType is not 8-bit");
            static_assert(sizeof(welkin::DeviceIndex) == 1, "DeviceIndex is not 8-bit");

            uint32_t bits = static_cast<uint32_t>(static_cast<uint8_t>(d.type())) << 16 | static_cast<uint32_t>(static_cast<uint8_t>(d.index()));
            return hash<uint32_t>()(bits);
        }
    };
    
}
#endif