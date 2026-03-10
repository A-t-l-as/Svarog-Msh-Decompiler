#include "MSHdecompiler.hpp"

static wsp Rotate(wsp& wsp_in, rotation_matrix& rot_matrix_t)
{
    wsp wsp_obrocone = {};

    wsp_obrocone.m_x = rot_matrix_t.m_t[0][0] * wsp_in.m_x + rot_matrix_t.m_t[0][1] * wsp_in.m_y + rot_matrix_t.m_t[0][2] * wsp_in.m_z;
    wsp_obrocone.m_y = rot_matrix_t.m_t[1][0] * wsp_in.m_x + rot_matrix_t.m_t[1][1] * wsp_in.m_y + rot_matrix_t.m_t[1][2] * wsp_in.m_z;
    wsp_obrocone.m_z = rot_matrix_t.m_t[2][0] * wsp_in.m_x + rot_matrix_t.m_t[2][1] * wsp_in.m_y + rot_matrix_t.m_t[2][2] * wsp_in.m_z;

    return wsp_obrocone;
}

//------------------------
// Input File Buffer Class
//------------------------

InputBinFile::InputBinFile(char* argfilename) : _file_name(argfilename), _is_opened(false), _is_open_now(false),
_file_length(0), _file_buffer(nullptr)
{
    DEBUG_PRINT("Nazwa pliku:");
    DEBUG_PRINT(_file_name);
    DEBUG_PRINT("\n");

    OpenFile();

    if (_is_open_now == true)
    {
        MeasureFile();
        PutFileToBuffer();
        _input_file.close();
        _is_open_now = _input_file.is_open();

    }
}

void InputBinFile::OpenFile()
{
    _input_file.open(_file_name, std::ios::in | std::ios::binary);

    _is_opened = _input_file.is_open();
    _is_open_now = _is_opened;

}

void InputBinFile::MeasureFile()
{
    _input_file.seekg(0, std::ios_base::end); // Przeskakuje na koniec pliku.
    _file_length = _input_file.tellg(); // Pobieram koncowa pozycje.
    _input_file.seekg(0, std::ios_base::beg); // Wracam na poczatek pliku.
}


void InputBinFile::PutFileToBuffer()
{
    _file_buffer = new uint8_t[_file_length];
    _input_file.read(reinterpret_cast<char*>(_file_buffer), _file_length);

}

bool InputBinFile::GetIsOpened() const { return _is_opened; }
bool InputBinFile::GetIsOpenNow() const { return _is_open_now; }
uint64_t InputBinFile::GetFileLength() const { return _file_length; }

InputBinFile::~InputBinFile()
{
    if (_is_open_now == false && _is_opened == true)
    {
        delete[] _file_buffer;
    }
}

void InputBinFile::Informations(std::ostream& o) const
{
    o << std::endl
        << "-------------------------------------" << std::endl
        << "Opening the file:" << _file_name << std::endl
        << "Was the file successfully opened?: " << _is_opened << std::endl
        << "Is the file currently open?: " << _is_open_now << std::endl
        << "-------------------------------------" << std::endl
        << std::endl;
}


