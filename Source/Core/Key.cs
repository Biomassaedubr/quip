using System;

namespace Quip {
  public class Key : IEquatable<Key> {
    public Key (string symbol) {
      m_value = symbol;
    }

    public Key (char character) {
      m_value = new string(character, 1);
    }

    public string ToText () {
      return m_value;
    }

    public override int GetHashCode () {
      return m_value.GetHashCode();
    }

    public override bool Equals (object other) {
      return !ReferenceEquals(null, other) && m_value == ((Key)other).m_value;
    }

    public bool Equals (Key other) {
      return !ReferenceEquals(null, other) && m_value == other.m_value;
    }

    public readonly static Key Enter = new Key("\n");
    public readonly static Key Semicolon = new Key(';');
    public readonly static Key Backspace = new Key('\b');
    public readonly static Key Escape = new Key("<esc>");

    public static bool operator == (Key left, Key right) {
      if (ReferenceEquals(left, null)) {
        return ReferenceEquals(right, null);
      }

      return left.Equals(right);
    }

    public static bool operator != (Key left, Key right) {
      return !(left == right);
    }

    string m_value = string.Empty;
  }
}
