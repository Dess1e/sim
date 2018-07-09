#include "Shader.h"

Shader::Shader(std::string _shader_name)
{
    this->id = 0;
    this->shader_name = _shader_name;
    // Create the shaders
    GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    Load(_shader_name, "fs.glsl", FragmentShaderID);
    Load(_shader_name, "vs.glsl", VertexShaderID);
    Load(_shader_name, "gs.glsl", GeometryShaderID);

    // Link the program
    printf("Linking shader...\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, GeometryShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, GeometryShaderID);
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(GeometryShaderID);
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    this->id = ProgramID;
}

void Shader::use()
{
    if (this->id)
    {
        glUseProgram(this->id);
    }
    else
    {
        throw ShaderUseException(this->shader_name);
    }
}

void Shader::Load(std::string ShaderName, std::string Filename, GLuint& ShaderID)
{
    // Read the Vertex Shader code from the file
    std::string ShaderCode;
    std::ifstream ShaderStream("Shaders/" + ShaderName + "/" + Filename, std::ios::in);
    if(ShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << ShaderStream.rdbuf();
        ShaderCode = sstr.str();
        ShaderStream.close();
    }
    else
    {
        throw ShaderCreateException(ShaderName, "Cant open file stream for " + Filename);
    }

    // Compile Shader
    printf("Compiling vertex shader : %s\n", shader_name.c_str());
    char const* SourcePointer = ShaderCode.c_str();
    glShaderSource(ShaderID, 1, &SourcePointer , NULL);
    glCompileShader(ShaderID);

    // Check Shader
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 )
    {
        std::vector<char> ShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        printf("%s\n", &ShaderErrorMessage[0]);
    }
}

GLuint Shader::getUniform(std::string uniform_name)
{
    return glGetUniformLocation(this->id, uniform_name.c_str());
}
