#pragma once
#include "Core.h"
#include "FilterNode.h"

namespace HandFilter
{
	inline URef<FilterNode> ParseFilter(const std::string& filterStr)
	{
		if (filterStr == "")
			return std::make_unique<FilterNode>();

		static const std::unordered_set<char> ranks{ 'A','K','Q','J','T','9','8','7','6','5','4','3','2' };
		static const std::unordered_set<char> suits{ 'd','c','h','s' };

		URef<FilterNode> topFilter = std::make_unique<FilterNode>();
		FilterNode* activeFilter = topFilter->AddChild(false);

		for (int i = 0; i < filterStr.size();)
		{
			char currChar = filterStr[i];

			//if (!currFilter)
			//{
			//	currFilter = topFilter->AddChild(false, isNextNegated);
			//}

			if (currChar == '!')
			{
				if (!activeFilter->IsEmpty())
					activeFilter->parentFilter->AddChild(true);
				activeFilter->isNegated = true;
				i += 1;
			}
			else if (currChar == ':')
			{
				activeFilter = activeFilter->parentFilter->AddChild(true);
				i += 1;
			}
			else if (currChar == ',')
			{
				activeFilter = activeFilter->parentFilter->AddChild(false);
				i += 1;
			}
			else if (currChar == '(')
			{
				//currFilter = currFilter->parentFilter->AddChild();

				//i += 1;
			}
			else if (currChar == ')')
			{
				/*	currFilter = currFilter->parentFilter->AddChild();*/
			}
			else if (ranks.contains(currChar))
			{
				if (activeFilter->IsNode())
					activeFilter = activeFilter->parentFilter->AddChild(false);

				if (i + 1 < filterStr.size() && suits.contains(filterStr[i + 1])) //if rank followed by suit
				{
					Card card = Card(filterStr.substr(i, 2));

					activeFilter->includedCards.emplace(card);
					activeFilter->ranks[card - card % 4] += 1;
					activeFilter->suits[card % 4] += 1;

					i += 2;
				}
				else //just rank
				{
					Card card = Card(filterStr.substr(i, 1) + "c");
					activeFilter->ranks[card] += 1;
					i += 1;
				}
			}
			else if (suits.contains(currChar))
			{
				if (activeFilter->IsNode())
					activeFilter = activeFilter->parentFilter->AddChild(false);

				Card card = Card("2" + filterStr.substr(i, 1));

				activeFilter->suits[card] += 1;
				i += 1;
			}
			else //invalid character;
			{
				return nullptr;
			}
		}
		return topFilter;
	}
};