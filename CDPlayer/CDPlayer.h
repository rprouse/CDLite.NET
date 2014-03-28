#pragma once

class NativePlayer;

namespace Alteridem { namespace CDPlayer
{
    public ref class CDPlayer : public System::IDisposable
    {
    public:
        CDPlayer();
        ~CDPlayer();
        !CDPlayer();

    private:
        NativePlayer* _nativePlayer;

        // Track whether Dispose has been called. 
        bool disposed;
    };
}}
