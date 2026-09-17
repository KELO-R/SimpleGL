#pragma once
#include <memory>
#include <expected>

class Image{
    static void free_data(unsigned char* data);//in cpp{
    std::unique_ptr<unsigned char, decltype(&free_data)> data;
    int height;
    int width;
    int channels_count;
    Image(std::unique_ptr<unsigned char, decltype(&free_data)>&& data,
	int height, int width, int channels_count)
	: data(std::move(data)), height(height), width(width), channels_count(channels_count){}
public:
    inline int get_height() const{
	return height;
    }
    inline int get_width() const{
	return width;
    }
    inline int get_channels_count() const{
	return channels_count;
    }
    inline const unsigned char* get_data() const{
	return data.get();
    }
    static std::expected<Image, std::string> from_file(
	    std::string_view path, 
	    int requested_channels_count = 0, 
	    bool flip_vertically = false);
};
