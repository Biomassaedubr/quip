using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Quip {
  class DocumentView : Control, IDocumentView {
    public DocumentView () {
      DoubleBuffered = true;

      Mode = new NormalMode(this);
      m_cursorTimer.Interval = SystemInformation.CaretBlinkTime;
      m_cursorTimer.Tick += HandleBlinkTimerTick;
      m_cursorTimer.Enabled = true;
    }

    public new Location Cursor {
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

    public void MoveTo (Location location) {
      if (location.Row >= 0 && location.Row < Document.Rows) {
        var line = Document.GetRow(location.Row);
        Cursor = new Location(Math.Min(line.Length - 1, location.Column), location.Row);
        HandleCursorMove();
      }
    }

    public void PushCursorStyle (CursorStyle style) {
      m_cursorStack.Push(style);
    }

    public void PopCursorStyle () {
      m_cursorStack.Pop();
    }

    protected override void OnPaint (PaintEventArgs e) {
      var frame = new RectangleF(0.0f, 0.0f, ClientSize.Width, ClientSize.Height);
      using (var font = new Font("Consolas", 11.0f)) {
        var position = new PointF();
        for (var row = 0; row < Document.Rows; ++row) {
          var line = Document.GetRow(row);
          e.Graphics.DrawString(line, font, Brushes.Black, position);
          position.Y += font.Height;
        }

        var ranges = new CharacterRange[] {
          new CharacterRange(Cursor.Column, 1)
        };

        var format = new StringFormat(StringFormatFlags.NoClip | StringFormatFlags.MeasureTrailingSpaces);
        format.SetMeasurableCharacterRanges(ranges);

        var text = Document.GetRow(Cursor.Row);
        var regions = e.Graphics.MeasureCharacterRanges(text, font, frame, format);
        var bounds = regions[0].GetBounds(e.Graphics);
        if (m_cursorIsVisible) {
          var style = m_cursorStack.Count == 0 ? CursorStyle.Underbar : m_cursorStack.Peek();
          var cursorX = (int)bounds.Left;
          var cursorY = (int)(font.Height * Cursor.Row + bounds.Top);
          var cursorWidth = (int)bounds.Width - 1;

          switch (style) {
            case CursorStyle.VerticalBar:
              e.Graphics.DrawLine(Pens.Black, new Point(cursorX, cursorY + font.Height), new Point(cursorX, cursorY));
              break;
            case CursorStyle.Underbar:
            default:
              e.Graphics.DrawLine(Pens.Black, new Point(cursorX, cursorY + font.Height), new Point(cursorX + cursorWidth, cursorY + font.Height));
              break;
          }
        }
      }

      base.OnPaint(e);
    }

    void HandleCursorMove () {
      m_cursorIsVisible = true;
      m_cursorTimer.Stop();
      m_cursorTimer.Start();
      Invalidate();
    }

    void HandleBlinkTimerTick (object sender, EventArgs e) {
      m_cursorIsVisible = !m_cursorIsVisible;
      Invalidate();
    }

    Stack<CursorStyle> m_cursorStack = new Stack<CursorStyle>();
    Timer m_cursorTimer = new Timer();
    bool m_cursorIsVisible = true;
  }
}
