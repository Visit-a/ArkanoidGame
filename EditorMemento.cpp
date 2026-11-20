#include "EditorMemento.h"

namespace Patterns
{
    void History::Save(const EditorDocument& document)
    {
        undoStack.push_back(document.CreateMemento());
        redoStack.clear();
    }

    bool History::Undo(EditorDocument& document)
    {
        if (undoStack.empty())
            return false;

        redoStack.push_back(document.CreateMemento());
        document.Restore(undoStack.back());
        undoStack.pop_back();
        return true;
    }

    bool History::Redo(EditorDocument& document)
    {
        if (redoStack.empty())
            return false;

        undoStack.push_back(document.CreateMemento());
        document.Restore(redoStack.back());
        redoStack.pop_back();
        return true;
    }
}
