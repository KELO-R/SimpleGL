#pragma once
#include "miniaudio.h"
#include <expected>
#include <memory>
#include <string>

class Audio{
private:
    static void free_engine(ma_engine* engine){
	ma_engine_uninit(engine);
	delete engine;
    }
    std::unique_ptr<ma_engine, decltype(&free_engine)> engine;
    Audio(std::unique_ptr<ma_engine, decltype(&free_engine)>&& engine)
	:engine(std::move(engine)){}
public:
    static std::expected<Audio, std::string> create();
    inline void play(std::string_view path){
	ma_engine_play_sound(engine.get(), path.cbegin(), NULL);
    }
};
