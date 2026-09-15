#pragma once
#include <algorithm>
#include <string>
#include <expected>
#include <vector>
#include <optional>
#include <memory>

template <typename T, typename... Args>
concept TryFromConstructor = requires(Args&&... args){
    T::try_from(std::forward<Args>(args)...);
};

/*
class ShaderProgram{
    int program;

    ShaderProgram(int program):
	program(program){};
public:
    static std::expected<ShaderProgram, std::string> try_from(int ggg){
	return ShaderProgram(ggg);
    };
};
*/

template <typename T>
//requires TryFromConstructor<T, Args...>
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
