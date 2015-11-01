using System;

namespace Quip {
  public class NormalMode : Mode {
    public NormalMode (DocumentView view)
      : base("Normal", view) {
      AddMapping(new Keystroke("h"), SelectCharacterLeftOfPrimary);
      AddMapping(new Keystroke("l"), SelectCharacterRightOfPrimary);
      AddMapping(new Keystroke("j"), SelectCharacterBelowPrimary);
      AddMapping(new Keystroke("k"), SelectCharacterAbovePrimary);

      AddMapping(new Keystroke("w"), MoveToNextWord);
      AddMapping(new Keystroke("b"), MoveToPriorWord);

      AddMapping(new Keystroke("i"), EnterInsertMode);
      AddMapping(new Keystroke("s"), EnterSearchMode);

      AddMapping(new Keystroke("gg"), SelectFirstCharacterInDocument);
      AddMapping(new Keystroke("G"), SelectLastCharacterInDocument);

      AddMapping(new Keystroke("x"), CutSelections);

      AddMapping(new Keystroke(Key.Enter), RotateSelection);
      AddMapping(new Keystroke("\\"), SelectPrimaryOnly);

    }

    bool EnterInsertMode (DocumentView view) {
      view.Mode = new InsertMode(view);

      return true;
    }

    bool EnterSearchMode (DocumentView view) {
      view.Mode = new SearchMode(view);

      return true;
    }

    bool SelectCharacterLeftOfPrimary (DocumentView view) {
      var iterator = new CharacterIterator(view.Document, view.Selections.Primary.LowerBound);
      if (iterator.CanMovePrior) {
        iterator.MovePrior();
        view.Selections.ReplaceWith(Selection.At(iterator.Location));
      }

      return true;
    }

    bool SelectCharacterRightOfPrimary (DocumentView view) {
      var iterator = new CharacterIterator(view.Document, view.Selections.Primary.UpperBound);
      if (iterator.CanMoveNext) {
        iterator.MoveNext();
        view.Selections.ReplaceWith(Selection.At(iterator.Location));
      }

      return true;
    }

    bool SelectCharacterBelowPrimary (DocumentView view) {
      var location = view.Selections.Primary.UpperBound.AdjustBy(0, 1);
      if (location.Row < view.Document.Rows) {
        location.Column = Math.Min(location.Column, view.Document.GetRow(location.Row).Length - 1);
        view.Selections.ReplaceWith(Selection.At(location));
      }

      return true;
    }

    bool SelectCharacterAbovePrimary (DocumentView view) {
      var location = view.Selections.Primary.UpperBound.AdjustBy(0, -1);
      if (location.Row >= 0) {
        location.Column = Math.Min(location.Column, view.Document.GetRow(location.Row).Length - 1);
        view.Selections.ReplaceWith(Selection.At(location));
      }

      return true;
    }

    bool MoveToNextWord (DocumentView view) {
      var iterator = view.Document.GetWordIterator(view.Selections.Primary.UpperBound);
      iterator.MoveNext();
      view.MoveTo(iterator.Location);

      return true;
    }

    bool MoveToPriorWord (DocumentView view) {
      var iterator = view.Document.GetWordIterator(view.Selections.Primary.LowerBound);
      iterator.MovePrior();
      view.MoveTo(iterator.Location);

      return true;
    }

    bool SelectFirstCharacterInDocument (DocumentView view) {
      view.MoveTo(Location.Zero);

      return true;
    }

    bool SelectLastCharacterInDocument (DocumentView view) {
      var row = view.Document.Rows - 1;
      var column = view.Document.GetRow(row).Length - 1;
      view.MoveTo(new Location(column, row));

      return true;
    }

    bool CutSelections (DocumentView view) {
      view.Document.Erase(view.Selections);
      return true;
    }

    bool SelectPrimaryOnly (DocumentView view) {
      view.Selections.ReplaceWith(new [] { view.Selections.Primary });
      return true;
    }

    bool RotateSelection (DocumentView view) {
      view.Selections.Rotate();
      return true;
    }
  }
}