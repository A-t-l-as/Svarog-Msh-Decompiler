#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <cmath>

#ifdef _DEBUG
#define DEBUG_PRINT(x) printf_s("%s",x)
#else
#define DEBUG_PRINT(x)
#endif

constexpr double my_m_pi = 3.14159265358979323846;

constexpr void WriteQuad(char symbol, std::stringstream& output, int count)
{
    for (int i = 0; i < count; ++i)
        output << symbol;
    
}

struct wsp
{
    float m_x;
    float m_y;
    float m_z;

    wsp(float arg_m_x = 0.0, float arg_m_y = 0.0, float arg_m_z = 0.0)
        : m_x(arg_m_x), m_y(arg_m_y), m_z(arg_m_z)
    {}
   
};

struct rotation_matrix
{
    constexpr const unsigned int static c_size_of_t = 3;
    float m_t[c_size_of_t][c_size_of_t] = {};
};

//---------------------
// InputBinFile CLASS
//---------------------

class InputBinFile
{
    //Pola:
    std::fstream _input_file;
    char* _file_name;
    uint64_t _file_length;
    bool _is_opened;
    bool _is_open_now;

    //Metody:
    void OpenFile();
    void MeasureFile();
    void PutFileToBuffer();

public:

    //Pola:
    uint8_t* _file_buffer;

    //Metody:
    void Informations(std::ostream& o) const;
    void ExitWhenFileDontExist(std::ostream& o) const;

    InputBinFile(char* argfilename);
    ~InputBinFile();

    //Getery:
    bool GetIsOpened() const;
    bool GetIsOpenNow() const;
    uint64_t GetFileLength() const;


};


class BinaryUtils
{
public:   
    //Pola:
    uint8_t* _file_buffer;
    uint64_t _current_offset;

    //Metody:

    // Utilities ----------------------

    std::string GetString(std::stringstream& data);

    template<typename T>
    T GetVALUE(void);

    std::string GetGUID(void);

    void WriteTIME32(std::string name, int quadcount, std::stringstream& output);

    void WriteSTRING(std::string name, int quadcount, std::string& text, std::stringstream& output);

    template<typename T>
    void WriteUINT(uint32_t mode, std::string name, int quadcount, std::stringstream& output, uint64_t count = 1);

    template<typename T>
    void WriteFLOATINGPOINT(uint32_t mode, std::string name, int quadcount, std::stringstream& output, uint64_t count = 1);

    void WriteGUID(std::string name, int quadcount, std::stringstream& output);
    
    BinaryUtils(uint8_t* argfilebuffer = nullptr, uint64_t argfilelength = 0);

    //Gettery:
    uint64_t GetActualOffset() const;
    uint64_t GetFileLength() const;

private:
    //Pola:
    std::string _line;
    uint64_t _file_length;
};



class Msh
{
public: 

    Msh(std::istream& argin, std::ostream& argo, int my_argc, char* my_argv[]);

private:

    // Pola:
    std::istream& m_in_stream;
    std::ostream& m_o;

    std::string m_rotation_inf;
    std::string m_argv_rotation_argument;
    std::string m_answer;
    std::string m_second_argument;

    float m_rotation_fraction;
    float m_theta;
    std::streamoff m_rest_of_file;

    std::stringstream m_addit_file_stream;
    std::string m_addit_file_name;
    std::fstream m_addit_file;

    BinaryUtils msh_file_util;

    std::stringstream m_output_obj_file_stream;
    std::string m_output_obj_file_name;
    std::fstream m_output_obj_file;

    constexpr const unsigned int static c_size_of_header_checking_tab = 4;
    uint8_t m_header_checking_tab[c_size_of_header_checking_tab] = {};

    std::string m_rotation_answer;
    rotation_matrix m_rotation_axis = {};

    std::string m_input_file_name;
    std::string m_sub_name;
    std::string m_texture;

    uint32_t m_number_of_meshes;
    uint32_t m_number_of_vertex;
    uint32_t m_number_of_normals;
    uint32_t m_number_of_faces;

    std::vector <unsigned char> m_vertex_data = {};
    std::vector <unsigned char> m_normals_data = {};
    std::vector <unsigned char> m_uvdata = {};

    wsp m_vertex = {};
    wsp m_normal_vertex = {};

    constexpr const unsigned int static c_size_of_uvvertex = 2;
    float m_uvvertex[c_size_of_uvvertex] = {};

    constexpr const unsigned int static c_size_of_face = 3;
    uint16_t m_face[c_size_of_face] = {};

    // Metody:

    void ArgcArgvExecution(char* my_argv[]);

    void StandardProgramExecution();
    //{
    void GetAnswerFromUser(std::string& answer);
    void GetAnswerFromUserAboutRotation();
    //}

    void GetAndWrtieFirstChunksOfMsh();

    void GetAndWriteTextureString();

    void GetAndWriteFaces();

    void GetAndWriteEndOfMsh();

    void InitializeRotationMatrix();

    void CheckIsThisParticles();


    void ReadAndWriteHeader();

    void ShowIntro();

    void GetFileName();

    void CreateSubName();

    void MshCreateFolder();

    void WriteAComment(std::string comment);

    template<typename T>
    void WriteUINTToAddi(uint32_t mode, std::string name, int quadcount, T& var);

    void ReadMshChunk();

    void CleanVectors();

    void GetFace();

    void GetMatrix(wsp& matrix, std::vector<unsigned char>& my_vector, int& index);

    void GetTabValues(float tab[2], std::vector<unsigned char>& my_vector, int& index);

    void WriteTabToObj(float tab[2], std::string v);

    void WriteInfoToObj(std::string info);

    void WriteMatrixToObj(wsp& matrix, const std::string& v);

    void WriteFaceToObj();

    void SaveAndCloseFile(std::fstream& output_file,
        std::string& output_file_name,
        std::ios_base::openmode mode,
        std::stringstream& output_file_stream);
    

    void ReadAndWriteMshToObj();
};






