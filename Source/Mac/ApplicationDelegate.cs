using System.Drawing;
using System.IO;
using MonoMac.AppKit;
using MonoMac.Foundation;

namespace Quip {
  public partial class ApplicationDelegate : NSApplicationDelegate {
    public ApplicationDelegate () {
    }

    public override bool OpenFile (NSApplication sender, string filename) {
      m_document = LoadFromFile(filename);
      return true;
    }

    public override void FinishedLaunching (NSObject notification) {
      var frame = new RectangleF(0.0f, 0.0f, 1024.0f, 768.0f);
      m_view = new MacDocumentView(frame);
      m_window = new NSWindow(frame, NSWindowStyle.Titled, NSBackingStore.Buffered, true);

      if (m_document == null) {
        m_document = new Document("Welcome to Quip.\n\nQuip is a toy text editor built around modal editing principles.");
        m_window.Title = "Untitled";
      } else {
        m_window.Title = m_document.FilePath;
      }

      m_view.Document = m_document;
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
        m_document = LoadFromFile(dialog.Url.Path);
        m_view.Document = m_document;
        m_window.Title = m_document.FilePath;
      }
    }

    [Export("saveDocument:")]
    void SaveDocument (NSObject sender) {
      var path = m_view.Document.FilePath;
      if (string.IsNullOrWhiteSpace(path)) {
        var dialog = NSSavePanel.SavePanel;
        dialog.CanCreateDirectories = true;

        if (dialog.RunModal() == 1) {
          path = dialog.Url.Path;
        } else {
          return;
        }
      }

      File.WriteAllText(path, m_view.Document.GetText());
      m_view.Document.FilePath = path;
      m_window.Title = path;
    }

    Document LoadFromFile (string path) {
      var document = new Document(File.ReadAllText(path));
      document.FilePath = path;

      return document;
    }

    NSWindow m_window;
    MacDocumentView m_view;
    Document m_document;
  }
}
