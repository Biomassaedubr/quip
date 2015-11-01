using System;
using System.Collections.Generic;

namespace Quip {
  public abstract class Mode {
    protected Mode (string name, DocumentView view) {
      Name = name;
      View = view;
    }

    public string Name {
      get;
      protected set;
    }

    public bool HandleKey (Key input, DocumentView view) {
      m_current.Append(input);

      var mapping = m_mappings.Find(m_current);
      if(mapping != null) {
        m_current.Clear();
        return mapping(view);
      } else {
        var handled = OnHandleKey(input, view);
        m_current.Clear();

        return handled;
      }
    }

    protected DocumentView View {
      get;
      private set;
    }

    protected virtual bool OnHandleKey (Key input, DocumentView view) {
      return false;
    }

    protected void AddMapping (Keystroke keystroke, MapHandler handler) {
      m_mappings.Insert(keystroke, handler);
    }

    Keystroke m_current = new Keystroke();
    MapTrie m_mappings = new MapTrie();
  }
}
