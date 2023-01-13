﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#pragma once

#include <functional>
#include <map>
#include <vector>

#include "XamlTypeInfo.xaml.g.h"
#include "XamlMetaDataProvider.g.h"

/*
    If the file above was not generated, you may be missing a declaration
    for the XamlMetaDataProvider runtimeclass in your IDL.

    namespace AssistRepo
    {
        runtimeclass XamlMetaDataProvider : Microsoft.UI.Xaml.Markup.IXamlMetadataProvider
        {
            XamlMetaDataProvider();
        };
    }
*/

namespace winrt::AssistRepo::implementation
{
    using IXamlMember = ::winrt::Microsoft::UI::Xaml::Markup::IXamlMember;
    using IXamlType = ::winrt::Microsoft::UI::Xaml::Markup::IXamlType;

    struct XamlMetaDataProvider : public ::winrt::AssistRepo::implementation::XamlMetaDataProvider_base<XamlMetaDataProvider>
    {
        IXamlType GetXamlType(::winrt::Windows::UI::Xaml::Interop::TypeName const& type);
        IXamlType GetXamlType(::winrt::hstring const& fullName);
        ::winrt::com_array<::winrt::Microsoft::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions();

    private:
        std::shared_ptr<XamlTypeInfoProvider> _provider;
        std::shared_ptr<XamlTypeInfoProvider> Provider();
    };
}

namespace winrt::AssistRepo::factory_implementation
{
    struct XamlMetaDataProvider : XamlMetaDataProviderT<XamlMetaDataProvider, implementation::XamlMetaDataProvider>
    {
    };
}
