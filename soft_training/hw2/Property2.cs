using System;
using System.Collections;

namespace hw2_Property2
{
    abstract class Shape
    {
        public abstract double Area
        {
            get;
            set;
        }
    }

    class Square : Shape
    {
        public double side;

        public Square(double s){ side = s; }

        public override double Area { 
            get { return side * side; }
            set { side = Math.Sqrt(value); }
        }
    }

    class Cube : Shape
    {
        public double side;

        public Cube(double s) { side = s; }

        public override double Area
        {
            get { return 6 * side * side; }
            set { side = Math.Sqrt(value / 6); }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Enter the side: ");
            double side = double.Parse(Console.ReadLine());

            Square s = new Square(side);
            Cube c = new Cube(side);

            Console.WriteLine("Area of the square = {0:F2}", s.Area);
            Console.WriteLine("Area of the Cube = {0:F2}", c.Area);

            Console.Write("Enter the area: ");
            double area = double.Parse(Console.ReadLine());

            s.Area = area;
            c.Area = area;

            Console.WriteLine("Side of the square = {0:F2}", s.side);
            Console.WriteLine("Side of the cube = {0:F2}", c.side);
        }
    }
}
