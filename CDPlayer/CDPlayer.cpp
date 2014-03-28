#include "stdafx.h"
#include "CDPlayer.h"
#include "NativePlayer.h"

namespace Alteridem { namespace CDPlayer
{
    CDPlayer::CDPlayer()
    {
        _nativePlayer = new NativePlayer();
    }

    // This method is called if the user explicitly disposes of the 
    // object (by calling the Dispose method in other managed languages,  
    // or the destructor in C++). The compiler emits as a call to  
    // GC::SuppressFinalize( this ) for you, so there is no need to  
    // call it here.
    CDPlayer::~CDPlayer()
    {
        // Dispose of managed resources.

        // Call C++ finalizer to clean up unmanaged resources. 
        this->!CDPlayer();

        // Mark the class as disposed. This flag allows you to throw an 
        // exception if a disposed object is accessed.
        disposed = true;
    }

    CDPlayer::!CDPlayer()
    {
        // Call the appropriate methods to clean up unmanaged  
        // resources here. If disposing is false when Dispose(bool, 
        // disposing) is called, only the following code is executed.
        delete _nativePlayer;
        _nativePlayer = nullptr;
    }
}}

