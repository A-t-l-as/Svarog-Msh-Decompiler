using System;

namespace Svarog.Factories
{
    internal class RotationMatrixFactory
    {
        public const int cRotationMatrixSize = 3;

        public static double[,] CreateForRotationViaX(double argAngle)
        {
            return new double[cRotationMatrixSize, cRotationMatrixSize]
            {
                { 1.0, 0.0, 0.0 },
                { 0.0, Math.Cos(argAngle), (-1.0) * ( Math.Sin(argAngle) ) },
                { 0.0, Math.Sin(argAngle), Math.Cos(argAngle) }
            };
        }

        public static double[,] CreateForRotationViaY(double argAngle)
        {
            return new double[cRotationMatrixSize, cRotationMatrixSize]
            {
                { Math.Cos(argAngle), 0.0, Math.Sin(argAngle) },
                { 0.0, 1.0, 0.0 },
                { ((-1.0) * (Math.Sin(argAngle))), 0.0, Math.Cos(argAngle) }
            };
        }

        public static double[,] CreateForRotationViaZ(double argAngle)
        {
            return new double[cRotationMatrixSize, cRotationMatrixSize]
            {
                { Math.Cos(argAngle), ((-1.0) * (Math.Sin(argAngle))),  0.0 },
                { Math.Sin(argAngle), Math.Cos(argAngle), 0.0 },
                { 0.0, 0.0, 1.0 }
            };
        }


    }
}
