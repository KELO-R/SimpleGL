#pragma once
#include <expected>
#include <string>

namespace SimpleGL{
	class IDrawObject{
	protected:
		IDrawObject(){};
	public:
		virtual std::expected<void, std::string> draw() = 0;
	};
}
