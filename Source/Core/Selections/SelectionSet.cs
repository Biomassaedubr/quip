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

    public IEnumerable<Selection> All {
      get {
        return m_selections;
      }
    }

    int m_primary = 0;
    List<Selection> m_selections = new List<Selection>();
  }
}

