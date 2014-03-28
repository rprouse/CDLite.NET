#pragma once

#include <Windows.h>
#include <string>

class NativePlayer
{
public:
    NativePlayer();
    virtual ~NativePlayer();

    DWORD PlayTrack( BYTE bTrack );
    DWORD Open();
    void  Close();
    void  Stop();
    void  Attach( HWND hWnd );
    DWORD NextTrack();
    DWORD PrevTrack();
    DWORD Play();
    DWORD Pause();
    void  Eject();

    /**
    * @return MCI_MODE_NOT_READY
    *         MCI_MODE_PAUSE
    *         MCI_MODE_PLAY
    *         MCI_MODE_STOP
    *         MCI_MODE_OPEN
    *         MCI_MODE_RECORD
    *         MCI_MODE_SEEK
    */
    DWORD GetMode();
    BYTE  GetTrack();
    BYTE  GetNumberOfTracks();
    DWORD GetPosition();

    // Pass in the error code returned from another function to display
    // a message box with the error.
    void  ShowError( DWORD dwError );

    /// @return A string containing the drive letters for valid CDROM drives
    wchar_t* GetDrives();

    /// @return The number of CD drives on a system
    unsigned int    GetNumDrives();

    /**
    * Sets the CD drive that we will be using
    * @param id The index into the string returned by GetDrives() that we want to open
    * @return 0 on success, 1 on failure
    **/
    DWORD   SetDriveID( unsigned int id );

    /**
    * @return The index into the string returned by GetDrives() that we are currently using
    **/
    unsigned int    GetDriveID();

protected:
    unsigned int    m_wDeviceID;
    HWND    m_hWnd;
    unsigned int    m_currentDevice;    // Which CD device in m_strCdRomDrives is currently being used
    unsigned int    m_numDevices;    // The number of valid CD drives
    wchar_t*  m_strCdRomDrives;    // A string containing valid CD drives, like "DE"

    /**
    MCI_STATUS_CURRENT_TRACK
    MCI_STATUS_LENGTH - Returns the total media length.
    MCI_STATUS_MODE
    MCI_STATUS_NUMBER_OF_TRACKS - Returns the total number of playable tracks.
    MCI_STATUS_POSITION - Returns the current position.
    MCI_STATUS_READY - Returns TRUE if the device is ready
    MCI_STATUS_TIME_FORMAT
    MCI_STATUS_MEDIA_PRESENT
    **/
    DWORD GetStatus( DWORD dwItem, DWORD dwTrack = 0L );
    DWORD SeekTrack( BYTE dwTrack );
};

