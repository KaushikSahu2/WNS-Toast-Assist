// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#pragma once
#include <iostream>
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

#include <ShObjIdl.h>

#include <winrt/Windows.ApplicationModel.Background.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Microsoft.Windows.PushNotifications.h>

#include "wintoastlib.h"
#include <string>  

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::Windows::AppLifecycle;
using namespace winrt::Microsoft::Windows::PushNotifications;
using namespace std;
using namespace winrt;

// "To obtain your Azure AppId,
// follow "Configure your app's identity in Azure Active Directory" at https://docs.microsoft.com/en-us/windows/apps/windows-app-sdk/notifications/push/push-quickstart
winrt::guid remoteId{ "62af49ea-00ae-431b-bca7-ebb9652ee31e" }; // Replace this with your own Azure AppId



//*********************************************************************************
//*********************************************************************************

using namespace WinToastLib;

void launchQA()
{
    LPCWSTR appUserModelId = L"MicrosoftCorporationII.QuickAssist_8wekyb3d8bbwe!App";

    //CoInitialize(NULL);
    IApplicationActivationManager* paam = NULL;
    HRESULT hr = E_FAIL;
    __try
    {
        hr = CoCreateInstance(CLSID_ApplicationActivationManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&paam));
        if (FAILED(hr)) return;

        DWORD pid = 0;
        hr = paam->ActivateApplication(appUserModelId, nullptr, AO_NONE, &pid);
        if (FAILED(hr)) return;

        if (hr == 0)
            wprintf(L"Activated  %s with pid %d\r\n", appUserModelId, pid);
    }
    __finally
    {
        if (paam) paam->Release();
    }

    // CoUninitialize();
}

class CustomHandler : public IWinToastHandler {
public:
    void toastActivated() const {
        std::wcout << L"The user clicked in this toast" << std::endl;
        exit(0);
    }

    void toastActivated(int actionIndex) const {
        std::wcout << L"The user clicked on action #" << actionIndex << std::endl;
        if (actionIndex == 0) {
            launchQA();
        }
        exit(16 + actionIndex);
    }

    void toastDismissed(WinToastDismissalReason state) const {
        switch (state) {
        case UserCanceled:
            std::wcout << L"The user dismissed this toast" << std::endl;
            exit(1);
            break;
        case TimedOut:
            std::wcout << L"The toast has timed out" << std::endl;
            exit(2);
            break;
        case ApplicationHidden:
            std::wcout << L"The application hid the toast using ToastNotifier.hide()" << std::endl;
            exit(3);
            break;
        default:
            std::wcout << L"Toast not activated" << std::endl;
            exit(4);
            break;
        }
    }

    void toastFailed() const {
        std::wcout << L"Error showing current toast" << std::endl;
        exit(5);
    }
};


enum Results {
    ToastClicked,					// user clicked on the toast
    ToastDismissed,					// user dismissed the toast
    ToastTimeOut,					// toast timed out
    ToastHided,						// application hid the toast
    ToastNotActivated,				// toast was not activated
    ToastFailed,					// toast failed
    SystemNotSupported,				// system does not support toasts
    UnhandledOption,				// unhandled option
    MultipleTextNotSupported,		// multiple texts were provided
    InitializationFailure,			// toast notification manager initialization failure
    ToastNotLaunched				// toast could not be launched
};


#define COMMAND_ACTION		L"--action"
#define COMMAND_AUMI		L"--aumi"
#define COMMAND_APPNAME		L"--appname"
#define COMMAND_APPID		L"--appid"
#define COMMAND_EXPIREMS	L"--expirems"
#define COMMAND_TEXT		L"--text"
#define COMMAND_HELP		L"--help"
#define COMMAND_IMAGE		L"--image"
#define COMMAND_SHORTCUT	L"--only-create-shortcut"
#define COMMAND_AUDIOSTATE  L"--audio-state"
#define COMMAND_ATTRIBUTE   L"--attribute"

void print_help() {
    std::wcout << "WinToast Console Example [OPTIONS]" << std::endl;
    std::wcout << "\t" << COMMAND_ACTION << L" : Set the actions in buttons" << std::endl;
    std::wcout << "\t" << COMMAND_AUMI << L" : Set the App User Model Id" << std::endl;
    std::wcout << "\t" << COMMAND_APPNAME << L" : Set the default appname" << std::endl;
    std::wcout << "\t" << COMMAND_APPID << L" : Set the App Id" << std::endl;
    std::wcout << "\t" << COMMAND_EXPIREMS << L" : Set the default expiration time" << std::endl;
    std::wcout << "\t" << COMMAND_TEXT << L" : Set the text for the notifications" << std::endl;
    std::wcout << "\t" << COMMAND_IMAGE << L" : set the image path" << std::endl;
    std::wcout << "\t" << COMMAND_ATTRIBUTE << L" : set the attribute for the notification" << std::endl;
    std::wcout << "\t" << COMMAND_SHORTCUT << L" : create the shortcut for the app" << std::endl;
    std::wcout << "\t" << COMMAND_AUDIOSTATE << L" : set the audio state: Default = 0, Silent = 1, Loop = 2" << std::endl;
    std::wcout << "\t" << COMMAND_HELP << L" : Print the help description" << std::endl;
}


