using System;
using System.Drawing;
using System.Windows.Forms;

namespace Quip {
  class WindowsDocumentView : Control {
    public WindowsDocumentView () {
      DoubleBuffered = true;

      m_cursorTimer.Interval = SystemInformation.CaretBlinkTime;
      m_cursorTimer.Tick += HandleBlinkTimerTick;
      m_cursorTimer.Enabled = true;
    }

    public Document Document {
      get {
        return m_view.Document;
      }
      set {
        m_view.Document = value;
      }
    }

    public Mode Mode {
      get {
        return m_view.Mode;
      }
      set {
        m_view.Mode = value;
      }
    }

    public DocumentView View {
      get {
        return m_view;
      }
    }

    protected override void OnPaint (PaintEventArgs e) {
      var frame = new RectangleF(0.0f, 0.0f, ClientSize.Width, ClientSize.Height);
      using (var font = new Font("Consolas", 11.0f)) {
        var position = new PointF();
        for (var row = 0; row < m_view.Document.Rows; ++row) {
          var line = m_view.Document.GetRow(row);
          e.Graphics.DrawString(line, font, Brushes.Black, position);
          position.Y += font.Height;
        }

        var ranges = new CharacterRange[] {
          new CharacterRange(m_view.Cursor.Column, 1)
        };

        var format = new StringFormat(StringFormatFlags.NoClip | StringFormatFlags.MeasureTrailingSpaces);
        format.SetMeasurableCharacterRanges(ranges);

        var text = m_view.Document.GetRow(m_view.Cursor.Row);
        var regions = e.Graphics.MeasureCharacterRanges(text, font, frame, format);
        var bounds = regions[0].GetBounds(e.Graphics);
        if (m_cursorIsVisible) {
          var cursorX = (int)bounds.Left;
          var cursorY = (int)(font.Height * m_view.Cursor.Row + bounds.Top);
          var cursorWidth = (int)bounds.Width - 1;

          switch (m_view.CursorStyle) {
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

    void HandleBlinkTimerTick (object sender, EventArgs e) {
      m_cursorIsVisible = !m_cursorIsVisible;
      Invalidate();
    }

    DocumentView m_view = new DocumentView();

    Timer m_cursorTimer = new Timer();
    bool m_cursorIsVisible = true;
  }
}
