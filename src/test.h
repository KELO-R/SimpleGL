#pragma once
#include <algorithm>
#include <string>
#include <expected>
#include <vector>
#include <optional>
#include <memory>

class Interface{
    Interface() = delete;
    Interface(const Interface&) = delete;
    Interface(const Interface&&) = delete;

    Interface& operator=(const Interface&) = delete;
    Interface& operator=(const Interface&&) = delete;
};

class Base : public Interface{

};


class ShaderProgram{
    int program;

    ShaderProgram(int program):
	program(program){};
public:
    static std::expected<ShaderProgram, std::string> try_from(){
	return ShaderProgram(10);
    };
};

template <typename T>
class Resource{
    std::shared_ptr<T> data;

    Resource(std::shared_ptr<T>&& data)
	: data(data){}


public:
    struct Handle{
	std::weak_ptr<T> wptr;
	Handle(std::weak_ptr<T>&& wptr)
	    : wptr(wptr){}
	std::optional<const T*> get() const{
	    if(wptr.expired()){
		return std::optional<const T*>();
	    }
	    return std::optional<const T*>(wptr.lock().get());
	}
    };
public:
    template <typename... Args>
    static std::expected<Resource<T>, std::string> try_from(Args&&... args){
	std::expected<T, std::string> obj = T::try_from(std::forward<Args>(args)...);
	if (!obj.has_value()){
	    return std::unexpected(obj.error());
	}
	return Resource{std::make_shared<T>(std::move(obj.value()))};
    }

    Handle handle(){
	return Handle{std::weak_ptr(data)};
    }
};

template <typename T>
struct ResourceManager{
    ResourceManager(){};
    std::vector<Resource<T>> resources;
    template <typename... Args>
    std::expected<typename Resource<T>::Handle, std::string> add_res(Args&&... args){
	std::expected<Resource<T>, std::string> obj = Resource<T>::try_from(std::forward<Args>(args)...);
	if(obj.has_value()){
	    resources.emplace_back(std::move(obj.value()));
	    return resources.at(resources.size()-1).handle();
	}
	return std::unexpected(obj.error());
    }
    
    //THIS FUNCTION IS BAD! //TODO change the thing 
    void del_res(const Resource<T>::Handle& handle){
	auto founded = std::find_if(resources.begin(), resources.end(),
	[handle](Resource<T>& res){
	    return res.handle().get().value() == handle.get().value();
	});
	if(resources.end() != founded){
	    resources.erase(founded);
	}
    }
};

/* 
class Resource{};
class Shader : public Resource{};

template <typename T>
requires std::derived_from<T, Resource>
class ResourceManager{
    std::vector<std::shared_ptr<T>> resources;
    struct ResourceHandle{
	ResourceHandle()
	    :ptr(ptr){};
	std::weak_ptr<T> ptr;
    };
public:
    template <typename... Args>
    std::expected<ResourceHandle, std::string> new_res(Args&&... args){
	std::expected<std::shared_ptr<T>, std::string> constructed = T::try_from(args...);
	if(!constructed.has_value())
	    return std::unexpected(constructed.error());
	resources.push_back(std::move(constructed.value()));
	return ResourceHandle(std::weak_ptr<T>(constructed.value()));
    }
    void del_res(ResourceHandle &handle){
	auto founded = std::find_if(resources.begin(), resources.end(),
	[handle](std::shared_ptr<T>& v){
	    return v.get() == handle.ptr;
	});
	if(resources.end() != founded){
	    resources.erase(founded);	
	};
    };
};

struct Renderer{
};
*/

