using System;

namespace Quip {
  /// <summary>
  /// A document iterator that moves line-wise.
  /// </summary>
  public class LineIterator {
    internal LineIterator(Document document, Location location) {
      m_document = document;
      m_location = location;
    }

    public char Value {
      get {
        return m_document.GetCharacter(m_location);
      }
    }

    public Location Location {
      get {
        return m_location;
      }
    }

    public bool CanMoveNext {
      get {
        return m_location.Row < m_document.Rows - 1;
      }
    }

    public bool CanMovePrior {
      get {
        return m_location.Row > 0;
      }
    }

    public LineIterator Clone () {
      return new LineIterator(m_document, m_location);
    }

    public void MoveNext () {
      if (!CanMoveNext) {
        throw new InvalidOperationException("Cannot step iterator forward.");
      }
    
      ++m_location.Row;
      var line = m_document.GetRow(m_location.Row);
      m_location.Column = Math.Min(m_location.Column, line.Length - 1);
    }

    public void MovePrior () {
      if (!CanMovePrior) {
        throw new InvalidOperationException("Cannot step iterator backward.");
      }

      --m_location.Row;
      var line = m_document.GetRow(m_location.Row);
      m_location.Column = Math.Min(m_location.Column, line.Length - 1);
    }

    Document m_document;
    Location m_location;
  }
}

