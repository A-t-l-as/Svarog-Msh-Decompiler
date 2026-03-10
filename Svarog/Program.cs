using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Utils;

namespace Svarog
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Apps.SvarogApp svarog = new Apps.SvarogApp();
            svarog.Run(args);
        }
    }
}
