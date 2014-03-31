// **********************************************************************************
// The MIT License (MIT)
// 
// Copyright (c) 2014 Rob Prouse
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// **********************************************************************************

using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using Alteridem.CD;
using Alteridem.CDLite.Annotations;

namespace Alteridem.CDLite
{
    public class CdViewModel : IDisposable, INotifyPropertyChanged
    {
        private readonly IWindow _window;
        private CDPlayer _player = new CDPlayer();
        private string _time = "00:00";
        private string _track = "00";
        private readonly DispatcherTimer _timer;

        public CdViewModel(IWindow window)
        {
            _window = window;
            PreviousTrack = new RelayCommand(_ => OnPreviousTrack(), _ => CanPreviousTrack());
            NextTrack = new RelayCommand(_ => OnNextTrack(), _ => CanNextTrack());
            Play = new RelayCommand(_ => OnPlay(), _ => CanPlay());
            Pause = new RelayCommand(_ => OnPause(), _ => CanPause());
            Stop = new RelayCommand(_ => OnStop(), _ => CanStop());
            Eject = new RelayCommand(_ => OnEject(), _ => CanEject());
            Minimize = new RelayCommand(_ => OnMinimize(), _ => true);
            Close = new RelayCommand(_ => OnClose(), _ => true);

            _timer = new DispatcherTimer();
            _timer.Tick += OnTimer;
            _timer.Interval = TimeSpan.FromMilliseconds(500);
            _timer.Start();
        }

        private void OnTimer(object sender, EventArgs e)
        {
            if ( _window.WindowState != WindowState.Minimized )
            {
                int track = _player.Track;
                _track = track.ToString( "00" );
                OnPropertyChanged( "Track" );
                TimeSpan time = _player.Time;
                _time = time.ToString( @"mm\:ss" );
                OnPropertyChanged( "Time" );
            }
        }

        #region Implementation of IDisposable

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        public void Dispose()
        {
            if(_player != null)
            {
                _player.Dispose();
                _player = null;
            }
        }

        #endregion

        #region Bindable Properties

        public string Time
        {
            get { return _time; }
        }

        public string Track
        {
            get { return _track; }
        }

        #endregion

        #region Commands

        public ICommand PreviousTrack { get; private set; }
        public ICommand NextTrack { get; private set; }
        public ICommand Play { get; private set; }
        public ICommand Pause { get; private set; }
        public ICommand Stop { get; private set; }
        public ICommand Eject { get; private set; }
        public ICommand Minimize { get; private set; }
        public ICommand Close { get; private set; }

        private void OnPreviousTrack()
        {
            _player.PreviousTrack();
        }

        private void OnNextTrack()
        {
            _player.NextTrack();
        }

        private void OnPlay()
        {
            _player.Play();
        }

        private void OnPause()
        {
            _player.Pause();
        }

        private void OnStop()
        {
            _player.Stop();
        }

        private void OnEject()
        {
            _player.Eject();
        }

        private void OnMinimize()
        {
            _window.WindowState = WindowState.Minimized;
        }

        private void OnClose()
        {
            _window.Close();
        }

        private bool CanPreviousTrack()
        {
            return true;
        }

        private bool CanNextTrack()
        {
            return true;
        }

        private bool CanPlay()
        {
            return true;
        }

        private bool CanPause()
        {
            return true;
        }

        private bool CanStop()
        {
            return true;
        }

        private bool CanEject()
        {
            return true;
        }

        #endregion

        #region INotifyPropertyChanged

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if(handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion
    }
}