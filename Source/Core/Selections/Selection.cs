using System;

namespace Quip {
  /// <summary>
  /// A inclusive, directed range of locations within a document.
  /// </summary>
  public class Selection {
    /// <summary>
    /// Initializes a new instance.
    /// </summary>
    /// <param name="origin">The origin.</param>
    /// <param name="extent">The extent.</param>
    public Selection (Location origin, Location extent) {
      Origin = origin;
      Extent = extent;
    }

    /// <summary>
    /// Gets the selection's origin.
    /// </summary>
    public Location Origin {
      get;
      set;
    }

    /// <summary>
    /// Gets the selection's extent.
    /// </summary>
    /// <value>The extent.</value>
    public Location Extent {
      get;
      set;
    }
  }
}

