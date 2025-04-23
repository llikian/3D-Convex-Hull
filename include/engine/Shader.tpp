/***************************************************************************************************
 * @file  Shader.tpp
 * @brief Implementation of the Shader class's template methods
 **************************************************************************************************/

template<typename... Value>
void Shader::setUniform(const std::string& uniform, Value... value) {
    try {
        setUniform(uniforms.at(uniform), value...);
    } catch(const std::out_of_range&) {
        if(!unknownUniforms.contains(uniform)) {
            std::cout << "The uniform named '" << uniform << "' is unknown or unused in shader program '"
                      << name << "'.\n";

            unknownUniforms.emplace(uniform, true);
        }
    }
}
