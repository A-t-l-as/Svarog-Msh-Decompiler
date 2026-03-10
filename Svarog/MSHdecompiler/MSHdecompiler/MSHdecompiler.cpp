

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <cmath>

using namespace std;

#define M_PI 3.14159265358979323846


struct matrix {

    float x;
    float y;
    float z;

};


struct obrot {

    float t[3][3]{};

};


matrix Obroc(matrix wsp_in, obrot o_os_T)
{
    matrix wsp_obrocone{};

    wsp_obrocone.x = o_os_T.t[0][0] * wsp_in.x + o_os_T.t[0][1] * wsp_in.y + o_os_T.t[0][2] * wsp_in.z; 
    wsp_obrocone.y = o_os_T.t[1][0] * wsp_in.x + o_os_T.t[1][1] * wsp_in.y + o_os_T.t[1][2] * wsp_in.z; 
    wsp_obrocone.z = o_os_T.t[2][0] * wsp_in.x + o_os_T.t[2][1] * wsp_in.y + o_os_T.t[2][2] * wsp_in.z; 

    return wsp_obrocone;

}

string RotationAnswer = "";
obrot obrot_o_os;

class MSH {

    public:

        uint8_t HeaderCheckingTab[4]{};

        string InputFileName;
        string SubName;

        streamoff DlugoscPliku = 0;

        string texture;



        string Format = "";

        uint32_t Byte = 0;

        uint16_t ShortVar = 0;


        float FloatBonusByte = 0;

        uint32_t NumberOfMeshes = 1;
        uint32_t NumberOfVertex = 0;
        uint32_t NumberOfNormals = 0;

        uint32_t NumberOfFaces = 0;

        vector <unsigned char> VertexData;
        vector <unsigned char> NormalsData;
        vector <unsigned char> UVData;

        matrix Vertex {.0,.0,.0};
        matrix NormalVertex { .0,.0,.0 };
        float UVVertex[2]{};
        uint16_t Face[3]{};


        fstream InputMSH;
        fstream AdditData;

        fstream OutputOBJ;
        
        

        //**pobor nazwy od uzytkownika
        void GetFileName()
        {
            cout << "Podaj nazwe pliku Msh:" << endl;
            cin >> InputFileName;
        }
        //***************************

        void MSHOpen()
        {
            InputMSH.open(InputFileName, ios::in | ios::binary);

            if (InputMSH.good() == false)
            {
                cout << "Podany plik .MSH nie istnieje!" << endl;
                exit(0);
            }
            else
            {
                cout << "Udalo sie otworzyc plik .MSH!" << endl;

                InputMSH.seekg(0, ios::end);
                DlugoscPliku = InputMSH.tellg();
                InputMSH.seekg(0, ios::beg);
                cout << "Dlugosc Pliku = " << DlugoscPliku << endl;
            }

        }


        void CreateSubName()
        {
            SubName = InputFileName.substr(0, InputFileName.length() - 4);
        }


        void MSHCreateFolder()
        {
            filesystem::create_directory(SubName);
        }



        void ShowMeSomething(int ile)
        {
            
            for (int i = 0; i < ile; i++)
            {
                printf_s("%02X ", InputMSH.get());
            }


        }


        void OpenAdditionalDataFile()
        {
            AdditData.open(SubName + "\\" + SubName + "_ExtraData.svarog", ios::out);

        }

        void WriteaComment(string comment)
        {
            AdditData << "//";
            for (int i = 0; i < 10; i++) { AdditData << "*"; }
            AdditData << comment;
            for (int i = 0; i < 10; i++) { AdditData << "*"; }
            AdditData << endl;

        }

        void CheckHeaderAndWrite()
        {
            for (int i = 0; i < 4; i++)
            {
                HeaderCheckingTab[i] = InputMSH.get();
                AdditData << "Header_main_flag_" << i << " = " << (int)HeaderCheckingTab[i] << ";" << endl;
            }

            if (HeaderCheckingTab[3] == 0x30)
            {
                cout << "Rozpoznano nieobslugiwany plik particlesa..." << endl;
                cout << "Blad - przerwano prace programu!" << endl;
                InputMSH.close();
                AdditData.close();
                exit(0);
            }

        }


