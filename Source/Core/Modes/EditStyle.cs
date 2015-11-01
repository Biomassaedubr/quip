namespace Quip {
  /// <summary>
  /// Values that indicate whether an edit occurs as the start of end of a selction.
  /// </summary>
  public enum EditStyle {
    /// <summary>
    /// An edit occurs before the start of a selection.
    /// </summary>
    Insert,

    /// <summary>
    /// An edit occurs after the end of a selection.
    /// </summary>
    Append,
  }
}

