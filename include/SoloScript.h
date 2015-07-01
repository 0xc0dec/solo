#pragma once

#include "SoloComponent.h"

namespace solo
{
	class Script : public ComponentBase<Script>
	{
	protected:
		Script(Node* node);

	private:
		friend class ScriptFactory;
		
		Script(const Script& other) = delete;
		Script(Script&& other) = delete;
		Script& operator=(const Script& other) = delete;
		Script& operator=(Script&& other) = delete;
	};

	class ScriptFactory
	{
	public:
		friend class Node;
		static shared<Script> create(Node* node, const std::string& componentObjectName);
	};
}