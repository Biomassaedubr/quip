namespace Quip {
  class InsertMode : Mode {
    public InsertMode (DocumentView view)
      : base("Insert", view) {
      AddMapping(new Keystroke(Key.Escape), LeaveInsertMode);

      view.PushCursorStyle(CursorStyle.VerticalBar);
    }

    protected override bool OnHandleKey (Key input, DocumentView view) {
      if (input != Key.Backspace) {
        view.MoveTo(view.Document.Insert(input.ToText(), view.Cursor));
      } else {
        view.MoveTo(view.Document.Erase(view.Cursor));
      }

      return true;
    }

    bool LeaveInsertMode (DocumentView view) {
      view.PopCursorStyle();
      view.Mode = new NormalMode(view);
      return true;
    }
  }
}