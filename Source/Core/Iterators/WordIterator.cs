using System;

namespace Quip {
  /// <summary>
  /// A document iterator that moves word-wise.
  /// </summary>
  public class WordIterator {
    internal WordIterator (Document document, Location location) {
      m_iterator = document.GetCharacterIterator(location);

      // Initialize to the word containing the location.
      m_iterator.BackwardTo(x => char.IsWhiteSpace(x));
      if (char.IsWhiteSpace(m_iterator.Value)) {
        m_iterator.MoveNext();
      }

      var origin = m_iterator.Location;
      m_iterator.ForwardTo(x => char.IsWhiteSpace(x));
      if (char.IsWhiteSpace(m_iterator.Value)) {
        m_iterator.MovePrior();
      }

      Current = new Selection(origin, m_iterator.Location);
    }

    public Selection Current {
      get;
      private set;
    }

    public Location Location {
      get {
        return m_iterator.Location;
      }
    }

    public char Value {
      get {
        return m_iterator.Value;
      }
    }

    public void MoveNext () {
      if (!m_iterator.CanMoveNext) {
        return;
      }

      m_iterator.MoveNext();
      m_iterator.ForwardTo(x => !char.IsWhiteSpace(x));

      var origin = m_iterator.Location;
      m_iterator.ForwardTo(x => char.IsWhiteSpace(x));
      if (char.IsWhiteSpace(m_iterator.Value)) {
        m_iterator.MovePrior();
      }

      Current = new Selection(origin, m_iterator.Location);
    }

    public void MovePrior () {
      if (!m_iterator.CanMovePrior) {
        return;
      }

      // The iterator will be at the end of the current word, and so much first
      // traverse that word.
      m_iterator.BackwardTo(x => char.IsWhiteSpace(x));
      m_iterator.BackwardTo(x => !char.IsWhiteSpace(x));

      var extent = m_iterator.Location;
      m_iterator.BackwardTo(x => char.IsWhiteSpace(x));
      if (char.IsWhiteSpace(m_iterator.Value)) {
        m_iterator.MoveNext();
      }

      // Similarly, once the origin is found the iterator must be
      // returned to the end of the word.
      var origin = m_iterator.Location;
      m_iterator.ForwardTo(x => char.IsWhiteSpace(x));
      if (char.IsWhiteSpace(m_iterator.Value)) {
        m_iterator.MovePrior();
      }

      Current = new Selection(origin, extent);
    }

    CharacterIterator m_iterator;
  }
}

