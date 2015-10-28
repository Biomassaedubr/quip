using System;

namespace Quip {
  /// <summary>
  /// Provides an interface for constraining numerical values.
  /// </summary>
  public static class Clamp {
    /// <summary>
    /// Constrains a value between two other values.
    /// </summary>
    /// <param name="minimum">The inclusive minimum value.</param>
    /// <param name="maximum">The inclusive maximum value.</param>
    /// <param name="value">The value to constrain.</param>
    /// <typeparam name="T">The type of the value.</typeparam>
    /// <returns>Returns minimum the value is lower than, the maximum if the value is higher than, or the value itself.</returns>
    public static T Between<T> (T minimum, T maximum, T value) where T : IComparable {
      if (value.CompareTo(minimum) <= 0) {
        return minimum;
      } else if (value.CompareTo(maximum) >= 0) {
        return maximum;
      }

      return value;
    }
  }
}

