using System;
using System.Drawing;
using MonoMac.AppKit;
using MonoMac.CoreGraphics;
using MonoMac.CoreText;
using MonoMac.Foundation;
using System.Collections.Generic;

namespace Quip {
  public class MacDocumentView : NSView {

    [Export("initWithFrame:")]
    public MacDocumentView (RectangleF frame)
      : base(frame) {
      m_cursorTimer = NSTimer.CreateRepeatingTimer(m_cursorBlinkInterval, new NSAction(UpdateCursor));
      NSRunLoop.Current.AddTimer(m_cursorTimer, NSRunLoopMode.Common);
    }

    public Document Document {
      get {
        return m_view.Document;
      }
      set {
        m_view.Document = value;
      }
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

      if (key != null && m_view.Mode.HandleKey(key, m_view)) {
        m_cursorIsVisible = true;
        SetNeedsDisplayInRect(Frame);
      }
    }

    public override void MouseDown (NSEvent eventData) {
      m_mouseDragOrigin = GetLocationFromCoordinate(eventData.LocationInWindow);
    }

    public override void MouseUp (NSEvent eventData) {
      var location = GetLocationFromCoordinate(eventData.LocationInWindow);
      if (location == m_mouseDragOrigin) {
        m_view.MoveTo(location);
      }
    }

    public override void MouseDragged (NSEvent eventData) {
      var location = GetLocationFromCoordinate(eventData.LocationInWindow);
      if (location != m_mouseDragOrigin) {
        m_view.MoveTo(m_mouseDragOrigin);
        m_view.Selections.Primary.Origin = m_mouseDragOrigin;
        m_view.Selections.Primary.Extent = location;
      }
    }

    public override void DrawRect (RectangleF dirtyRect) {
      base.DrawRect(dirtyRect);

      var context = NSGraphicsContext.CurrentContext.GraphicsPort;
      using (var font = new CTFont(m_fontName, m_fontSize)) {
        var queryDictionary = new NSDictionary(CTStringAttributeKey.Font, font);
        var queryString = new NSString("m");
        m_textCellSize = queryString.StringSize(queryDictionary);

        var dictionary = new NSDictionary(CTStringAttributeKey.Font, font);
        var attributes = new CTStringAttributes(dictionary);
        var y = Frame.Height - m_textCellSize.Height;
        for (var index = 0; index < Document.Rows; ++index) {
          var line = Document.GetRow(index);
          var attributed = new NSAttributedString(line, attributes);
          using (var drawable = new CTLine(attributed)) {
            context.TextPosition = new PointF(0.0f, y);

            drawable.Draw(context);
            y -= m_textCellSize.Height;
          }
        }
      
        // Draw mode name.
        using (var drawable = new CTLine(new NSAttributedString(m_view.Mode.Name, attributes))) {
          context.TextPosition = new PointF(5.0f, 5.0f);
          drawable.Draw(context);
        }

        DrawSelection(m_view.Selections.Primary, context, true);
        foreach (var selection in m_view.Selections.Secondary) {
          DrawSelection(selection, context, false);
        }

        if (m_cursorIsVisible) {
          var cursorX = m_view.Cursor.Column * m_textCellSize.Width;
          var cursorY = Frame.Height - m_textCellSize.Height - (m_view.Cursor.Row * m_textCellSize.Height);
          var style = m_view.CursorStyle;

          context.SetFillColor(0.0f, 0.0f, 0.0f, 1.0f);
          switch (style) {
            case CursorStyle.VerticalBar:
              context.FillRect(new RectangleF(cursorX, cursorY - 2, 1, m_textCellSize.Height - 2));
              break;
            case CursorStyle.Underbar:
            default:
              context.FillRect(new RectangleF(cursorX, cursorY - 2, m_textCellSize.Width, 1));
              break;
          }
        }
      }
    }

    void DrawSelection(Selection selection, CGContext context, bool primary) {
      var earlier = selection.Origin < selection.Extent ? selection.Origin : selection.Extent;
      var later = selection.Origin < selection.Extent ? selection.Extent : selection.Origin;
      var row = earlier.Row;

      do {
        var firstColumn = row == earlier.Row ? earlier.Column : 0;
        var lastColumn = row == later.Row ? later.Column : Document.GetRow(row).Length - 1;

        var x = firstColumn * m_textCellSize.Width;
        var y = Frame.Height - m_textCellSize.Height - (row * m_textCellSize.Height);

        if(primary) {
          context.SetFillColor(1.0f, 0.0f, 0.0f, 1.0f);
        } else {
          context.SetFillColor(0.8f, 0.2f, 0.2f, 1.0f);
        }

        context.FillRect(new RectangleF(x, y - 1, m_textCellSize.Width * (lastColumn + 1 - firstColumn), 1));

        ++row;
      } while (row <= later.Row);
    }

    public override bool AcceptsFirstResponder () {
      return true;
    }

    void UpdateCursor () {
      m_cursorIsVisible = !m_cursorIsVisible;
      SetNeedsDisplayInRect(Frame);
    }

    Location GetLocationFromCoordinate (PointF coordinate) {
      var row = (int)Clamp.Between(0, Document.Rows - 1, (Frame.Height - coordinate.Y) / m_textCellSize.Height);
      var column = (int)Clamp.Between(0, Document.GetRow(row).Length - 1, coordinate.X / m_textCellSize.Width);

      return new Location(column, row);
    }
      
    DocumentView m_view = new DocumentView();

    bool m_cursorIsVisible;

    NSTimer m_cursorTimer;
    const double m_cursorBlinkInterval = 0.57;

    Location m_mouseDragOrigin;

    const int m_enterKeyCode = 36;
    const int m_backspaceKeyCode = 51;
    const int m_escapeKeyCode = 53;

    SizeF m_textCellSize;
    const string m_fontName = "Menlo";
    const float m_fontSize = 13.0f;
  }
}
