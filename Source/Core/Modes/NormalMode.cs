
namespace Quip {
  public class NormalMode : Mode {
    public NormalMode (IDocumentView view)
      : base("Normal", view) {
      AddMapping(new Key('h'), MoveCursorLeft);
      AddMapping(new Key('j'), MoveCursorDown);
      AddMapping(new Key('k'), MoveCursorUp);
      AddMapping(new Key('l'), MoveCursorRight);
      AddMapping(new Key('w'), MoveToNextWord);
      AddMapping(new Key('b'), MoveToPriorWord);
      AddMapping(new Key('i'), EnterInsertMode);
    }

    bool EnterInsertMode (IDocumentView view) {
      view.Mode = new InsertMode(view);

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
  }
}