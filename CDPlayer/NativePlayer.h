#pragma once

#include <windows.h>

class NativePlayer
{
public:
    NativePlayer();
    virtual ~NativePlayer();

    /**
    * Attaches a window that will then receive MCI_NOTIFY messages
    * as a result of the various MCI commands
    **/
    void Attach( HWND hWnd );

    bool PlayTrack( BYTE bTrack );
    bool Open();
    bool Close();
    bool Stop();
    bool NextTrack();
    bool PrevTrack();
    bool Play();
    bool Pause();
    bool Eject();

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

    /// @return A string containing the drive letters for valid CDROM drives
    wchar_t* GetDrives();

    /// @return The number of CD drives on a system
    unsigned int GetNumDrives();

    /**
    * Sets the CD drive that we will be using
    * @param id The index into the string returned by GetDrives() that we want to open
    * @return true on success, false on failure
    **/
    bool SetDriveID( unsigned int id );

    /**
    * @return The index into the string returned by GetDrives() that we are currently using
    **/
    unsigned int GetDriveID();

    /**
    * Gets the last error
    * @return The last error that happened
    **/
    wchar_t* GetLastError();

protected:
    DWORD        m_callback;
    unsigned int m_wDeviceID;
    unsigned int m_currentDevice;  // Which CD device in m_strCdRomDrives is currently being used
    unsigned int m_numDevices;     // The number of valid CD drives
    wchar_t*     m_strCdRomDrives; // A string containing valid CD drives, like "DE"
    wchar_t      m_szLastError[MAXERRORLENGTH];

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
    bool  SeekTrack( BYTE dwTrack );

    // Pass in the error code returned from another function to 
    // set the error
    void  SetLastError( DWORD dwError );

    // A wrapper for mciSendCommand that checks the result and sets last error
    bool SendCommand(
        _In_ MCIDEVICEID mciId,
        _In_ UINT uMsg,
        _In_opt_ DWORD_PTR dwParam1,
        _In_opt_ DWORD_PTR dwParam2
        );
};