        void GetBytesAndWrite(int ile, string nazwa)
        {
            for (int i = 0; i < ile; i++)
            {
                AdditData << nazwa << i << " = " << InputMSH.get() << ";" << endl;
            }

        }


        void GetSimpleDataAndWrite(int ile,string nazwa)
        {
            for (int i = 0; i < (ile / 4); i++)
            {
                InputMSH.read(reinterpret_cast<char*>(&Byte), sizeof(Byte));
                AdditData << nazwa << i << " = " << Byte << ";" << endl;
            }

        }

        void GetBonusFloatAndWrite(int ile, string nazwa)
        {
            for (int i = 0; i < (ile / 4); i++)
            {
                InputMSH.read(reinterpret_cast<char*>(&FloatBonusByte), sizeof(FloatBonusByte));
                AdditData << nazwa << i << " = " << fixed << setprecision(4) << FloatBonusByte << ";" << endl;
            }

        }

        void GetShortDataAndWrite(int ile, string nazwa)
        {
            for (int i = 0; i < (ile / 2); i++)
            {
                InputMSH.read(reinterpret_cast<char*>(&ShortVar), sizeof(ShortVar));
                AdditData << nazwa << i << " = " << ShortVar << ";" << endl;
            }

        }


        void GetShortDataAndWriteSpecial(int ile, string nazwa)
        {
            for (int i = 0; i < (ile / 2); i++)
            {
                InputMSH.read(reinterpret_cast<char*>(&ShortVar), sizeof(ShortVar));
                AdditData << nazwa << " = " << ShortVar << ";" << endl;
            }

        }



        void WriteDataToAddi(string nazwa,uint32_t var)
        {
            AdditData << nazwa << " = "<< var <<";" << endl;
        }

        void GetFileFormat()
        {
            for(int i = 0; i<4;i++)
            Format += InputMSH.get();

            cout << "Format = " << Format << endl;
            AdditData << "Format = \"" << Format << "\";" << endl;
        }

        

        void GetVariable(uint32_t &variable)
        {
            InputMSH.read(reinterpret_cast<char*>(&variable), sizeof(variable));
        }


        string GetString()
        {
            uint32_t sizeofstring = 0;
            string SimpleString = "";

            while (sizeofstring == 0)
            {
                GetVariable(sizeofstring);

                for (int i = 0; i < (int)sizeofstring; i++)
                    SimpleString += InputMSH.get();

                if (sizeofstring == 0)
                {
                    AdditData << "StringSeparator = " << sizeofstring << ";" << endl;

                }

            }
            return SimpleString;
        }

        void PutString(string Name,string MojString)
        {
            AdditData << Name << " = \"" << MojString << "\";" << endl;

        }

        
        void ReadMSHchunk()
        {
            for (int i = 0; i < 0xC; i++)
            {
                VertexData.push_back(InputMSH.get());
            }

            for (int i = 0; i < 0xC; i++)
            {
                NormalsData.push_back(InputMSH.get());
            }

            GetBonusFloatAndWrite(0x24, "MatrixBonusData");
            
            for (int i = 0; i < 0x8; i++)
            {
                UVData.push_back(InputMSH.get());
            }

            GetSimpleDataAndWrite(0x4, "MatrixSeparatorValue");
            GetShortDataAndWrite(0x8, "MatrixIndexes");

        }

        void CleanVectors()
        {
            VertexData.clear();
            NormalsData.clear();
            UVData.clear();

        }


        void CreateOBJ(string OutputName)
        {
            OutputOBJ.open(OutputName,ios::out);
            OutputOBJ.close();

        }

        void OpenOBJ(string OutputName)
        {
            OutputOBJ.open(OutputName, ios::out|ios::app);
           

        }

        void GetFace()
        {
            InputMSH.read(reinterpret_cast<char*>(&Face[0]), sizeof(uint16_t));
            InputMSH.read(reinterpret_cast<char*>(&Face[1]), sizeof(uint16_t));
            InputMSH.read(reinterpret_cast<char*>(&Face[2]), sizeof(uint16_t));
            
            //ja tu tylko czytam XD
        }

        void GetMatrix(matrix &macierz, vector<unsigned char> wektor,int index)
        {
            

            macierz.x = (reinterpret_cast<float&>(wektor[index]));
           

            macierz.y = (reinterpret_cast<float&>(wektor[index +4]));
           

            macierz.z = (reinterpret_cast<float&>(wektor[index + 8]));
           

        }

