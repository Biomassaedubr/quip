
namespace Quip {
  public class NormalMode : Mode {
    public NormalMode (IDocumentView view)
      : base("Normal", view) {
      AddMapping(new Keystroke("h"), MoveCursorLeft);
      AddMapping(new Keystroke("j"), MoveCursorDown);
      AddMapping(new Keystroke("k"), MoveCursorUp);
      AddMapping(new Keystroke("l"), MoveCursorRight);
      AddMapping(new Keystroke("w"), MoveToNextWord);
      AddMapping(new Keystroke("b"), MoveToPriorWord);

      AddMapping(new Keystroke("i"), EnterInsertMode);
      AddMapping(new Keystroke("/"), EnterSearchMode);

      AddMapping(new Keystroke("gg"), GoToStart);
      AddMapping(new Keystroke("G"), GoToEnd);
    }

    bool EnterInsertMode (IDocumentView view) {
      view.Mode = new InsertMode(view);

      return true;
    }

    bool EnterSearchMode (IDocumentView view) {
      view.Mode = new SearchMode(view);

      return true;
    }

    bool MoveCursorLeft (IDocumentView view) {
      var iterator = new CharacterIterator(view.Document, view.Cursor);
      if (iterator.CanMovePrior) {
        iterator.MovePrior();
        view.MoveTo(iterator.Location);
      }

      return true;
    }

    bool MoveCursorRight (IDocumentView view) {
      var iterator = new CharacterIterator(view.Document, view.Cursor);
      if (iterator.CanMoveNext) {
        iterator.MoveNext();
        view.MoveTo(iterator.Location);
      }

      return true;
    }

    bool MoveCursorDown (IDocumentView view) {
      view.MoveTo(new Location(view.Cursor.Column, view.Cursor.Row + 1));

      return true;
    }

    bool MoveCursorUp (IDocumentView view) {
      view.MoveTo(new Location(view.Cursor.Column, view.Cursor.Row - 1));

      return true;
    }

    bool MoveToNextWord (IDocumentView view) {
      var iterator = view.Document.GetWordIterator(view.Cursor);
      iterator.MoveNext();
      view.MoveTo(iterator.Location);

      return true;
    }

    bool MoveToPriorWord (IDocumentView view) {
      var iterator = view.Document.GetWordIterator(view.Cursor);
      iterator.MovePrior();
      view.MoveTo(iterator.Location);

      return true;
    }

    bool GoToStart (IDocumentView view) {
      view.MoveTo(Location.Zero);

      return true;
    }

    bool GoToEnd (IDocumentView view) {
      var row = view.Document.Rows - 1;
      var column = view.Document.GetRow(row).Length - 1;
      view.MoveTo(new Location(column, row));

      return true;
    }
  }
}