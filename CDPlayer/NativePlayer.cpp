#include "stdafx.h"
#include "NativePlayer.h"


NativePlayer::NativePlayer() 
    : m_wDeviceID( 0 ), m_hWnd( nullptr ), m_currentDevice( 0 ), m_strCdRomDrives( nullptr )
{
    // List all valid CD-ROM Devices
    unsigned int nCount = 0;
    wchar_t  strTemp[4];
    wchar_t  strTemp2[32];

    DWORD nDriveMask = ::GetLogicalDrives();
    for ( unsigned int nDrive = 0; nDrive < 32; nDrive++ )
    {
        if ( nDriveMask & ( 1 << nDrive ) )
        {
            //This drive is available
            swprintf_s( strTemp, TEXT( "%c:\\" ), nDrive + 'A' );
            unsigned int nType = ::GetDriveType( strTemp );
            if ( nType == DRIVE_CDROM ) 
            {
                //Only report on CDROM drives
                strTemp2[nCount] = (wchar_t)( nDrive + 'A' );
                nCount++;
            }
        }
    }
    m_numDevices = nCount;
    m_strCdRomDrives = new wchar_t[nCount];
    wcsncpy( m_strCdRomDrives, strTemp2, nCount );

    SetDriveID( 0 );
}


NativePlayer::~NativePlayer()
{
    if ( m_strCdRomDrives != nullptr )
        delete[] m_strCdRomDrives;

    Close();
}

void NativePlayer::Attach( HWND hWnd )
{
    m_hWnd = hWnd;
}

DWORD NativePlayer::Open()
{
    DWORD dwReturn;
    TCHAR szElementName[4];
    TCHAR szAliasName[32];
    DWORD dwAliasCount = GetCurrentTime();

    // Opens a CD audio device by specifying a device-type constant.
    MCI_OPEN_PARMS mciOpenParms;
    ZeroMemory( &mciOpenParms, sizeof( mciOpenParms ) );

    mciOpenParms.lpstrDeviceType = (LPCWSTR)MCI_DEVTYPE_CD_AUDIO;
    if ( m_numDevices == 0 )
    {
        //AfxMessageBox( TEXT( "No CD Audio devices found on your system" ), MB_OK | MB_ICONSTOP );
        return 1L;
    }
    wsprintf( szElementName, TEXT( "%c:" ), m_strCdRomDrives[m_currentDevice] );
    wsprintf( szAliasName, TEXT( "CD%lu:" ), dwAliasCount );

    mciOpenParms.lpstrElementName = szElementName;
    mciOpenParms.lpstrAlias = szAliasName;

    DWORD dwFlags = MCI_OPEN_ELEMENT | MCI_OPEN_SHAREABLE | MCI_OPEN_ALIAS |
        MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_WAIT;

    if ( dwReturn = mciSendCommand( NULL, MCI_OPEN, dwFlags, (DWORD)(LPVOID)&mciOpenParms ) )
    {
        return dwReturn;
    }

    // If we already have a device open, we should close it
    Close();

    // The device opened successfully; get the device ID.
    m_wDeviceID = mciOpenParms.wDeviceID;

    // Set the time format to track/minute/second/frame (TMSF).
    MCI_SET_PARMS mciSetParms;
    mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;
    if ( dwReturn = mciSendCommand( m_wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&mciSetParms ) )
    {
        Close();
        return ( dwReturn );
    }
    return ( 0L );
}

void NativePlayer::Close()
{
    if ( m_wDeviceID )
    {
        mciSendCommand( m_wDeviceID, MCI_CLOSE, 0, NULL );
        m_wDeviceID = 0;
    }
}

void NativePlayer::Stop()
{
    if ( !m_wDeviceID ) return;

    MCI_GENERIC_PARMS mciParms;
    mciParms.dwCallback = (DWORD)m_hWnd;
    mciSendCommand( m_wDeviceID, MCI_STOP, MCI_NOTIFY, (DWORD)(LPVOID)&mciParms );

    SeekTrack( GetTrack() );
}

// Plays a specified audio track using MCI_OPEN, MCI_PLAY. Returns as 
// soon as playback begins. The window procedure function for the 
// specified window will be notified when playback is complete. 
// Returns 0L on success; otherwise, returns an MCI error code.
DWORD NativePlayer::PlayTrack( BYTE bTrack )
{
    DWORD dwReturn;

    if ( !m_wDeviceID && ( dwReturn = Open() ) != 0 )
        return dwReturn;

    // Begin play from the specified track and play to the beginning 
    // of the next track. The window procedure function for the parent 
    // window will be notified with an MM_MCINOTIFY message when 
    // playback is complete. Unless the play command fails, the window 
    // procedure closes the device.    
    MCI_PLAY_PARMS mciPlayParms;
    mciPlayParms.dwFrom = 0L;
    mciPlayParms.dwTo = 0L;
    mciPlayParms.dwFrom = MCI_MAKE_TMSF( bTrack, 0, 0, 0 );
    //mciPlayParms.dwTo = MCI_MAKE_TMSF(bTrack + 1, 0, 0, 0);
    mciPlayParms.dwCallback = (DWORD)m_hWnd;
    if ( dwReturn = mciSendCommand( m_wDeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY,
        (DWORD)(LPVOID)&mciPlayParms ) )
    {
        Close();
        return ( dwReturn );
    }
    return ( 0L );
}

