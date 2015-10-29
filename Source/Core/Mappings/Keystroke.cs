using System;
using System.Collections.Generic;

namespace Quip {
  public class Keystroke {
    public Keystroke () {
    }

    public Keystroke (string definition) {
      foreach (var token in definition) {
        m_keys.Add(new Key(new string(token, 1)));
      }
    }

    public Keystroke (params Key[] keys) {
      m_keys.AddRange(keys);
    }

    public int Length {
      get {
        return m_keys.Count;
      }
    }

    public Key this[int index] {
      get {
        return m_keys[index];
      }
    }

    public void Append (Key key) {
      m_keys.Add(key);
    }

    public void Clear () {
      m_keys.Clear();
    }

    List<Key> m_keys = new List<Key>();
  }
}

