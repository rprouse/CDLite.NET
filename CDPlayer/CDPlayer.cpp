#include "stdafx.h"
#include "CDPlayer.h"
#include "NativePlayer.h"

namespace Alteridem { namespace CD
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

    bool CDPlayer::PreviousTrack()
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

    Mode CDPlayer::Mode::get()
    {
        DWORD_PTR mode = _nativePlayer->GetMode();
        switch ( mode )
        {
        case MCI_MODE_NOT_READY:
            return Alteridem::CD::Mode::NotReady;
        case MCI_MODE_PAUSE:
            return Alteridem::CD::Mode::Pause;
        case MCI_MODE_PLAY:
            return Alteridem::CD::Mode::Play;
        case MCI_MODE_STOP:
            return Alteridem::CD::Mode::Stop;
        case MCI_MODE_OPEN:
            return Alteridem::CD::Mode::Open;
        case MCI_MODE_RECORD:
            return Alteridem::CD::Mode::Record;
        case MCI_MODE_SEEK:
            return Alteridem::CD::Mode::Seek;
        default:
            return Alteridem::CD::Mode::Unknown;
        }
    }

    int  CDPlayer::Track::get()
    {
        return _nativePlayer->GetTrack();
    }

    int  CDPlayer::NumberOfTracks::get()
    {
        return _nativePlayer->GetNumberOfTracks();
    }

    System::TimeSpan CDPlayer::Time::get()
    {
        DWORD_PTR dwTMSF = _nativePlayer->GetPosition();
        int min = MCI_TMSF_MINUTE( dwTMSF );
        int sec = MCI_TMSF_SECOND( dwTMSF );
        return System::TimeSpan( 0, min, sec );
    }

    /// @return A string containing the drive letters for valid CDROM drives
    //wchar_t* CDPlayer::GetDrives();

    /// @return The number of CD drives on a system
    unsigned int CDPlayer::NumberOfDrives::get()
    {
        return _nativePlayer->GetNumDrives();
    }

    /**
    * Sets the CD drive that we will be using
    * @param id The index into the string returned by GetDrives() that we want to open
    * @return 0 on success, 1 on failure
    **/
    void CDPlayer::Drive::set( unsigned int id )
    {
        _nativePlayer->SetDriveID( id );
    }

    /**
    * @return The index into the string returned by GetDrives() that we are currently using
    **/
    unsigned int CDPlayer::Drive::get()
    {
        return _nativePlayer->GetDriveID();
    }

    /**
    * Gets the last error
    * @return The last error that happened
    **/
    System::String^ CDPlayer::LastError::get()
    {
        return gcnew System::String( _nativePlayer->GetLastError() );
    }
}}

