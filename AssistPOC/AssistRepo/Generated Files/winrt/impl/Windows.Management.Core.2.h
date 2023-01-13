// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.220929.3

#pragma once
#ifndef WINRT_Windows_Management_Core_2_H
#define WINRT_Windows_Management_Core_2_H
#include "winrt/impl/Windows.Management.Core.1.h"
WINRT_EXPORT namespace winrt::Windows::Management::Core
{
    struct __declspec(empty_bases) ApplicationDataManager : winrt::Windows::Management::Core::IApplicationDataManager
    {
        ApplicationDataManager(std::nullptr_t) noexcept {}
        ApplicationDataManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Core::IApplicationDataManager(ptr, take_ownership_from_abi) {}
        static auto CreateForPackageFamily(param::hstring const& packageFamilyName);
    };
}
#endif
