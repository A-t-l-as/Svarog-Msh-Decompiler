using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Svarog.MeshFiles
{
    internal class MeshFile
    {
        public MeshFile()
        {
            this.mMeshContainer = new MeshContainer();
        }


        public void LoadFrom(string argPath)
        {
            string normalizedPath = Path.GetFullPath(argPath);

            if (!File.Exists(normalizedPath))
                throw new FileNotFoundException($"Mesh file not found: {normalizedPath}");

            string ext = Path.GetExtension(normalizedPath).ToLowerInvariant();

            if (ext != ".msh")
                throw new InvalidOperationException($"Unsupported file format: {ext}");

            Utils.BinaryBuffer buffer = Utils.BinaryBuffer.LoadFromFile(normalizedPath);
            this.mMeshContainer.ReadFrom(buffer);
        }

        public void TestPrint()
        {
            Console.WriteLine(mMeshContainer.ToString());
        }

        public MeshContainer GetMeshContainer
        {
            get { return mMeshContainer; }
        }

        //-------------

        private readonly MeshContainer mMeshContainer;

    }
}
