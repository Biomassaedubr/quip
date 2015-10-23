using System;

namespace Quip {
  /// <summary>
  /// A document iterator that moves word-wise.
  /// </summary>
  public class WordIterator {
    internal WordIterator (Document document, Location location) {
      m_iterator = document.GetCharacterIterator(location);
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
      var scout = m_iterator.Clone();
      while (!char.IsWhiteSpace(scout.Value)) {
        if (scout.CanMoveNext) {
          scout.MoveNext();
        } else {
          return;
        }
      }

      while (char.IsWhiteSpace(scout.Value)) {
        if (scout.CanMoveNext) {
          scout.MoveNext();
        } else {
          return;
        }
      }

      m_iterator = scout;
    }

    public void MovePrior () {
      if (!m_iterator.CanMovePrior) {
        return;
      }

      // The first unqualified backwards step moves into whitespace if the
      // iterator was current at the start of a word.
      var scout = m_iterator.Clone();
      scout.MovePrior();

      while (char.IsWhiteSpace(scout.Value)) {
        if (scout.CanMovePrior) {
          scout.MovePrior();
        } else {
          return;
        }
      }

      // Bail out if nothing but whitespace exists behind the original word.
      if (!scout.CanMovePrior) {
        return;
      }

      while (!char.IsWhiteSpace(scout.Value)) {
        if (!scout.CanMovePrior) {
          m_iterator = scout;
          return;
        }

        scout.MovePrior();
      }

      // At this point, the scout iterator will be referring to the first
      // whitespace before the desired word, so it needs to step forward 
      // one final time.
      scout.MoveNext();
      m_iterator = scout;
    }

    CharacterIterator m_iterator;
  }
}

