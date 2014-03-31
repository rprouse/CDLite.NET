#pragma once

class NativePlayer;

namespace Alteridem { namespace CDPlayer
{
    public enum Mode
    {
        Unknown,
        NotReady,
        Pause,
        Play,
        Stop,
        Open,
        Record,
        Seek
    };

    public ref class CDPlayer : public System::IDisposable
    {
    public:
        CDPlayer();
        ~CDPlayer();
        !CDPlayer();

        bool PlayTrack( int track );
        bool Open();
        bool Close();
        bool Stop();
        bool NextTrack();
        bool PrevTrack();
        bool Play();
        bool Pause();
        bool Eject();
        
        Mode GetMode();
        int  GetTrack();
        int  GetNumberOfTracks();
        System::TimeSpan^ GetTime();

        /// @return A string containing the drive letters for valid CDROM drives
        //wchar_t* GetDrives();

        /// @return The number of CD drives on a system
        unsigned int GetNumDrives();

        /**
        * Sets the CD drive that we will be using
        * @param id The index into the string returned by GetDrives() that we want to open
        * @return 0 on success, 1 on failure
        **/
        bool SetDrive( unsigned int id );

        /**
        * @return The index into the string returned by GetDrives() that we are currently using
        **/
        unsigned int GetDrive();

        /**
        * Gets the last error
        * @return The last error that happened
        **/
        System::String^ GetLastError();

    private:
        NativePlayer* _nativePlayer;

        // Track whether Dispose has been called. 
        bool disposed;
    };
}}
