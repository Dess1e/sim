#include "resource_loader.hpp"

ResourceLoader::ResourceLoader()
{

}

GLuint ResourceLoader::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }



    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

std::vector<Model> ResourceLoader::LoadModels(std::string file_list)
{
    std::ifstream file_stream(file_list.c_str());
    std::vector<std::string> models_paths;
    while (true)
    {
        std::string curr_str;
        if (std::getline(file_stream, curr_str))
            models_paths.push_back(curr_str);
        else
            break;
    }
    if (models_paths.empty())
    {
        fprintf(stderr, "Model list is empty, exiting...");
        quick_exit(1);
    }
    std::vector<Model> models;
    for (std::string model_path : models_paths)
    {
        //Load every and push to vec
    }
    if (models.empty())
        quick_exit(1);
    else
        return models;
}

GLuint ResourceLoader::LoadTextureBMP(const char * image_path)
{
    unsigned char header[54];
    unsigned int data_pos;
    unsigned int image_size;
    unsigned int width, height;

    unsigned char * data;

    FILE * file = fopen(image_path, "rb");
    if (!file)
    {
        std::cerr << "Can't open file" << std::endl;
        return 0;
    }
    if (fread(header, 1, 54, file) != 54)
    {
        std::cerr << "Header is not 54 bytes" << std::endl;
        return 0;
    }
    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "Header does not contain B and M bytes" << std::endl;
        return 0;
    }
    if ( *(int*)&(header[0x1E]) != 0)
    {
        std::cerr << "0x1E is not 0" << std::endl;
        return 0;
    }
    if ( *(int*)&(header[0x1C]) != 24)
    {
        std::cerr << "0x1C is not 0" << std::endl;
        return 0;
    }
    data_pos = *(int*)&(header[0x0A]);
    image_size = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if (!image_size)
        image_size = width * height * 3;
    if (!data_pos)
        data_pos = 54;
    data = new unsigned char[image_size];
    fread(data, 1, image_size, file);
    fclose(file);
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    delete [] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture_id;
}















