using System.Windows;

namespace Alteridem.CDLite
{
    /// <summary>
    /// Interaction logic for CdView.xaml
    /// </summary>
    public partial class CdView : Window, IClosable
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
    }
}
