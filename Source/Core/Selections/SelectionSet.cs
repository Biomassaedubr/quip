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

    /// <summary>
    /// Rotate the primary selection.
    /// </summary>
    /// <remarks>>
    /// This method causes the next selection in the set to become the primary selection.
    /// If the current selection is the last selection, the first selection becomes the
    /// primary instead.
    /// </remarks>
    public void Rotate () {
      m_primary = (m_primary + 1) % m_selections.Count;
    }

    public void Add (Selection selection) {
      m_selections.Add(selection);
    }

    /// <summary>
    /// Replace the selections int he set with the specified selection.
    /// </summary>
    /// <param name="selection">The new selection.</param>
    public void ReplaceWith (Selection selection) {
      ReplaceWith(new [] { selection });
    }

    /// <summary>
    /// Replace the selections in the set with the specified selections.
    /// </summary>
    /// <param name="selections">The new selections.</param>
    public void ReplaceWith (IEnumerable<Selection> selections) {
      m_primary = 0;
      m_selections.Clear();
      m_selections.AddRange(selections);
    }

    int m_primary = 0;
    List<Selection> m_selections = new List<Selection>();
  }
}

