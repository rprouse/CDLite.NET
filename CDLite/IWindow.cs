using System.Windows;

namespace Alteridem.CDLite
{
    public interface IWindow
    {
        void Close();
        WindowState WindowState { get; set; }
    }
}