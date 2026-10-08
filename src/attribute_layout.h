#pragma once
#include "gl_helper.h"
#include "glad/glad.h"

namespace SimpleGL{
	struct AttributeLayout {
		unsigned int count;
		GLenumAttributeWrapper type;
		GLboolean normalize;

		AttributeLayout(unsigned int count, GLenumAttributeWrapper type,
						GLboolean normalize)
			: count(count), type(type), normalize(normalize) {}
	};
}
