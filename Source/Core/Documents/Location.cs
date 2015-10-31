using System;

namespace Quip {
  /// <summary>
  /// A 2D (column, row) location within a document.
  /// </summary>
  public struct Location : IEquatable<Location> {
    /// <summary>
    /// Initializes a new instance.
    /// </summary>
    /// <param name="column">The column.</param>
    /// <param name="row">The row.</param>
    public Location (int column, int row)
      : this() {
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

    public static Location Zero {
      get {
        return new Location(0, 0);
      }
    }

    /// <summary>
    /// Compute a hash value for the object.
    /// </summary>
    /// <returns>Returns a hash value for the object.</returns>
    public override int GetHashCode () {
      return Row.GetHashCode() ^ Column.GetHashCode();
    }

    public override bool Equals (object other) {
      return !ReferenceEquals(null, other) && this == (Location)other;
    }

    public bool Equals (Location other) {
      return !ReferenceEquals(null, other) && this == other;
    }

    public override string ToString () {
      return string.Format("{0}, {1}", Column, Row);
    }

    /// <summary>
    /// Determines if two locations are equal.
    /// </summary>
    /// <param name="left">The left location.</param>
    /// <param name="right">The right location.</param>
    /// <returns>Returns true if the lcoations are equal, otherwise returns false.</returns>
    public static bool operator== (Location left, Location right) {
      return left.Row == right.Row && left.Column == right.Column;
    }

    /// <summary>
    /// Determines if two locations are not equal.
    /// </summary>
    /// <param name="left">The left location.</param>
    /// <param name="right">The right location.</param>
    /// <returns>Returns true if the lcoations are not equal, otherwise returns false.</returns>
    public static bool operator!= (Location left, Location right) {
      return !(left == right);
    }

    /// <summary>
    /// Determines the left location is earlier in the document than the right location.
    /// </summary>
    /// <param name="left">The left location.</param>
    /// <param name="right">The right location.</param>
    /// <returns>Returns true if the left location is earlier, otherwise returns false.</returns>
    public static bool operator< (Location left, Location right) {
      if (left.Row < right.Row) {
        return true;
      }

      if (left.Row == right.Row && left.Column < right.Column) {
        return true;
      }

      return false;
    }

    /// <summary>
    /// Determines the left location is later in the document than the right location.
    /// </summary>
    /// <param name="left">The left location.</param>
    /// <param name="right">The right location.</param>
    /// <returns>Returns true if the left location is later, otherwise returns false.</returns>
    public static bool operator> (Location left, Location right) {
      return !(left <= right);
    }

    /// <summary>
    /// Determines the left location is earlier in the document than (or the same as) the right location.
    /// </summary>
    /// <param name="left">The left location.</param>
    /// <param name="right">The right location.</param>
    /// <returns>Returns true if the left location is earlier or equal, otherwise returns false.</returns>
    public static bool operator<= (Location left, Location right) {
      return left == right || left < right;
    }

    /// <summary>
    /// Determines the left location is later in the document than (or the same as) the right location.
    /// </summary>
    /// <param name="left">The left location.</param>
    /// <param name="right">The right location.</param>
    /// <returns>Returns true if the left location is later or equal, otherwise returns false.</returns>
    public static bool operator>= (Location left, Location right) {
      return left == right || left > right;
    }
  }
}
