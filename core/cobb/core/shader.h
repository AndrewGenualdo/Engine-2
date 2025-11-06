//
// Created by andrew.genualdo on 9/10/2024.
//
#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>
#include "glm/fwd.hpp"

#include "../../ew/external/glad.h"

namespace cobb {
    class Shader {
    public:

        std::map<std::string, GLint> uniformCache = std::map<std::string, GLint>();

        unsigned int ID; //the shader ID

        Shader();
        Shader(std::string shaderPath);
        Shader(std::string vertexPath, std::string fragmentPath);

        //"activate" the shader to whatever you draw until a different shader is activated? I think
        void use() const;
        [[nodiscard]] GLint getUniformLocation(const std::string &name);


        //set uniforms for inside the shaders
        void setBool(const std::string &name, bool value);
        void setInt(const std::string &name, int value);
        void set8BitUnsignedIntArray(const std::string &name, const unsigned int *value, int size);
        void setFloat(const std::string &name, float value);
        void setVec2(const std::string &name, const glm::vec2 &value);
        void setVec3(const std::string &name, const glm::vec3 &value);
        void setVec4(const std::string &name, const glm::vec4 &value);
        void setMat3(const std::string &name, const glm::mat3 &value);
        void setMat4(const std::string &name, const glm::mat4 &value);

    private:
        void load(std::string vertexPath, std::string fragmentPath);

    };
}

#endif