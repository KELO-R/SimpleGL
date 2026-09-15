#include "vbo.h"

std::expected<Vbo, std::string> try_from(){
    unsigned int id;
    glGenBuffers(1, &id);
    Vbo(id);
}
