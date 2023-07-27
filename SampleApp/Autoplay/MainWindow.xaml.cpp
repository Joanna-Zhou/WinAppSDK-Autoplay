// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <shobjidl.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

static CLSID CLSID_SyncEngineFileInfoProviderLocalDef = { 0x71DCE5D6, 0x4B57, 0x496B, 0xAC, 0x21, 0xCD, 0x5B, 0x54, 0xEB, 0x93, 0xFD };

namespace winrt::Autoplay::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
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

    HRESULT MainWindow::RegisterComponent(IUnknown* punk)
    {
        HRESULT hr = E_FAIL;
        HINSTANCE hinstShSvcs = LoadLibrary(TEXT("shsvcs.dll"));
        DWORD _dwRegisterROT;
        DWORD _dwRegisterClass;

        if (hinstShSvcs)
        {
            CREATEHARDWAREEVENTMONIKER fct = (CREATEHARDWAREEVENTMONIKER)GetProcAddress(hinstShSvcs, "CreateHardwareEventMoniker");
            if (fct)
            {
                IMoniker* pmoniker;

                hr = fct(CLSID_SyncEngineFileInfoProviderLocalDef, TEXT("ShowPicturesOnArrival"), &pmoniker);

                if (SUCCEEDED(hr))
                {
                    IRunningObjectTable* prot;

                    if (SUCCEEDED(GetRunningObjectTable(0, &prot)))
                    {
                        hr = prot->Register(ROTFLAGS_ALLOWANYCLIENT | ROTFLAGS_REGISTRATIONKEEPSALIVE, punk, pmoniker, &_dwRegisterROT);
                        prot->Release();
                    }
                    pmoniker->Release();
                }
                CoRegisterClassObject(CLSID_SyncEngineFileInfoProviderLocalDef, static_cast<IClassFactory*>(this), CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &_dwRegisterClass);
            }
            FreeLibrary(hinstShSvcs);
        }
        return hr;
    }
}
