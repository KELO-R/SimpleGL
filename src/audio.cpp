#include "audio.h"

std::expected<Audio, std::string> Audio::create(){
    std::unique_ptr<ma_engine, decltype(&free_engine)> engine {new ma_engine, free_engine};
    ma_result result = ma_engine_init(NULL, engine.get());
    if(result != MA_SUCCESS){
	return std::unexpected("Failed to create initialize ma_engine for Audio!");
    }
    return Audio(std::move(engine));
}
