namespace Quip {
  /// <summary>
  /// A 2D (column, row) location within a document.
  /// </summary>
  public struct Location {
    /// <summary>
    /// Initializes a new instance.
    /// </summary>
    /// <param name="column">The column.</param>
    /// <param name="row">The row.</param>
    public Location (int column, int row) {
      Column = column;
      Row = row;
    }

    /// <summary>
    /// Gets or sets the column.
    /// </summary>
    /// <value>The column.</value>
    public int Column {
      get;
      set;
    }

    /// <summary>
    /// Gets or sets the row.
    /// </summary>
    /// <value>The row.</value>
    public int Row {
      get;
      set;
    }
  }
}
