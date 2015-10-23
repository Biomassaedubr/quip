using System;
using System.Collections.Generic;

namespace Quip {
  public abstract class Mode {
    protected Mode (string name, IDocumentView view) {
      Name = name;
      Status = Name;
      View = view;
    }

    public string Name {
      get;
      private set;
    }

    public string Status {
      get;
      private set;
    }

    public bool HandleKey (Key input, IDocumentView view) {
      var mapping = default(Func<IDocumentView, bool>);
      if (m_mappings.TryGetValue(input, out mapping)) {
        return mapping(view);
      } else {
        return OnHandleKey(input, view);
      }
    }

    protected IDocumentView View {
      get;
      private set;
    }

    protected virtual bool OnHandleKey (Key input, IDocumentView view) {
      return false;
    }

    protected void AddMapping (Key input, Func<IDocumentView, bool> callback) {
      m_mappings[input] = callback;
    }

    Dictionary<Key, Func<IDocumentView, bool>> m_mappings = new Dictionary<Key, Func<IDocumentView, bool>>();
  }
}
