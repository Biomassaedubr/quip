using System;
using System.Collections.Generic;

namespace Quip {
  /// <summary>
  /// Provides the core logic of a document view.
  /// </summary>
  /// <remarks>
  /// The DocumentView type implements the common, platform-agnostic logic of a Quip
  /// view of a document. Platform-specific view types instantiate and defer to an instance
  /// of DocumentView for most of their work, implementing on platform-specific IO
  /// and rendering code themselves.
  /// </remarks>
  public class DocumentView {
    public DocumentView () {
      Mode = new NormalMode(this);
      Selections = new SelectionSet(new Selection(Location.Zero, Location.Zero));
    }

    /// <summary>
    /// Gets the view's document.
    /// </summary>
    public Document Document {
      get;
      set;
    }

    /// <summary>
    /// Gets the view's active mode.
    /// </summary>
    public Mode Mode {
      get;
      set;
    }

    /// <summary>
    /// Gets the view's active selection set.
    /// </summary>
    public SelectionSet Selections {
      get;
      private set;
    }

    /// <summary>
    /// Get's the view's cursor location.
    /// </summary>
    public Location Cursor {
      get;
      private set;
    }

    public CursorStyle CursorStyle {
      get {
        return m_cursorStack.Count == 0 ? CursorStyle.Underbar : m_cursorStack.Peek();
      }
    }

    public void MoveTo (Location location) {
      if (location.Row >= 0 && location.Row < Document.Rows) {
        var line = Document.GetRow(location.Row);

        Cursor = new Location(Math.Min(line.Length - 1, location.Column), location.Row);
        Selections.Primary.Origin = Cursor;
        Selections.Primary.Extent = Cursor;
      }
    }

    public void PushCursorStyle(CursorStyle style) {
      m_cursorStack.Push(style);
    }

    public void PopCursorStyle() {
      m_cursorStack.Pop();
    }

    Stack<CursorStyle> m_cursorStack = new Stack<CursorStyle>();
  }
}

