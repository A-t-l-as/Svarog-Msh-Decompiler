using System;
using System.Collections.Generic;
using System.IO;

using Svarog.ObjFiles;
using Svarog.MeshFiles;
using Svarog.Utils;

namespace Svarog.Apps
{
    internal class SvarogApp : IApp
    {
        public SvarogApp()
        {}

        public void Run(string[] args)
        {
            string inputFilePath = string.Empty;
            string rotationAnswer = string.Empty;
            string rotationAxis = string.Empty;
            double rotationFraction = 0.0;

            if (args.Length == 4)
            {
                inputFilePath = args[0];
                Console.WriteLine($"inputFilePath = args[0] = {args[0]}");

                string argvRotationArgument = args[1];
                Console.WriteLine($"argvRotationArgument = args[1] = {args[1]}");

                if (argvRotationArgument.Equals("r"))
                {
                    rotationAnswer = "y";
                }
                else
                {
                    rotationAnswer = "n";
                }

                rotationAxis = args[2];
                Console.WriteLine($"rotationAxis = args[2] = {args[2]}");

                rotationFraction = Double.Parse(args[3]);
                Console.WriteLine($"rotationFraction = args[3] = {args[3]}");
            }
            else
            {
                StandardProgramExecution
                (
                    ref inputFilePath, 
                    ref rotationAnswer, 
                    ref rotationAxis,
                    ref rotationFraction
                );
            }
            
            string objectNameOnly = Path.GetFileNameWithoutExtension(inputFilePath);
            string inputDir = Path.GetDirectoryName(Path.GetFullPath(inputFilePath));

            MeshFile myMeshFile = new MeshFile();
            try
            {
                myMeshFile.LoadFrom(inputFilePath);
            }
            catch(Exception e) 
                when 
                (
                   e is FileNotFoundException 
                   || e is InvalidOperationException
                   || e is FormatException
                   || e is InvalidDataException
                )
            {
                Console.WriteLine($"Exception: {e.Message}");
                return;
            }

            string subDir = Path.Combine(inputDir, objectNameOnly);
            Directory.CreateDirectory(subDir);

            List<Obj> objs = MeshUtils.ConvertToObjList(myMeshFile.GetMeshContainer);

            if (rotationAnswer.Equals("y"))
            {
                double rotationAngle = rotationFraction * (Math.PI / 2.0);

                if(
                    Enum.TryParse<Utils.MeshUtils.Axis>
                    (rotationAxis, ignoreCase: true, out Utils.MeshUtils.Axis axis) 
                    && 
                    Enum.IsDefined(typeof(Utils.MeshUtils.Axis), axis)
                  )
                {
                    ulong i = 0;
                    foreach (Obj obj in objs)
                    {
                        obj.RotateVia(axis, rotationAngle);
                        string outputPath = Path.Combine(subDir, $"{objectNameOnly}_{i++}.obj");
                        obj.ExportTo(outputPath);
                    }
                }
                else
                {
                    Console.WriteLine("Nieprawidłowa wartość axis!");
                    return;
                }

            }
            else
            {
                ulong i = 0;
                foreach (Obj obj in objs)
                {
                    string outputPath = Path.Combine(subDir, $"{objectNameOnly}_{i++}.obj");
                    obj.ExportTo(outputPath);
                }
            }
        }

        //----------------------

        private void ShowIntro()
        {
            Console.ForegroundColor = ConsoleColor.Blue;
            Console.WriteLine(  "   _______      __     _____   ____   _____  \n" + 
                                "  / ____\\ \\    / /\\   |  __ \\ / __ \\ / ____| \n" +
                                " | (___  \\ \\  / /  \\  | |__) | |  | | |  __  \n" +
                                "  \\___ \\  \\ \\/ / /\\ \\ |  _  /| |  | | | |_ | \n" +
                                "  ____) |  \\  / ____ \\| | \\ \\| |__| | |__| | \n" +
                                " |_____/    \\/_/    \\_\\_|  \\_\\_____/ \\_____| \n" +
                                "*********************************************\n" +
                                "**   A MSH->OBJ CONVERTER BY ATLAS 2026    **\n" +
                                "*********************************************");

            Console.ResetColor();
        }

        private void StandardProgramExecution
        (
            ref string argInputFilePath, 
            ref string argRotationAnswer,
            ref string argRotationAxis,
            ref double argRotationFraction
        )
        {
            ShowIntro();

            Console.WriteLine("Enter the path to the Msh file:");
            argInputFilePath = Console.ReadLine();

            Console.WriteLine("Rotate the model?[y/n]");
            GetAnswerFromUser(ref argRotationAnswer);

            GetAnswerFromUserAboutRotation
            (
                ref argRotationAnswer,
                ref argRotationAxis,
                ref argRotationFraction
            );
        }


        private void GetAnswerFromUser(ref string answer)
        {
            do
            {
                answer = Console.ReadLine();
            }
            while ( (!answer.Equals("y")) && ( !answer.Equals("n") ) );
        }

        private void GetAnswerFromUserAboutRotation
        (
            ref string argRotationAnswer,
            ref string argRotationAxis, 
            ref double argRotationFraction
        )
        {
            if ( argRotationAnswer.Equals("y") )
            {
                Console.WriteLine("Do you want to rotate the model about the x, y or z axis?[x,y,z]");

                do
                {
                    argRotationAxis = Console.ReadLine();
                } 
                while 
                ( 
                    ( !argRotationAxis.Equals("x") ) 
                    && 
                    (!argRotationAxis.Equals("y")) 
                    && 
                    (!argRotationAxis.Equals("z"))
                );

                Console.WriteLine("Specify the value in a fraction:\n" +
                                  "(Introduce a fraction of a 90-degree rotation)");

                argRotationFraction = Double.Parse(Console.ReadLine());
            }

        }
    }
}
