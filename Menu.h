#pragma once
#include "SFML/Graphics.hpp"
#include "Text.h"
#include <vector>

namespace SnakeGame
{
	class Menu;

	class MenuItem
	{
	public:
		sf::Text& GetText() { return text; }
		const sf::Text& GetText() const { return text; }
		sf::Text& GetHintText() { return hintText; }

		Orientation GetChildrenOrientation() const { return childrenOrientation; }
		void SetChildrenOrientation(Orientation o) { childrenOrientation = o; }
		Alignment GetChildrenAlignment() const { return childrenAlignment; }
		void SetChildrenAlignment(Alignment a) { childrenAlignment = a; }
		float GetChildrenSpacing() const { return childrenSpacing; }
		void SetChildrenSpacing(float s) { childrenSpacing = s; }

		const std::vector<MenuItem*>& GetChildren() const { return children; }
		void AddChild(MenuItem* child) { children.push_back(child); }

		MenuItem* GetParent() const { return parent; }
		void SetParent(MenuItem* p) { parent = p; }

		void SetEnabled(bool e) { isEnabled = e; }
		bool IsEnabled() const { return isEnabled; }

		void SetSelectedColor(sf::Color c) { selectedColor = c; }
		sf::Color GetSelectedColor() const { return selectedColor; }
		void SetDeselectedColor(sf::Color c) { deselectedColor = c; }
		sf::Color GetDeselectedColor() const { return deselectedColor; }

		friend void InitMenuItem(MenuItem& item);
		friend void SelectMenuItem(Menu& menu, MenuItem* item);
		friend bool SelectPreviousMenuItem(Menu& menu);
		friend bool SelectNextMenuItem(Menu& menu);
		friend bool ExpandSelectedItem(Menu& menu);
		friend bool CollapseSelectedItem(Menu& menu);
		friend void DrawMenu(Menu& menu, sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);

	private:
		sf::Text text;
		sf::Text hintText;
		Orientation childrenOrientation = Orientation::Vertical;
		Alignment childrenAlignment = Alignment::Min;
		float childrenSpacing = 10.f;
		sf::Color selectedColor = sf::Color::Yellow;
		sf::Color deselectedColor = sf::Color::White;
		bool isEnabled = true;
		std::vector<MenuItem*> children;
		MenuItem* parent = nullptr;
	};

	class Menu
	{
	public:
		MenuItem& GetRootItem() { return rootItem; }
		MenuItem* GetSelectedItem() const { return selectedItem; }

		friend void SelectMenuItem(Menu& menu, MenuItem* item);
		friend bool SelectPreviousMenuItem(Menu& menu);
		friend bool SelectNextMenuItem(Menu& menu);
		friend bool ExpandSelectedItem(Menu& menu);
		friend bool CollapseSelectedItem(Menu& menu);
		friend MenuItem* GetCurrentMenuContext(Menu& menu);
		friend void DrawMenu(Menu& menu, sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);

	private:
		MenuItem rootItem;
		MenuItem* selectedItem = nullptr;
	};

	void InitMenuItem(MenuItem& menu);
	void SelectMenuItem(Menu& menu, MenuItem* item);
	bool SelectPreviousMenuItem(Menu& menu);
	bool SelectNextMenuItem(Menu& menu);
	bool ExpandSelectedItem(Menu& menu);
	bool CollapseSelectedItem(Menu& menu);

	MenuItem* GetCurrentMenuContext(Menu& menu);

	void DrawMenu(Menu& menu, sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);
}