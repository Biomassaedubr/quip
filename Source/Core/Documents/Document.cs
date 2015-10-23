using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Quip {
  /// <summary>
  /// A Quip document.
  /// </summary>
  public class Document {
    public Document () {
      m_lines = new List<string>();
    }

    public Document (string content) {
      m_lines = new List<string>(SplitText(content));
    }

    public Document (IEnumerable<string> lines) {
      m_lines = new List<string>(lines);
    }
      
    public int Rows {
      get {
        return m_lines.Count;
      }
    }

    public CharacterIterator GetCharacterIterator (Location location) {
      return new CharacterIterator(this, location);
    }

    public WordIterator GetWordIterator (Location location) {
      return new WordIterator(this, location);
    }

    public LineIterator GetLineIterator (Location location) {
      return new LineIterator(this, location);
    }

    public char GetCharacter (Location location) {
      return m_lines[location.Row][location.Column];
    }

    public string GetRow (int row) {
      return m_lines[row];
    }

    public string GetText () {
      return string.Join(string.Empty, m_lines);
    }

    public Location Insert (string text, Location target) {
      if (text == null || text.Length == 0) {
        return target;
      }

      var existing = m_lines[target.Row];
      var prefix = existing.Substring(0, target.Column);
      var suffix = existing.Substring(target.Column);
      if (text != "\n") {
        m_lines[target.Row] = prefix + text + suffix;
        return new Location(target.Column + 1, target.Row);
      } else {
        m_lines[target.Row] = prefix + "\n";
        if (target.Row == Rows - 1) {
          m_lines.Add(suffix);
        } else {
          m_lines.Insert(target.Row + 1, suffix);
        }

        return new Location(0, target.Row + 1);
      }
    }

    public Location Erase (Location target) {
      if (target.Column == 0) {
        // The character to be erased is immediately prior to the target, so
        // there's nothing to erase at the start of the document.
        if (target.Row == 0) {
          return target;
        }

        var line = m_lines[target.Row - 1];
        var result = line.Substring(0, line.Length - 1);
        m_lines[target.Row - 1] = result + m_lines[target.Row];
        m_lines.RemoveAt(target.Row);
        return new Location(line.Length - 1, target.Row - 1);
      } else {
        var line = m_lines[target.Row];
        var first = line.Substring(0, target.Column - 1);
        var second = line.Substring(target.Column);

        m_lines[target.Row] = first + second;
        return new Location(target.Column - 1, target.Row);
      }
    }

    static IEnumerable<string> SplitText (string text) {
      if (text == null || text.Length == 0) {
        yield break;
      }

      var start = 0;
      var stop = text.IndexOf(Environment.NewLine);
      while (stop > 0) {
        var line = text.Substring(start, stop - start + 1);
        yield return line;

        start = stop + 1;
        stop = text.IndexOf(Environment.NewLine, start);
      }

      yield return text.Substring(start);
    }

    List<string> m_lines;
  }
}
