using System.Drawing;
using System.IO;
using MonoMac.AppKit;
using MonoMac.Foundation;

namespace Quip {
  public partial class ApplicationDelegate : NSApplicationDelegate {
    public ApplicationDelegate () {
    }

    public override void FinishedLaunching (NSObject notification) {
      var frame = new RectangleF(0.0f, 0.0f, 1024.0f, 768.0f);
      m_view = new MacDocumentView(frame);
      m_view.Document = new Document("Welcome to Quip.\n\nQuip is a toy text editor built around modal editing principles.");

      m_window = new NSWindow(frame, NSWindowStyle.Titled, NSBackingStore.Buffered, true);
      m_window.ContentView = m_view;
      m_window.Center();
      m_window.MakeKeyAndOrderFront(this);
    }

    [Export("openDocument:")]
    void OpenDocument (NSObject sender) {
      var dialog = NSOpenPanel.OpenPanel;
      dialog.CanChooseFiles = true;
      dialog.CanChooseDirectories = false;

      if (dialog.RunModal() == 1) {
        var lines = File.ReadAllLines(dialog.Url.Path);
        m_view.Document = new Document(lines);
      }
    }

    [Export("saveDocument:")]
    void SaveDocument (NSObject sender) {
      var dialog = NSSavePanel.SavePanel;
      dialog.CanCreateDirectories = true;

      if (dialog.RunModal() == 1) {
        File.WriteAllText(dialog.Url.Path, m_view.Document.GetText());
      }
    }

    NSWindow m_window;
    MacDocumentView m_view;
  }
}
