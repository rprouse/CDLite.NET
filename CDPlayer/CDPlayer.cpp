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

    bool CDPlayer::PlayTrack( int track )
    {
        return _nativePlayer->PlayTrack( track );
    }

    bool CDPlayer::Open()
    {
        return _nativePlayer->Open();
    }

    bool CDPlayer::Close()
    {
        return _nativePlayer->Close();
    }

    bool CDPlayer::Stop()
    {
        return _nativePlayer->Stop();
    }

    bool CDPlayer::NextTrack()
    {
        return _nativePlayer->NextTrack();
    }

    bool CDPlayer::PrevTrack()
    {
        return _nativePlayer->PrevTrack();
    }

    bool CDPlayer::Play()
    {
        return _nativePlayer->Play();
    }

    bool CDPlayer::Pause()
    {
        return _nativePlayer->Pause();
    }

    bool CDPlayer::Eject()
    {
        return _nativePlayer->Eject();
    }

    Mode CDPlayer::GetMode()
    {
        DWORD mode = _nativePlayer->GetMode();
        switch ( mode )
        {
        case MCI_MODE_NOT_READY:
            return Mode::NotReady;
        case MCI_MODE_PAUSE:
            return Mode::Pause;
        case MCI_MODE_PLAY:
            return Mode::Play;
        case MCI_MODE_STOP:
            return Mode::Stop;
        case MCI_MODE_OPEN:
            return Mode::Open;
        case MCI_MODE_RECORD:
            return Mode::Record;
        case MCI_MODE_SEEK:
            return Mode::Seek;
        default:
            return Mode::Unknown;            
        }
    }

    int  CDPlayer::GetTrack()
    {
        return _nativePlayer->GetTrack();
    }

    int  CDPlayer::GetNumberOfTracks()
    {
        return _nativePlayer->GetNumberOfTracks();
    }

    System::TimeSpan^ CDPlayer::GetTime()
    {
        DWORD dwTMSF = _nativePlayer->GetPosition();
        int min = MCI_TMSF_MINUTE( dwTMSF );
        int sec = MCI_TMSF_SECOND( dwTMSF );
        return gcnew System::TimeSpan( 0, min, sec );
    }

    /// @return A string containing the drive letters for valid CDROM drives
    //wchar_t* CDPlayer::GetDrives();

    /// @return The number of CD drives on a system
    unsigned int CDPlayer::GetNumDrives()
    {
        return _nativePlayer->GetNumDrives();
    }

    /**
    * Sets the CD drive that we will be using
    * @param id The index into the string returned by GetDrives() that we want to open
    * @return 0 on success, 1 on failure
    **/
    bool CDPlayer::SetDrive( unsigned int id )
    {
        return _nativePlayer->SetDriveID( id );
    }

    /**
    * @return The index into the string returned by GetDrives() that we are currently using
    **/
    unsigned int CDPlayer::GetDrive()
    {
        return _nativePlayer->GetDriveID();
    }

    /**
    * Gets the last error
    * @return The last error that happened
    **/
    System::String^ CDPlayer::GetLastError()
    {
        return gcnew System::String( _nativePlayer->GetLastError() );
    }
}}

