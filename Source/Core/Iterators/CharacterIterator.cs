using System;

namespace Quip {
  /// <summary>
  /// A document iterator that moves character-wise.
  /// </summary>
  public class CharacterIterator {
    internal CharacterIterator(Document document, Location location) {
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
        var line = m_document.GetRow(m_location.Row);
        return (m_location.Row != m_document.Rows - 1) || (m_location.Column != line.Length - 1);
      }
    }

    public bool CanMovePrior {
      get {
        return (m_location.Row != 0) || (m_location.Column != 0);
      }
    }

    public CharacterIterator Clone () {
      return new CharacterIterator(m_document, m_location);
    }

    public void MoveNext () {
      if (!CanMoveNext) {
        throw new InvalidOperationException("Cannot step iterator forward.");
      }

      ++m_location.Column;
      if (m_location.Column >= m_document.GetRow(m_location.Row).Length) {
        m_location.Column = 0;
        ++m_location.Row;
      }
    }

    public void MovePrior () {
      if (!CanMovePrior) {
        throw new InvalidOperationException("Cannot step iterator backward.");
      }

      if (m_location.Column > 0) {
        --m_location.Column;
      } else {
        --m_location.Row;
        m_location.Column = m_document.GetRow(m_location.Row).Length - 1;
      }
    }

    Document m_document;
    Location m_location;
  }
}

