using System;
using System.Collections.Generic;

namespace Quip {
  /// <summary>
  /// A collection of selections.
  /// </summary>
  public class SelectionSet {
    /// <summary>
    /// Initializes a new instance.
    /// </summary>
    /// <param name="primary">The primary selection.</param>
    public SelectionSet (Selection primary) {
      if (primary == null) {
        throw new ArgumentNullException("primary");
      }

      m_selections.Add(primary);
    }

    /// <summary>
    /// Gets the primary selection.
    /// </summary>
    public Selection Primary {
      get {
        return m_selections[m_primary];
      }
    }

    /// <summary>
    /// Gets the secondary selections in the set.
    /// </summary>
    public IEnumerable<Selection> Secondary {
      get {
        var primary = m_primary;
        var current = 0;
        while (current < m_selections.Count) {
          if (current != primary) {
            yield return m_selections[current];
          }

          ++current;
        }

        yield break;
      }
    }

    /// <summary>
    /// Gets all selections in the set.
    /// </summary>
    public IEnumerable<Selection> All {
      get {
        return m_selections;
      }
    }

    public void Add (Selection selection) {
      m_selections.Add(selection);
    }

    int m_primary = 0;
    List<Selection> m_selections = new List<Selection>();
  }
}

