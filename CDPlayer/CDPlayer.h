#pragma once

class NativePlayer;

namespace Alteridem { namespace CD
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
        bool PreviousTrack();
        bool Play();
        bool Pause();
        bool Eject();
        
        property Alteridem::CD::Mode Mode{ Alteridem::CD::Mode get(); };
        property int Track{ int get(); };
        property int NumberOfTracks{ int get(); };
        property System::TimeSpan Time{ System::TimeSpan get(); };

        /// @return A string containing the drive letters for valid CDROM drives
        //wchar_t* GetDrives();

        /// @return The number of CD drives on a system
        property unsigned int NumberOfDrives{ unsigned int get(); };

        /**
        * Sets the CD drive that we will be using
        * @param id The index into the string returned by GetDrives() that we want to open
        * @return 0 on success, 1 on failure
        **/
        property unsigned int Drive
        {
            unsigned int get();
            void set( unsigned int );
        }

        /**
        * Gets the last error
        * @return The last error that happened
        **/
        property System::String^ LastError{ System::String^ get(); };

    private:
        NativePlayer* _nativePlayer;

        // Track whether Dispose has been called. 
        bool disposed;
    };
}}
