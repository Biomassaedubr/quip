using System;
using System.Drawing;
using MonoMac.AppKit;
using MonoMac.CoreGraphics;
using MonoMac.CoreText;
using MonoMac.Foundation;
using System.Collections.Generic;

namespace Quip {
  public class DocumentView : NSView, IDocumentView {
    [Export("initWithFrame:")]
    public DocumentView (RectangleF frame)
      : base(frame) {
      Mode = new NormalMode(this);

      m_cursorTimer = NSTimer.CreateRepeatingTimer(m_cursorBlinkInterval, new NSAction(UpdateCursor));
      NSRunLoop.Current.AddTimer(m_cursorTimer, NSRunLoopMode.Common);
    }

    public Location Cursor {
      get;
      set;
    }

    public Document Document {
      get;
      set;
    }

    public Mode Mode {
      get;
      set;
    }

    public void MoveTo (Location cursor) {
      if (cursor.Row >= 0 && cursor.Row < Document.Rows) {
        var line = Document.GetRow(cursor.Row);
        Cursor = new Location(Math.Min(line.Length - 1, cursor.Column), cursor.Row);
        UpdateCursor();
      }
    }

    public void PushCursorStyle(CursorStyle style) {
      m_cursorStack.Push(style);
    }

    public void PopCursorStyle() {
      m_cursorStack.Pop();
    }

    public override void KeyDown (NSEvent eventData) {
      var key = default(Key);
      if (eventData.KeyCode == m_enterKeyCode) {
        key = Key.Enter;
      }
      else if (eventData.KeyCode == m_backspaceKeyCode) {
        key = Key.Backspace;
      }
      else if (eventData.KeyCode == m_escapeKeyCode) {
        key = Key.Escape;
      }
      else if (eventData.Characters.Length > 0) {
        key = new Key(eventData.Characters[0]);
      }

      if (key != null && Mode.HandleKey(key, this)) {
        m_cursorIsVisible = true;
        SetNeedsDisplayInRect(Frame);
      }
    }

    public override void DrawRect (RectangleF dirtyRect) {
      base.DrawRect(dirtyRect);

      var context = NSGraphicsContext.CurrentContext.GraphicsPort;
      using (var font = new CTFont("Menlo", 13.0f)) {
        var queryDictionary = new NSDictionary(CTStringAttributeKey.Font, font);
        var queryString = new NSString("m");
        var cellSize = queryString.StringSize(queryDictionary);

        var dictionary = new NSDictionary(CTStringAttributeKey.Font, font);
        var attributes = new CTStringAttributes(dictionary);
        var y = Frame.Height - cellSize.Height;
        for (var index = 0; index < Document.Rows; ++index) {
          var line = Document.GetRow(index);
          var attributed = new NSAttributedString(line, attributes);
          using (var drawable = new CTLine(attributed)) {
            context.TextPosition = new PointF(0.0f, y);

            drawable.Draw(context);
            y -= cellSize.Height;
          }
        }
      
        // Draw status.
        using (var drawable = new CTLine(new NSAttributedString(Mode.Status, attributes))) {
          context.TextPosition = new PointF(5.0f, 5.0f);
          drawable.Draw(context);
        }

        if (m_cursorIsVisible) {
          var cursorX = Cursor.Column * cellSize.Width;
          var cursorY = Frame.Height - cellSize.Height - (Cursor.Row * cellSize.Height);
          var style = m_cursorStack.Count == 0 ? CursorStyle.Underbar : m_cursorStack.Peek();
          switch (style) {
            case CursorStyle.VerticalBar:
              context.FillRect(new RectangleF(cursorX, cursorY - 2, 1, cellSize.Height - 2));
              break;
            case CursorStyle.Underbar:
            default:
              context.FillRect(new RectangleF(cursorX, cursorY - 2, cellSize.Width, 1));
              break;
          }
        }
      }
    }

    public override bool AcceptsFirstResponder () {
      return true;
    }

    void UpdateCursor () {
      m_cursorIsVisible = !m_cursorIsVisible;
      SetNeedsDisplayInRect(Frame);
    }
      
    bool m_cursorIsVisible;

    Stack<CursorStyle> m_cursorStack = new Stack<CursorStyle>();
    NSTimer m_cursorTimer;
    const double m_cursorBlinkInterval = 0.57;

    const int m_enterKeyCode = 36;
    const int m_backspaceKeyCode = 51;
    const int m_escapeKeyCode = 53;
  }
}
