using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace Quip {
  /// <summary>
  /// A Quip document.
  /// </summary>
  public class Document {
    /// <summary>
    /// Initializes a new instance.
    /// </summary>
    public Document () {
      m_lines = new List<string>();
    }

    /// <summary>
    /// Initializes a new instance.
    /// </summary>
    /// <param name="content">The initial document content.</param>
    public Document (string content) {
      m_lines = new List<string>(SplitText(content));
    }

    /// <summary>
    /// Initializes a new instance.
    /// </summary>
    /// <param name="lines">The initial document content.</param>
    public Document (IEnumerable<string> lines) {
      m_lines = new List<string>(lines);
    }
      
    /// <summary>
    /// Gets the number of rows in the document.
    /// </summary>
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

    public IEnumerable<Selection> Search (string pattern) {
      var results = new List<Selection>();
      var regex = new Regex(pattern);
      var row = 0;
      foreach (var line in m_lines) {
        foreach (var item in regex.Matches(line)) {
          var match = item as Match;
          if (match != null) {
            var start = new Location(match.Index, row);
            var stop = new Location(match.Index + match.Length - 1, row);
            results.Add(new Selection(start, stop));
          }
        }

        ++row;
      }

      return results;
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

    /// <summary>
    /// Inserts the given text prior to the origin of the specified selections.
    /// </summary>
    /// <param name="text">Text to insert.</param>
    /// <param name="selections">The selection set.</param>
    public void Insert (string text, SelectionSet selections) {
      var rowDelta = 0;
      var columnDelta = 0;
      var priorRow = -1;

      foreach (var selection in selections.All) {
        // Reset the column delta when changing rows.
        if (selection.LowerBound.Row != priorRow) {
          priorRow = selection.LowerBound.Row;
          columnDelta = 0;
        }

        var lower = selection.LowerBound.AdjustBy(columnDelta, rowDelta);
        var prefix = m_lines[lower.Row].Substring(0, lower.Column);
        var suffix = m_lines[lower.Row].Substring(lower.Column);
        m_lines[lower.Row] = prefix + text + suffix;

        columnDelta += text.Length;

        selection.Origin = selection.Origin.AdjustBy(columnDelta, 0);
        selection.Extent = selection.Origin;
      }
    }

    /// <summary>
    /// Appens the given text after the extent of the specified selections.
    /// </summary>
    /// <param name="text">Text to append.</param>
    /// <param name="selections">The selection set.</param>
    public void Append (string text, SelectionSet selections) {
      var rowDelta = 0;
      var columnDelta = 0;
      var priorRow = -1;

      foreach (var selection in selections.All) {
        // Reset the column delta when changing rows.
        if (selection.LowerBound.Row != priorRow) {
          priorRow = selection.LowerBound.Row;
          columnDelta = 0;
        }

        var lower = selection.LowerBound.AdjustBy(columnDelta, rowDelta);
        var upper = selection.UpperBound.AdjustBy(columnDelta, rowDelta);
        var prefix = m_lines[lower.Row].Substring(0, upper.Column);
        var suffix = m_lines[upper.Row].Substring(upper.Column);
        m_lines[lower.Row] = prefix + text + suffix;

        columnDelta += text.Length;

        selection.Origin = selection.Origin.AdjustBy(columnDelta, 0);
        selection.Extent = selection.Origin;
      }
    }

    /// <summary>
    /// Erases the text covered by the specified selections.
    /// </summary>
    /// <param name="selections">The selections describing the text to erase.</param>
    public void Erase (SelectionSet selections) {
      var rowDelta = 0;
      var columnDelta = 0;
      var priorRow = -1;

      foreach (var selection in selections.All) {
        // Reset the column delta when changing rows.
        if (selection.LowerBound.Row != priorRow) {
          priorRow = selection.LowerBound.Row;
          columnDelta = 0;
        }

        var lower = selection.LowerBound.AdjustBy(columnDelta, rowDelta);
        var upper = selection.UpperBound.AdjustBy(columnDelta, rowDelta);
        var prefix = m_lines[lower.Row].Substring(0, lower.Column);
        var suffix = m_lines[upper.Row].Substring(upper.Column + 1);
        m_lines[lower.Row] = prefix + suffix;

        var remove = selection.Height - 1;
        selection.Origin = selection.Origin.AdjustBy(columnDelta, 0);
        selection.Extent = selection.Origin;

        if (selection.Height == 1) {
          columnDelta -= (upper.Column - lower.Column + 1);
        } else {
          columnDelta = upper.Column;
          while(remove > 0) {
            m_lines.RemoveAt(lower.Row + 1);
            --rowDelta;
            --remove;
          }
        }
      }
    }

    public void EraseBefore (SelectionSet selections) {
      var rowDelta = 0;
      var columnDelta = 0;
      var priorRow = -1;

      foreach (var selection in selections.All) {
        // Reset the column delta when changing rows.
        if (selection.LowerBound.Row != priorRow) {
          priorRow = selection.LowerBound.Row;
          columnDelta = 0;
        }

        var lower = selection.LowerBound.AdjustBy(columnDelta, rowDelta);
        var upper = selection.UpperBound.AdjustBy(columnDelta, rowDelta);
        var prefix = m_lines[lower.Row].Substring(0, lower.Column - 1);
        var suffix = m_lines[upper.Row].Substring(lower.Column);
        m_lines[lower.Row] = prefix + suffix;

        var remove = selection.Height - 1;
        selection.Origin = selection.Origin.AdjustBy(-1, 0);
        selection.Extent = selection.Origin;

        if (selection.Height == 1) {
          columnDelta -= (upper.Column - lower.Column + 1);
        } else {
          columnDelta = upper.Column;
          while(remove > 0) {
            m_lines.RemoveAt(lower.Row + 1);
            --rowDelta;
            --remove;
          }
        }
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
