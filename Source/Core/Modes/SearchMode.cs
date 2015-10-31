namespace Quip {
  class SearchMode : Mode {
    public SearchMode (DocumentView view)
      : base("/", view) {
      AddMapping(new Keystroke(Key.Escape), LeaveMode);
      AddMapping(new Keystroke(Key.Enter), JumpToMatchAndLeave);
    }

    protected override bool OnHandleKey (Key input, DocumentView view) {
      if (input != Key.Backspace) {
        m_pattern += input.ToText();
      } else {
        m_pattern = m_pattern.Substring(0, m_pattern.Length - 1);
      }

      view.Selections.ReplaceWith(view.Document.Search(m_pattern));
      Name = string.Format("/{0}", m_pattern);

      return true;
    }

    bool LeaveMode (DocumentView view) {
      view.Mode = new NormalMode(view);
      return true;
    }

    bool JumpToMatchAndLeave (DocumentView view) {
      view.Mode = new NormalMode(view);
      view.MoveTo(view.Selections.Primary.Origin);

      return true;
    }

    string m_pattern = string.Empty;
  }
}