int localToast()
{
    if (!WinToast::isCompatible()) {
        std::wcerr << L"Error, your system in not supported!" << std::endl;
        return Results::SystemNotSupported;
    }

    LPWSTR appName = L"Assist POC",
        appUserModelID = L"Assist POC",
        text = NULL,
        imagePath = NULL,
        attribute = L"default";
    std::vector<std::wstring> actions;
    INT64 expiration = 0;

    bool onlyCreateShortcut = false;
    WinToastTemplate::AudioOption audioOption = WinToastTemplate::AudioOption::Loop;

    WinToast::instance()->setAppName(appName);
    WinToast::instance()->setAppUserModelId(appUserModelID);

    onlyCreateShortcut = false;

    if (onlyCreateShortcut) {
        if (imagePath || text || actions.size() > 0 || expiration) {
            std::wcerr << L"--only-create-shortcut does not accept images/text/actions/expiration" << std::endl;
            return 9;
        }
        enum WinToast::ShortcutResult result = WinToast::instance()->createShortcut();
        return result ? 16 + result : 0;
    }

    if (!text)
        text = L"Hello, world!";

    if (!WinToast::instance()->initialize()) {
        std::wcerr << L"Error, your system in not compatible!" << std::endl;
        return Results::InitializationFailure;
    }

    bool withImage = (imagePath != NULL);
    WinToastTemplate templ = WinToastTemplate(WinToastTemplate::ImageAndText02);
    templ.setTextField(L"Inner Circle", WinToastTemplate::FirstLine);
    templ.setTextField(L"Kaushik wants assist from you", WinToastTemplate::SecondLine);
    templ.setImagePath(L"E:\\Windows\\POC\\AssistLaunchPushToast\\image.png");
    templ.setScenario(WinToastTemplate::Scenario::IncomingCall);
    templ.setAudioOption(WinToastTemplate::AudioOption::Loop);
    templ.addAction(L"Assist");
    templ.addAction(L"Decline");

    if (WinToast::instance()->showToast(templ, new CustomHandler()) == -1L)
    {

    }

    // Give the handler a chance for 15 seconds (or the expiration plus 1 second)
    Sleep(expiration ? (DWORD)expiration + 1000 : 15000);

    exit(2);
}

//*************************************************************************************
//*************************************************************************************
/*
// The GUID must be unique to your app. Create a new GUID if copying this code.
class DECLSPEC_UUID("replaced-with-your-guid-C173E6ADF0C3") NotificationActivator WrlSealed WrlFinal
    : public RuntimeClass<RuntimeClassFlags<ClassicCom>, INotificationActivationCallback>
{
public:
    virtual HRESULT STDMETHODCALLTYPE Activate(
        _In_ LPCWSTR appUserModelId,
        _In_ LPCWSTR invokedArgs,
        _In_reads_(dataCount) const NOTIFICATION_USER_INPUT_DATA * data,
        ULONG dataCount) override
    {
        std::wstring arguments(invokedArgs);
        HRESULT hr = S_OK;

        // Background: Quick reply to the conversation
        if (arguments.find(L"action=reply") == 0)
        {
            // Get the response user typed.
            // We know this is first and only user input since our toasts only have one input
            LPCWSTR response = data[0].Value;

            hr = DesktopToastsApp::SendResponse(response);
        }

        else
        {
            // The remaining scenarios are foreground activations,
            // so we first make sure we have a window open and in foreground
            hr = DesktopToastsApp::GetInstance()->OpenWindowIfNeeded();
            if (SUCCEEDED(hr))
            {
                // Open the image
                if (arguments.find(L"action=viewImage") == 0)
                {
                    hr = DesktopToastsApp::GetInstance()->OpenImage();
                }

                // Open the app itself
                // User might have clicked on app title in Action Center which launches with empty args
                else
                {
                    // Nothing to do, already launched
                }
            }
        }

        if (FAILED(hr))
        {
            // Log failed HRESULT
        }

        return S_OK;
    }

    ~NotificationActivator()
    {
        // If we don't have window open
        if (!DesktopToastsApp::GetInstance()->HasWindow())
        {
            // Exit (this is for background activation scenarios)
            exit(0);
        }
    }
};

// Flag class as COM creatable
CoCreatableClass(NotificationActivator);*/


//*************************************************************************************
//*************************************************************************************