        void GetTabValues(float tab[2], vector<unsigned char> wektor, int index)
        {

            tab[0] = (reinterpret_cast<float&>(wektor[index]));

            tab[1] = (reinterpret_cast<float&>(wektor[index + 4]));

        }


        void WriteTabToObj(float tab[2], string V)
        {
            OutputOBJ << V << "  " << fixed << setprecision(4) << tab[0] << " " << (1-tab[1]) << endl;
        }



        void WriteInfoToOBJ(string info)
        {
            OutputOBJ << info << endl;

        }

        void WriteMatrixToObj(matrix macierz,string V)
        {
            if (isnan(macierz.x) == true) { macierz.x = 0.0; }
            if (isnan(macierz.y) == true) { macierz.y = 0.0; }
            if (isnan(macierz.z) == true) { macierz.z = 0.0; }

            macierz.y = ((macierz.y) * (-1));

            if (RotationAnswer == "y")
            {
                macierz = Obroc(macierz, obrot_o_os);
            }

            OutputOBJ << V << "  " << fixed << setprecision(4) << macierz.x << " " << macierz.y << " " << macierz.z << endl;
        }


        void WriteFaceToObj()
        {
            OutputOBJ << "f  " << Face[0] + 1 << "/" << Face[0] + 1 << "/" << Face[0] + 1; OutputOBJ << " ";
         
            OutputOBJ << Face[1] + 1 << "/" << Face[1] + 1 << "/" << Face[1] + 1; OutputOBJ << " ";
            
            OutputOBJ << Face[2] + 1 << "/" << Face[2] + 1 << "/" << Face[2] + 1 << endl;
          
        }
        

        void CloseOBJ()
        {
            OutputOBJ.close();

        }

        void MSHClose()
        {
            InputMSH.close();

        }

        void CloseAdditionalDataFile()
        {
            AdditData.close();
        }

};


void ShowIntro()
{
    cout << "   _______      __     _____   ____   _____  " << endl;
    cout << "  / ____\\ \\    / /\\   |  __ \\ / __ \\ / ____| " << endl;
    cout << " | (___  \\ \\  / /  \\  | |__) | |  | | |  __  " << endl;
    cout << "  \\___ \\  \\ \\/ / /\\ \\ |  _  /| |  | | | |_ | " << endl;
    cout << "  ____) |  \\  / ____ \\| | \\ \\| |__| | |__| | " << endl;
    cout << " |_____/    \\/_/    \\_\\_|  \\_\\\_____/ \\_____| " << endl;
    cout << "*********************************************" << endl;
    cout << "**   A MSH->OBJ CONVERTER BY ATLAS 2024    **" << endl;
    cout << "*********************************************" << endl;

}



