using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Quip {
  public partial class QuipWindow : Form {
    public QuipWindow () {
      InitializeComponent();

      m_view.Dock = DockStyle.Fill;
      m_view.Document = new Document("Welcome to Quip.\n\nQuip is a toy text editor built around modal editing principles.");
      m_view.KeyDown += HandleKeyDown;
      Controls.Add(m_view);
    }

    void HandleKeyDown (object sender, KeyEventArgs e) {
      var key = CreateFromWindowsEvent(e);
      if (key != null) {
        if (m_view.Mode.HandleKey(key, m_view)) {
          m_modeStatusLabel.Text = m_view.Mode.Status;
        }
      }
    }

    public static Key CreateFromWindowsEvent (KeyEventArgs eventArguments) {
      char result = MapVirtualKey(eventArguments.KeyValue, MapVirtualKeyMode.VirtualKeyToCharacter);
      if (result == 0) {
        return null;
      }
      else if (char.IsLetter(result) && !eventArguments.Shift) {
        // MapVirtualKey provides unshifted uppercase output.
        result = char.ToLower(result);
      }
      else if (result == ';' && eventArguments.Shift) {
        result = ':';
      }

      return new Key(result);
    }

    enum MapVirtualKeyMode : uint {
      VirtualKeyToVirtualScanCode = 0x00,
      VirtualScanCodeToVirtualKey = 0x01,
      VirtualKeyToCharacter = 0x02,
      VirtualScanCodeToHandedVirtualKey = 0x03,
    }

    [DllImport("user32.dll")]
    static extern char MapVirtualKey (int code, MapVirtualKeyMode mode);

    DocumentView m_view = new DocumentView();
  }
}
