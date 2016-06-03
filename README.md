# Quip

[![Build Status](https://api.travis-ci.org/jpetrie/quip.svg?branch=master)](https://travis-ci.org/jpetrie/quip)

Quip is a text editor. It's long-term goal is to be a modern *modal* editing environment.
However, right now it's just a very early proof-of-concept and playground for various
potential implementation techniques.

It is usable enough to edit basic text files (such as this one), but much of the functionality to make it
useful for editing code is still forthcoming.

## Principles

These are Quip's (current) key ideals:

 - Quip operates on directed, inclusive selections. Solving a problem should be accomplished by crafting
   a selection set and then operating on that selection set.
 - Quip is modal, and supports many modes. A mode should reflect a task and expose functionality for
   making that task happen: navigate the document, edit the document, search the document.
 - Be orthogonal; there should generally be one way to accomplish a task.
 - Be modern; leverage the surrounding OS conventions and tools where possible.
 - Be pragmatic; focusing on comfortably editing text above all else.

On the other hand, these are some commonly-asked-about *non*-goals for Quip:

 - Quip is not a command-line text editor.
 - Quip is not an IDE, file browser, e-mail client, or calculator.
 - Quip is not vim or emacs. They are both excellent tools, but Quip is not about creating "another" vim or even
   about maintaining keybinding compability.
