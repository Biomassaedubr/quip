using System;
using System.Collections.Generic;

namespace Quip {
  public class MapTrie {
    class Node {
      Dictionary<Key, Node> children = new Dictionary<Key, Node>();
      MapHandler item;

      public void Insert (Keystroke keystroke, int index, MapHandler handler) {
        if (index == keystroke.Length) {
          item = handler;
        } else {
          var letter = keystroke[index];
          var child = default(Node);
          if (!children.TryGetValue(letter, out child)) {
            child = new Node();
            children[letter] = child;
          }

          child.Insert(keystroke, index + 1, handler);
        }
      }

      public MapHandler Find (Keystroke keystroke, int index) {
        if (index == keystroke.Length) {
          return item;
        } else {
          var letter = keystroke[index];
          var child = default(Node);
          if (children.TryGetValue(letter, out child)) {
            return child.Find(keystroke, index + 1);
          }
        }

        return null;
      }
    }

    public void Insert (Keystroke keystroke, MapHandler handler) {
      m_root.Insert(keystroke, 0, handler); 
    }

    public MapHandler Find (Keystroke keystroke) {
      return m_root.Find(keystroke, 0);
    }

    Node m_root = new Node();
  }
}

