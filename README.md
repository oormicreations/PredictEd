# PredictEd


![Logo](https://github.com/oormicreations/PredictEd/blob/master/PredictEd/res/icon01.png)

**PredictEd** is a text editor with text prediction and other smart features.

---

**Introduction and Tutorial**

Help Document
https://oormi.in/software/predicted/help.html

Video Introduction
https://www.youtube.com/watch?v=RZPQNAE9kYs

---

**Download link**

https://github.com/oormicreations/PredictEd/releases

---

**Features:**

* Word prediction as you type.
* Word and phrase completion.
* Learns new words as you type.
* Train using existing text files of your own.
* Format as you type (basic).
* Auto sentence case and paragraphs.
* Auto saves the work.
* Save and open RTF and Text files.
* Usual editor functions like cut, copy, paste, font type, font size, find and replace, undo/redo etc.
* Background colors and text colors.
* Unicode UTF-8 support. Works for many languages.
* Supports MS Windows 7 onwards.

---

**For Users**

Download the installer and run it. Launch it from Desktop shortcut or from Start menu. Start typing.

The suggestions are shown as you type. Press TAB to select a suggestion. More TAB presses cycle through the suggestions. Three of the suggestions are phrases.
When spacebar is pressed it shows a probable word that may occur next.

Enter after a period (full stop) adds a new line and is a paragraph break.
Type a formatting pair (like **, // or __ ) around the words to format as you type.

The installation comes with a built in word list (knowledge map), which was generated using some default text files. So it will immediately show suggestions, but it is highly recommended to train it using your own writings, so that you get most relevant suggestions. Anyhow it will learn as you type.

This tool helps to speed up typing and ease the stress on fingers, especially for those with RSI or carpel tunnel etc. Initially it will appear as if you have slowed down because now you need to check the suggestions or predictions and decide whether to continue typing or press TAB. But gradually it becomes your habit and the typing will speed up. Takes a bit of practice and getting used to predictive typing.

See the tutorial video or help document for more details (linked above).

---

**For Developers**

PredictEd uses two lists (maps) to predict the next word or to provide autocomplete suggestions.

The first is the LTM (long term memory) which is a list made from feeding text files to it or appending the STM (short term memory) data, which is the second list. STM has data which is gathered while the user types in the editor.

The entries in the list are in this form:

`keyword` | `frequency` | `prediction#1` | `prediction#2` | `...` | `prediction#N` | `prediction weight #1` | `prediction weight #2` | `...` | `prediction weight #N`
| --- | --- | --- | --- | --- | --- | --- | --- | --- | ---

Both the lists are sorted on Frequency. Frequency is the count of a word found during the training. More frequently used words are at the top.

Prediction#X is a word that was found to occur after the keyword. Prediction weight #X is the count of such consecutive occurances. So more weight means that word occurs with more probability after the specified keyword. The predictions are sorted on weights. Exactly five such predictions are presented when a space bar is pressed after a word, which becomes the keyword. Pressing the TAB puts the predicted word in the editor window. Pressing the TAB again puts the next most probable word in the editor in place of the previous word. 

When a match is not found in LTM, the function searches the STM.

When some letters are typed, a function searches the LTM for keywords beginning with those exact letters. Since the list is sorted, the output is matches with decreasing frequency. We get the most probable word at the top. Total 5 such words are suggested. 

When no match is found in LTM, the function searches in STM.

Currently both lists have 5000 entries. When the STM is full, the data is merged with LTM and STM is cleared. It is ready to receive new data via real time learning.

If the LTM also gets filled up, only weights and frequencies get updated. So the LTM holds the knowledge as well as it builts up new relations as it takes new data from STM. Both LTM and STM are dynamic knowledge bases. LTM is less dynamic compared to STM.

At any time one can erase the LTM and retrain it using text files. The good thing about PredictEd is that the knowledge can be customized by the user, it is not hard coded. This has advantages. For example a user writing mostly on the subject of Economics will get predictions and suggestions in that context, and another user writing on the subject of History will get relevant predictions, not something from Economics.

Anyhow, because of STM, the knowledge base gradually acquires the user specific context. The whole knowledge gets biased according to the writing style and character of the user, and changes as the user shifts to different contexts. The user gets the right suggestions most of the time.

Three of the suggestions are multiple words or phrases, which are recursively predicted using the last prediction as a keyword.

_IDE:_

MS Visual Studio Community 2015. C++ and MFC.

_Classes:_

CPredictEdDlg: Handles the main window, controls, menu and their messages.

CPredictEdCtrl: Derived from RichEditControl 2.0, handles the typing, formation of word queue, formatting, sentence case, and prediction, real time training, autocompletion and other tasks.

CPreWordsDlg: Displays the predicted words and handles the cycling of words from bottom to top.

CWordList: It is the object that contains the word lists. Handles adding new entries, forming relations, sorting, returning the matches, saving and loading the lists in CSV format.

CCharQueue: Object that maintains last 100 characters from the editor window as the user types and interacts with the text. The words are counted backwords from currect cursor position. Used for sentence case, capitalization, paragraph breaks etc.

CKeyWordMap: This object holds a single entry of the word list. Adds, returns and sorts the prediction and weights.

CTrain: Handles training of LTM from text input. Displays the training dialog.

CSysHelper: Helper class for open, save, and all file/OS related functions.

CNetHelper: Helper class for internet related functions.

---

_About_

A FOSS Project by Oormi Creations

http://oormi.in

oormicreations@gmail.com

---