void InputBinFile::ExitWhenFileDontExist(std::ostream& o) const
{
    if (_is_opened == false && _is_open_now == false)
    {
        o << "The specified file does not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------------------------

//------------------
// MIS BINARY UTILS
//------------------

BinaryUtils::BinaryUtils(uint8_t* argfilebuffer, uint64_t argfilelength) : _file_buffer(argfilebuffer),
_current_offset(0), _line(std::string()), _file_length(argfilelength)
{
}

void BinaryUtils::WriteSTRING(std::string name, int quadcount, std::string& text , std::stringstream& output)
{
	WriteQuad('\t', output, quadcount);
	output << "string " << name << " = \"" << text << "\";" << std::endl;
}

template<typename T>
void BinaryUtils::WriteUINT(uint32_t mode, std::string name, int quadcount, std::stringstream& output, uint64_t count)
{
	for (uint64_t i = 0; i < count; ++i)
	{
		WriteQuad('\t', output, quadcount);
		output << "uint" << mode << "_t " << name << ((count == 1) ? ("") : (std::to_string(i))) << " = ";

		if (mode == 8)
		{
			output << static_cast<uint32_t>(GetVALUE<T>());
		}
		else
		{
			output << GetVALUE<T>();
		}

		output << ";" << std::endl;
	}
}

template<typename T>
void BinaryUtils::WriteFLOATINGPOINT(uint32_t mode, std::string name, int quadcount, std::stringstream& output, uint64_t count)
{
    for (uint64_t i = 0; i < count; ++i)
    {
        WriteQuad('\t', output, quadcount);
        
        if (mode == 32)
        {
            output << "float";
        }

        if (mode == 64)
        {
            output << "double";
        }

        output << " " << name << ((count == 1) ? ("") : (std::to_string(i))) << " = "
         << GetVALUE<T>()
         << ";" << std::endl;
    }
}

void BinaryUtils::WriteGUID(std::string name, int quadcount, std::stringstream& output)
{
	WriteQuad('\t', output, quadcount);
	output << "GUID " << name << " = " << GetGUID() << ";" << std::endl;
}

void BinaryUtils::WriteTIME32(std::string name, int quadcount, std::stringstream& output)
{
	std::uint32_t timevalue = GetVALUE<uint32_t>();

	time_t mytime = static_cast<time_t>(timevalue);

	char buffer[64];
	std::tm timeinfo;

	WriteQuad('\t', output, quadcount);

	output << "uint32_t " << name << " = " << timevalue << "; // ";

	if (localtime_s(&timeinfo, &mytime) == 0)
	{
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
		output << buffer << std::endl;
	}
	else
	{
		output << "Incorrect time value: " << timevalue << std::endl;
	}

}

std::string BinaryUtils::GetString(std::stringstream& data)
{
    uint32_t size_of_string = 0;
    std::string simple_string = std::string();

    while (size_of_string == 0)
    {
        size_of_string = GetVALUE<uint32_t>();

        for (int i = 0; i < static_cast<int32_t>(size_of_string); ++i)
        {
            simple_string += _file_buffer[_current_offset];
            ++_current_offset;
        }

        if (size_of_string == 0)
        {
            data << "uint32_t string_separator = " << size_of_string << ";" << std::endl;
        }

    }
    return simple_string;
}

template<typename T>
T BinaryUtils::GetVALUE(void)
{
	T value = reinterpret_cast<T&>(_file_buffer[_current_offset]);
	_current_offset += sizeof(T);
	return value;
}

std::string BinaryUtils::GetGUID(void)
{
	// // // #define NumberOfUINT16Fields 2
	constexpr const int static NumberOfUINT16Fields = 2;

	std::string myGUID = std::string();

	myGUID += "{" + std::format("{:08X}", GetVALUE<uint32_t>());

	for (int i = 0; i < NumberOfUINT16Fields; ++i)
		myGUID += "-" + std::format("{:04X}", GetVALUE<uint16_t>());

	myGUID += "-" + std::format("{:04X}", _byteswap_ushort(GetVALUE<uint16_t>())) + "-";
	myGUID += std::format("{:08X}", _byteswap_ulong(GetVALUE<uint32_t>()));
	myGUID += std::format("{:04X}", _byteswap_ushort(GetVALUE<uint16_t>())) + "}";

	return myGUID;
}

//gettery
uint64_t BinaryUtils::GetActualOffset() const { return _current_offset; }
uint64_t BinaryUtils::GetFileLength() const { return _file_length;  }

//---------------------------------------------------------------

// klasa MSH :

void Msh::ArgcArgvExecution(char* my_argv[])
{
    m_input_file_name = *(my_argv + 1);

    m_argv_rotation_argument = *(my_argv + 2);

    if (m_argv_rotation_argument == "r")
        m_rotation_answer = "y";
    else
        m_rotation_answer = "n";
    

    m_rotation_inf = *(my_argv + 3);

    m_rotation_fraction = std::stof(*(my_argv + 4));

    m_second_argument = *(my_argv + 5);


    if (m_second_argument == "float")
        m_answer = "y";
    else
        m_answer = "n";
    
}

void Msh::GetAnswerFromUser(std::string& answer)
{
    do
    {
        m_in_stream >> answer;

    }
    while ((answer != "y") && (answer != "n"));
}

void Msh::GetAnswerFromUserAboutRotation()
{
    if (m_rotation_answer == "y")
    {
        m_o << "Do you want to rotate the model about the x, y or z axis?[x,y,z]" << std::endl;

        do
        {
            m_in_stream >> m_rotation_inf;
        } while ((m_rotation_inf != "x") && (m_rotation_inf != "y") && (m_rotation_inf != "z"));

        m_o << "Specify the value in a fraction:" << std::endl
            << "(Introduce a fraction of a 90-degree rotation)" << std::endl;

        m_in_stream >> m_rotation_fraction;
    }

}

void Msh::StandardProgramExecution()
{
    ShowIntro();
    GetFileName();

    m_o << "Rotate the model?[y/n]" << std::endl;
    GetAnswerFromUser(m_rotation_answer);

    GetAnswerFromUserAboutRotation();

    m_o << "Should the final data block after faces be of type float[y/n]?" << std::endl;
    GetAnswerFromUser(m_answer);

}

void Msh::InitializeRotationMatrix()
{
    m_theta = m_rotation_fraction * (static_cast<float>(my_m_pi / 2.0) );

    if (m_rotation_inf == "x") //obrot wzgl osi x
    {
        m_rotation_axis.m_t[0][0] = 1.0; m_rotation_axis.m_t[0][1] = 0.0;           m_rotation_axis.m_t[0][2] = 0.0;
        m_rotation_axis.m_t[1][0] = 0.0; m_rotation_axis.m_t[1][1] = cos(m_theta);  m_rotation_axis.m_t[1][2] = (-1) * (sin(m_theta));
        m_rotation_axis.m_t[2][0] = 0.0; m_rotation_axis.m_t[2][1] = sin(m_theta);  m_rotation_axis.m_t[2][2] = cos(m_theta);
    }

    if (m_rotation_inf == "y") //obrot wzgl osi y
    {
        m_rotation_axis.m_t[0][0] = cos(m_theta);               m_rotation_axis.m_t[0][1] = 0.0;      m_rotation_axis.m_t[0][2] = sin(m_theta);
        m_rotation_axis.m_t[1][0] = 0.0;                        m_rotation_axis.m_t[1][1] = 1.0;      m_rotation_axis.m_t[1][2] = 0.0;
        m_rotation_axis.m_t[2][0] = ((-1) * (sin(m_theta)));    m_rotation_axis.m_t[2][1] = 0.0;      m_rotation_axis.m_t[2][2] = cos(m_theta);
    }

    if (m_rotation_inf == "z") //obrot wzgl osi z
    {
        m_rotation_axis.m_t[0][0] = cos(m_theta);  m_rotation_axis.m_t[0][1] = ((-1) * (sin(m_theta)));  m_rotation_axis.m_t[0][2] = 0.0;
        m_rotation_axis.m_t[1][0] = sin(m_theta);  m_rotation_axis.m_t[1][1] = cos(m_theta);             m_rotation_axis.m_t[1][2] = 0.0;
        m_rotation_axis.m_t[2][0] = 0.0;           m_rotation_axis.m_t[2][1] = 0.0;                      m_rotation_axis.m_t[2][2] = 1.0;
    }

}

void Msh::CheckIsThisParticles()
{
    if (m_header_checking_tab[3] == 0x30)
    {
        m_o << "An unsupported particles file was recognized...." << std::endl
            << "Error - the work of the program has been interrupted!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Msh::ReadAndWriteHeader()
{

    WriteAComment("Header");

    memcpy(m_header_checking_tab, &msh_file_util._file_buffer[msh_file_util._current_offset], sizeof(uint32_t));
    msh_file_util._current_offset += sizeof(uint32_t);


    for (int i = 0; i < c_size_of_header_checking_tab; i++)
        WriteUINTToAddi<uint8_t>(8, "header_main_flag_" + std::to_string(i), 0, m_header_checking_tab[i]);

    CheckIsThisParticles();

    msh_file_util.WriteGUID("msh_guid", 0, m_addit_file_stream);

    msh_file_util.WriteUINT<uint8_t>(8, "file_format", 0, m_addit_file_stream, sizeof(uint32_t));

    WriteAComment("DataBlock");

    msh_file_util.WriteUINT<uint32_t>(32, "data_var", 0, m_addit_file_stream, 0xFBC / sizeof(uint32_t));

    //----------------------------------------------
    WriteAComment("MainInformations");
    //--------------------------------------------

    m_number_of_meshes = msh_file_util.GetVALUE<uint32_t>();

    WriteUINTToAddi<uint32_t>(32, "number_of_meshes", 0, m_number_of_meshes);
    m_o << "number of meshes = " << m_number_of_meshes << std::endl;

    if (m_number_of_meshes > 32767)
    {
        m_o << "The number of meshes is too high. I terminate the program for safety reasons..." << std::endl;
        exit(EXIT_FAILURE);
    }

}

void Msh::GetAndWrtieFirstChunksOfMsh()
{

    for (int h = 0; h < static_cast<int32_t>(m_number_of_meshes); ++h)
    {
        m_o << std::endl
            << "Model no. = " << h << std::endl
            << "***************" << std::endl;

        //----------------------------------------------
        WriteAComment(m_sub_name + "_" + std::to_string(h) + ".obj");
        //--------------------------------------------

        m_number_of_vertex = msh_file_util.GetVALUE<uint32_t>();
        WriteUINTToAddi<uint32_t>(32, "number_of_vertex", 0, m_number_of_vertex);
        m_o << "number of vertex = " << m_number_of_vertex << std::endl;

        m_number_of_normals = msh_file_util.GetVALUE<uint32_t>();
        WriteUINTToAddi<uint32_t>(32, "number_of_normals", 0, m_number_of_normals);
        m_o << "number of normals = " << m_number_of_normals << std::endl;

        for (size_t j = 0; j < m_number_of_vertex; ++j)
        {
            WriteAComment("Matrix_" + std::to_string(j));
            ReadMshChunk();
        }

        m_output_obj_file_name = m_sub_name + "\\" + m_sub_name + "_" + std::to_string(h) + ".obj";

        WriteInfoToObj("# Svarog");
        WriteInfoToObj("# OBJ Model " + m_sub_name + "_" + std::to_string(h) + ".msh");
        WriteInfoToObj("");
        WriteInfoToObj("# Vertices " + std::to_string(m_number_of_vertex));

        for (int j = 0; j < m_vertex_data.size(); j += 3 * sizeof(float))
        {
            GetMatrix(m_vertex, m_vertex_data, j);
            WriteMatrixToObj(m_vertex, "v");
        }

        WriteInfoToObj("");
        WriteInfoToObj("# UVs " + std::to_string(m_number_of_vertex));

        for (int j = 0; j < m_uvdata.size(); j += 2 * sizeof(float))
        {
            GetTabValues(m_uvvertex, m_uvdata, j);
            WriteTabToObj(m_uvvertex, "vt");
        }

        WriteInfoToObj("");
        WriteInfoToObj("# Normals " + std::to_string(m_number_of_normals));

        for (int j = 0; j < m_normals_data.size(); j += 3 * sizeof(float))
        {
            GetMatrix(m_normal_vertex, m_normals_data, j);
            WriteMatrixToObj(m_normal_vertex, "vn");
        }

        SaveAndCloseFile(m_output_obj_file, m_output_obj_file_name, std::ios::out, m_output_obj_file_stream);
        CleanVectors();
        m_output_obj_file_stream = std::stringstream();
    }

}

void Msh::GetAndWriteTextureString()
{
    WriteAComment("TexturesData");

    m_texture = msh_file_util.GetString(m_addit_file_stream);

    m_o << std::endl << "texture Name = " << m_texture << std::endl;

    msh_file_util.WriteSTRING("texture_name", 0, m_texture, m_addit_file_stream);

}

void Msh::GetAndWriteFaces()
{
    for (int h = 0; h < static_cast<int32_t>(m_number_of_meshes); ++h)
    {

        m_o << std::endl
            << "Model no. = " << h << std::endl
            << "***************" << std::endl;

        m_output_obj_file_name = m_sub_name + "\\" + m_sub_name + "_" + std::to_string(h) + ".obj";

        m_number_of_faces = msh_file_util.GetVALUE<uint32_t>();

        m_o << "number of faces = " << m_number_of_faces << std::endl;

        WriteAComment(m_sub_name + "_" + std::to_string(h) + "_" + "FaceData");

        WriteUINTToAddi<uint32_t>(32, "number_of_faces", 0, m_number_of_faces);

        WriteInfoToObj("");
        WriteInfoToObj("# Faces " + std::to_string(m_number_of_faces));

        for (int j = 0; j < static_cast<int32_t>(m_number_of_faces); ++j)
        {
            GetFace();
            msh_file_util.WriteUINT<uint16_t>(16, "face_separator_" + std::to_string(j), 0, m_addit_file_stream);
            WriteFaceToObj();
        }

        SaveAndCloseFile(m_output_obj_file, m_output_obj_file_name, std::ios::out | std::ios::app, m_output_obj_file_stream);

        m_output_obj_file_stream = std::stringstream();

    }

}

void Msh::GetAndWriteEndOfMsh()
{
    WriteAComment("EndDataBlock");

    m_rest_of_file = (msh_file_util.GetFileLength() - msh_file_util.GetActualOffset()) % sizeof(uint32_t);

    if (m_answer == "n")
    {
        msh_file_util.WriteUINT<uint32_t>(32, "end_variable", 0, m_addit_file_stream,
            (msh_file_util.GetFileLength() - msh_file_util.GetActualOffset()) / sizeof(uint32_t));

    }

    if (m_answer == "y")
    {
        msh_file_util.WriteFLOATINGPOINT<float>(32, "end_variable", 0, m_addit_file_stream,
            (msh_file_util.GetFileLength() - msh_file_util.GetActualOffset()) / sizeof(float));
    }

    msh_file_util.WriteUINT<uint8_t>(8, "single_byte", 0, m_addit_file_stream, m_rest_of_file);

}

void Msh::ReadAndWriteMshToObj()
{

    InputBinFile msh_file(const_cast<char*>(m_input_file_name.c_str()));
    msh_file.Informations(m_o);
    msh_file.ExitWhenFileDontExist(m_o);

    CreateSubName();
    MshCreateFolder();

    m_addit_file_name = m_sub_name + "\\" + m_sub_name + "_ExtraData.cpp";

    msh_file_util = BinaryUtils(msh_file._file_buffer, msh_file.GetFileLength());

    ReadAndWriteHeader();

    GetAndWrtieFirstChunksOfMsh();

    GetAndWriteTextureString();

    GetAndWriteFaces();

    GetAndWriteEndOfMsh();

    SaveAndCloseFile(m_addit_file, m_addit_file_name, std::ios::out, m_addit_file_stream);

}

Msh::Msh(std::istream& argin, std::ostream& argo, int my_argc, char* my_argv[]) :
    m_in_stream(argin), m_o(argo),

    m_rotation_inf(std::string()), m_argv_rotation_argument(std::string()), m_answer(std::string()),
    m_second_argument(std::string()), m_rotation_fraction(0.0), m_theta(0.0), m_rest_of_file(0),
    m_addit_file_stream(std::stringstream()), m_addit_file_name(std::string()), m_output_obj_file_stream(std::stringstream()),
    m_output_obj_file_name(std::string()), m_rotation_answer (std::string()), m_input_file_name(std::string()),
    m_sub_name(std::string()), m_texture(std::string()), m_number_of_meshes(1), m_number_of_vertex(0), m_number_of_normals(0),
    m_number_of_faces(0)
{

    if (my_argc == 6)
        ArgcArgvExecution(my_argv);
    else
        StandardProgramExecution();


    if (m_rotation_answer == "y") InitializeRotationMatrix();

    ReadAndWriteMshToObj();

    m_o << std::endl
     << m_input_file_name << " ---> DONE!" << std::endl;

}

void Msh::ShowIntro()
{
    m_o << "   _______      __     _____   ____   _____  " << std::endl
     << "  / ____\\ \\    / /\\   |  __ \\ / __ \\ / ____| " << std::endl
     << " | (___  \\ \\  / /  \\  | |__) | |  | | |  __  " << std::endl
     << "  \\___ \\  \\ \\/ / /\\ \\ |  _  /| |  | | | |_ | " << std::endl
     << "  ____) |  \\  / ____ \\| | \\ \\| |__| | |__| | " << std::endl
     << " |_____/    \\/_/    \\_\\_|  \\_\\\_____/ \\_____| " << std::endl
     << "*********************************************" << std::endl
     << "**   A MSH->OBJ CONVERTER BY ATLAS 2025    **" << std::endl
     << "*********************************************" << std::endl;

}


//**pobor nazwy od uzytkownika
void Msh::GetFileName()
{
    m_o << "Enter the name of the Msh file:" << std::endl;
    m_in_stream >> m_input_file_name;
}
//***************************

void Msh::CreateSubName()
{
    m_sub_name = m_input_file_name.substr(0, m_input_file_name.length() - 4);
}

void Msh::MshCreateFolder()
{
    std::filesystem::create_directory(m_sub_name);
}


void Msh::WriteAComment(std::string comment)
{
    const int len_of_comment = 10;
    m_addit_file_stream << "//";

    for (int i = 0; i < len_of_comment; i++)
        m_addit_file_stream << "*"; 
    

    m_addit_file_stream << comment;


    for (int i = 0; i < len_of_comment; i++)
        m_addit_file_stream << "*"; 
    
    m_addit_file_stream << std::endl;
}

template<typename T>
void Msh::WriteUINTToAddi(uint32_t mode, std::string name, int quadcount, T& var)
{
    m_addit_file_stream << "uint" << mode << "_t " << name << " = " << ((mode == 8) ? (static_cast<uint32_t>(var) ) : (var) ) 
        << "; " << std::endl;
}


void Msh::ReadMshChunk()
{

    for (int i = 0; i < 0xC; ++i) m_vertex_data.push_back( msh_file_util.GetVALUE<uint8_t>() );

    for (int i = 0; i < 0xC; ++i) m_normals_data.push_back(msh_file_util.GetVALUE<uint8_t>());

    msh_file_util.WriteFLOATINGPOINT<float>(32, "matrix_bonus_data",0, m_addit_file_stream, 0x24 / sizeof(float));

    for (int i = 0; i < 0x8; ++i) m_uvdata.push_back( msh_file_util.GetVALUE<uint8_t>() );
    
    msh_file_util.WriteUINT<uint32_t>(32, "matrix_separator_value", 0, m_addit_file_stream);
    
    msh_file_util.WriteUINT<uint16_t>(16, "matrix_indexes", 0, m_addit_file_stream, 0x8 / sizeof(uint16_t));

}

void Msh::CleanVectors()
{
    m_vertex_data.clear();
    m_normals_data.clear();
    m_uvdata.clear();

}


void Msh::GetFace()
{
    memcpy(m_face, &msh_file_util._file_buffer[msh_file_util._current_offset], c_size_of_face * sizeof(uint16_t));
    msh_file_util._current_offset += c_size_of_face * sizeof(uint16_t);
}

void Msh::GetMatrix(wsp& macierz, std::vector<unsigned char>& my_vector, int& index)
{
    macierz.m_x = (reinterpret_cast<float&>(my_vector[index]));
    macierz.m_y = (reinterpret_cast<float&>(my_vector[index + sizeof(float)]));
    macierz.m_z = (reinterpret_cast<float&>(my_vector[index + ( 2 * sizeof(float)) ]));
}

void Msh::GetTabValues(float tab[2], std::vector<unsigned char>& my_vector, int& index)
{
    tab[0] = (reinterpret_cast<float&>(my_vector[index]));
    tab[1] = (reinterpret_cast<float&>(my_vector[index + sizeof(float)]));
}

void Msh::WriteTabToObj(float tab[2], std::string v)
{
    m_output_obj_file_stream << v << "  " << std::fixed << std::setprecision(4) << tab[0] << " " << (1 - tab[1]) << std::endl;
}

void Msh::WriteInfoToObj(std::string info)
{
    m_output_obj_file_stream << info << std::endl;
}

void Msh::WriteMatrixToObj(wsp& matrix, const std::string& v)
{
    if (isnan(matrix.m_x) == true) matrix.m_x = 0.0; 

    if (isnan(matrix.m_y) == true) matrix.m_y = 0.0; 

    if (isnan(matrix.m_z) == true) matrix.m_z = 0.0; 

    matrix.m_y *= -1.0;

    if (m_rotation_answer == "y")
    {
        matrix = Rotate(matrix, m_rotation_axis);
    }

    m_output_obj_file_stream << v << "  " << std::fixed << std::setprecision(4) 
        << matrix.m_x << " " << matrix.m_y << " " << matrix.m_z << std::endl;
}

void Msh::WriteFaceToObj()
{
    for (int i = 0; i < c_size_of_face; ++i)
    {
        m_output_obj_file_stream << ((i == 0) ? ("f  ") : (" ")) 
         << m_face[i] + 1 << "/" << m_face[i] + 1 << "/" << m_face[i] + 1;
    }

    m_output_obj_file_stream << std::endl;
}

void Msh::SaveAndCloseFile(std::fstream& output_file, std::string& output_file_name, std::ios_base::openmode mode, std::stringstream& output_file_stream)
{
    output_file.open(output_file_name, mode);

    auto file_data(output_file_stream.str());
    output_file.write(&file_data[0], file_data.size());
    output_file.close();
}


