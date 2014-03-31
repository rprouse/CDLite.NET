using System.Windows;
using System.Windows.Input;

namespace Alteridem.CDLite
{
    /// <summary>
    /// Interaction logic for CdView.xaml
    /// </summary>
    public partial class CdView : Window, IWindow
    {
        private readonly CdViewModel _viewModel;

        public CdView()
        {
            InitializeComponent();
            _viewModel = new CdViewModel(this);
            DataContext = _viewModel;
        }

        private void OnClosed(object sender, System.EventArgs e)
        {
            _viewModel.Dispose();
        }

        private void OnMouseDown( object sender, MouseButtonEventArgs e )
        {
            if ( e.ChangedButton == MouseButton.Left )
                DragMove();
        }
    }
}
