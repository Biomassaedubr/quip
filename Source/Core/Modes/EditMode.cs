namespace Quip {
  class EditMode : Mode {
    public EditMode (DocumentView view, EditStyle style)
      : base("Edit", view) {
      m_style = style;

      AddMapping(new Keystroke(Key.Escape), LeaveInsertMode);
      view.PushCursorStyle(CursorStyle.VerticalBar);
    }

    protected override bool OnHandleKey (Key input, DocumentView view) {
      if (input != Key.Backspace) {
        switch (m_style) {
          case EditStyle.Insert:
            view.Document.Insert(input.ToText(), view.Selections);
            break;
          case EditStyle.Append:
            view.Document.Append(input.ToText(), view.Selections);
            break;
          default:
            break;
        }
      } else {
        //view.MoveTo(view.Document.Erase(view.Selections.Primary.LowerBound));
      }

      return true;
    }

    bool LeaveInsertMode (DocumentView view) {
      view.PopCursorStyle();
      view.Mode = new NormalMode(view);
      return true;
    }

    EditStyle m_style;
  }
}