#include "image.h"
#include <format>

namespace SimpleGL{
	void Image::free_data(unsigned char *data) { stbi_image_free(data); }

	std::expected<Image, std::string> Image::from_file(std::string_view path,
													   int requested_channels_count,
													   bool flip_vertically) {
		int width, height, channels_count;
		stbi_set_flip_vertically_on_load(!flip_vertically);
		std::unique_ptr<unsigned char, decltype(&free_data)> data(
			stbi_load(path.cbegin(), &width, &height, &channels_count,
					  requested_channels_count),
			free_data);
		if (!data.get()) {
			return std::unexpected(
				std::format("failed ot create image form file! path:{}", path));
		}
		return Image(std::move(data), width, height, channels_count);
	};
}