DWORD NativePlayer::NextTrack()
{
    BYTE track = GetTrack();
    track++;
    BYTE total = GetNumberOfTracks();
    if ( track > total )
        track = 1;
    if ( GetMode() == MCI_MODE_PLAY )
        return PlayTrack( track );
    else
        return SeekTrack( track );
}

DWORD NativePlayer::PrevTrack()
{
    BYTE track = GetTrack();
    track--;
    if ( track < 1 ) track = GetNumberOfTracks();
    if ( GetMode() == MCI_MODE_PLAY )
        return PlayTrack( track );
    else
        return SeekTrack( track );
}

DWORD NativePlayer::Play()
{
    if ( GetMode() == MCI_MODE_PLAY )
        return ( 0L );

    DWORD dwReturn;
    if ( !m_wDeviceID && ( dwReturn = Open() ) != 0 )
        return dwReturn;

    MCI_PLAY_PARMS mciPlayParms;
    mciPlayParms.dwFrom = 0L;
    mciPlayParms.dwTo = 0L;
    mciPlayParms.dwCallback = (DWORD)m_hWnd;
    if ( dwReturn = mciSendCommand( m_wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms ) )
    {
        Close();
        return ( dwReturn );
    }
    return ( 0L );
}

DWORD NativePlayer::Pause()
{
    DWORD dwReturn;

    if ( !m_wDeviceID && ( dwReturn = Open() ) != 0 )
        return dwReturn;

    switch ( GetMode() )
    {
    case MCI_MODE_PAUSE:
        Play();
        break;
    case MCI_MODE_PLAY:
    {
        MCI_GENERIC_PARMS mciParms;
        dwReturn = mciSendCommand( m_wDeviceID, MCI_PAUSE, NULL, (DWORD)(LPVOID)&mciParms );
        break;
    }
    default:
        break;
    }
    return ( dwReturn );
}

void NativePlayer::Eject()
{
    DWORD dwReturn;

    if ( !m_wDeviceID && ( dwReturn = Open() ) != 0 )
        return;

    MCI_SET_PARMS mciset;
    mciSendCommand( m_wDeviceID, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD)(LPCWSTR)&mciset );
}

DWORD NativePlayer::GetStatus( DWORD dwItem, DWORD dwTrack )
{
    if ( !m_wDeviceID )
        return 0L;

    DWORD flags = MCI_STATUS_ITEM;
    if ( dwTrack )
        flags |= MCI_TRACK;
    MCI_STATUS_PARMS mciParms;
    mciParms.dwItem = dwItem;
    mciParms.dwTrack = dwTrack;
    mciParms.dwReturn = 0L;
    mciSendCommand( m_wDeviceID, MCI_STATUS, flags, (DWORD)(LPVOID)&mciParms );
    return mciParms.dwReturn;
}

DWORD NativePlayer::GetMode()
{
    return GetStatus( MCI_STATUS_MODE );
}

BYTE NativePlayer::GetTrack()
{
    return (BYTE)GetStatus( MCI_STATUS_CURRENT_TRACK );
}

BYTE NativePlayer::GetNumberOfTracks()
{
    return (BYTE)GetStatus( MCI_STATUS_NUMBER_OF_TRACKS );
}

DWORD NativePlayer::GetPosition()
{
    return GetStatus( MCI_STATUS_POSITION );
}

void NativePlayer::ShowError( DWORD dwError )
{
    TCHAR szErrorBuf[MAXERRORLENGTH];
    if ( mciGetErrorString( dwError, (LPWSTR)szErrorBuf, MAXERRORLENGTH ) )
    {
        //MessageBox( m_hWnd, szErrorBuf, TEXT( "CD Player Error" ), MB_ICONEXCLAMATION );
    }
    else
    {
        //MessageBox( m_hWnd, TEXT( "Unknown Error" ), TEXT( "CD Player Error" ), MB_ICONEXCLAMATION );
    }
}

DWORD NativePlayer::SeekTrack( BYTE bTrack )
{
    if ( !m_wDeviceID )
        return 0L;

    MCI_SEEK_PARMS mciParms;
    mciParms.dwTo = MCI_MAKE_TMSF( bTrack, 0, 0, 0 );
    return mciSendCommand( m_wDeviceID, MCI_SEEK, MCI_TO, (DWORD)(LPVOID)&mciParms );
}

wchar_t* NativePlayer::GetDrives()
{
    return m_strCdRomDrives;
}

unsigned int NativePlayer::GetNumDrives()
{
    return m_numDevices;
}

DWORD NativePlayer::SetDriveID( unsigned int id )
{
    if ( id < m_numDevices )
    {
        m_currentDevice = id;
        return Open();
    }
    else
        return 1L;
}

unsigned int NativePlayer::GetDriveID()
{
    return m_currentDevice;
}
