#pragma once
#include "draw_objects/draw_textured_mesh_3d_t.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

class ParticleGeneratorCPU{
public:
    struct Particle{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 rotation;
	glm::vec3 spin;
	glm::vec3 scale;
	glm::vec3 growth;
	double dead_at{0.0f};
	DrawTexturedMesh3DT draw_obj;

	Particle(DrawTexturedMesh3DT draw_obj)
	    :draw_obj(draw_obj){}

	void apply_transformation(){
	    glm::mat4& mat = draw_obj.mat;
	    mat = glm::scale(glm::mat4(1.0f), scale);
	    mat = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * mat;
	    mat = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) * mat;
	    mat = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * mat;
	    mat = glm::translate(glm::mat4(1.0f), position) * mat;
	}
    };
private:
    float particle_spawn_clock {0.0f};
    unsigned int particle_count_max {0};
    std::vector<Particle> particles;
    bool stop_spawning {false};
    bool stop_spawning_clock {false};
    bool stop_drawing {false};
    unsigned int particle_alive_count {0};
public:
    DrawTexturedMesh3DT draw_object_prefab;
    unsigned int particle_spawn_queue {0};
    unsigned int particle_spawn_queue_max_size {0};
    float particle_spawn_rate {0.1f};
    double particle_lifetime {10.0f};
    glm::vec3 particle_start_position {0.0f};
    glm::vec3 particle_start_velocity {0.0f};
    glm::vec3 particle_start_rotation {0.0f};
    glm::vec3 particle_start_spin {0.0f};
    glm::vec3 particle_start_scale {1.0f};
    glm::vec3 particle_start_growth {0.0f};

    ParticleGeneratorCPU(DrawTexturedMesh3DT draw_object_prefab)
	:draw_object_prefab(draw_object_prefab){};

    unsigned int get_particle_avile_count() const{
	return particle_alive_count;
    }

    void kill_particles(){
	for(auto& it: particles)
	    it.dead_at = 0.0f;
    }

    void set_particle_count_max(unsigned int max_count){
	particle_count_max = max_count;
	particles.resize(max_count, Particle(draw_object_prefab));
    }
    unsigned int get_particle_count_max() const{
	return particle_count_max;
    }

    void bring_to_life(Particle& particle, double current_time){
	particle.dead_at = current_time + particle_lifetime;
	particle.position = particle_start_position;
	particle.velocity = particle_start_velocity;
	particle.rotation = particle_start_rotation;
	particle.spin = particle_start_spin;
	particle.scale = particle_start_scale;
	particle.growth = particle_start_growth;
	particle.draw_obj.mat = draw_object_prefab.mat;
    }

    std::expected<void, std::string> update(double current_time, float delta_time){
	if(!stop_spawning_clock){
	    particle_spawn_clock += delta_time;
	    unsigned int new_particles_requested = particle_spawn_clock / particle_spawn_rate;
	    particle_spawn_clock -= new_particles_requested * particle_spawn_rate;
	    particle_spawn_queue += new_particles_requested;
	}
	if(particle_spawn_queue > particle_spawn_queue_max_size)
	   particle_spawn_queue = particle_spawn_queue_max_size; 

	particle_alive_count = 0;
	for(size_t i{}; i < particles.size(); i++){
	    Particle& particle = particles[i];
	    if(particle.dead_at > current_time){
		particle.position += particle.velocity * delta_time;
		particle.rotation += particle.spin * delta_time;
		particle.scale += particle.growth * delta_time;
		particle.apply_transformation();
		if(!stop_drawing){
		    auto draw_r = particle.draw_obj.draw();
		    if(!draw_r.has_value())
			return std::unexpected(std::move(draw_r.error()));
		}
		particle_alive_count++;
	    }
	    else if(particle_spawn_queue > 0){
		if(!stop_spawning){
		    bring_to_life(particle, current_time);
		    particle_spawn_queue--;
		}
	    }
	}
	return {};
    }
};
