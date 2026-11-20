#pragma once
#include <string>
#include <vector>

namespace Patterns
{
    class DocumentMemento
    {
    public:
        explicit DocumentMemento(const std::string& value) : savedText(value) {}
        const std::string& GetSavedText() const { return savedText; }
    private:
        std::string savedText;
    };

    class EditorDocument
    {
    public:
        void SetText(const std::string& value) { text = value; }
        const std::string& GetText() const { return text; }
        DocumentMemento CreateMemento() const { return DocumentMemento(text); }
        void Restore(const DocumentMemento& memento) { text = memento.GetSavedText(); }
    private:
        std::string text;
    };

    class History
    {
    public:
        void Save(const EditorDocument& document);
        bool Undo(EditorDocument& document);
        bool Redo(EditorDocument& document);
        bool CanUndo() const { return !undoStack.empty(); }
        bool CanRedo() const { return !redoStack.empty(); }
    private:
        std::vector<DocumentMemento> undoStack;
        std::vector<DocumentMemento> redoStack;
    };
}
