﻿using System;
using System.Collections;

namespace hw2_Indexer
{
    class MyIndexerClass
    {
        private string[] data = new string[5];

        public int Length
        {
            get { return data.Length; }
        }

        public string this[int index]
        {
            get { return data[index]; }
            set { data[index] = value; }
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            MyIndexerClass mic = new MyIndexerClass();
            mic[0] = "Seoul"; mic[1] = "Nowon"; mic[2] = "Kwangwoon";
            mic[3] = "-road 20"; mic[4] = "Kwangwoon University";

            for (int i = 0; i < mic.Length; i++)
                Console.Write("{0} ", mic[i]);

            Console.WriteLine();
        }
    }
}
