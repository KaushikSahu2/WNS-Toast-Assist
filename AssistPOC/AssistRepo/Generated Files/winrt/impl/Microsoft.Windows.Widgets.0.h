// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.220929.3

#pragma once
#ifndef WINRT_Microsoft_Windows_Widgets_0_H
#define WINRT_Microsoft_Windows_Widgets_0_H
WINRT_EXPORT namespace winrt::Microsoft::Windows::Widgets
{
    enum class WidgetSize : int32_t
    {
        Small = 0,
        Medium = 1,
        Large = 2,
    };
    struct WidgetContract;
}
namespace winrt::impl
{
    template <> struct category<winrt::Microsoft::Windows::Widgets::WidgetSize>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<winrt::Microsoft::Windows::Widgets::WidgetSize> = L"Microsoft.Windows.Widgets.WidgetSize";
    template <> inline constexpr auto& name_v<winrt::Microsoft::Windows::Widgets::WidgetContract> = L"Microsoft.Windows.Widgets.WidgetContract";
}
#endif