winrt::Windows::Foundation::IAsyncOperation<PushNotificationChannel> RequestChannelAsync()
{
    auto channelOperation{ PushNotificationManager::Default().CreateChannelAsync(remoteId) };

    // Setup the in-progress event handler
    channelOperation.Progress(
        [](auto&& sender, auto&& args)
        {
            if (args.status == PushNotificationChannelStatus::InProgress)
            {
                // This is basically a noop since it isn't really an error state
                std::cout << "\nWNS Channel URI request is in progress." << std::endl;
            }
            else if (args.status == PushNotificationChannelStatus::InProgressRetry)
            {
               
            }
        });

    auto result{ co_await channelOperation };

    if (result.Status() == PushNotificationChannelStatus::CompletedSuccess)
    {
        auto channel{ result.Channel() };

        std::cout << "\nWNS Channel URI: " << winrt::to_string(channel.Uri().ToString()) << std::endl;

        // It's the caller's responsibility to keep the channel alive
        co_return channel;
    }
    else if (result.Status() == PushNotificationChannelStatus::CompletedFailure)
    {
        co_return nullptr;
    }
    else
    {
        co_return nullptr;
    }

};

winrt::Microsoft::Windows::PushNotifications::PushNotificationChannel RequestChannel()
{
    auto task{ RequestChannelAsync() };
    if (task.wait_for(std::chrono::minutes(5)) != winrt::Windows::Foundation::AsyncStatus::Completed)
    {
        task.Cancel();
        return nullptr;
    }

    auto result{ task.GetResults() };
    return result;
}

// Subscribe to an event which will get signaled whenever a foreground notification arrives.
void SubscribeForegroundEventHandler()
{
    winrt::event_token token{ PushNotificationManager::Default().PushReceived([](auto const&, PushNotificationReceivedEventArgs const& args)
    {
        auto payload{ args.Payload() };

        std::string payloadString(payload.begin(), payload.end());
        std::cout << "\nPush notification content received in the FOREGROUND: " << payloadString << std::endl;
        localToast();
        //launchQA();
    }) };
}

void setupNotification()
{
    // Setup an event handler, so we can receive notifications in the foreground while the app is running.
    SubscribeForegroundEventHandler();

    PushNotificationManager::Default().Register();

    auto args{ AppInstance::GetCurrent().GetActivatedEventArgs() };
    switch (args.Kind())
    {
        // When it is launched normally (by the users, or from the debugger), the sample requests a WNS Channel URI and
        // displays it, then waits for notifications. This user can take a copy of the WNS Channel URI and use it to send
        // notifications to the sample
    case ExtendedActivationKind::Launch:
    {
        // Request a WNS Channel URI which can be passed off to an external app to send notifications to.
        // The WNS Channel URI uniquely identifies this app for this user and device.
        PushNotificationChannel channel{ RequestChannel() };
        if (!channel)
        {
            std::cout << "\nThere was an error obtaining the WNS Channel URI" << endl;

            if (remoteId == winrt::guid{ "00000000-0000-0000-0000-000000000000" })
            {
                std::cout << "\nThe remoteId has not been set. Refer to the readme file accompanying this sample\nfor the instructions on how to obtain and setup a remote id" << std::endl;
            }
        }

        std::cout << "\nPress 'Enter' at any time to exit App." << std::endl;
        std::cin.ignore();
    }
    break;

    // When it is activated from a push notification, the sample only displays the notification.
    // It doesnít register for foreground activation of perform any other actions
    // because background activation is meant to let app perform only small tasks in order to preserve battery life.
    case ExtendedActivationKind::Push:
    {
        PushNotificationReceivedEventArgs pushArgs{ args.Data().as<PushNotificationReceivedEventArgs>() };

        // Call GetDeferral to ensure that code runs in low power
        auto deferral{ pushArgs.GetDeferral() };

        auto payload{ pushArgs.Payload() };

        // Do stuff to process the raw notification payload
        std::string payloadString(payload.begin(), payload.end());
        std::cout << "\nPush notification content received in the BACKGROUND: " << payloadString.c_str() << std::endl;
        std::cout << "\nPress 'Enter' to exit the App." << std::endl;
        localToast();
        launchQA();

        // Call Complete on the deferral when finished processing the payload.
        // This removes the override that kept the app running even when the system was in a low power mode.
        deferral.Complete();
        std::cin.ignore();
    }
    break;

    default:
        std::cout << "\nUnexpected activation type" << std::endl;
        std::cout << "\nPress 'Enter' to exit the App." << std::endl;
        std::cin.ignore();
        break;
    }

    // We do not call PushNotificationManager::UnregisterActivator
    // because then we wouldn't be able to receive background activations, once the app has closed.
    // Call UnregisterActivator once you don't want to receive push notifications anymore.
}

namespace winrt::AssistRepo::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        setupNotification();
        //localToast();
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
