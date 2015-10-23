namespace Quip {
  class InsertMode : Mode {
    public InsertMode (IDocumentView view)
      : base("Insert", view) {
      AddMapping(Key.Escape, LeaveInsertMode);

      view.PushCursorStyle(CursorStyle.VerticalBar);
    }

    protected override bool OnHandleKey (Key input, IDocumentView view) {
      if (input != Key.Backspace) {
        view.Cursor = view.Document.Insert(input.ToText(), view.Cursor);
      } else {
        view.Cursor = view.Document.Erase(view.Cursor);
      }

      return true;
    }

    bool LeaveInsertMode (IDocumentView view) {
      view.PopCursorStyle();
      view.Mode = new NormalMode(view);
      return true;
    }
  }
}