int main(int argc, char* argv[])
{
    

    string RotationInf = "";
    
    string ARGVrotationargument = "";
    
    float podana = 0.0;
    float theta = 0.0;

    streamoff reszta = 0;

    
    string answer = "";

    string secondargument = "";

    string PomocniczaNazwa = "";

    MSH mymsh;

    if (argc == 6)
    {
        mymsh.InputFileName = *(argv + 1);

        ARGVrotationargument = *(argv + 2);

        if(ARGVrotationargument == "r")
        {
            RotationAnswer = "y";
        }
        else
        {
            RotationAnswer = "n";
        }


        RotationInf = *(argv + 3);

        podana = stof(*(argv + 4));

        secondargument = *(argv + 5);
        

        if (secondargument == "float")
        {
            answer = "y";
        }

        if (secondargument != "float")
        {
            answer = "n";
        }




    }
    else
    {
        ShowIntro();
        mymsh.GetFileName();
        
        cout << "Obrocic model?[y/n]" << endl;

        do {
            cin >> RotationAnswer;

        } while ((RotationAnswer != "y") && (RotationAnswer != "n"));

        if (RotationAnswer == "y")
        {
            cout << "Chcesz obrocic model o os x, y czy z?[x,y,z]" << endl;

            do {
                cin >> RotationInf;

            } while ((RotationInf != "x") && (RotationInf != "y") && (RotationInf != "z"));

            cout << "Podaj wartosc w ulamku:" << endl;
            cout << "(Wprowadz ulamek wzgledem obrotu o 90 stopni)" << endl;

            cin >> podana;

        }


        if (RotationAnswer == "n")
        {

        }


        cout << "Czy koncowy blok danych po faceach ma byc typu float[y/n]?" << endl;

        do {
            cin >> answer;

        } while ((answer != "y") && (answer != "n"));

    }
    

    if(RotationAnswer == "y")
    { 
        theta = podana * ((float)M_PI / 2);

        if (RotationInf == "x") //obrot wzgl osi x
        {
            
            obrot_o_os.t[0][0] = 1.0; obrot_o_os.t[0][1] = 0.0;        obrot_o_os.t[0][2] = 0.0; 
            obrot_o_os.t[1][0] = 0.0; obrot_o_os.t[1][1] = cos(theta); obrot_o_os.t[1][2] = (-1) * (sin(theta)); 
            obrot_o_os.t[2][0] = 0.0; obrot_o_os.t[2][1] = sin(theta); obrot_o_os.t[2][2] = cos(theta); 

        }

        if (RotationInf == "y") //obrot wzgl osi y
        {
            obrot_o_os.t[0][0] = cos(theta);              obrot_o_os.t[0][1] = 0.0;        obrot_o_os.t[0][2] = sin(theta);
            obrot_o_os.t[1][0] = 0.0;                     obrot_o_os.t[1][1] = 1.0;        obrot_o_os.t[1][2] = 0.0;
            obrot_o_os.t[2][0] = ((-1) * (sin(theta)));   obrot_o_os.t[2][1] = 0.0;        obrot_o_os.t[2][2] = cos(theta);
        }

        if (RotationInf == "z") //obrot wzgl osi z
        {
       
            obrot_o_os.t[0][0] = cos(theta);            obrot_o_os.t[0][1] = ((-1) * (sin(theta)));      obrot_o_os.t[0][2] = 0.0;
            obrot_o_os.t[1][0] = sin(theta);            obrot_o_os.t[1][1] = cos(theta);                 obrot_o_os.t[1][2] = 0.0;
            obrot_o_os.t[2][0] = 0.0;                   obrot_o_os.t[2][1] = 0.0;                        obrot_o_os.t[2][2] = 1.0;

        }
    
    }




    mymsh.MSHOpen();

    mymsh.CreateSubName();

    mymsh.MSHCreateFolder();

    mymsh.OpenAdditionalDataFile();


    mymsh.WriteaComment("Header");

    mymsh.CheckHeaderAndWrite();
    mymsh.GetSimpleDataAndWrite(0x10, "Headerflag");
    mymsh.GetFileFormat();


    mymsh.WriteaComment("DataBlock");
    mymsh.GetSimpleDataAndWrite(0xFBC, "DataVar");

    //----------------------------------------------
    mymsh.WriteaComment("MainInformations");
    //--------------------------------------------
    mymsh.GetVariable(mymsh.NumberOfMeshes);
    mymsh.WriteDataToAddi("Numberofmeshes", mymsh.NumberOfMeshes);
    cout << "Number of meshes = " << mymsh.NumberOfMeshes << endl;

    //petla
    for (int h = 0; h < (int)mymsh.NumberOfMeshes; h++)
    {
        cout << "\n";
        cout << "Model nr = " << h << endl;
        cout << "***************" << endl;

        //----------------------------------------------
        mymsh.WriteaComment(mymsh.SubName + "_" + to_string(h) + ".obj");
        //--------------------------------------------



        mymsh.GetVariable(mymsh.NumberOfVertex);
        mymsh.WriteDataToAddi("Numberofvertex", mymsh.NumberOfVertex);
        cout << "Number of vertex = " << mymsh.NumberOfVertex << endl;

        mymsh.GetVariable(mymsh.NumberOfNormals);
        mymsh.WriteDataToAddi("Numberofnormals", mymsh.NumberOfNormals);
        cout << "Number of normals = " << mymsh.NumberOfNormals << endl;

        for (size_t j = 0; j < mymsh.NumberOfVertex; j++)
        {
            mymsh.WriteaComment("Matrix_" + to_string(j));
            mymsh.ReadMSHchunk();
        }



        PomocniczaNazwa = mymsh.SubName + "\\" + mymsh.SubName + "_" + to_string(h) + ".obj";

        mymsh.CreateOBJ(PomocniczaNazwa);

        mymsh.OpenOBJ(PomocniczaNazwa);

        mymsh.WriteInfoToOBJ("# Svarog");
        mymsh.WriteInfoToOBJ("# OBJ Model " + mymsh.SubName + "_" + to_string(h) + ".msh");
        mymsh.WriteInfoToOBJ("");

        mymsh.WriteInfoToOBJ("# Vertices " + to_string(mymsh.NumberOfVertex));

        for (int j = 0; j < mymsh.VertexData.size(); j += 12)
        {
            mymsh.GetMatrix(mymsh.Vertex, mymsh.VertexData, j);
            mymsh.WriteMatrixToObj(mymsh.Vertex, "v");
        }

        mymsh.WriteInfoToOBJ("");
        mymsh.WriteInfoToOBJ("# UVs " + to_string(mymsh.NumberOfVertex));

        for (int j = 0; j < mymsh.UVData.size(); j += 8)
        {
            mymsh.GetTabValues(mymsh.UVVertex, mymsh.UVData, j);
            mymsh.WriteTabToObj(mymsh.UVVertex, "vt");
        }


        mymsh.WriteInfoToOBJ("");
        mymsh.WriteInfoToOBJ("# Normals " + to_string(mymsh.NumberOfNormals));

        for (int j = 0; j < mymsh.NormalsData.size(); j += 12)
        {
            mymsh.GetMatrix(mymsh.NormalVertex, mymsh.NormalsData, j);
            mymsh.WriteMatrixToObj(mymsh.NormalVertex, "vn");
        }




        mymsh.CloseOBJ();

        mymsh.CleanVectors();

    }


    mymsh.WriteaComment("TexturesData");

    mymsh.texture = mymsh.GetString();

    cout << endl;
    cout << "Texture Name = " << mymsh.texture << endl;
    mymsh.PutString("TextureName", mymsh.texture);




    

    for(int h = 0;h<(int)mymsh.NumberOfMeshes;h++)
    {

        cout << "\n";
        cout << "Model nr = " << h << endl;
        cout << "***************" << endl;

    PomocniczaNazwa = mymsh.SubName + "\\" + mymsh.SubName + "_" + to_string(h) + ".obj";

    mymsh.OpenOBJ(PomocniczaNazwa);

    mymsh.GetVariable(mymsh.NumberOfFaces);
    

    cout << "Number of Faces = " << mymsh.NumberOfFaces << endl;
    
    mymsh.WriteaComment(mymsh.SubName + "_" + to_string(h) + "_" + "FaceData");
    mymsh.WriteDataToAddi("NumberofFaces", mymsh.NumberOfFaces);


    mymsh.WriteInfoToOBJ("");
    mymsh.WriteInfoToOBJ("# Faces " + to_string(mymsh.NumberOfFaces));


    for (int j = 0; j < (int)mymsh.NumberOfFaces; j++)
    {
        mymsh.GetFace();
        mymsh.GetShortDataAndWriteSpecial(0x2, "FaceSeparator_" + to_string(j));
        mymsh.WriteFaceToObj();
    }

    mymsh.CloseOBJ();

    }

    
    
    

    mymsh.WriteaComment("EndDataBlock");

    reszta = (mymsh.DlugoscPliku - mymsh.InputMSH.tellg()) % 4;

    //cout << mymsh.DlugoscPliku - mymsh.InputMSH.tellg() << endl;

    if (answer == "n")
    {
        mymsh.GetSimpleDataAndWrite((int)(mymsh.DlugoscPliku - mymsh.InputMSH.tellg()), "EndVariable");
        
    }

    if(answer == "y")
    {
        mymsh.GetBonusFloatAndWrite((int)(mymsh.DlugoscPliku - mymsh.InputMSH.tellg()), "EndVariable");
        
    }

    mymsh.GetBytesAndWrite((int)reszta, "SingleByte");
   



    mymsh.MSHClose();
    mymsh.CloseAdditionalDataFile();

    cout << endl;
    cout << mymsh.InputFileName << " ---> DONE!" << endl;


  
     



}

