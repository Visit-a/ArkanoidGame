#include "Menu.h"
#include <assert.h>
#include <algorithm>

namespace SnakeGame
{
	void InitMenuItem(MenuItem& item)
	{
		for (auto& child : item.GetChildren())
		{
			child->SetParent(&item);
			InitMenuItem(*child);
		}
	}

	void SelectMenuItem(Menu& menu, MenuItem* item)
	{
		assert(item != &menu.GetRootItem());
		if (menu.GetSelectedItem() == item) return;
		if (item && !item->IsEnabled()) return;

		if (menu.GetSelectedItem())
			menu.GetSelectedItem()->GetText().setFillColor(menu.GetSelectedItem()->GetDeselectedColor());

		menu.selectedItem = item;

		if (menu.GetSelectedItem())
			menu.GetSelectedItem()->GetText().setFillColor(menu.GetSelectedItem()->GetSelectedColor());
	}

	bool SelectPreviousMenuItem(Menu& menu)
	{
		if (menu.GetSelectedItem())
		{
			MenuItem* parent = menu.GetSelectedItem()->GetParent();
			assert(parent);
			const auto& children = parent->GetChildren();
			auto it = std::find(children.begin(), children.end(), menu.GetSelectedItem());
			if (it != children.begin())
			{
				SelectMenuItem(menu, *(--it));
				return true;
			}
		}
		return false;
	}

	bool SelectNextMenuItem(Menu& menu)
	{
		if (menu.GetSelectedItem())
		{
			MenuItem* parent = menu.GetSelectedItem()->GetParent();
			assert(parent);
			const auto& children = parent->GetChildren();
			auto it = std::find(children.begin(), children.end(), menu.GetSelectedItem());
			if (it != children.end() - 1)
			{
				SelectMenuItem(menu, *(++it));
				return true;
			}
		}
		return false;
	}

	bool ExpandSelectedItem(Menu& menu)
	{
		if (menu.GetSelectedItem() && !menu.GetSelectedItem()->GetChildren().empty())
		{
			SelectMenuItem(menu, menu.GetSelectedItem()->GetChildren().front());
			return true;
		}
		return false;
	}

	bool CollapseSelectedItem(Menu& menu)
	{
		if (menu.GetSelectedItem() && menu.GetSelectedItem()->GetParent() &&
			menu.GetSelectedItem()->GetParent() != &menu.GetRootItem())
		{
			SelectMenuItem(menu, menu.GetSelectedItem()->GetParent());
			return true;
		}
		return false;
	}

	MenuItem* GetCurrentMenuContext(Menu& menu)
	{
		return menu.GetSelectedItem() ? menu.GetSelectedItem()->GetParent() : &menu.GetRootItem();
	}

	void DrawMenu(Menu& menu, sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
	{
		MenuItem* expandedItem = GetCurrentMenuContext(menu);

		std::vector<sf::Text*> texts;
		for (auto& child : expandedItem->GetChildren())
		{
			if (child->IsEnabled()) texts.push_back(&child->GetText());
		}

		DrawTextList(window, texts, expandedItem->GetChildrenSpacing(), expandedItem->GetChildrenOrientation(), expandedItem->GetChildrenAlignment(), position, origin);
	}
}